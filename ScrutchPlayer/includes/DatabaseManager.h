//
// Created by Nox on 05-Sep-20.
//

#pragma once

class QAbstractItemView;
class QSqlQueryModel;

#include "ThumbnailManager.h"

namespace ScrutchPlayer
{

    class Track;
    /**
    * \class DatabaseManager
    * \brief
    */
    class DatabaseManager
    {
        // -- Attributes
    private:
        static DatabaseManager *	_instance;
        QString _libraryPath;
        bool _isInitialized = false;
        QSqlQueryModel * _model = nullptr;
        ThumbnailManager _thumbnailManager;
        // --!Attributes

        // -- CTors & DTor
    public:
        static DatabaseManager &	getInstance();
        // unwanted ctors
        DatabaseManager(const DatabaseManager &) = delete;
        DatabaseManager &		operator=(const DatabaseManager &) = delete;

    private:
        DatabaseManager();
        ~DatabaseManager();

        static void			deleteDatabaseManager();
        // --!CTors & DTor

        // -- Methods
    private:
        void createDatabase();

    public:
        ThumbnailManager & getThumbnailManager();
        void connectToLocalDatabase();
        void connectViewToModel(QAbstractItemView *);
        bool addTrackToDatabase(const Track &);
        const std::string getCoverUUIDFromPath(const std::wstring & path);
        // --!Methods
    };
}



