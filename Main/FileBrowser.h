/*************************************************************** 
	FileBrowser.h

	The AnalogDesigner (AD) Project (since 2012)
 
	Contributor(s): 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn) 
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2010 - 2012
******************************************************************/ 


#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QDialog>

class QComboBox;
class QDir;
class QLabel;
class QCheckBox;
class QPushButton;
class QTableWidget;

class FileBrowser : public QDialog
{
	Q_OBJECT

public:
	FileBrowser(QWidget *parent = 0);
	QString getfileNameFromItem(int row, int column);
	
signals:
	void fileDoubleClicked(int row, int column);
	void currentWorkspacePath(const QString &path);

private slots:
	void browse();
	void find();

private:
	QStringList findFiles(const QDir &directory, 
				const QStringList &files,
                           	const QString &text);
	void showFiles(const QDir &directory, const 
				QStringList &files);
     	QPushButton *createButton(const QString &text, 
				const char *member);
	QComboBox *createFilterComb();
     	QComboBox *createWorkpathComb(const QString &text = QString());
	
     	void createFileListingTable();

     	QComboBox 	*filterComb;
   	QComboBox 	*workpathComb;
	QCheckBox 	*subfoldersCheckBox;
	QLabel 		*workpathLabel;
	QPushButton 	*browseButton;
 	QTableWidget *fileListingTable;

};

#endif
