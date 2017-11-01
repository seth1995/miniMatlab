/*************************************************************** 
	CommandView.cpp

	The AnalogDesigner (AD) Project (since 2012)
 
	Contributor(s): 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn) 
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2010 - 2012
******************************************************************/ 

#include <QtGui>
#include <string>
#include <iostream>

#include "CommandView.h"
#include "CmdLineParserUtility.h"
#include "CmdLineParser.hpp"
#include "Macro_Defs.h"

using namespace std;

extern char _CommandLine[];

CommandView::CommandView( QWidget* parent )
	: QTextEdit(parent)
{	
	/*
	append ( tr("Qt-Studio command window.\n"
		    "Type \"man cmd\" for the command details."));
	*/
	append(tr("Type \"help\" to view a list of commands."));
	
	append ( CMD_PROMPT );	
	textContent = toPlainText();
	currentLen = textContent.length();

	QTextDocument *doc = document();
	QTextCursor cursor(doc);
	//cursor.movePosition( QTextCursor::End );
	cursor.setPosition(10);
	setTextCursor(cursor);

	setCursorWidth (2);
	//QTextCursor cursor = textCursor ();
	//setTextCursor(cursor);

	createAction();
	
	connect( this, SIGNAL(cursorPositionChanged()),
            		this, SLOT(setEditable()) ); 
	connect( this, SIGNAL(selectionChanged()),
			this, SLOT(updateContextMenu()) );
	connect( this, SIGNAL(cursorPositionChanged()),
			this, SLOT(updateContextMenu()) );

	parserUtility = new CmdLineParserUtility;
	cmdLineParser = new Cmd::CmdLineParser(parserUtility);
}

CommandView::~CommandView()
{
	delete cmdLineParser;
	cmdLineParser = NULL;

	delete parserUtility;
	parserUtility = NULL;
}

void CommandView::clearCommand()
{
	clear();
}

/*
   create action for context menu
*/
void CommandView::createAction()
{
	// cut action    	
	cutAction = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"),
                            	this);
    	cutAction->setShortcut(tr("Ctrl+X"));
    	cutAction->setStatusTip(tr("Cut the current selection to the "
                               		"clipboard"));
    	connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    	// copy action
	copyAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy"),
                             	this);
    	copyAction->setShortcut(tr("Ctrl+C"));
    	copyAction->setStatusTip(tr("Copy the current selection to the "
                                	"clipboard"));
    	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

	// paste action
    	pasteAction = new QAction(QIcon(":/images/paste.png"), tr("&Paste"),
                              this);
    	pasteAction->setShortcut(tr("Ctrl+V"));
    	pasteAction->setStatusTip(tr("Paste the clipboard's contents at "
                                 	"the cursor position"));
    	connect( pasteAction, SIGNAL(triggered()), this, SLOT(paste()) );

	// select all action
	selectAllAction = new QAction( tr("Select &All"), this );
	selectAllAction->setShortcut( tr("Ctrl+A") );
	selectAllAction->setStatusTip( tr("Select ths entire text") );
	connect( selectAllAction, SIGNAL(triggered()), 
				this, SLOT(selectAll()) );
}

/* 
   overload the context menu event.
   make the user can operate the text with right button of mouse.
   now v1.0 only has Cut,Copy, Paste & Select All menu.
*/ 
void CommandView::contextMenuEvent( QContextMenuEvent * event )
{
	QMenu *menu = createStandardContextMenu();
	menu->clear();	// clear all default menu

	// add menu item
	menu->addAction(cutAction);
	menu->addAction(copyAction);
	menu->addAction(pasteAction);
	menu->addSeparator();
	menu->addAction(selectAllAction);

	// popup the dynamic menu at the pos where right button clicked.
	menu->exec(event->globalPos());		
	
	// must delete the menu to free memory, because this menu will be generated frequently.
	delete menu;	
}

QString CommandView::getCommandLine()
{

	textContent = toPlainText();
	int index = textContent.lastIndexOf(">> ");
	int len = textContent.length()-index-3;
	QString cmd_line = textContent.right(len);

	return cmd_line;
}

/*
   These functions make the command window works like konsole.
   Users can input "help" to search all the commands that it supported.
   If users know the command, but do not know its usage, they can input
   "man command1" for details about "command1".
*/
void CommandView::helpCommand()
{
	append( tr("<font color=blue>Qt Studio command window\n"
		   "<div>Type \"man &lt; command &gt;\" to see the usage of each command.</div>"
		   "<div>Commands:</div>"
		   "<div>-------- clean workspce {-f filename.sp}</div>"
		   "<div>-------- clear </div>"
		   "<div>-------- exit </div>"
		   "<div>-------- open filename1 filename2 ...</div>"
		   "<div>-------- waves [option][filename]</div>"
		   "<div>-------- simulate filename.sp [options]</div>"
		   "<div>-------- version </div></font>"		   		   
		   ) );	
}

/* 
   overload the event-respose function.
   If user input "\n" or "Enter", then process it as command-line window.
   If user didn't input any key, Backspace key is forbidden.
   Else process the text as text editor.
*/ 
void CommandView::keyPressEvent( QKeyEvent *event )
{
	if( event->key() == Qt::Key_Enter ||
		event->key() == Qt::Key_Return ) 
	{ 
		//cout << LINE_INFO << "Enter key pressed." << endl;
		/*
		textContent = toPlainText();
		int index = textContent.lastIndexOf(">> ");
		int len = textContent.length()-index-3;
		
		QString cmd_line = textContent.right(len);
		*/

		QString cmd_line = getCommandLine();
		QString cmd = cmd_line.left(3);  // The lestmost three characters; "man xxx" ==> "man"
		QString str;
		
		if(cmd_line == "clear")
			clearCommand();			
		else if(cmd_line == "help")
			helpCommand();		
		else if(cmd_line == "version")
			versionCommand();
		else if(cmd == "man")
			manCommand(cmd_line);
		else if(cmd_line == "exit") {
			append("<font color=green>quiting ... </font>");
			emit exitCommand();
		}
		else if(cmd_line.left(4) == "open") {
			QStringList list=cmd_line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if(list.count()>1){
				str = "<p><font color=green>" + cmd_line + "</font>";
				append( str );
				emit openfileCommand(cmd_line);				
			}
			else append( tr("<p><font color=red>open: You must sepecify the files "
					"you want to open</font>") );
		}
		else if(cmd_line.left(5) == "waves") {
			QStringList list=cmd_line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if(list.count()>=1){
				str = "<p><font color=green>" + cmd_line + \
				   	"</font><p><font color=green>starting Wave Viewer..." + \
				  	 "<div>......</div><div>......</div></font>";
				append( str );
				emit wavesCommand(cmd_line);				
			}
			else append( tr("<p><font color=red>plot: Bad arguments"
					" for \"waves\" command</font>") );
		}
		else if(cmd_line.left(5)=="clean"){
			QStringList list=cmd_line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if(list.count()>1){
				str = "<p><font color=green>" + cmd_line + \
					"</font><p><font color=green>start cleaning the worksapce...."\
					+ "<div>......</div><div>......</div></font>";
				append( str );
				emit cleanCommand(cmd_line);
				append(tr("<p><font color=green>cleaning worksapce was completed!"
					  "</font>") );
			}
			else append( tr("<p><font color=red>You must sepecify the mode "
						"of cleaning the workspace</font>"));
		}
		else if(cmd_line.left(8)=="simulate") {
			QStringList list=cmd_line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if(list.count()>1){
				str = "<p><font color=green>" + cmd_line + \
				   	"</font><p><font color=green>start simulating..." + \
				   	"<div>......</div><div>......</div></font>";
				append( str);
				emit simulateCommand(cmd_line);				
			}
			else append( tr("<p><font color=red>simulate: You must sepecify the files "
					"you want to simulate</font>") );
		}
		else if(cmd_line.size()) {
#if 0			
			cmd_line = "\n<font color=red>" + cmd_line + ": command not found</font>";
			append( cmd_line );
#endif			
			parseCommandLine();
		}
		append ( CMD_PROMPT );

		// update the text contents and length
		textContent = toPlainText ();
		currentLen = textContent.length();
		//printf("cmd_line length: %d\n",len);  // debug
	}
	else if(currentLen >= toPlainText ().length() )	// user input nothing
	{
		if( event->key() == Qt::Key_Backspace ) ;	// "Backspace"
		else QTextEdit::keyPressEvent(event);	// other keys,use the base class's function
	}
	else 	
		QTextEdit::keyPressEvent(event);		// use the base class's function
}

void CommandView::manCommand(QString cmd)
{
	QStringList list = cmd.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	int num = list.count();
	if(num < 2)
		append( tr("<p><font color=blue>\"man\" usage: </font><font color=green>"
			   "man cmd</font><font color=blue> "
                           "<div>--------------------- Returns usage of \"cmd\"</div></font>"
			   "<p><font color=blue>More info about \"cmd\", type \"help\" in"
			   "konsole.</font>") );
	else {
		if(list[1] == "clean")
			append( tr("<p><font color=blue>\"clean\" usage: <font color=green>"
				   "clean workspace {-f filename.sp}</font>"
				   "<div>----------------------- Clean the workspace, remove all"
				   " simulation-generated files, such as *.lis, *.dc, *.ac, "
				   "*.tr0, and so on.</div>"
				   "<div>----------------------- If optinal arg exists, only "
				   " remove filename.lis, .dc, .ac,.tr0</div></font>") );
		else if(list[1] == "clear")
			append( tr("<p><font color=blue>\"clear\" usage: <font color=green>"
				   "clear</font>"
				   "<div>---------------------- Clear the konsole</div>"
				   "</font>") );
		else if(list[1] == "exit")
			append( tr("<p><font color=blue>\"exit\" usage: <font color=green>"
				   "exit</font>"
				   "<div>-------------------- Terminate Qspice and quit "
				   "</div></font>") );
		else if(list[1] == "open")
			append( tr("<p><font color=blue>\"open\" usage: <font color=green>"
				   "open filename1 filename2 ...</font>"
				   "<div>---------------------- Open files and show them in "
				   " the editor.User can open more than one file once.</div>"
				   "</font>") );
		else if(list[1] == "waves")
			append( tr("<p><font color=blue>\"waves\" usage: <font color=green>"
				   "waves [option][filename]</font>"
				   "<div>--------------------- Open the Wave Viewer."
				   "<div>--------------------- option can be *.dc or *.tr files."
				   "</div>") );
		else if(list[1] == "simulate")
			append( tr("<p><font color=blue>\"simulate\" usage: "
				   "<font color=green>simulate filename.sp [options]</font>"
				   "<div>--------------------------- Simulate the circuit "
				   "netlist file.</div>"
				   "<div>--------------------------- If user do not specify "
				   "the option, 0 is default. Setting option=1 will implement"
				   " Trapezoidal Rule when you do TRAN analysis.</div></font>") );
		else if(list[1] == "version")
			append( tr("<p><font color=blue>\"version\" usage: <font color=green>"
				   "version</font>"
				   "<div>------------------------- Return version of Qspice and "
				   "its updates.</div></font>") );
		else 
			append( tr("<p><font color=red>man: You must specify one of correct commands"
				"<div>-------- Try \"help\" for more correct commands.</dic></font>") );	
	}

}

/*
	Parse the command line.
*/
void CommandView::parseCommandLine()
{
	string text_str;
  	const char *text;

	/* Extract the command line text. */
	QString cmd_line = getCommandLine();

	text_str = cmd_line.toStdString();
	text = text_str.c_str();
	
	//sprintf(_CommandLine, "%s", text);
	strcpy(_CommandLine, text);
	
#ifdef TRACE_
	//qDebug() << LINE_INFO << cmd_line;
	cout << LINE_INFO << cmd_line.toStdString() << endl;
	cout << LINE_INFO << endl;
	printf("\tText is \"%s\"\n", text); 
#endif

#ifdef TRACE_
	cout << LINE_INFO << endl;
	printf("\tCommand line is \"%s\"\n", _CommandLine); 
#endif

	parserUtility->initResultString();

	/**===============================================================**/
	
	cmdLineParser->parse();  // Parse the text in <_CommandLine> 
	
	/**===============================================================**/
#if 0
	if (strlen(parserUtility->CmdLineParseResult) > 0) {
		cout << LINE_INFO << endl;
		printf("parse_result = %s", parserUtility->CmdLineParseResult); 
		fflush(stdout);
		append(QString(parserUtility->CmdLineParseResult));
	}
#endif

	if (! parserUtility->isEmptyResult()) {
		//cout << LINE_INFO << endl;
		//printf("parse_result = %s", parserUtility->CmdLineParseResult); 
		//fflush(stdout);
		append(QString(parserUtility->getResultString().c_str()));
	}

}

/*
   This slot will set the widget to be editable or not.
   When the cursor's position was changed by user,
   this slot will deal with it acoording to cursor's pos.
   If the pos < length of text, user cannot edit the previous text.
   else if pos >= length, user can operate it as text editor.
*/
void CommandView::setEditable()
{
	// current text length	
	int length = toPlainText ().length();

	// get the position of cursor in all text area.
	QTextCursor cur = textCursor();
	int pos = cur.position();

	if( pos<length) {	// uneditable
		//printf("pos: %d uneditable!\n",pos);	// debug
		setReadOnly (true);
	}
	else {		// editable
		//printf("pos: %d editable!\n",pos);	// debug
		setReadOnly (false);
	}
}

void CommandView::showMessage(const QString &msg)
{
	append(msg);
}

/*----------------2007-12-03 author modification start---------------------*/
/*
   when user move cursor or select text, we need to set if the 
   context menu is enable. This is depends on user's operation.
*/
void CommandView::updateContextMenu()
{
	if( isReadOnly() ) {
		cutAction->setEnabled(false);
		pasteAction->setEnabled(false);
	}
	else {
		cutAction->setEnabled( textCursor().hasSelection() );
		pasteAction->setEnabled(true);
	}

	if( textCursor().hasSelection() ) {
		cutAction->setEnabled( !isReadOnly() );	
		copyAction->setEnabled(true);
	}	
	else {
		cutAction->setEnabled(false);
		copyAction->setEnabled(false);
	}
}

void CommandView::versionCommand()
{
	append( tr("<font color=blue>Qt Studio, version 2017 - beta\n"
		   "<div>Development kits:</div>"
		   "<div>------------------- Interface : Qt V4.3 </div>"
		   "<div>------------------- Parser    : PCCTS"
		   "(Purdue Compiler Construction Tool Set) V1.33M</div>"
		   "<div>------------------- Solver    : TNT(Template Numerical Toolkit) V1.26</div>"
		   "<div>Latest update: 2007-12-24</div>") );

}

