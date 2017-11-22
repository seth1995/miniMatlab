/***************************************************************
	MainWindow.cpp

	QT4 GUI for AnalogDesigner (AD)

	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn)
	School of Microelectronics, Shanghai Jiao Tong University
	(c) 2010 - 2012
******************************************************************/

#include <QtGui>
#include <QString>
#include <QSplashScreen>


#include <string>
#include <iostream>
#include <QtWebKit>
#include "Macro_Defs.h"
#include "MainWindow.h"
#include "FileBrowser.h"
#include "CommandView.h"
#include "TextFileEditor.h"
#include "../NeuralNetwork_UI/NeuralNetInterf.h"
#include <QTextCodec>
#include <QProcess>
#if 0
#include "waveviewer.h"
#include "DesignPanel.h"
#include "../QtSchematic/SchematicEditor.h"
#endif

using namespace std;


///int read_netlist(const char *filename);  // defined in "netlist.g"


MainWindow::MainWindow()
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");//情况2
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	grassSimulated = false;

//====================================================================

	TimerStopped = false;

	// Create a starting splash display.
	qSplash = new QSplashScreen;
	qSplash->setPixmap(QPixmap(":/images/Qt_Studio2.png"));
	qSplash->show();

	Qt::Alignment bottomRight = Qt::AlignRight | Qt::AlignBottom;
	qSplash->showMessage(QObject::tr("Starting Qt Studio ..."),
					bottomRight, Qt::white);

	qTimer = new QTimer;
	QObject::connect(qTimer, SIGNAL(timeout()), this, SLOT(stopTimer()));

	qTimer->start(2000);  // in milliseconds

}

void MainWindow::doRestConstruct()
{

//====================================================================

	workingPath = ".";		// file path default to "."

	fileBrowser = new FileBrowser;

	fileFilters = tr("Files (*.sp)\n"  "All files (*)");

	myWorkspace = new QWorkspace;
	connect(myWorkspace, SIGNAL(windowActivated(QWidget *)), this, SLOT(updateMenus()));

	theCmdWindow = new CommandView;

	// Let the left splitter contain the file browser & myWorkspace.
	QSplitter  *horizontalSplitter = new QSplitter(Qt::Horizontal);
	horizontalSplitter->addWidget(fileBrowser);
	horizontalSplitter->addWidget(theCmdWindow);
	//horizontalSplitter->addWidget(myWorkspace);
	horizontalSplitter->setStretchFactor(1, 1);

	// Let the main splitter contains the horizontal splitter & command window.
	QSplitter  *mainSplitter = new QSplitter(Qt::Vertical);
	mainSplitter->addWidget(horizontalSplitter);
	mainSplitter->addWidget(myWorkspace);
	//mainSplitter->addWidget(theCmdWindow);
	mainSplitter->setStretchFactor(1, 1);

	setCentralWidget(mainSplitter);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();


	// connect fileBrowser signal
	connect( fileBrowser, SIGNAL(fileDoubleClicked(int, int)),
			this, SLOT(respondItemClick(int,int)) );
	connect( fileBrowser, SIGNAL(currentWorkspacePath(const QString &)),
			this, SLOT(setCurWorkspacePath(const QString &)) );

	// connect console commands with operation of main wondow
	connect( theCmdWindow, SIGNAL(exitCommand()), this, SLOT(close()) );

	connect( theCmdWindow, SIGNAL(simulateCommand(const QString &)),
			this, SLOT(simulate(const QString &)) );
	connect( theCmdWindow, SIGNAL(wavesCommand(const QString &)),
			this, SLOT(LaunchWaveViewer(const QString &)) );
	connect( theCmdWindow, SIGNAL(openfileCommand(const QString &)),
			this, SLOT(openCommand(const QString &)) );
	connect( this, SIGNAL(MessageSignal(const QString &)),
			theCmdWindow, SLOT(showMessage(const QString &)) );

	setWindowTitle(tr("Qt Studio 2017"));

	resize(900, 600);
	setWindowIcon(QPixmap(":/images/Qt_Studio.png"));

#if 0
	theDesignPanel = NULL;
	schematicEditor = NULL;
#endif

	nnInterface = NULL;

}


void MainWindow::stopTimer()
{
	TimerStopped = true;

	qSplash->finish(this);
	delete qSplash;
	delete qTimer;

	doRestConstruct();

}

/*
   Pupop a message box to show something about this software.
*/
void MainWindow::about()
{
    QMessageBox::about(this, tr("About the Qt Studio"),
            		tr("<h2>QStudio 2017 </h2>"
                	"<p>A graphical user interface program"
              	"<p>Developed by the EDA Research Laboratory"
              	"<p>Shanghai Jiao Tong University"
	      		 "<div>Copyright (c) 2007-2007</div>"));
}


void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef TRACE_
	cout << LINE_INFO << endl << flush;
#endif
	UNUSED_VAR(event);

	if (theCmdWindow) {
		delete theCmdWindow;
		theCmdWindow = NULL;
	}

#if 0
	if (schematicEditor) {
		schematicEditor->close();
	}

	if (getActiveEditor()) {
    	event->ignore();
	}
	else {
		event->accept();
		printf("\n\tThank you for using Analog Designer. Goodbye!\n");
	}
#endif

	myWorkspace->closeAllWindows();
}

/**
	Remove the file name from the file-to-editor map.
*/
void MainWindow::closeFile(QString &filename)
{
	UNUSED_VAR(filename);

#if 1
	File_To_Editor_Map.erase(filename);
#endif
}


void MainWindow::createActions()
{
	// New file action
	newAction = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
	newAction->setShortcut(tr("Ctrl+N"));
	newAction->setStatusTip(tr("Create a new file"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	// Open file action
	openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openAction->setShortcut(tr("Ctrl+O"));
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	// Save file action
	saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAction->setShortcut(tr("Ctrl+S"));
	saveAction->setStatusTip(tr("Save the file to disk"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	// Save as action
	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setStatusTip(tr("Save the file under a new name"));
	saveAction->setEnabled(false);
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	// Separator action, between recent files and save as
	recentfileSepAction = new QAction(this);
	recentfileSepAction->setSeparator(true);

	// Recent file action
	for (int i=0; i < MaxRecentFiles; i++)
	{
		recentFileActions[i] = new QAction(this);
		recentFileActions[i]->setVisible(false);
		connect( recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()) );
	}

	// Exit action
	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	// Cut text action
	cutAction = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
	cutAction->setShortcut(tr("Ctrl+X"));
	cutAction->setStatusTip(tr("Cut the current selection to the "
	                       		"clipboard"));
	cutAction->setEnabled(false);
	connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

	// Copy text action
	copyAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
	copyAction->setShortcut(tr("Ctrl+C"));
	copyAction->setStatusTip(tr("Copy the current selection to clipboard"));
	copyAction->setEnabled(false);
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

	// Paste test action
	pasteAction = new QAction(QIcon(":/images/paste.png"), tr("&Paste"),
	                      this);
	pasteAction->setShortcut(tr("Ctrl+V"));
	pasteAction->setStatusTip(tr("Paste the clipboard contents at the cursor"));
	pasteAction->setEnabled(false);
	connect( pasteAction, SIGNAL(triggered()), this, SLOT(paste()) );

	// View toolbar action
	toolbarviewAction = new QAction(QIcon(":/images/cross.ico"), tr("&ToolBar"), this );
	toolbarviewAction->setCheckable(true);
	toolbarviewAction->setChecked(true);
	toolbarviewAction->setStatusTip( tr("Show or hide the toolbar") );
	connect( toolbarviewAction, SIGNAL(toggled(bool)),
				this, SLOT(setToolbarShow(bool)) );

	// View statusbar action
	statusbarviewAction = new QAction(QIcon(":/images/cross.ico"), tr("&StatusBar"), this );
	statusbarviewAction->setCheckable(true);
	statusbarviewAction->setChecked(true);
	statusbarviewAction->setStatusTip( tr("Show or hide the status toolbar") );
	connect( statusbarviewAction, SIGNAL(toggled(bool)),
				this, SLOT(setStatusbarShow(bool)) );

   	// View design bar action
  	designbarviewAction = new QAction(QIcon(":/images/cross.ico"), tr("&Designbar"), this );
	designbarviewAction->setCheckable(true);
	designbarviewAction->setChecked(true);
	designbarviewAction->setStatusTip( tr("Show or hide the design toolbar") );
	connect( designbarviewAction, SIGNAL(toggled(bool)),
				this, SLOT(setDesignbarShow(bool)) );

	//--------- Design & Simulation actions -----------------------------------

	// Schematic editor action ==============
	schematicAction = new QAction(QIcon(":/images/schematic.png"), tr("&Schematic Editor"), this);
	schematicAction->setShortcut( tr("Ctrl+S") );
	schematicAction->setStatusTip( tr("Lauch the schematic editor") );
	connect( schematicAction, SIGNAL(triggered()), this, SLOT(startSchematicEditor()) );

	// Neural Network Design action ==============
	neuralNetworkAction = new QAction(QIcon(":/images/neural_network.png"),
						tr("&Neural network interface"), this);
	neuralNetworkAction->setShortcut( tr("Ctrl+N") );
	neuralNetworkAction->setStatusTip( tr("Lauch the neural network editor") );
	connect( neuralNetworkAction, SIGNAL(triggered()), this,
						SLOT(startNeuralNetworkEditor()) );

	// Waveviewer action ==============
	waveviewAction = new QAction(QIcon(":/images/waveviewer.ico"), tr("&View Waves"), this);
	waveviewAction->setShortcut( tr("Ctrl+W") );
	waveviewAction->setStatusTip( tr("Open the wave viewer") );
	connect( waveviewAction, SIGNAL(triggered()), this, SLOT(startWaveViewer()) );

	// Symbolic simulate action =================
	simuAction = new QAction(QIcon(":/images/simulate.ico"), tr("&Run Simulation"), this);
	simuAction->setShortcut( tr("F5") );
	simuAction->setStatusTip( tr("Simulate the active netlist") );
	//simuAction->setCheckable(true);
	//simuAction->setChecked(false);
	simuAction->setEnabled(false);
	connect( simuAction, SIGNAL(triggered()), this, SLOT(RunSimulation()) );

	// Design Panel action ====================
	designAction = new QAction(QIcon(":/images/design.png"), tr("Start &Design Panel"), this);
	designAction->setShortcut( tr("Ctrl+D") );
	designAction->setStatusTip( tr("Start design panel") );
	//designAction->setCheckable(true);
	//designAction->setChecked(false);
	designAction->setEnabled(false);
	connect( designAction, SIGNAL(triggered()), this, SLOT(startDesignPanel()) );
	//----------------------------------------------------------------------------

	// Close actived editor action
	closeAction = new QAction(tr("Cl&ose"), this);
	closeAction->setShortcut(tr("Ctrl+F4"));
	closeAction->setStatusTip(tr("Close the active window"));
	connect( closeAction, SIGNAL(triggered()), myWorkspace, SLOT(closeActiveWindow()) );

	// Cose all editor action
	closeAllAction = new QAction(tr("Close &All"), this);
	closeAllAction->setStatusTip(tr("Close all the windows"));
	connect( closeAllAction, SIGNAL(triggered()),
		myWorkspace, SLOT(closeAllWindows()) );

	// Tile windows action
	tileAction = new QAction(tr("&Tile"), this);
	tileAction->setStatusTip(tr("Tile the windows"));
	connect( tileAction, SIGNAL(triggered()), myWorkspace, SLOT(tile()) );

	// Cascade windows action
	cascadeAction = new QAction(tr("&Cascade"), this);
	cascadeAction->setStatusTip(tr("Cascade the windows"));
	connect( cascadeAction, SIGNAL(triggered()),
		myWorkspace, SLOT(cascade()) );

	// Next window action
	nextAction = new QAction(tr("Ne&xt"), this);
	nextAction->setShortcut(tr("Ctrl+F6"));
	nextAction->setStatusTip(tr("Move the focus to the next window"));
	connect( nextAction, SIGNAL(triggered()),
			myWorkspace, SLOT(activateNextWindow()) );

	// Previous window action
	previousAction = new QAction(tr("Pre&vious"), this);
	previousAction->setShortcut(tr("Ctrl+Shift+F6"));
	previousAction->setStatusTip(tr("Move the focus to the previous "
	                            	"window"));
	connect( previousAction, SIGNAL(triggered()),
	    	myWorkspace, SLOT(activatePreviousWindow()) );

	// Separator action, between window list and previous action
	windowSepAction = new QAction(this);
	windowSepAction->setSeparator(true);

	// about action, popup an about message box
	aboutAction = new QAction(QIcon(":/images/help.png"),tr("&About Analog Designer ..."), this);
	aboutAction->setStatusTip(tr("About Analog Designer ..."));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	// about Qt action, popup an about dialog
	//	aboutQtAction = new QAction(tr("About &Qt"), this);
	//	aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
	//	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    	windowActionGroup = new QActionGroup(this);

	//Creat action group of student
	ButtonBoxAction = new QAction(QIcon(":/images/Classroom.png"), tr("&BottonBox"), this);
	ButtonBoxAction ->setStatusTip( tr("2017_Fall_DocCourse") );
	//connect( ButtonBoxAction, SIGNAL(triggered()), this, SLOT(CourseButtonBox()) );
}

#if 1
TextFileEditor *MainWindow::startTextFileEditor()
{
    	TextFileEditor *editor = new TextFileEditor;

    	connect(editor, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
    	connect(editor, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));

    	myWorkspace->addWindow(editor);
    	windowMenu->addAction(editor->windowMenuAction());
    	windowActionGroup->addAction(editor->windowMenuAction());

    	return editor;
}
#endif

void MainWindow::createMenus()
{
	// File menu
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addAction(recentfileSepAction);

	//recentfileSepAction = fileMenu->addSeparator();
	for (int i=0; i<MaxRecentFiles; ++i)
		fileMenu->addAction(recentFileActions[i]);

	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	// Edit menu
	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);

	// View menu
	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(toolbarviewAction);
	viewMenu->addAction(statusbarviewAction);
	viewMenu->addAction(designbarviewAction);

	// Simulation menu
	simuMenu = menuBar()->addMenu(tr("&Simulate"));
	simuMenu->addAction(simuAction);

	// Tools menu
	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(schematicAction);
	toolsMenu->addAction(neuralNetworkAction);
	toolsMenu->addAction(waveviewAction);

	designMenu = menuBar()->addMenu(tr("&Design"));
	designMenu->addAction(designAction);

	// Window menu
	windowMenu = menuBar()->addMenu(tr("&Window"));
	windowMenu->addAction(closeAction);
	windowMenu->addAction(closeAllAction);
	windowMenu->addSeparator();
	windowMenu->addAction(tileAction);
	windowMenu->addAction(cascadeAction);
	windowMenu->addSeparator();
	windowMenu->addAction(nextAction);
	windowMenu->addAction(previousAction);
	windowMenu->addAction(windowSepAction);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	//    	helpMenu->addAction(aboutQtAction);
}

void MainWindow::createStatusBar()
{
    	readyLabel = new QLabel(tr(" Ready"));
    	statusBar()->addWidget(readyLabel, 1);
}

void MainWindow::createToolBars()
{
	// File toolbar
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	fileToolBar->addSeparator();
	fileToolBar->addAction(cutAction);
	fileToolBar->addAction(copyAction);
	fileToolBar->addAction(pasteAction);

	// Add actions to the Design toolbar
	designToolBar = addToolBar(tr("Design"));
	designToolBar->addAction(schematicAction);
	designToolBar->addAction(neuralNetworkAction);
	designToolBar->addAction(waveviewAction);
	designToolBar->addAction(simuAction);
	designToolBar->addAction(designAction);
	//designToolBar->addAction(ButtonBoxAcgrouption);

	// Add Course action  to toolbar
	projectToolBar = addToolBar(tr("Project"));
	projectToolBar -> addWidget(creatGroupBox());

	//	designAction->setEnabled(false);

}

QPushButton *MainWindow::creatGroupBox()
{
	//QGroupBox *groupBox = new QGroupBox(tr("&Push Buttons"));
	QPushButton *popupButton = new QPushButton(QIcon(":/images/Classroom.png"), tr("&CourseProject"), this);
    QMenu *menu = new QMenu(this);
    action_Xie = menu->addAction(tr("&1 谢起旭"));
    menu->addAction(tr("&2 吴赟韬"));
    menu->addAction(tr("&3 张宇航"));
	menu->addAction(tr("&4 甄沛宁"));
	menu->addAction(tr("&5 张异凡"));
	menu->addAction(tr("&6 赵永磊"));
	action_HaoYu=menu->addAction(tr("&7 Hao Yu"));
	menu->addAction(tr("&8 陈佳鸣"));
	menu->addAction(tr("&9 杨科"));
	menu->addAction(tr("&10 郭人颂"));
	menu->addAction(tr("&11 齐一衡"));
	menu->addAction(tr("&12 梁威猛"));
	menu->addAction(tr("&13 章伦"));
	menu->addAction(tr("&14 贲月晶"));
	menu->addAction(tr("&15 张灏"));

	//set personal signal here
	connect(action_Xie, SIGNAL(triggered()), this, SLOT(startYourProgram()) );
	connect(action_HaoYu, SIGNAL(triggered()), this, SLOT(startHaoYuWindow()) );
	popupButton->setMenu(menu);

	/*
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox -> addWidget(popupButton);
	vbox -> addStretch(1);
	groupBox -> setLayout(vbox);
	*/
	return popupButton;

};

void MainWindow::startYourProgram()
{

	QMainWindow *tempWidget = new QMainWindow(this);

	tempWidget->show();
	tempWidget->setWindowTitle(tr("Testing ..."));

}
void MainWindow::startHaoYuWindow()
{
	QMainWindow *myWidget = new QMainWindow(this);

	imageLabel = new QLabel;
   imageLabel->setBackgroundRole(QPalette::Base);
   imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
   imageLabel->setScaledContents(true);

   scrollArea = new QScrollArea;
   scrollArea->setBackgroundRole(QPalette::Dark);
   scrollArea->setWidget(imageLabel);
   myWidget->setCentralWidget(scrollArea);
	// QWebView *view = new QWebView(myWidget);
	// view->resize( QSize( 800,600 ));
  //   view->load(QUrl("file:///code/R/temp.html"));
  //  QProcess *process=new QProcess;
	//process->start("Rscript", QStringList() << "/code/R/test.R"<<"/code/R/gifts.csv");
	//cout<<"exit code: "<<process->exitCode()<<endl;
	//QDeclarativeView *qmlView = new QDeclarativeView;
	//qmlView->setSource(QUrl::fromLocalFile("../HaoYu/HaoYuProject/HaoYuProject.qml"));
	//action
	openFileAction = new QAction(QIcon(":/images/cut.png"), tr("Open"), myWidget);
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(HaoYuOpen()));
	//
	runScriptAction = new QAction(QIcon(":/images/cut.png"), tr("Run"), myWidget);
	//connect(runScriptAction, SIGNAL(triggered()), this, SLOT());
	//end of action
	//UI part

	HaoYuFileMenu=myWidget->menuBar()->addMenu(tr("&File"));
	HaoYuFileMenu->addAction(openFileAction);
	HaoYuRunMenu=myWidget->menuBar()->addMenu(tr("&Run"));
	HaoYuRunMenu->addAction(runScriptAction);
	HaoYuShowMenu=myWidget->menuBar()->addMenu(tr("&Show"));


	myWidget->menuBar()->addSeparator();

	//end of UI

	QVBoxLayout *layout = new QVBoxLayout(myWidget);
	//layout->addWidget(view);

	myWidget->show();
	myWidget->resize(1024,800);
	myWidget->setWindowTitle(tr("Hao Yu"));
}
void MainWindow::HaoYuOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this,
															tr("Open File"), QDir::currentPath());
if (!fileName.isEmpty()) {
	QImage image(fileName);
	if (image.isNull()) {
			QMessageBox::information(this, tr("Image Viewer"),
															 tr("Cannot load %1.").arg(fileName));
			return;
	}
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->adjustSize();
	cout<<fileName.toStdString()<<endl;

	// scaleFactor = 1.0;
  //
	// printAct->setEnabled(true);
	// fitToWindowAct->setEnabled(true);
	// updateActions();
  //
	// if (!fitToWindowAct->isChecked())
	// 		imageLabel->adjustSize();
}
}

/*
   If user wanna cut some text, it must has at least one actived editor.
*/
void MainWindow::cut()
{
#if 1
    if (getActiveEditor())
        getActiveEditor()->cut();
#endif
}

/*
   If user wanna copy some text, it must has at least one actived editor.
*/
void MainWindow::copy()
{
#if 1
    if (getActiveEditor())
        getActiveEditor()->copy();
#endif
}

/*
   If user wanna paste text to a doc, it must has at least one actived editor.
*/
void MainWindow::paste()
{
#if 1
    if (getActiveEditor())
        getActiveEditor()->paste();
#endif
}

/**
	This function (as a SLOT) responds to the signal emitted by closing the
	design panel. Note that when the design panel is closed, it's object is
	not destructed. Hence, we explicitly destruct the design panel before
	resetting the design action.
*/
void MainWindow::enableDesignAction()
{
#if 0
	delete theDesignPanel;
	theDesignPanel = NULL;

	designAction->setEnabled(true);		// enable the "Design" action.
#endif
}

/**
	This slot is activated when the schematic editor is closed.
*/
void MainWindow::enableSchematicAction()
{
#if 0
	delete schematicEditor;
	schematicEditor = NULL;

	schematicAction->setEnabled(true);		// enable the schematic action.
#endif
}

/**
	This slot is activated when the schematic editor is closed.
*/
void MainWindow::enableNeuralNetworkAction()
{
#if 0
	delete schematicEditor;
	schematicEditor = NULL;
#endif

	neuralNetworkAction->setEnabled(true);		// enable the schematic action.
}

TextFileEditor *MainWindow::getActiveEditor()
{
	return qobject_cast<TextFileEditor *>(myWorkspace->activeWindow());
}

void MainWindow::newFile()
{
	cout << LINE_INFO << endl;

#if 1
	TextFileEditor *editor = startTextFileEditor();
	editor->newFile();
	editor->show();
#endif

}


void MainWindow::openFile(const QString &fileName)
{
	UNUSED_VAR(fileName);

#if 1
	TextFileEditor *editor = startTextFileEditor();

	if (editor->openFile(fileName)) {
		editor->show();
	}
	else {
		editor->close();
	}
#endif
}

/*
	This function is incomplete.
*/
void MainWindow::openRecentFile()
{
#if 0
	TextFileEditor *editor = startTextFileEditor();

	if (editor->open()) {
    	editor->show();
	}
	else {
    	editor->close();
	}
#endif
}


/**
  	The file name obtained from QFileDialog contains the full path.
*/
void MainWindow::open()
{
#if 1
	//cout << LINE_INFO << endl;

	QString filename = QFileDialog::getOpenFileName(this, tr("Open file"),
						workingPath, fileFilters);

	workingPath = QFileInfo(filename).path();	// keep the current file path

	//cout << LINE_INFO << "\n\tworkingPath = " << workingPath.toStdString() << endl;

	if (QFileInfo(filename).fileName().isEmpty())
		return;

	TextFileEditor *editor;

	map<QString, TextFileEditor*>::const_iterator  it;
	it = File_To_Editor_Map.find(filename);

	if (it != File_To_Editor_Map.end()) {
		// Found the editor
		editor = it->second;
		editor->show();
		if (editor->Simulated()) {
			simuAction->setEnabled(false);  // second simulation is not allowed
		} else {
			simuAction->setEnabled(true);
		}
	}
	else {
		// Did not find the editor
	    	editor = startTextFileEditor();
		connect(editor, SIGNAL(netlistClosed(QString &)), this,
						SLOT(closeFile(QString &)) );

		File_To_Editor_Map.insert(make_pair(filename, editor));

		editor->openFile(filename);
		editor->show();

		simuAction->setEnabled(true);	// Enable the simulation action button/menu
	}

//	cout << LINE_INFO << "\n\tfilename = " << filename.toStdString() << endl;
#endif
}

/*
   If user wanna svae a doc, it must has at least one actived editor.
*/
void MainWindow::save()
{
#if 1
	if (getActiveEditor())
    	getActiveEditor()->save();

	simuAction->setEnabled(true);		// enable the "Design" action.

//	ActiveEditor->SetSimulatedFlag(false);  // this netlist has not been simulated.
#endif
}

/*
   If user wanna svae a doc as, it must has at least one actived editor.
*/
void MainWindow::saveAs()
{
#if 1
    if (getActiveEditor())
        getActiveEditor()->saveAs();

	simuAction->setEnabled(true);		// enable the "Design" action.
#endif
}

void MainWindow::setToolbarShow(bool checked)
{
	fileToolBar->setVisible(checked);
}

void MainWindow::setStatusbarShow(bool checked)
{
	statusBar()->setVisible(checked);
}

void MainWindow::setDesignbarShow(bool checked)
{
	designToolBar->setVisible(checked);
}


void MainWindow::simulate(const QString &cmd)
{
	QStringList list = cmd.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	QString fullPath;

	if ( workingPath.right(1) != "/" )
		fullPath = workingPath + "/" + list[1];
	else
		fullPath = workingPath + list[1];

	if ( !QFile::exists(fullPath) ) {
		QString info = "Cannot find file " + fullPath;
		QMessageBox::critical(this, tr("Open Error"), info);
	}
	else
	{
		std::string spFile = fullPath.toStdString();

		/*
		int type = 0;

		if (list.count() >= 3)
			type = list[2].toInt();
		*/

		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#if 0
		///if (read_netlist( spFile.c_str() )==0)
		if (GPDDrunner->Run_GPDD( spFile.c_str() ) == OK)
		{
			double time_elapsed = 0.0;   // timeQ.read();
			QString msg;
 				QTextStream(&msg)<<"<p><font color=green>"
					 <<"Simulation was completed successfully!"<<endl
					 <<"<div>Elapsed time is "<<time_elapsed
					 <<"</div></font>";
			emit MessageSignal(msg);
			QApplication::restoreOverrideCursor();
			printf("\nSimulation was completed successfully!\n");
			printf("Elapsed time is %f\n",time_elapsed);
		}
		else {
			QString msg = "<p><font color=red> Simulation Error!</font></p>";
			emit MessageSignal(msg);
		}
#endif
	}
}


void MainWindow::openCommand(const QString &cmd)
{
	cout << LINE_INFO << endl;

	QStringList list = cmd.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	int num = list.count();
	QString filelist;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	if ( num>1 ) {
		for( int i=1; i<num; i++) {
			//QMessageBox::information(this, tr("Open"), list[i]);
			openFile(list[i]);
			filelist = filelist + " " + list[i];
		}
	}
	QApplication::restoreOverrideCursor();
	QString msg;
	QTextStream(&msg) << "<p><font color=green>"
			 << "Open file " << filelist << " successfully!</font>";
	emit MessageSignal(msg);
}

/*
	This function is called when the "simulation" button/menu is clicked.
	Each netlist is simulated only ONCE.
	Before starting another netlist simulation, all previously simulated results must be cleaned.
*/
void MainWindow::RunSimulation()
{
	cout << LINE_INFO << endl;

#if 0
	QString Qfilename = getActiveEditor()->getFileName();
	string filename = Qfilename.toStdString();

//	cout << LINE_INFO << "\n\tNetlist file is : " << filename.toStdString() << endl;

	if (GPDDrunner->Run_GPDD(filename) == OK)
	{
		grassSimulated = true;		// simulation finished
		designAction->setEnabled(true);
		simuAction->setEnabled(false);  // disable the simulation button (forbid repeated simulation!)
		ActiveEditor->SetSimulationFlag(true);  // this netlist has been simulated.

		QString msg;
		QTextStream(&msg) << "<p><font color=green>"
			  << "The netlist has been simulated successfully.\n"
			  << "Click the <B>Design</B> icon \"D\" to start the design panel."
			  << "</font></p>";

		QMessageBox::warning(this, tr("Simulation Successful"), msg);
	}
	else {
		QString message = "Failed to simulate the netlist \"" + Qfilename + "\".\n" + \
				"Check the message in the console.\n";
		QMessageBox::warning(this, tr("Simulation Failed"), message);
	}
#endif
}


void MainWindow::plotDCWaves()
{
	cout << LINE_INFO << endl;
#if 0
	QString filename;

	if (getActiveEditor()) {
		filename = getActiveEditor()->windowTitle();
	}

	if ( filename.isEmpty() ) {
		QString info = "Nothing to plot\n";
		QMessageBox::warning(this, tr("Plot Error"), info);
		return;
	}

	filename = filename.left( filename.length()-3 );

	if (filename.right(3)!=".sp"){
		QString info = "The current actived file " + filename + \
				" is not netlist file(*.sp).\n" + \
				"You must active the sp file you want to plot.\n" + \
				"Or implement plot in the konsole.\n" + \
				"Command like this: plot -dc/tr filename.sp\n";
		QMessageBox::warning(this, tr("DC Plot Error"), info);
	}
	else {
		// *.sp -> *.dc
		filename = filename.replace( filename.length()-2, 2, "dc" );
		QString cmd = "waves " + filename;
		LaunchWaveViewer(cmd);
    }
#endif
}

void MainWindow::plotTRWaves()
{
#if 0
	QString filename = getActiveEditor()->windowTitle();
	filename = filename.left( filename.length()-3 );
	if(filename.right(3)!=".sp"){
		QString info = "The current actived file " + filename + \
				" is not netlist file(*.sp).\n" + \
				"You must active the sp file you want to plot.\n" + \
				"Or implement plot in the konsole.\n" + \
				"Command like this: plot -dc/tr filename.sp\n";
		QMessageBox::warning(this, tr("TRAN Plot Error"), info);
	}
	else {
		// *.sp -> *.tr
		filename = filename.replace( filename.length()-2, 2, "tr" );
		QString cmd = "waves " + filename;
		LaunchWaveViewer(cmd);
	     }
#endif
}

/*------------------ Author's Addition-----------------------*/
void MainWindow::respondItemClick(int row, int column)
{
	QString fileName = fileBrowser->getfileNameFromItem(row, column);

	if (!fileName.isEmpty())
		openFile(fileName);
}

void MainWindow::startWaveViewer()
{
#if 0
	WaveViewer *viewer = new WaveViewer;

	viewer->resize(600, 400);
	viewer->show();
#endif
}

void MainWindow::LaunchWaveViewer(const QString &cmd)
{
	cout << LINE_INFO << endl;

	UNUSED_VAR(cmd);

#if 0
	QStringList list = cmd.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	QString dataFile = NULL;

	if (list.count()==2)
	{
		dataFile = list[1];
		if(!workingPath.isEmpty()) {
			if( workingPath.right(1)!="/" )
				dataFile = workingPath + "/" + dataFile;
			else
				dataFile = workingPath + dataFile;
		}
	}

	if (dataFile!=NULL)
	{
		if( !QFile::exists(dataFile) )
		{
			QString info = "Cann't found file " + dataFile;
			QMessageBox::critical(this, tr("Open Error"), info);
		}
		else
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			WaveViewer *viewer;
			viewer = new WaveViewer(dataFile, this);
			viewer->resize(600, 400);
			viewer->show();

			QString msg;
			QTextStream(&msg) << "<p><font color=green>"
				  << "Wave Viewer started successfully!"
				  << "<div>" << dataFile << " was opened successfully!"
				  << "</div></font>";
			emit MessageSignal(msg);

			QApplication::restoreOverrideCursor();
		}
	}
#endif
}

void MainWindow::readSettings()
{

}

/*
	Only one neural network interface instance is created in this implementation.
*/
void MainWindow::startNeuralNetworkEditor()
{
#ifdef TRACE
	cout << LINE_INFO << endl;
#endif

	/* By inheriting "this", the QWidget instance will be closed when
	the MainWindow is closed. */

	if (! nnInterface) {
		nnInterface = new NN_Interface(this);
	    nnInterface->setGeometry(100, 40, 600, 600);
		connect(nnInterface, SIGNAL(NN_InterfaceClosed()),
						this, SLOT(enableNeuralNetworkAction()) );
	}

	neuralNetworkAction->setEnabled(false);	// disable the action button

    nnInterface->show();
}


/**
	Multiple schematic editors are allowed.
*/
void MainWindow::startSchematicEditor()
{
#ifdef TRACE
	cout << LINE_INFO << endl;
#endif

#if 0
	if (schematicEditor) {
		schematicEditor->close();
		delete schematicEditor;
		// Only one design panel is allowed.
	}

	// By inheriting "this", the schematic editor will be closed when the MainWindow is closed.
	schematicEditor = new SchematicEditor(this);

	// Connect signals from the schematic editor:
	connect(schematicEditor, SIGNAL(SchematicEditorClosed()),
					this, SLOT(EnableSchematicAction()) );
	connect(schematicEditor, SIGNAL(SpiceSimRequested()),
					this, SLOT(RunSimulation()) );
	connect(schematicEditor, SIGNAL(SymbolicSimRequested()),
					this, SLOT(RunSimulation()) );
	connect(schematicEditor, SIGNAL(DesignPanelRequested()),
					this, SLOT(startDesignPanel()) );

	schematicEditor->setGeometry(100, 20, 1000, 700);
	schematicEditor->show();
	schematicAction->setEnabled(false);	// disable the "Schematic" action
#endif
}

void MainWindow::startDesignPanel()
{
	//cout << LINE_INFO << endl;
#if 0
	if (theDesignPanel) {
		theDesignPanel->close();
		delete theDesignPanel;
		// Only one design panel is allowed.
	}

	// By inheriting "this", the DesignPanel will be closed when the MainWindow is closed.
	theDesignPanel = new DesignPanel(this);
	connect( theDesignPanel, SIGNAL(DesignPanelClosed()), this, SLOT(EnableDesignAction()) );

	theDesignPanel ->resize(900, 600);
	theDesignPanel ->show();

	QString msg;
	QTextStream(&msg) << "<p><font color=green>"
			  << "Click the <B>PLOT</B> icon to plot the frequency response."
			  << "</font></p>";
	QMessageBox::warning(this, tr("AC Analysis Done"), msg);

	designAction->setEnabled(false);		// disable the "Design" action.
#endif
}


void MainWindow::writeSettings()
{

}

void MainWindow::setCurWorkspacePath(const QString &path)
{
	cout << LINE_INFO << endl;

	workingPath = path;
}


/**
	Most menu options only make sense if there is an active window,
	so we disable them if there isn't one.
	At the end, we call setChecked() on the QAction representing the
	active window.
	Thanks to the QActionGroup, we don't need to explicitly uncheck
	the previously active window.
*/
void MainWindow::updateMenus()
{
#if 0
	bool hasEditor = (getActiveEditor() != 0);
	bool hasSelection = getActiveEditor()
	                && getActiveEditor()->textCursor().hasSelection();

	saveAction->setEnabled(hasEditor);
	saveAsAction->setEnabled(hasEditor);
	pasteAction->setEnabled(hasEditor);

	cutAction->setEnabled(hasSelection);
	copyAction->setEnabled(hasSelection);

//	designAction->setEnabled(grassSimulated);

	closeAction->setEnabled(hasEditor);
	closeAllAction->setEnabled(hasEditor);
	tileAction->setEnabled(hasEditor);
	cascadeAction->setEnabled(hasEditor);
	nextAction->setEnabled(hasEditor);
	previousAction->setEnabled(hasEditor);
	windowSepAction->setVisible(hasEditor);

	if ( (ActiveEditor = getActiveEditor()) ) {
		ActiveEditor->windowMenuAction()->setChecked(true);
		simuAction->setEnabled( ! (ActiveEditor->Simulated()) );
	}
#endif
}
