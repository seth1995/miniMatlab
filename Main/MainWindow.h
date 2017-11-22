/***************************************************************
	MainWindow.h

	The Qt-Studio Project (since 2017)

	Contributor(s):
	SHI, Guoyong 		(shiguoyong@sjtu.edu.cn)
	School of Microelectronics, Shanghai Jiao Tong University
	(c) 2007 - 2007

******************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qmainwindow.h>
#include <map>
#include <QScrollArea>

// forward declearation of class
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QToolBar;
class QWorkspace;
class QTreeWidget;
class QDirModel;
class QSplitter;
class QString;
class QTimer;
class QSplashScreen;
//class QGroupBox;
class QPushButton;

class CommandView;
class FileBrowser;
class NN_Interface;
class TextFileEditor;
//class DesignPanel;
//class SchematicEditor;
//class WaveViewer;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	void doRestConstruct();

signals:
	void MessageSignal(const QString &msg);

public slots:
	void newFile();
	void openFile(const QString &fileName);
	void closeFile(QString &filename);


protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void stopTimer();

	void open();
 	void save();
	void saveAs();
	void openRecentFile();
	void cut();
	void copy();
	void paste();
	void setToolbarShow(bool checked);
	void setStatusbarShow(bool checked);
	void setDesignbarShow(bool checked);
	void about();
	void updateMenus();

	void enableDesignAction();
	void enableSchematicAction();
	void enableNeuralNetworkAction();

	// Slots for file tables
	void respondItemClick(int row, int column);
	void setCurWorkspacePath(const QString &path);

	// Slots for cmd window's commands
	void simulate(const QString &cmd);
 	void openCommand(const QString &cmd);
	void LaunchWaveViewer(const QString &path);

	void RunSimulation();

	void startSchematicEditor();
	void startNeuralNetworkEditor();
	void startDesignPanel();
	void startYourProgram();


	void plotDCWaves();
	void plotTRWaves();
	void startWaveViewer();
	void startHaoYuWindow();
	void HaoYuOpen();


private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	//void createCmdWindow();
	//void createFileBrowser();

	TextFileEditor* startTextFileEditor();
	TextFileEditor* getActiveEditor();

	TextFileEditor *ActiveEditor;

	std::map<QString, TextFileEditor*> File_To_Editor_Map;

	void readSettings();
	void writeSettings();
	QPushButton *creatGroupBox();
	bool grassSimulated;

	bool TimerStopped;

	QString workingPath;

//	QString PrevNetlist;
//	QString	CurrNetlist;


	// widget in main window
//	QSplitter *mainSplitter;
//	QSplitter *leftSplitter;
QScrollArea *scrollArea;
	QLabel *readyLabel;
	QLabel *imageLabel;
	QWidgetList windows;
	QWorkspace *myWorkspace;

	FileBrowser *fileBrowser;
	CommandView	*theCmdWindow;

	NN_Interface *nnInterface;

#if 0
	DesignPanel	*theDesignPanel;	// Only one design panel for each design
	SchematicEditor  *schematicEditor;	// only allows one schematic editor running
	//WaveViewer *viewer;
#endif

	// Menu list
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *simuMenu;
	QMenu *plotMenu;  // submenu
	QMenu *toolsMenu;
	QMenu *designMenu;
	QMenu *windowMenu;
	QMenu *helpMenu;

	QMenu *HaoYuFileMenu;
	QMenu *HaoYuRunMenu;
	QMenu *HaoYuShowMenu;
	// Toolbar list
	QToolBar *fileToolBar;
	//QToolBar *editToolBar;
	QToolBar *designToolBar;
	QToolBar *projectToolBar;

	QActionGroup *windowActionGroup;

	// File menu
	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction;

	QAction *openFileAction;
	QAction *runScriptAction;

	// Recent file
	QStringList recentFiles;
	QString curFile;
	enum { MaxRecentFiles = 5 };

	// File actions
	QAction *recentFileActions[MaxRecentFiles];
	QAction *recentfileSepAction;

	// Text Edit actions
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;

	// View actions
	QAction *toolbarviewAction;
	QAction *statusbarviewAction;
	QAction *designbarviewAction;

	// Design & Simulation actions
	QAction *designAction;		// the action launches the Design Panel
	QAction *schematicAction;
	QAction *waveviewAction;
	QAction *simuAction;

	QAction *neuralNetworkAction;

	// Window menu
	QAction *closeAction;
	QAction *closeAllAction;
	QAction *tileAction;
	QAction *cascadeAction;
	QAction *nextAction;
	QAction *previousAction;
	QAction *windowSepAction;

	QAction *aboutAction;
	QAction *aboutQtAction;

	//2017 Fall Doc Course Group
	QAction *ButtonBoxAction;
	//Add Class Action here
	QAction *action_Xie;
	QAction *action_HaoYu;


	QString fileFilters;

	QTimer *qTimer;
	QSplashScreen *qSplash;
};

#endif
