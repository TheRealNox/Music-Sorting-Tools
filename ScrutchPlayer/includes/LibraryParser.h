//
// Created by Nox on 04-Sep-20.
//

#pragma once

#include <string>
#include <map>
#include <typeindex>

#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/mp4file.h>

namespace ScrutchPlayer
{


    /**
    * \class LibraryParser
    * \brief
    */
    class LibraryParser
    {
        //Another typedef: the map containing a type_info for key and the functor for value
        typedef std::map<std::type_index, bool (LibraryParser::*)(const TagLib::FileRef, const std::wstring &)> AudioDispatcherMap;

        // -- Attributes
    private:
        AudioDispatcherMap			_audioFileDispatcher{{typeid(TagLib::MPEG::File), &LibraryParser::addMP3FileToDB},
                                                     {typeid(TagLib::MP4::File), &LibraryParser::addM4AFileToDB}};
        // --!Attributes

        // -- CTors & DTor
    public:
        LibraryParser() = default;

        ~LibraryParser() = default;

        // Let's 'delete' all those to avoid them being used inadvertently.
        LibraryParser(const LibraryParser &) = delete;

        LibraryParser & operator=(const LibraryParser &) = delete;

        LibraryParser(LibraryParser &&) = delete;

        LibraryParser & operator=(LibraryParser &&) = delete;
        // --!CTors & DTor

        // -- Methods
    private:
        bool addMP3FileToDB(const TagLib::FileRef, const std::wstring &);
        bool addM4AFileToDB(const TagLib::FileRef, const std::wstring &);

    public:
        void parsePath(const std::string & path);
        // --!Methods
    };
}


