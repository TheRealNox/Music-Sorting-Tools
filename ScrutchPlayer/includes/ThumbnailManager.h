//
// Created by Nox on 06-Sep-20.
//

#pragma once

#include <memory>
#include <map>
#include <string>

class QDir;
class QImage;

namespace ScrutchPlayer
{
/**
* \class ThumbnailManager
* \brief
*/
    class ThumbnailManager
    {
        // -- Attributes
    private:
        std::unique_ptr<QDir> _cacheDir;
        std::map<std::string, QImage *> _thumbnails;
        // --!Attributes

        // -- CTors & DTor
    public:
        ThumbnailManager();

        ~ThumbnailManager();

        // Let's 'delete' all those to avoid them being used inadvertently.
        ThumbnailManager(const ThumbnailManager &) = delete;

        ThumbnailManager & operator=(const ThumbnailManager &) = delete;

        ThumbnailManager(ThumbnailManager &&) = delete;

        ThumbnailManager & operator=(ThumbnailManager &&) = delete;
        // --!CTors & DTor

        // -- Methods
    public:
        std::string createThumbnail(const std::wstring & path, const QImage &image);
        std::string getThumbnailUUIDFromPath(const std::wstring & path);
        QImage * getThumbnailForUuid(const std::string &uuid);
    private:
        std::string getPathFromUUID(const std::string & uuid);
        // --!Methods
    };
}
