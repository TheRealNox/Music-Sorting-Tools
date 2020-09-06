//
// Created by Nox on 04-Sep-20.
//

#include "LibraryParser.h"
#include "Track.h"

#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QString>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/mp4file.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>

using namespace ScrutchPlayer;

void LibraryParser::parsePath(const std::string & path)
{
    QFileInfo   file(QString((path.c_str())));

    QDir filesDir(file.absoluteFilePath());
    filesDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const auto& fileInfo : filesDir.entryInfoList())
    {
        if (fileInfo.fileName().toLower().endsWith(".mp3") || fileInfo.fileName().toLower().endsWith(".m4a"))
        {
            TagLib::FileRef f(fileInfo.filePath().toStdWString().c_str());
            if (!f.isNull())
            {
                if (LibraryParser::_audioFileDispatcher[typeid(*f.file())] != nullptr)
                    if (!(this->*(_audioFileDispatcher[typeid(*f.file())]))(f))
                        std::cerr << "Couldn't add " << path << std::endl;
            }
        }
    }
}

bool LibraryParser::addMP3FileToDB(const TagLib::FileRef file)
{
    Track toAdd;

    auto* mp3File = dynamic_cast<TagLib::MPEG::File*>(file.file());
    if (mp3File->ID3v2Tag() && file.audioProperties())
    {
        TagLib::ID3v2::Tag *tag = mp3File->ID3v2Tag();
        // f.audioProperties()->bitrate();
        toAdd._bitrate = file.audioProperties()->bitrate();
    }
    return false;
}

bool LibraryParser::addM4AFileToDB(const TagLib::FileRef file)
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