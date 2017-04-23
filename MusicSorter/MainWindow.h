#pragma once

#include <QDebug>
#include <QFileSystemModel>
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class						MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass		_ui;
	// Make two models instead of one
	// to filter them separately
	QFileSystemModel *		_dirModel;
	QFileSystemModel *		_fileModel;

private:
	bool					isAlbum(const QFileInfo &);
	QString					printChildren(const QFileInfoList & entries, const int & level);

private slots:
	void					treeItemClicked(const QModelIndex &index);

};
