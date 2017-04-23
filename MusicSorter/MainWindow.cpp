#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	this->_ui.setupUi(this);

	//DIRECTORIES
	// Creates our new model and populate
	QString mPath = "C:/";
	this->_dirModel = new QFileSystemModel(this);

	// Set filter
	this->_dirModel->setFilter(QDir::NoDotAndDotDot |
		QDir::AllDirs);

	// QFileSystemModel requires root path
	this->_dirModel->setRootPath(mPath);

	// Attach the model to the view
	this->_ui._explorerView->setModel(this->_dirModel);

	// FILES

	this->_fileModel = new QFileSystemModel(this);

	// Set filter
	this->_fileModel->setFilter(QDir::NoDotAndDotDot |
		QDir::AllEntries);

	// QFileSystemModel requires root path
	this->_fileModel->setRootPath(mPath);

	// Attach the model to the view
	this->_ui._fileListView->setModel(this->_fileModel);

	connect(this->_ui._explorerView, &QTreeView::clicked, this, &MainWindow::treeItemClicked);
}

void			MainWindow::treeItemClicked(const QModelIndex &index)
{
	// TreeView clicked
	// 1. We need to extract path
	// 2. Set that path into our ListView

	// Get the full path of the item that's user clicked on
	QString		mPath = this->_dirModel->fileInfo(index).absoluteFilePath();
	QString		plainText;

	QFileInfo	fileInfo(mPath);

	if (fileInfo.isDir())
	{
		QDir dir(mPath);
		dir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		plainText.append("\t");
		plainText.append(dir.dirName());
		plainText.append(" (");
		plainText.append(QString::number(dir.entryInfoList().size()));
		plainText.append(")\n");
		plainText.append(this->printChildren(dir.entryInfoList(), 2));
	}

	this->_ui._previewPlainTextEdit->setPlainText(plainText);

	this->_ui._fileListView->setRootIndex(this->_fileModel->setRootPath(mPath));

	this->_ui._explorerView->header()->resizeSections(QHeaderView::ResizeToContents);
}

bool	MainWindow::isAlbum(const QFileInfo &file)
{
	if (file.fileName().split(" ").at(0).toInt() > 1024 && file.fileName().split(" ").at(0).toInt() < 4096)
		return true;

	return false;
}

QString MainWindow::printChildren(const QFileInfoList & entries, const int & level)
{
	QString plainText;

	for (QFileInfo entry : entries)
	{
		if (entry.isDir())
		{
			if (this->isAlbum(entry))
				qDebug() << "We guessed that's an album" << entry.absoluteFilePath();
			QDir subdir(entry.absoluteFilePath());
			subdir.setFilter(QDir::NoDotAndDotDot |	QDir::AllDirs);
			if (subdir.entryInfoList().size() > 0)
			{
				for (int i = 0; i < level; ++i)
					plainText.append("\t");
				plainText.append(entry.fileName());
				plainText.append(" (");
				plainText.append(QString::number(subdir.entryInfoList().size()));
				plainText.append(")\n");
				plainText.append(this->printChildren(subdir.entryInfoList(), level + 1));
			}
			else
			{
				qDebug() << subdir.absolutePath();
				QDir filesDir(entry.absoluteFilePath());
				filesDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
				if (entry.fileName().endsWith(".mp3") || entry.fileName().endsWith(".m4a"))
				{
					for (int i = 0; i < level; ++i)
						plainText.append("\t");
					plainText.append(entry.fileName());
					plainText.append(" (");
					plainText.append(QString::number(filesDir.entryInfoList().size()));
					plainText.append(")\n");
				}
				else
				{
					qDebug() << entry.fileName();
					plainText.append(this->printChildren(filesDir.entryInfoList(), level + 1));
				}
			}
		}
		else
		{
			int tracksNumber = 0;
			for (QFileInfo file : entries)
			{
				if (file.fileName().endsWith(".mp3") || file.fileName().endsWith(".m4a"))
					++tracksNumber;
			}

			for (int i = 1; i < level; ++i)
				plainText.append("\t");
			plainText.append(entry.dir().dirName());
			plainText.append(" (");
			plainText.append(QString::number(tracksNumber));
			plainText.append(")\n");
			break;
		}

	}

	return plainText;
}
