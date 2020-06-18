#pragma once

#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QtWidgets/QMainWindow>
#include <QSize>
#include "ui_MainWindow.h"

class						MainWindow : public QMainWindow
{
	struct audioFolderInfo
	{
		int audioFilesNbr = 0;
		int averageBitRate = 0;
		QSize averageCoverSize = QSize(0, 0);
	};

    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass		_ui;
	bool					_displayAverage = false;

	// Make two models instead of one
	// to filter them separately
	QFileSystemModel *		_leftDirModel;
	QFileSystemModel *		_rightDirModel;

private:
	audioFolderInfo			containsAudioFiles(const QFileInfo &);
	void					handleIfAlbum(const QFileInfo &, const int & level, QTextEdit* textEdit);
	void					outputAlbumInfoToTextEdit(QString & toAppendto, const int & bitrate, const QSize & artworkSize, QTextEdit* textEdit);
	
private slots:
	void					leftTreeItemExpanded(const QModelIndex &index);
	void					leftTreeItemClicked(const QModelIndex &index);
	void					rightTreeItemExpanded(const QModelIndex &index);
	void					rightTreeItemClicked(const QModelIndex &index);
};
