/*************************************************************** 
	FileBrowser.cpp

	The AnalogDesigner (AD) Project (since 2012)
 
	Contributor(s): 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn) 
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2010 - 2012
******************************************************************/ 

#include <QtGui>

#include "FileBrowser.h"

FileBrowser::FileBrowser(QWidget *parent) : QDialog(parent)
{

     	browseButton = new QPushButton(tr("Browse..."));
     	//findButton = createButton(tr("&Find"), SLOT(find()));

     	filterComb   = createFilterComb();
     	workpathComb = createWorkpathComb(QDir::currentPath());

	workpathLabel = new QLabel(tr("Workspace"));
     	//filesFoundLabel = new QLabel;
	
	subfoldersCheckBox = new QCheckBox(tr("Include subdirectories"));

	createFileListingTable();

	QGridLayout *mainLayout = new QGridLayout;
     	mainLayout->addWidget(workpathLabel, 0, 0);
     	mainLayout->addWidget(workpathComb, 1, 0);
    	mainLayout->addWidget(browseButton, 1, 1);
	mainLayout->addWidget(subfoldersCheckBox, 2, 0);	
	mainLayout->addWidget(filterComb, 2, 1);
  	mainLayout->addWidget(fileListingTable, 3, 0, 1, 3);

     	setLayout(mainLayout);

	// signals and slots
	connect( browseButton, SIGNAL(clicked()), this, SLOT(browse()) );

	connect( workpathComb, SIGNAL(editTextChanged(const QString &)), this, SLOT(find()) );

	connect( workpathComb, SIGNAL(currentIndexChanged (int)), this, SLOT(find()) );

	connect( workpathComb, SIGNAL(editTextChanged(const QString &)),
			this, SIGNAL(currentWorkspacePath(const QString &)) );	

	connect( filterComb, SIGNAL(currentIndexChanged (int)), this, SLOT(find()) );

	connect( subfoldersCheckBox, SIGNAL(stateChanged(int)), this, SLOT(find()) );

	connect( fileListingTable, SIGNAL(cellDoubleClicked(int, int)),
			this, SIGNAL(fileDoubleClicked(int, int)) );

     	//setWindowTitle(tr("Find Files"));
     	//resize(700, 300);
}

void FileBrowser::browse()
{
	QString path = QFileDialog::getExistingDirectory(this,
			tr("Set workspace path"), QDir::currentPath());
	if (!path.isEmpty()) {
	workpathComb->addItem(path);
	workpathComb->setCurrentIndex(workpathComb->currentIndex() + 1);
	}
}

void FileBrowser::find()
{
	// set the No. of row is 0 for each search
	fileListingTable->setRowCount(0);

	//QString fileName = filterComb->currentText();
	QString filter = filterComb->currentText().left(2);	
	QString path = workpathComb->currentText();
	
	// specify the file filter
	if ( filter.isEmpty() || (filter=="Al") )
         	filter = "*";
	else 	
		filter = "*." + filter;
	
	// get the files or subfolders list in current path
	QDir directory = QDir(path);
	QStringList files;
	// decide whether includes sub-folders
	if( subfoldersCheckBox->checkState()==Qt::Unchecked )
		files = directory.entryList(QStringList(filter),
			QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks );
	else if( subfoldersCheckBox->checkState()==Qt::Checked )
		files = directory.entryList(QStringList(filter),
			QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	
	showFiles(directory, files);
}

// show files in the file table
void FileBrowser::showFiles(const QDir &directory, const QStringList &files)
{
	for (int i=0; i<files.size(); ++i) 
	{
		QFile file(directory.absoluteFilePath(files[i]));

		// get file information
		QFileInfo info(file);
		info.refresh();	// refresh the information

		QString type = info.suffix();
		
		if (!type.isEmpty()) 
			type = type + " File";
		
		if (info.isDir()) 
			type = "Folder";
		
		qint64 size = info.size(); 	
		QDateTime datetime = info.lastModified();
		
		// file name item
		QTableWidgetItem *nameItem = new QTableWidgetItem(files[i]);
		nameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		
		// file type item
		QTableWidgetItem *typeItem = new QTableWidgetItem(type);
		typeItem->setFlags(Qt::ItemIsEnabled);

		// file size item
		QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
                                              .arg(int((size + 1023) / 1024)));
		//sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		sizeItem->setFlags(Qt::ItemIsEnabled);

		// file last modified date & time item
		QTableWidgetItem *dtItem = new QTableWidgetItem(datetime.toString(Qt::ISODate));
		dtItem->setFlags(Qt::ItemIsEnabled);

		int row = fileListingTable->rowCount();
		fileListingTable->insertRow(row);
		fileListingTable->setItem(row, 0, nameItem);
		fileListingTable->setItem(row, 1, typeItem);
		fileListingTable->setItem(row, 2, sizeItem);
		fileListingTable->setItem(row, 3, dtItem  );
	}
	//filesFoundLabel->setText(tr("%1 file(s) found").arg(files.size()));
}


QComboBox *FileBrowser::createWorkpathComb(const QString &text)
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->addItem(text);
	// set it expandable
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}

// create file filter combo box
QComboBox *FileBrowser::createFilterComb()
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(false);
	// add items
	QStringList itemList;
	itemList <<tr("sp file")<<tr("dc file")<<tr("tr file")<<tr("All file");
	comboBox->addItems(itemList);
	// fix the size
	comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	return comboBox;
}

/**
	Create a file listing table with 0 row & 4 columns	
*/	
void FileBrowser::createFileListingTable()
{
	fileListingTable = new QTableWidget(0, 4);
	QStringList labels;
	
	labels << tr("Name") << tr("Type") << tr("Size") << tr("Modified");
	
	fileListingTable->setHorizontalHeaderLabels(labels);
	
	// set first column header to be resizeable
	fileListingTable->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	// hide the vertical header
	fileListingTable->verticalHeader()->hide();
	//fileListingTable->setSortingEnabled(true);
	// hide the grid
	fileListingTable->setShowGrid(false);
}

/*-------------------2007-12-06 Author's addition-----------------------*/
QString FileBrowser::getfileNameFromItem(int row, int column)
{
	if (column != 0)
		return tr("");
	else if (fileListingTable->item (row,1)->text()=="Folder")
		return tr("");
	else {
		QString path = workpathComb->currentText();
		if (path.right(1)!="/")path.append(tr("/"));

		QTableWidgetItem *itemClicked = fileListingTable->item (row, 0);		
		QString fullName = path + itemClicked->text();
		return fullName;
	}
}

