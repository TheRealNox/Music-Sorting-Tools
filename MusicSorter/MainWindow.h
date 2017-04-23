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
	int						containsAudioFiles(const QFileInfo &);
	void					handleIfAlbum(const QFileInfo &, const int & level);

private slots:
	void					treeItemExpanded(const QModelIndex &index);
	void					treeItemClicked(const QModelIndex &index);

};
