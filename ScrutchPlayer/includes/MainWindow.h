//
// Created by Nox on 04-Sep-20.
//
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

namespace ScrutchPlayer
{
    /**
    * \class MainWindow
    * \brief Main UI class, it holds all other widgets
    */
    class MainWindow : public QMainWindow
    {
    Q_OBJECT
        // -- Attributes
    private:
        Ui::MainWindow _ui;

        // --!Attributes

        // -- CTors & DTor
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        // --!CTors & DTor

        // -- Methods
    private:
        void connectUI();
    public:
        // --!Methods

        // -- Signals and Slots
    private slots:
        void openFolderPushed();
        // --!Signals and Slots

    };
}