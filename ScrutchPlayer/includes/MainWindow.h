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

    public:
        explicit MainWindow(QWidget *parent = nullptr);

    private:
        Ui::MainWindow _ui;

    private:

    private slots:
    };
}