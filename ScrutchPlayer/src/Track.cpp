//
// Created by Nox on 04-Sep-20.
//

#include "Track.h"

const std::string & Track::getAlbumArtist() const
{
    return _albumArtist;
}

void Track::setAlbumArtist(const std::string &albumArtist)
{
    _albumArtist = albumArtist;
}

const std::string & Track::getAlbumTitle() const
{
    return _albumTitle;
}

void Track::setAlbumTitle(const std::string &albumTitle)
{
    _albumTitle = albumTitle;
}

const std::string & Track::getArtist() const
{
    return _artist;
}

void Track::setArtist(const std::string &artist)
{
    _artist = artist;
}

const std::string & Track::getComments() const
{
    return _comments;
}

void Track::setComments(const std::string &comments)
{
    _comments = comments;
}

const std::string & Track::getGenre() const
{
    return _genre;
}

void Track::setGenre(const std::string &genre)
{
    _genre = genre;
}

const std::string & Track::getTitle() const
{
    return _title;
}

void Track::setTitle(const std::string &title)
{
    _title = title;
}

unsigned short Track::getBitrate() const
{
    return _bitrate;
}

void Track::setBitrate(unsigned short bitrate)
{
    _bitrate = bitrate;
}

unsigned short Track::getDisc() const
{
    return _disc;
}

void Track::setDisc(unsigned short disc)
{
    _disc = disc;
}

unsigned short Track::getTotalDisk() const
{
    return _totalDisk;
}

void Track::setTotalDisk(unsigned short totalDisk)
{
    _totalDisk = totalDisk;
}

unsigned short Track::getTrackNumber() const
{
    return _trackNumber;
}

void Track::setTrackNumber(unsigned short trackNumber)
{
    _trackNumber = trackNumber;
}

unsigned short Track::getTotalTrackNumber() const
{
    return _totalTrackNumber;
}

void Track::setTotalTrackNumber(unsigned short totalTrackNumber)
{
    _totalTrackNumber = totalTrackNumber;
}

unsigned short Track::getYear() const
{
    return _year;
}

void Track::setYear(unsigned short year)
{
    _year = year;
}

unsigned int Track::getLengthInMs() const
{
    return _lengthInMS;
}

void Track::setLengthInMs(unsigned int lengthInMs)
{
    _lengthInMS = lengthInMs;
}
