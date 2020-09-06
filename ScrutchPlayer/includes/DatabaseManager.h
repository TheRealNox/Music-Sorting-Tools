//
// Created by Nox on 05-Sep-20.
//

#pragma once

namespace ScrutchPlayer
{
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
        // --!Attributes

        // -- CTors & DTor
    public:
        static DatabaseManager &	getInstance();
        // unwanted ctors
        DatabaseManager(const DatabaseManager &) = delete;
        DatabaseManager &		operator=(const DatabaseManager &) = delete;

    private:
        DatabaseManager();
        ~DatabaseManager() = default;

        static void			deleteDatabaseManager();
        // --!CTors & DTor

        // -- Methods
    private:
        void createDatabase();

    public:
        void connectToLocalDatabase();

        // --!Methods
    };
}



