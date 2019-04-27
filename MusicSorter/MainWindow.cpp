#include "MainWindow.h"

#include <QMediaObject>
#include <QMediaMetaData>
#include <QMediaPlayer>

#include "MediaInfoDLL.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	this->_ui.setupUi(this);

	//DIRECTORIES
	// Creates our new model and populate
	QString mPath = "C:/";
	this->_leftDirModel = new QFileSystemModel(this);
	this->_rightDirModel = new QFileSystemModel(this);

	// Set filter
	this->_leftDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	this->_rightDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

	// QFileSystemModel requires root path
	this->_leftDirModel->setRootPath(mPath);
	this->_rightDirModel->setRootPath(mPath);

	// Attach the model to the view
	this->_ui._explorerView->setModel(this->_leftDirModel);
	this->_ui._explorerView_2->setModel(this->_rightDirModel);

	connect(this->_ui._explorerView, &QTreeView::clicked, this, &MainWindow::leftTreeItemClicked);
	connect(this->_ui._explorerView, &QTreeView::expanded, this, &MainWindow::leftTreeItemExpanded);
	connect(this->_ui._explorerView_2, &QTreeView::clicked, this, &MainWindow::rightTreeItemClicked);
	connect(this->_ui._explorerView_2, &QTreeView::expanded, this, &MainWindow::rightTreeItemExpanded);
	connect(this->_ui.displayKbpsCheckBox, &QCheckBox::toggled, this, [&](bool checked) {
		this->_displayAverage = checked;
	});

}

MainWindow::audioFolderInfo	MainWindow::containsAudioFiles(const QFileInfo & entry)
{
	MainWindow::audioFolderInfo infos;
	MediaInfoDLL::MediaInfo MI;
	MI.Option(L"Internet", L"No");
	MI.Option(__T("Info_Version"), __T("0.7.13;MediaInfoDLL_Example_MSVC;0.7.13"));

	QDir filesDir(entry.absoluteFilePath());
	filesDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
	for (auto fileInfo : filesDir.entryInfoList())
	{
		if (fileInfo.fileName().endsWith(".mp3") || fileInfo.fileName().endsWith(".m4a"))
		{
			if (this->_displayAverage)
				MI.Open(fileInfo.filePath().toStdWString());
			infos.audioFilesNbr++;
			if (this->_displayAverage)
				infos.averageBitRate += QString::QString::fromStdWString(MI.Get(MediaInfoDLL::Stream_Audio, 0, __T("BitRate"), MediaInfoDLL::Info_Text, MediaInfoDLL::Info_Name)).toInt();
		}
	}

	if (infos.audioFilesNbr && this->_displayAverage)
	{
		infos.averageBitRate /= infos.audioFilesNbr;
		infos.averageBitRate /= 1000;
	}

	return infos;
}

void	MainWindow::handleIfAlbum(const QFileInfo &entry, const int & level, QPlainTextEdit * textEdit)
{
	if (entry.fileName().split(" ").at(0).toInt() > 1024 && entry.fileName().split(" ").at(0).toInt() < 4096)
	{
		QDir subdir(entry.absoluteFilePath());
		subdir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		MainWindow::audioFolderInfo infos = this->containsAudioFiles(entry);
		if (int i = infos.audioFilesNbr)
		{
			QString output;
			for (int i = 0; i < level; ++i)
				output.append("\t");
			output.append(entry.fileName());
			output.append(" (");
			output.append(QString::number(i));
			output.append(")");
			if (this->_displayAverage)
			{
				output.append("    [Avg. BitRate: ");
				output.append(QString::number(infos.averageBitRate));
				output.append(" kbps]");
			}
			textEdit->appendPlainText(output);
		}
		else
		{
			if (subdir.entryInfoList().size() > 0)
			{
				std::vector<MainWindow::audioFolderInfo> tracksInfoPerDisk;
				for (auto fileInfo : subdir.entryInfoList())
				{
					MainWindow::audioFolderInfo trackInfo;
					trackInfo = this->containsAudioFiles(fileInfo);
					if (trackInfo.audioFilesNbr > 0)
						tracksInfoPerDisk.push_back(trackInfo);
				}

				if (tracksInfoPerDisk.size())
				{
					QString output;
					int averageAllCds = 0;
					for (int i = 0; i < level; ++i)
						output.append("\t");
					output.append(entry.fileName());
					output.append(" (");
					for (int i = 0; i < tracksInfoPerDisk.size(); ++i)
					{
						output.append(QString::number(tracksInfoPerDisk.at(i).audioFilesNbr));
						averageAllCds += tracksInfoPerDisk.at(i).averageBitRate;
						if (i < tracksInfoPerDisk.size() - 1)
							output.append("-");
					}
					output.append(")");
					if (this->_displayAverage)
					{
						averageAllCds /= tracksInfoPerDisk.size();
						output.append("    [Avg. BitRate: ");
						output.append(QString::number(averageAllCds));
						output.append(" kbps]");
					}
					textEdit->appendPlainText(output);
				}
			}
		}
	}
	else
	{
		QString output;
		for (int i = 0; i < level; ++i)
			output.append("\t");
		output.append(entry.fileName());
		textEdit->appendPlainText(output);
		QDir dir(entry.absoluteFilePath());
		dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		for (auto entry : dir.entryInfoList())
			this->handleIfAlbum(entry, level + 1, textEdit);
	}
}

void			MainWindow::leftTreeItemExpanded(const QModelIndex &index)
{
	QCoreApplication::processEvents();
	this->_ui._explorerView->header()->resizeSections(QHeaderView::ResizeToContents);
}

void			MainWindow::leftTreeItemClicked(const QModelIndex &index)
{
	// TreeView clicked
	// 1. We need to extract path
	// 2. Set that path into our ListView

	// Get the full path of the item that's user clicked on
	QString		mPath = this->_leftDirModel->fileInfo(index).absoluteFilePath();

	QFileInfo	fileInfo(mPath);

	if (fileInfo.isDir())
	{
		this->_ui._previewPlainTextEdit->clear();
		QDir dir(mPath);
		dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		for (auto entry : dir.entryInfoList())
			this->handleIfAlbum(entry, 0, this->_ui._previewPlainTextEdit);
		QTextCursor cursor(this->_ui._previewPlainTextEdit->textCursor());
		cursor.movePosition(QTextCursor::Start);
		this->_ui._previewPlainTextEdit->setTextCursor(cursor);
	}

	this->_ui._explorerView->header()->resizeSections(QHeaderView::ResizeToContents);
}

void			MainWindow::rightTreeItemExpanded(const QModelIndex &index)
{
	QCoreApplication::processEvents();
	this->_ui._explorerView_2->header()->resizeSections(QHeaderView::ResizeToContents);
}

void			MainWindow::rightTreeItemClicked(const QModelIndex &index)
{
	// TreeView clicked
	// 1. We need to extract path
	// 2. Set that path into our ListView

	// Get the full path of the item that's user clicked on
	QString		mPath = this->_rightDirModel->fileInfo(index).absoluteFilePath();

	QFileInfo	fileInfo(mPath);

	if (fileInfo.isDir())
	{
		this->_ui._previewPlainTextEdit_2->clear();
		QDir dir(mPath);
		dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		for (auto entry : dir.entryInfoList())
			this->handleIfAlbum(entry, 0, this->_ui._previewPlainTextEdit_2);
		QTextCursor cursor(this->_ui._previewPlainTextEdit_2->textCursor());
		cursor.movePosition(QTextCursor::Start);
		this->_ui._previewPlainTextEdit_2->setTextCursor(cursor);
	}

	this->_ui._explorerView_2->header()->resizeSections(QHeaderView::ResizeToContents);
}
