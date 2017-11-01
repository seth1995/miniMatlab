/*************************************************************** 
	QStudio_Main.cpp

	The Qt-Studio Project (since 2017)
 
	Contributor(s): 
	SHI, Guoyong 		(shiguoyong@sjtu.edu.cn) 
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2007 - 2007 

******************************************************************/ 



#include <QApplication>
#include <QSplashScreen>
#include <QtCore/QTimer>
#include <QtGui/QDialog>

#include "MainWindow.h"

using namespace std;


/**
	The main function starting the main user window.
*/
int main(int argc, char *argv[])
{

	QApplication app(argc, argv);	
	//QDialog dialog;

#if 0
	// Create a starting splash display.
	QSplashScreen *splash = new QSplashScreen;
	splash->setPixmap(QPixmap(":/images/Qt_Studio2.png"));
	splash->show();

	Qt::Alignment bottomRight = Qt::AlignRight | Qt::AlignBottom;
	splash->showMessage(QObject::tr("Starting Qt Studio ..."), 
					bottomRight, Qt::white);
#endif

	MainWindow mainWin;
	
	mainWin.show();	

#if 0
	QTimer timer;
	QObject::connect(&timer, SIGNAL(timeout()), &mainWin, SLOT(show()));
	QObject::connect(&timer, SIGNAL(timeout()), &dialog, SLOT(accept()));

	timer.start(3000);  // in milliseconds

	//timer.singleShot(6000, &mainWin, SLOT(show()));

	//while (timer.isActive());
	dialog.exec();
  
	splash->finish(&mainWin);
	delete splash;	
#endif

	return app.exec();
}

