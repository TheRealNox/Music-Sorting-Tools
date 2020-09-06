//
// Created by Nox on 04-Sep-20.
//

#include "MainWindow.h"

#include "DatabaseManager.h"
#include "LibraryParser.h"

using namespace ScrutchPlayer;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->_ui.setupUi(this);
    DatabaseManager::getInstance();
}