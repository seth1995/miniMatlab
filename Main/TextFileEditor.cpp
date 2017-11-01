/*************************************************************** 
	TextFileEditor.cpp

	QT4 GUI for Qt Studio
 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn)
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2007 - 2007 
******************************************************************/ 

/**
	TextFileEditor.cpp

	Since each netlist has a netlist editor object associated to it,
	we also let the netlist editor manages the simulation status
	(simulated or not) of the netlist.
	
*/	


#include <QtGui>
#include "Macro_Defs.h"
#include "TextFileEditor.h"

using namespace std;


TextFileEditor::TextFileEditor(QWidget *parent) : QTextEdit(parent)
{
	editorAction = new QAction(this);
	editorAction->setCheckable(true);
	
	connect(editorAction, SIGNAL(triggered()), this, SLOT(show()));
	connect(editorAction, SIGNAL(triggered()), this, SLOT(setFocus()));

	isUntitled = true;
	
	fileFilters = tr("Netlist files (*.sp)\n"  "All files (*)");

	connect(document(), SIGNAL(contentsChanged()), this, SLOT(netlistModified()));

	setWindowIcon(QPixmap(":/images/document.png"));

	setAttribute(Qt::WA_DeleteOnClose);

	netlistSimulated = false;		// flagging "not simulated"
	fileModified = false;			// flagging "not modified"
	currNetlistPath = ".";
	
}


void TextFileEditor::closeEvent(QCloseEvent *event)
{
	//cout << LINE_INFO << endl;
	
	if (okToContinue())  {
		emit netlistClosed(NetlistName);
		event->accept();
	} else {
		event->ignore();
	}
}

void TextFileEditor::netlistModified()
{
	setWindowModified(true);
}


void TextFileEditor::newFile()
{
	static int docnum = 1;
	activeNetlist = tr("netlist%1.sp").arg(docnum);
	
	setWindowTitle(tr("%1[*]").arg(activeNetlist));
	//setWindowTitle(tr("%1").arg(activeNetlist));
	
	editorAction->setText(activeNetlist);
	
	isUntitled = true;
	++docnum;
}

/**
  	The file name obtained from QFileDialog contains the full path.
*/
bool TextFileEditor::open(QString path)
{
	cout << LINE_INFO << "\n\tpath = " << path.toStdString() << endl;
	
	QString filename = QFileDialog::getOpenFileName(this, tr("Open a netlist"), path,  fileFilters);

	currNetlistPath = QFileInfo(filename).path();	// keep the current file path

	cout << LINE_INFO << "\n\tfilename = " << filename.toStdString() << endl;
	
	if (filename.isEmpty())
	    return false;

	return openFile(filename);
}

bool TextFileEditor::openFile(const QString &filename)
{
	if (readFile(filename)) 
	{
		setCurrentFile(filename);
		return true;
	} else {
		return false;
	}
}

bool TextFileEditor::okToContinue()
{
	if (document()->isModified()) 
	{
		int r = QMessageBox::warning(this, tr("GRASS"),
		            tr("File %1 has been modified.\n"
		               "Do you want to save your changes?")
		            .arg(trimmedFileName(activeNetlist)),
		            QMessageBox::Yes | QMessageBox::Default,
		            QMessageBox::No,
		            QMessageBox::Cancel | QMessageBox::Escape);
		
		if (r == QMessageBox::Yes) {
			return save();
		} else if (r == QMessageBox::Cancel) {
			return false;
		}
	}
	
	return true;
}


bool TextFileEditor::readFile(const QString &filename)
{
	NetlistName = filename;
	
	QFile file(filename);
	
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		    QMessageBox::warning(this, tr("Analog Designer"),
	                         tr("Cannot read file %1:\n%2.")
	                         .arg(file.fileName())
	                         .arg(file.errorString()));
			
		cout << LINE_INFO << flush;
		return false;
	}

	setWindowTitle(tr("%1[*]").arg(trimmedFileName(filename)));

	QTextStream in(&file);
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	setPlainText(in.readAll());
	
	QApplication::restoreOverrideCursor();

	setWindowModified(false);	// should put this line behind QApplication
	return true;
}


bool TextFileEditor::save()
{
	if (isUntitled) {
		return saveAs();
	} else {
		return saveFile(activeNetlist);
	}
}


bool TextFileEditor::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), activeNetlist, fileFilters);

	if (filename.isEmpty())
		return false;

	return saveFile(filename);
}


bool TextFileEditor::saveFile(const QString &filename)
{
	if (writeFile(filename)) {
		setCurrentFile(filename);
		return true;
	} else {
		return false;
	}
}

void TextFileEditor::setCurrentFile(const QString &filename)
{
	//cout << LINE_INFO << endl;
	
	activeNetlist = filename;
	isUntitled = false;
	
	//editorAction->setText(trimmedFileName(activeNetlist));

	document()->setModified(false);
	setWindowModified(false);

	fileModified = true;
	netlistSimulated = false;
}


QSize TextFileEditor::sizeHint() const
{
	return QSize(72 * fontMetrics().width('x'), 25 * fontMetrics().lineSpacing());
}


QString TextFileEditor::trimmedFileName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}


bool TextFileEditor::writeFile(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("Analog Designer"),
		                     tr("Cannot write file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	out << document()->toPlainText();
	
	QApplication::restoreOverrideCursor();
	return true;
}

