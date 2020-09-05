//
// Created by Nox on 04-Sep-20.
//

#pragma once


/**
* \class LibraryParser
* \brief 
*/
class LibraryParser
{
    // -- Attributes
private:
    // --!Attributes

    // -- CTors & DTor
public:
    LibraryParser() = default;
    ~LibraryParser() = default;

    // Let's 'delete' all those to avoid them being used inadvertently.
    LibraryParser(const LibraryParser &) = delete;
    LibraryParser &operator=(const LibraryParser &) = delete;
    LibraryParser(LibraryParser &&) = delete;
    LibraryParser &operator=(LibraryParser &&) = delete;
    // --!CTors & DTor
};



