#include "MainWindow.h"

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/mp4file.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>

#include <QImage>
#include <QLabel>
#include <QProgressDialog>

#include <typeinfo>

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

	this->_ui._explorerView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->_ui._explorerView_2->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

	connect(this->_ui._explorerView, &QTreeView::clicked, this, &MainWindow::leftTreeItemClicked);
	connect(this->_ui._explorerView_2, &QTreeView::clicked, this, &MainWindow::rightTreeItemClicked);
	connect(this->_ui.displayKbpsCheckBox, &QCheckBox::toggled, this, [&](bool checked) {
		this->_displayAverage = checked;
	});

}

MainWindow::audioFolderInfo	MainWindow::containsAudioFiles(const QFileInfo & entry)
{
	MainWindow::audioFolderInfo infos;

	QDir filesDir(entry.absoluteFilePath());
	filesDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
	for (auto fileInfo : filesDir.entryInfoList())
	{
		QCoreApplication::processEvents();
		if (fileInfo.fileName().toLower().endsWith(".mp3") || fileInfo.fileName().toLower().endsWith(".m4a"))
		{
			infos.audioFilesNbr++;
			if (this->_displayAverage)
			{
				std::wstring wpath = fileInfo.filePath().toStdWString();
				TagLib::FileRef f(wpath.c_str());
				if (!f.isNull() && f.audioProperties())
				{
					infos.averageBitRate += f.audioProperties()->bitrate();
					if (typeid(TagLib::MPEG::File) == typeid(*f.file()))
					{
						TagLib::MPEG::File* file = static_cast<TagLib::MPEG::File*>(f.file());
						if (file->ID3v2Tag())
						{
							auto tag = file->ID3v2Tag();
							const TagLib::ID3v2::FrameList& frameList = tag->frameList("APIC");
							if (!frameList.isEmpty())
							{
								// Just grab the first image.
								const auto* frame = (TagLib::ID3v2::AttachedPictureFrame*)frameList.front();
								QImage cover;
								cover.loadFromData((const uchar*)frame->picture().data(), frame->picture().size());
								infos.averageCoverSize += cover.size();
							}
						}
					}
					else if (typeid(TagLib::MP4::File) == typeid(*f.file()))
					{
						TagLib::MP4::File* file = static_cast<TagLib::MP4::File*>(f.file());
						TagLib::MP4::Tag* tag = file->tag();
						const TagLib::MP4::ItemListMap& itemListMap = tag->itemListMap();
						if (itemListMap.contains("covr"))
						{
							const TagLib::MP4::CoverArtList& coverArtList = itemListMap["covr"].toCoverArtList();
							if (!coverArtList.isEmpty())
							{
								const TagLib::MP4::CoverArt* pic = &(coverArtList.front());
								QImage cover;
								cover.loadFromData((const uchar*)pic->data().data(), pic->data().size());
								infos.averageCoverSize += cover.size();
							}
						}
					}
				}
			}

		}
	}

	if (infos.audioFilesNbr && this->_displayAverage)
	{
		infos.averageBitRate /= infos.audioFilesNbr;
		infos.averageCoverSize /= infos.audioFilesNbr;
	}

	return infos;
}

void	MainWindow::handleIfAlbum(const QFileInfo &entry, const int & level, QTextEdit * textEdit)
{
	// Check if we see a year, which is usually how we define an album folder
	if (entry.fileName().split(" ").at(0).toInt() > 1900 && entry.fileName().split(" ").at(0).toInt() < 2199)
	{
		QDir subdir(entry.absoluteFilePath());
		subdir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		MainWindow::audioFolderInfo infos = this->containsAudioFiles(entry);
		if (infos.audioFilesNbr)
		{
			QString output;
			for (int i = 0; i < level; ++i)
				output.append("\t");
			output.append(entry.fileName());
			output.append(" (");
			output.append(QString::number(infos.audioFilesNbr));
			output.append(")");
			textEdit->append(output);
			if (this->_displayAverage)
				this->outputAlbumInfoToTextEdit(output, infos.averageBitRate, infos.averageCoverSize, textEdit);

		}
		else
		{
			if (subdir.entryInfoList().size() > 0)
			{
				std::vector<MainWindow::audioFolderInfo> tracksInfoPerDisk;
				for (auto fileInfo : subdir.entryInfoList())
				{
					QCoreApplication::processEvents();
					MainWindow::audioFolderInfo trackInfo;
					trackInfo = this->containsAudioFiles(fileInfo);
					if (trackInfo.audioFilesNbr > 0)
						tracksInfoPerDisk.push_back(trackInfo);
				}

				if (tracksInfoPerDisk.size())
				{
					QString output;
					int averageAllCds = 0;
					QSize averageCover;
					for (int i = 0; i < level; ++i)
						output.append("\t");
					output.append(entry.fileName());
					output.append(" (");
					for (int i = 0; i < tracksInfoPerDisk.size(); ++i)
					{
						output.append(QString::number(tracksInfoPerDisk.at(i).audioFilesNbr));
						averageAllCds += tracksInfoPerDisk.at(i).averageBitRate;
						averageCover += tracksInfoPerDisk.at(i).averageCoverSize;
						if (i < tracksInfoPerDisk.size() - 1)
							output.append("-");
					}
					output.append(")");
					textEdit->append(output);
					if (this->_displayAverage)
					{
						averageAllCds /= tracksInfoPerDisk.size();
						averageCover /= tracksInfoPerDisk.size();
						this->outputAlbumInfoToTextEdit(output, averageAllCds, averageCover, textEdit);
					}
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
		textEdit->append(output);
		QDir dir(entry.absoluteFilePath());
		dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		for (auto entry : dir.entryInfoList())
		{
			QCoreApplication::processEvents();
			this->handleIfAlbum(entry, level + 1, textEdit);
		}
	}
}

void MainWindow::outputAlbumInfoToTextEdit(QString & toAppendto, const int& bitrate, const QSize& artworkSize, QTextEdit* textEdit)
{
	QString toAdd;
	toAdd.append("    [Avg. BitRate: ");
	toAdd.append(QString::number(bitrate));
	toAdd.append(" kbps]");

	if (bitrate < 192)
		textEdit->setTextColor(QColor("red"));
	textEdit->insertPlainText(toAdd);
	textEdit->setTextColor(QColor("black"));
	toAdd = "    [Avg. Cover: ";

	toAdd.append(QString::number(artworkSize.width()));
	toAdd.append("x");
	toAdd.append(QString::number(artworkSize.height()));
	toAdd.append("]");
	if (artworkSize.width() != artworkSize.height())
		textEdit->setTextColor(QColor("orange"));
	if (artworkSize.width() < 500)
		textEdit->setTextColor(QColor("chocolate"));
	textEdit->insertPlainText(toAdd);
	textEdit->setTextColor(QColor("black"));
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
		this->_ui._previewTextEdit->clear();
		QDir dir(mPath);
		dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		for (QFileInfo entry : dir.entryInfoList())
			this->handleIfAlbum(entry, 0, this->_ui._previewTextEdit);
		QTextCursor cursor(this->_ui._previewTextEdit->textCursor());
		cursor.movePosition(QTextCursor::Start);
		this->_ui._previewTextEdit->setTextCursor(cursor);
	}
}

void			MainWindow::rightTreeItemClicked(const QModelIndex &index)
{
	// TreeView clicked
	// 1. We need to extract path
	// 2. Set that path into our ListView

	// Get the full path of the item that's user clicked on
	QProgressDialog progress("Analyzing files...", "Cancel", 0, 0, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.show();
	QCoreApplication::processEvents();
	QString		mPath = this->_rightDirModel->fileInfo(index).absoluteFilePath();

	QFileInfo	fileInfo(mPath);

	if (fileInfo.isDir())
	{
		this->_ui._previewTextEdit_2->clear();
		QDir dir(mPath);
		dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		for (QFileInfo entry : dir.entryInfoList())
		{
			QCoreApplication::processEvents();
			this->handleIfAlbum(entry, 0, this->_ui._previewTextEdit_2);
			if (progress.wasCanceled())
				return;
		}
		QTextCursor cursor(this->_ui._previewTextEdit_2->textCursor());
		cursor.movePosition(QTextCursor::Start);
		this->_ui._previewTextEdit_2->setTextCursor(cursor);
	}
}
