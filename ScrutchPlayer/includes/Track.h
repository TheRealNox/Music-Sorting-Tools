//
// Created by Nox on 04-Sep-20.
//

#pragma once

#include <string>

namespace ScrutchPlayer
{
    /**
    * \struct Track
    * \brief Hold all information about an audio track
    */
    struct Track
    {
        int _id;
        std::string _albumArtist;
        std::string _albumTitle;
        std::string _artist;
        std::string _comments;
        std::string _genre;
        std::string _filePath;
        std::string _title;
        std::string _coverUUID;
        unsigned short _bitrate = 0;
        unsigned short _disc = 0;
        unsigned short _totalDisk = 0;
        unsigned short _trackNumber = 0;
        unsigned short _totalTrackNumber = 0;
        unsigned short _year = 0;
        unsigned int _lengthInMS = 0;
    };
}


