//
// Created by Nox on 05-Sep-20.
//

#include <cstdlib>
#include <iostream>

#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QStandardPaths>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>

#include "DatabaseManager.h"

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
        //artwork db
        query.exec("create table artwork (id int primary key, uuid text NOT NULL, path text NOT NULL)");
        //track db
        query.exec("create table track (id int primary key, album_artist text, album_title text, artist text,"
                   "comments text, genre text, file_path text, title text, cover_uuid text, bitrate int, disc int,"
                   "total_disk int, track_nbr int, total_track_nbr int, year int, length int");

        std::cout << "error if any:" << query.lastError().text().toStdString() << std::endl;

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
}
