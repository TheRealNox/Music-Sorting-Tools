//
// Created by Nox on 06-Sep-20.
//

#include "ThumbnailManager.h"

#include <QStandardPaths>
#include <QByteArray>
#include <QImage>
#include <QDir>
#include <QFileInfo>
#include <QUuid>
#include <QCryptographicHash>

#include <iostream>

#include <QtWidgets/QLabel>
#include <QCoreApplication>

using namespace ScrutchPlayer;

ThumbnailManager::ThumbnailManager()
{
    this->_cacheDir = std::make_unique<QDir>(QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0));
    std::cout << "cache dir init at: " << QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0).toStdString() << std::endl;
}


ThumbnailManager::~ThumbnailManager()
{
    for (auto pair : this->_thumbnails)
        delete pair.second;
}

std::string ThumbnailManager::createThumbnail(const std::wstring & path, const QImage & image)
{
    std::string uuid = QCryptographicHash::hash(QString::fromWCharArray(path.c_str()).toLocal8Bit(), QCryptographicHash::Md5).toHex().toStdString();
    if (!image.isNull())
    {
        QImage * cachedImage = new QImage(image.bits(), image.width(), image.height(), image.format());
        std::string newPath = this->getPathFromUUID(uuid);
        if (!this->_cacheDir->mkpath(newPath.c_str()))
        {
            std::cerr << "Could not create directory " << newPath << std::endl;
        }
        newPath = newPath + '/' + uuid + ".bmp";
        if (!cachedImage->save(QString(newPath.c_str()), "BMP", 100))
            return "";
        std::cout << "uui created: " << uuid << std::endl;
        this->_thumbnails[uuid] = cachedImage;
    }

    return uuid;
}

std::string ThumbnailManager::getPathFromUUID(const std::string & uuid)
{
    if (uuid == "")
        return uuid;
    std::string	thumbnailPath = this->_cacheDir->path().toStdString();

    thumbnailPath += '/';
    thumbnailPath += QByteArray::fromStdString(uuid).toHex().at(0);
    thumbnailPath += '/';
    thumbnailPath += QByteArray::fromStdString(uuid).toHex().at(1);
    thumbnailPath += '/';
    thumbnailPath += QByteArray::fromStdString(uuid).toHex().at(2);

    return thumbnailPath;
}

QImage * ThumbnailManager::getThumbnailForUuid(const std::string & uuid)
{
    if (this->_thumbnails.find(uuid) !=  this->_thumbnails.end())
        return this->_thumbnails.at(uuid);
    if (QFileInfo(QString(this->getPathFromUUID(uuid).c_str()) + '/' + uuid.c_str() + ".bmp").exists())
    {
        this->_thumbnails[uuid] = new QImage(QString(this->getPathFromUUID(uuid).c_str()) + '/' + uuid.c_str() + ".bmp");
        return this->_thumbnails[uuid];
    }
    return nullptr;
}

std::string ThumbnailManager::getThumbnailUUIDFromPath(const std::wstring & path)
{
    std::string uuid = QCryptographicHash::hash(QString::fromWCharArray(path.c_str()).toLocal8Bit(), QCryptographicHash::Md5).toHex().toStdString();
    return uuid;
}
