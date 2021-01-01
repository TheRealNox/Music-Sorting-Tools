//
// Created by Nox on 04-Sep-20.
//

#include "MainWindow.h"

#include "DatabaseManager.h"
#include "LibraryParser.h"

#include <QFileDialog>
#include <QSqlTableModel>

using namespace ScrutchPlayer;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->_ui.setupUi(this);
    this->connectUI();
}

void MainWindow::connectUI()
{
    connect(this->_ui._openFolderButton, &QPushButton::clicked, this, &MainWindow::openFolderPushed);
    DatabaseManager::getInstance().connectViewToModel(this->_ui._dbView, this->_ui._columnView);
    this->_ui.statusbar->showMessage(QString("Tracks: " + QString::number(this->_ui._dbView->model()->rowCount())));
}

void MainWindow::openFolderPushed()
{
    this->_ui.statusbar->showMessage(QString("Tracks: " + QString::number(this->_ui._dbView->model()->rowCount())));
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
        return;

    LibraryParser parser;
    parser.parsePath(dir.toStdString());
}
