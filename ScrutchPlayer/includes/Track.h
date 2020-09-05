//
// Created by Nox on 04-Sep-20.
//

#pragma once

#include <string>

/**
* \class Track
* \brief Hold all information about an audio track
*/
class Track
{
    // -- Attributes
private:
    std::string _albumArtist;
    std::string _albumTitle;
    std::string _artist;
    std::string _comments;
    std::string _genre;
    std::string _title;
    unsigned short _bitrate;
    unsigned short _disc;

private:
    unsigned short _totalDisk;
    unsigned short _trackNumber;
    unsigned short _totalTrackNumber;
    unsigned short _year;
    unsigned int _lengthInMS;
    // --!Attributes

    // -- CTors & DTor
public:
    Track() = default;
    ~Track() = default;

    // Let's 'delete' all those to avoid them being used inadvertently.
    Track(const Track &) = delete;
    Track &operator=(const Track &) = delete;
    Track(Track &&) = delete;
    Track &operator=(Track &&) = delete;
    // --!CTors & DTor

    // -- Methods
public:
    const std::string &getAlbumArtist() const;

    void setAlbumArtist(const std::string &albumArtist);

    const std::string &getAlbumTitle() const;

    void setAlbumTitle(const std::string &albumTitle);

    const std::string &getArtist() const;

    void setArtist(const std::string &artist);

    const std::string &getComments() const;

    void setComments(const std::string &comments);

    const std::string &getGenre() const;

    void setGenre(const std::string &genre);

    const std::string &getTitle() const;

    void setTitle(const std::string &title);

    unsigned short getBitrate() const;

    void setBitrate(unsigned short bitrate);

    unsigned short getDisc() const;

    void setDisc(unsigned short disc);

    unsigned short getTotalDisk() const;

    void setTotalDisk(unsigned short totalDisk);

    unsigned short getTrackNumber() const;

    void setTrackNumber(unsigned short trackNumber);

    unsigned short getTotalTrackNumber() const;

    void setTotalTrackNumber(unsigned short totalTrackNumber);

    unsigned short getYear() const;

    void setYear(unsigned short year);

    unsigned int getLengthInMs() const;

    void setLengthInMs(unsigned int lengthInMs);

private:
    // --!Methods


};



