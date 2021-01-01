//
// Created by Nox on 04-Sep-20.
//

#include "LibraryParser.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QImage>
#include <QString>
#include <QProgressDialog>
#include <QCoreApplication>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/mp4file.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/tpropertymap.h>
#include <taglib/attachedpictureframe.h>

#include "DatabaseManager.h"
#include "Track.h"
#include "ThumbnailManager.h"

using namespace ScrutchPlayer;

void LibraryParser::parsePath(const std::string & path)
{
    QFileInfo   file(QString((path.c_str())));

    QProgressDialog progress("Adding files...", "Abort import", 0, 0);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    QDirIterator it(path.c_str(), QStringList() << "*.mp3" << "*.m4a", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QCoreApplication::processEvents();
        if (progress.wasCanceled())
            break;
        QString currentFile = it.next();
        TagLib::FileRef f(currentFile.toStdWString().c_str());
        if (!f.isNull())
        {
            if (LibraryParser::_audioFileDispatcher[typeid(*f.file())] != nullptr)
                if (!(this->*(_audioFileDispatcher[typeid(*f.file())]))(f, currentFile.toHtmlEscaped().toStdWString()))
                    std::cerr << "Couldn't add " << currentFile.toStdWString() << std::endl;
        }
    }
}

bool LibraryParser::addMP3FileToDB(const TagLib::FileRef file, const std::wstring & path)
{
    Track toAdd;

    auto* mp3File = dynamic_cast<TagLib::MPEG::File*>(file.file());
    if (mp3File->ID3v2Tag() && file.audioProperties())
    {
        TagLib::ID3v2::Tag *tag = mp3File->ID3v2Tag();
        /*
         *    * Basic tags:
           *
           *  - TITLE
           *  - ALBUM
           *  - ARTIST
           *  - ALBUMARTIST
           *  - SUBTITLE
           *  - TRACKNUMBER
           *  - DISCNUMBER
           *  - DATE
           *  - ORIGINALDATE
           *  - GENRE
           *  - COMMENT
           *
           * Sort names:
           *
           *  - TITLESORT
           *  - ALBUMSORT
           *  - ARTISTSORT
           *  - ALBUMARTISTSORT
         */
        const TagLib::ID3v2::FrameList& frameList = tag->frameList("APIC");
        if (!frameList.isEmpty())
        {
            // Just grab the first image.
            const auto* frame = (TagLib::ID3v2::AttachedPictureFrame*)frameList.front();
            QImage * cover = new QImage();
            cover->loadFromData((const uchar*)frame->picture().data(), frame->picture().size());
            toAdd._coverUUID = DatabaseManager::getInstance().getThumbnailManager().createThumbnail(path, *cover);
        }
        toAdd._bitrate = file.audioProperties()->bitrate();
        toAdd._filePath = path;
        toAdd._albumArtist = mp3File->properties()["ALBUMARTIST"].toString().toCWString();
        toAdd._albumTitle = mp3File->properties()["ALBUM"].toString().toCWString();
        toAdd._artist = mp3File->properties()["ARTIST"].toString().toCWString();
        toAdd._comments = mp3File->properties()["COMMENT"].toString().toCWString();
        toAdd._genre = mp3File->properties()["GENRE"].toString().toCWString();
        toAdd._title = mp3File->properties()["TITLE"].toString().toCWString();
        std::string discs = mp3File->properties()["DISC"].toString().toCString();
        toAdd._disc = std::atoi(mp3File->properties()["DISC"].toString().toCString());
        toAdd._totalDisk = 0;
        std::string tracks = mp3File->properties()["TRACKNUMBER"].toString().toCString();
        toAdd._trackNumber = std::atoi(mp3File->properties()["TRACKNUMBER"].toString().toCString());
        toAdd._totalTrackNumber = 0;
        toAdd._year = std::atoi(mp3File->properties()["DATE"].toString().toCString());
        toAdd._lengthInMS = file.audioProperties()->lengthInMilliseconds();
        return DatabaseManager::getInstance().addTrackToDatabase(toAdd);
    }
    return false;
}

bool LibraryParser::addM4AFileToDB(const TagLib::FileRef file, const std::wstring & path)
{
    return false;
}

//             if (!f.isNull() && f.audioProperties())
//            {
//                //infos.averageBitRate += f.audioProperties()->bitrate();
//                if (typeid(TagLib::MPEG::File) == typeid(*f.file()))
//                {
//                    auto* mp3File = dynamic_cast<TagLib::MPEG::File*>(f.file());
//                    if (mp3File->ID3v2Tag())
//                    {
//                        TagLib::ID3v2::Tag * tag = mp3File->ID3v2Tag();
//                        const TagLib::ID3v2::FrameList& frameList = tag->frameList("APIC");
//                        for (const auto& truc : tag->frameListMap())
//                        {
//                            std::string test = truc.first.data();
//                            std::cout << truc.first << std::endl;
//                        }
//                        if (!frameList.isEmpty())
//                        {
//                            // Just grab the first image.
//                            const auto* frame = (TagLib::ID3v2::AttachedPictureFrame*)frameList.front();
//                            QImage cover;
//                            cover.loadFromData((const uchar*)frame->picture().data(), frame->picture().size());
//                            //infos.averageCoverSize += cover.size();
//                        }
//                    }
//                }
//                else if (typeid(TagLib::MP4::File) == typeid(*f.file()))
//                {
//                    auto* mp4File = dynamic_cast<TagLib::MP4::File*>(f.file());
//                    TagLib::MP4::Tag * tag = mp4File->tag();
//                    const TagLib::MP4::ItemMap& itemListMap = tag->itemMap();
//                    if (itemListMap.contains("covr"))
//                    {
//                        const TagLib::MP4::CoverArtList& coverArtList = itemListMap["covr"].toCoverArtList();
//                        if (!coverArtList.isEmpty())
//                        {
//                            const TagLib::MP4::CoverArt* pic = &(coverArtList.front());
//                            QImage cover;
//                            cover.loadFromData((const uchar*)pic->data().data(), pic->data().size());
//                            //infos.averageCoverSize += cover.size();
//                        }
//                    }
//                }
//            }