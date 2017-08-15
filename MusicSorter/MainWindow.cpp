#include "MainWindow.h"

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

}

int	MainWindow::containsAudioFiles(const QFileInfo & entry)
{
	int audioFiles = 0;

	QDir filesDir(entry.absoluteFilePath());
	filesDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
	for (auto fileInfo : filesDir.entryInfoList())
	{
		if (fileInfo.fileName().endsWith(".mp3") || fileInfo.fileName().endsWith(".m4a"))
			audioFiles++;
	}

	return audioFiles;
}

void	MainWindow::handleIfAlbum(const QFileInfo &entry, const int & level, QPlainTextEdit * textEdit)
{
	if (entry.fileName().split(" ").at(0).toInt() > 1024 && entry.fileName().split(" ").at(0).toInt() < 4096)
	{
		QDir subdir(entry.absoluteFilePath());
		subdir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		if (int i = this->containsAudioFiles(entry))
		{
			QString output;
			for (int i = 0; i < level; ++i)
				output.append("\t");
			output.append(entry.fileName());
			output.append(" (");
			output.append(QString::number(i));
			output.append(")");
			textEdit->appendPlainText(output);
		}
		else
		{
			if (subdir.entryInfoList().size() > 0)
			{
				std::vector<int> tracksPerDisk;
				for (auto fileInfo : subdir.entryInfoList())
				{
					int trackWithin = 0;
					trackWithin = this->containsAudioFiles(fileInfo);
					if (trackWithin > 0)
						tracksPerDisk.push_back(trackWithin);
				}

				if (tracksPerDisk.size())
				{
					QString output;
					for (int i = 0; i < level; ++i)
						output.append("\t");
					output.append(entry.fileName());
					output.append(" (");
					for (int i = 0; i < tracksPerDisk.size(); ++i)
					{
						output.append(QString::number(tracksPerDisk.at(i)));
						if (i < tracksPerDisk.size() - 1)
							output.append("-");
					}
					output.append(")");
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
