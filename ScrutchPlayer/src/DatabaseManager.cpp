//
// Created by Nox on 05-Sep-20.
//

#include <cstdlib>
#include <iostream>

#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QStandardPaths>
#include <QAbstractItemView>

#include "DatabaseManager.h"
#include "TrackItemDelegate.h"
#include "Track.h"

using namespace ScrutchPlayer;

DatabaseManager* DatabaseManager::_instance = nullptr;

DatabaseManager &				DatabaseManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new DatabaseManager();
        std::atexit(&DatabaseManager::deleteDatabaseManager);
    }

    return *_instance;
}

void DatabaseManager::deleteDatabaseManager()
{
    delete _instance;
    _instance = nullptr;
}

DatabaseManager::DatabaseManager()
{
    this->_libraryPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/library.sqlite";
    this->connectToLocalDatabase();
}

void DatabaseManager::createDatabase()
{
    // Check if we have the proper dll loaded
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        std::cerr << "Couldn't load SQLite drivers" << std::endl;
    else
        std::cout << "SQLite drivers found" << std::endl;

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    // If the folder doesn't  exist, let's create it as QSqlDatabase can't do that for you...
    if (!dir.exists())
        dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->_libraryPath);

    if (db.open())
    {
        std::cout << "db created at " << this->_libraryPath.toStdString() << std::endl;
        // Let's init our database
        QSqlQuery query;
        query.exec("PRAGMA encoding = \"UTF-16\"");
        //artwork db
        query.exec("create table artwork (uuid text NOT NULL, path text NOT NULL)");
        //track db
        query.exec("create table track (album_artist text, album_title text, artist text,"
                   "comments text, genre text, file_path text unique, title text, cover_uuid text, bitrate int, disc int,"
                   "total_disk int, track_nbr int, total_track_nbr int, year int, length int)");

        this->_model = new QSqlQueryModel;
        this->_model->setQuery("SELECT * FROM track", db);
    }
    else
    {
        std::cerr << "couldn't open db at " << this->_libraryPath.toStdString() << " error: " << db.lastError().text().toStdString() << std::endl;
    }

}

void DatabaseManager::connectToLocalDatabase()
{
    if (!QFileInfo::exists(this->_libraryPath))
    {
        std::cout << "No Database found, let's create one" << std::endl;
        this->createDatabase();
    }
    else
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(this->_libraryPath);
        if (db.open())
        {
            this->_model = new QSqlQueryModel;
            this->_model->setQuery("SELECT * FROM track", db);
        }
    }
}

void DatabaseManager::connectViewToModel(QAbstractItemView *tableView)
{
    tableView->setModel(this->_model);
    tableView->setItemDelegate(new TrackItemDelegate());
    this->_model->setQuery(this->_model->query());
}

bool DatabaseManager::addTrackToDatabase(const Track & track)
{
    if (QSqlDatabase::database().isOpen())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO track (album_artist, album_title, artist, comments, genre, file_path,"
                      "title, cover_uuid, bitrate, disc, total_disk, track_nbr, total_track_nbr, year, length) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(QString::fromWCharArray(track._albumArtist.c_str()));
        query.addBindValue(QString::fromWCharArray(track._albumTitle.c_str()));
        query.addBindValue(QString::fromWCharArray(track._artist.c_str()));
        query.addBindValue(QString::fromWCharArray(track._comments.c_str()));
        query.addBindValue(QString::fromWCharArray(track._genre.c_str()));
        query.addBindValue(QString::fromWCharArray(track._filePath.c_str()));
        query.addBindValue(QString::fromWCharArray(track._title.c_str()));
        query.addBindValue(QString::fromStdString(track._coverUUID));
        query.addBindValue(track._bitrate);
        query.addBindValue(track._disc);
        query.addBindValue(track._totalDisk);
        query.addBindValue(track._trackNumber);
        query.addBindValue(track._totalTrackNumber);
        query.addBindValue(track._year);
        query.addBindValue(track._lengthInMS);
        query.exec();
        if (query.lastError().text().isEmpty())
        {
            this->_model->setQuery(this->_model->query());
            this->_model->query().exec();
            return true;
        }
        else
        {
            QSqlDatabase::database().rollback();
            return false;
        }
    }
    return false;
}

DatabaseManager::~DatabaseManager()
{
    QSqlDatabase::database().close();
}

const std::string DatabaseManager::getCoverUUIDFromPath(const std::wstring & path)
{
    return this->_thumbnailManager.getThumbnailUUIDFromPath(path);
}

ThumbnailManager & DatabaseManager::getThumbnailManager()
{
    return this->_thumbnailManager;
}
