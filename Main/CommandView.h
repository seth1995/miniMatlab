/*************************************************************** 
	CommandView.h

	The AnalogDesigner (AD) Project (since 2012)
 
	Contributor(s): 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn) 
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2010 - 2012
******************************************************************/ 

#ifndef COMMAND_VIEW_H
#define COMMAND_VIEW_H

#include <QTextEdit>

#include "CmdLineParser.hpp"

#define	CMD_PROMPT "Studio >> "

class QAction;

//class Cmd::CmdLineParser;
class CmdLineParserUtility;

class	CommandView	: public QTextEdit
{
	Q_OBJECT;

public:
	CommandView( QWidget* parent = 0 );
	~CommandView();

signals:
	void exitCommand();
	void openfileCommand(const QString &cmd);
	void simulateCommand(const QString &cmd);
	void wavesCommand(const QString &cmd);
	void cleanCommand(const QString &cmd);
	
public slots:
	void showMessage(const QString &msg);	

private slots:
	// slot that be used to set the text area if be editable
	void setEditable();
	void updateContextMenu();	

protected:
	// overload the context menu event
	void contextMenuEvent( QContextMenuEvent * event ); 
	// overload the key press event
	void keyPressEvent( QKeyEvent * e );

private:
	void clearCommand();
	void createAction();
	QString getCommandLine();
	void helpCommand();
	void manCommand(QString cmd);
	void parseCommandLine();
	void versionCommand();
	
private:
	Cmd::CmdLineParser *cmdLineParser;
	CmdLineParserUtility *parserUtility;
	
	QString	textContent;  // text contents
	int	currentLen;	// length of text

	QAction *copyAction;
	QAction *cutAction;
	QAction *pasteAction;
	QAction *selectAllAction;

};

#endif
		
	
