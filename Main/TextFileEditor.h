/*************************************************************** 
	TextFileEditor.h

	QT4 GUI for Qt Studio
 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn)
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2007 - 2007 
******************************************************************/ 

/**
	TextFileEditor.h

	Each text file has an editor object associated to it.
*/	

#ifndef TEXT_FILE_EDITOR_H
#define TEXT_FILE_EDITOR_H

#include <QTextEdit>


class TextFileEditor : public QTextEdit
{
	Q_OBJECT

public:
	TextFileEditor(QWidget *parent = 0);

	inline QString&  getFileName() { return  NetlistName; }

	inline bool	Simulated()  { return  netlistSimulated; }
	inline void	SetSimulationFlag(bool flag)  { netlistSimulated = flag; }

	inline bool	isModified()  { return  fileModified; }
	inline void	SetNetlistModification(bool flag)  { fileModified = flag; }
	
	void 	newFile();
	bool 	open(QString path);
	bool 	openFile(const QString &NetlistName);
	bool 	save();
	bool 	saveAs();
	QSize 	sizeHint() const;
	QAction 	*windowMenuAction() const { return editorAction; }

signals:
	void 	netlistClosed(QString &netlistname);
	
protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void netlistModified();

private:
	bool okToContinue();
	bool saveFile(const QString &NetlistName);
	void setCurrentFile(const QString &NetlistName);
	bool readFile(const QString &NetlistName);
	bool writeFile(const QString &NetlistName);
	QString trimmedFileName(const QString &fullFileName);

	QString 	fileFilters;		// for the file filter in a file dialog
	QString 	activeNetlist;	// the currently active netlist
	QString 	NetlistName;
	QString	currNetlistPath;	// keep the last operated file path
	
	QAction 	*editorAction;

	bool 	isUntitled;
	bool		netlistSimulated;   	// flags whether the netlist has been simulated
	bool		fileModified;			// flags whether the netlist has been modified
};

#endif
