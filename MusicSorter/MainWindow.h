#pragma once

#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class						MainWindow : public QMainWindow
{
	struct audioFolderInfo
	{
		int audioFilesNbr = 0;
		int averageBitRate = 0;
	};

    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass		_ui;

	// Make two models instead of one
	// to filter them separately
	QFileSystemModel *		_leftDirModel;
	QFileSystemModel *		_rightDirModel;

private:
	audioFolderInfo			containsAudioFiles(const QFileInfo &);
	void					handleIfAlbum(const QFileInfo &, const int & level, QPlainTextEdit * textEdit);

private slots:
	void					leftTreeItemExpanded(const QModelIndex &index);
	void					leftTreeItemClicked(const QModelIndex &index);
	void					rightTreeItemExpanded(const QModelIndex &index);
	void					rightTreeItemClicked(const QModelIndex &index);
};
