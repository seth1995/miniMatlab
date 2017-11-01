/*************************************************************** 
	NN_Interface.cpp

	QT4 GUI for Neural Network Interface.
 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn)
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2007 - 2007
******************************************************************/ 

#include  <QtGui>
#include  <qwt_slider.h>
#include  <qwt_scale_engine.h>
#include  <qwt_scale_map.h>
//#include  <qwt_plot_panner.h>
#include  <qwt_plot_zoomer.h>	// has the QwtPlotPicker
#include <qwt_text.h>

#include  <cmath>
#include  <map>
#include  <string>
#include  <iostream>

#include "../Main/Macro_Defs.h"
#include  "NeuralNetInterf.h"
#include  "NN_plotter.h"
#include  "DotMatrixEditor.h"

//#include  "SliderDialog.h"
//#include  "QtUI.h"
//#include  "Plotter.h"

#if 0
#include  "../GPDD/GPDD.h"
#include  "../Analysis/Analysis_SAC.h"
#include  "../Analysis/Analysis_ACSens.h"
#endif

using namespace std;

/**
	The neural network interface is a QMainWindow which has a 
	standard manual and toolbar system.
	
	A plotter is created in the NN_Interface constructor.
*/
NN_Interface::NN_Interface(QWidget *parent) : QMainWindow(parent)
{
#ifdef TRACE	
	cout << LINE_INFO << endl;
#endif

/*
	theGPDD = GPDDrunner->Get_Current_GPDD();
	sacAnalysis = GPDDrunner->Get_SAC_Analysis();
	sensAnalysis = GPDDrunner->Get_ACSens_Analysis();
*/
/*
	FreqMin = sacAnalysis->Get_Start_Freq();
	FreqMax = sacAnalysis->Get_Stop_Freq();
	FreqStepName = tr("%1").arg(sacAnalysis->Get_Step_Type_Name().c_str());
*/	

	//SymbolTable = CreateSymbolTable();

    DotEditor1 = new DotMatrixEditor(this);

#if 0	
	acPlotter = new Plotter(this, NULL /*filename*/);  
	acPlotter->setMargin(5);
	acPlotter->setTitle(tr("Frequency response"));  
	acPlotter->setAxisTitle(QwtPlot::xBottom, "Frequency (Hz)");
    	acPlotter->setAxisTitle(QwtPlot::yLeft, "Magnitude (dB)");
   	acPlotter->setAxisTitle(QwtPlot::yRight, "Phase (deg)");
	
	sensPlotter = new Plotter(this, NULL);  
	sensPlotter->setMargin(5);
	sensPlotter->setTitle(tr("Sensitivity curves"));  
	sensPlotter->setAxisTitle(QwtPlot::xBottom, "Frequency (Hz)");
    	sensPlotter->setAxisTitle(QwtPlot::yLeft, "Real Sens (no unit)");
   	sensPlotter->setAxisTitle(QwtPlot::yRight, "Imag Sens (no unit)");
#endif

#if 0
	// Let the vertical splitter hold the frequency ranger and the plotter.
	QSplitter  *verticalSplitter = new QSplitter(Qt::Vertical);
	verticalSplitter->addWidget(acPlotter);
	verticalSplitter->addWidget(sensPlotter);
	verticalSplitter->setStretchFactor(1, 1);

	// Let the horizontal splitter hold the symbol table & the vertical splitter
	QSplitter  *horizontalSplitter = new QSplitter(Qt::Horizontal);
	horizontalSplitter->addWidget(SymbolTable);
	horizontalSplitter->addWidget(verticalSplitter);
	horizontalSplitter->setStretchFactor(1, 1);
#endif

	//setCentralWidget(horizontalSplitter);
	setCentralWidget(DotEditor1);

	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();

#if 0
//	connect(SymbolTable, SIGNAL(cellDoubleClicked(int, int)), 
//			this, SIGNAL(SymbolDoubleClicked(int, int)) );

	connect(SymbolTable, SIGNAL(cellDoubleClicked(int, int)), 
			this, SLOT(ShowSymbolSliderDialog(int, int)) );
	connect(SymbolTable, SIGNAL(cellPressed(int, int)), 
			this, SLOT(SetSensitivitySymbol(int, int)) );

//	connect( this, SIGNAL(EmitMessage(const QString &)),
//			this, SLOT(DisplayMessage(const QString &)) );

	DisplaySymbols();
//	SymbolClicked = tr("");
#endif

	//setWindowTitle(tr("Design Panel"));
    setWindowTitle(QObject::tr("Neural Network Plotter"));
	setWindowIcon(QPixmap(":/images/neural_network.png"));

//---------------------------------------------------------------


    ImageViewSetting rect_settings;
    rect_settings.minX = 0.0;		rect_settings.maxX = 100.0;
    rect_settings.minY = 0.0;		rect_settings.maxY = 100.0;
	
    DotEditor1->setPlotSettings(rect_settings);

	// (x0, y0, grid_size, #grids_x, #grids_y); 
	// (x0, y0) are left-upper user coordinates,
	// grid_size is in user coordiantes,
	// (grids_x, grids_y) are numbers of grids in x and y dirctions.
	DotEditor1->setMatrixGrid(10, 90, 6, 6, 8);

    //DotEditor1->setGeometry(100, 40, 600, 600);
    //DotEditor1->show();
}


void  NN_Interface::ClearSensitivity()
{
	//sensPlotter->ClearAllCurves();
}


void NN_Interface::closeEvent(QCloseEvent *event)
{
	int reply = QMessageBox::warning(this, tr("Neural Network Designer"),
	            tr("Do you want to close the Neural Network Designer?"),
	            QMessageBox::Yes | QMessageBox::Default,
	            QMessageBox::No,
	            QMessageBox::Cancel | QMessageBox::Escape);
	
	if (reply == QMessageBox::Yes) {
		emit NN_InterfaceClosed();
    	event->accept();
	} 
	else if (reply == QMessageBox::No || reply == QMessageBox::Cancel) {
        	event->ignore();
	}
}

/**
	Create a symbol table for listing the symbol names and their values.
	The symbol names are clickable for displaying a slider dialog.
*/
QTableWidget* NN_Interface::CreateSymbolTable()
{
	QTableWidget* symbol_table = new QTableWidget(0, 2);  // 0 row and 2 columns

	QStringList  labels;

	labels << tr("Symbol Name") << tr("Symbol Value");

	symbol_table->setHorizontalHeaderLabels(labels);
	
	// Set first column header to be resizeable
	symbol_table->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	
	// Hide the vertical header
	symbol_table->verticalHeader()->hide();
	
	//symbol_table->setSortingEnabled(true);
	
	// Hide the grid
	symbol_table->setShowGrid(false);

	return symbol_table;
}



void NN_Interface::CreateActions()
{
#if  0
	// Open
	OpenAction = new QAction(QIcon(":/images/open.png"),
					tr("&Open ..."), this);
	OpenAction->setShortcut(tr("Ctrl+O"));
	OpenAction->setStatusTip(tr("Open a file"));
	connect( OpenAction, SIGNAL(triggered()), this, SLOT(open()));
#endif

	// Export pdf action
	pdfExportAction = new QAction(QIcon(":/images/pdf.png"),
				tr("&Export PDF..."), this);
	pdfExportAction->setStatusTip(tr("Export the plot in PDF format"));
	connect( pdfExportAction, SIGNAL(triggered()), this, SLOT(ExportPDF()) );
	
	// Print action
	PrintAction = new QAction(QIcon(":/images/print.png"),
				tr("&Print..."), this);
	PrintAction->setShortcut(tr("Ctrl+P"));
	PrintAction->setStatusTip(tr("Print the plot"));
	connect( PrintAction, SIGNAL(triggered()), this, SLOT(Print()) );

	// Reset fmin action 
	fminAction = new QAction(QIcon(":/images/fmin.ico"),
			tr("Change m&in freq"), this);
	fminAction->setStatusTip(tr("Adjust min freq"));
	connect( fminAction, SIGNAL(triggered()),this, SLOT(ShowMinFreqDialog()) );

	// Reset fmax action 
	fmaxAction = new QAction(QIcon(":/images/fmax.ico"),
			tr("Change m&ax freq"), this);
	fmaxAction->setStatusTip(tr("Adjust max freq"));
	connect( fmaxAction, SIGNAL(triggered()),this, SLOT(ShowMaxFreqDialog()) );

	// Rerun action 
	RerunAction = new QAction(QIcon(":/images/rerun.ico"),
			tr("&Rerun AC analysis"), this);
	RerunAction->setStatusTip(tr("Rerun AC analysis"));
	connect( RerunAction, SIGNAL(triggered()), 
			this, SLOT(RerunAnalysisForNewFreqRange()) );
	
	// Plot AC
	plotACAction = new QAction(QIcon(":/images/plot_ac.png"),
					tr("&Plot AC"), this);
	plotACAction->setShortcut(tr("Ctrl+P"));
	plotACAction->setStatusTip(tr("Plot AC response"));
	connect( plotACAction, SIGNAL(triggered()), this, SLOT(Plot_AC_Response()));
	
	// Sensitivity
	plotSensAction = new QAction(QIcon(":/images/sens.ico"),
					tr("Run &Sens"), this);
	plotSensAction->setShortcut(tr("Ctrl+A"));
	plotSensAction->setStatusTip(tr("Run sensitivity analysis"));
	connect( plotSensAction, SIGNAL(triggered()), this, SLOT(RunSensitivity()));

	clearSensAction = new QAction(QIcon(":/images/clear_sens.ico"),
					tr("&Clear sens plots"), this);
	clearSensAction->setShortcut(tr("Ctrl+C"));
	clearSensAction->setStatusTip(tr("Clear all sensitivity plots"));
	connect( clearSensAction, SIGNAL(triggered()), this, SLOT(ClearSensitivity()));

	// help action 
	AboutAction = new QAction(QIcon(":/images/help.png"),
			tr("&About Design Panel"), this);
	AboutAction->setStatusTip(tr("About"));
	connect( AboutAction, SIGNAL(triggered()),this, SLOT(help()) );
	
	// Exit
	ExitAction = new QAction(QIcon(":/images/exit.png"), tr("&Quit"), this);
	ExitAction->setShortcut(tr("Ctrl+Q"));
	ExitAction->setStatusTip(tr("Quit Design Panel"));
	connect( ExitAction, SIGNAL(triggered()), this, SLOT(close()) );

	// Background colors for ac plot
	bkgDarkBuleAction_ac = new QAction(tr("Dark Blue"), this);
	bkgDarkBuleAction_ac->setCheckable(true);	
	bkgDarkBuleAction_ac->setStatusTip(tr("Set background color to dark blue"));
	/*
	connect( bkgDarkBuleAction_ac, SIGNAL(triggered()), 
			acPlotter, SLOT(ChangeBkgColorDarkBule()) );
	*/

	bkgDarkGrayAction_ac = new QAction(tr("Dark Gray"), this);
	bkgDarkGrayAction_ac->setCheckable(true);	
	bkgDarkGrayAction_ac->setStatusTip(tr("Set background color to dark gray"));
	/*
	connect( bkgDarkGrayAction_ac, SIGNAL(triggered()), 
			acPlotter, SLOT(ChangeBkgColorDarkGray()) );
	*/
	
	bkgWhiteAction_ac = new QAction(tr("White"), this);
	bkgWhiteAction_ac->setCheckable(true);	
	bkgWhiteAction_ac->setStatusTip(tr("White background"));
	/*
	connect( bkgWhiteAction_ac, SIGNAL(triggered()), 
			acPlotter, SLOT(ChangeBkgColorWhite()) );
	*/
	
	bkgColorGroup_ac = new QActionGroup(this);
 	bkgColorGroup_ac->addAction(bkgDarkBuleAction_ac);
	bkgColorGroup_ac->addAction(bkgDarkGrayAction_ac);
 	bkgColorGroup_ac->addAction(bkgWhiteAction_ac);
	
 	bkgWhiteAction_ac->setChecked(true);	// initial selection for bkg color

	// Background colors for sens plot
	bkgDarkBuleAction_sens = new QAction(tr("Dark Blue"), this);
	bkgDarkBuleAction_sens->setCheckable(true);	
	bkgDarkBuleAction_sens->setStatusTip(tr("Set background color to dark blue"));
	/*
	connect( bkgDarkBuleAction_sens, SIGNAL(triggered()), 
			sensPlotter, SLOT(ChangeBkgColorDarkBule()) );
	*/
	
	bkgDarkGrayAction_sens = new QAction(tr("Dark Gray"), this);
	bkgDarkGrayAction_sens->setCheckable(true);	
	bkgDarkGrayAction_sens->setStatusTip(tr("Set background color to dark gray"));
	/*
	connect( bkgDarkGrayAction_sens, SIGNAL(triggered()), 
			sensPlotter, SLOT(ChangeBkgColorDarkGray()) );
	*/
	
	bkgWhiteAction_sens = new QAction(tr("White"), this);
	bkgWhiteAction_sens->setCheckable(true);	
	bkgWhiteAction_sens->setStatusTip(tr("White background"));
	/*
	connect( bkgWhiteAction_sens, SIGNAL(triggered()), 
			sensPlotter, SLOT(ChangeBkgColorWhite()) );
	*/
	
	bkgColorGroup_sens = new QActionGroup(this);
 	bkgColorGroup_sens->addAction(bkgDarkBuleAction_sens);
	bkgColorGroup_sens->addAction(bkgDarkGrayAction_sens);
 	bkgColorGroup_sens->addAction(bkgWhiteAction_sens);
	
 	bkgWhiteAction_sens->setChecked(true);		// initial selection for bkg color

}

void NN_Interface::CreateMenus()
{
//	cout << LINE_INFO << endl;
	
	FileMenu = menuBar()->addMenu(tr("&File"));
//	FileMenu->addAction(OpenAction);
	FileMenu->addAction(pdfExportAction);
	FileMenu->addAction(PrintAction);
	FileMenu->addSeparator();
	FileMenu->addAction(ExitAction);

	AnalysisMenu = menuBar()->addMenu(tr("&Analysis"));
	AnalysisMenu->addAction(RerunAction);

	PlotMenu = menuBar()->addMenu(tr("&Plot"));
	PlotMenu->addAction(plotACAction);
	PlotMenu->addSeparator();
	PlotMenu->addAction(plotSensAction);
	PlotMenu->addAction(clearSensAction);
	PlotMenu->addSeparator();
	
	BkgColorMenu_ac = PlotMenu->addMenu(tr("&AC Background"));
	BkgColorMenu_ac->addAction(bkgDarkBuleAction_ac);
	BkgColorMenu_ac->addAction(bkgDarkGrayAction_ac);
	BkgColorMenu_ac->addAction(bkgWhiteAction_ac);
	PlotMenu->addSeparator();
	
	BkgColorMenu_sens = PlotMenu->addMenu(tr("&Sens Background"));
	BkgColorMenu_sens->addAction(bkgDarkBuleAction_sens);
	BkgColorMenu_sens->addAction(bkgDarkGrayAction_sens);
	BkgColorMenu_sens->addAction(bkgWhiteAction_sens);

	HelpMenu = menuBar()->addMenu(tr("&Help"));
	HelpMenu->addAction(AboutAction);
	
}


void NN_Interface::CreateStatusBar()
{
    	ReadyLabel = new QLabel(tr(" Ready"));
    	statusBar()->addWidget(ReadyLabel, 1);
}


void NN_Interface::CreateToolBars()
{
	ToolBar = addToolBar(tr("ToolBar"));
//	ToolBar->addAction(OpenAction);
	ToolBar->addAction(pdfExportAction);
	ToolBar->addAction(PrintAction);

	ToolBar->addAction(fminAction);
	ToolBar->addAction(fmaxAction);
	ToolBar->addAction(RerunAction);
	ToolBar->addAction(plotACAction);
	ToolBar->addAction(plotSensAction);
	ToolBar->addAction(clearSensAction);
//	ToolBar->addAction(AboutAction);
}

/**
	Display the list of symbols and values to the SymbolTable Widget.
*/
void NN_Interface::DisplaySymbols()
{
#if	0	// Temporarily disabled

//	cout << LINE_INFO << endl;
	
//	map<string, double*> 	symbol_value_map;

	theParaToValueMap = theGPDD->Get_Para_To_Value_Map();
	
	map<string, double*>::const_iterator  iter;

//	cout << "map size = " << symbol_value_map.size() << endl;

	QTableWidgetItem *symbnameItem, *valueItem;

	for (iter = theParaToValueMap.begin(); iter != theParaToValueMap.end(); ++iter) 
	{
//		cout << "\n\t" << iter->first << ",  " << *(iter->second);
		
		symbnameItem = new QTableWidgetItem(tr("%1")
							.arg(iter->first.c_str()) );
		symbnameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		if (iter->first[0] == 'W') {
			// Treat the "Width" parameter differently
			valueItem = new QTableWidgetItem(tr("%1u").arg(*(iter->second) * 1e6) );
		} else {
			// A parameter other than a "Width".
			valueItem = new QTableWidgetItem(tr("%1").arg(*(iter->second)) );
		}
		
		//valueItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		valueItem->setFlags(Qt::ItemIsEnabled);

		int row = SymbolTable->rowCount();
		SymbolTable->insertRow(row);
		SymbolTable->setItem(row, 0, symbnameItem);
		SymbolTable->setItem(row, 1, valueItem);
	}
	//filesFoundLabel->setText(tr("%1 file(s) found").arg(files.size()));

#endif

}

void NN_Interface::DisplayMessage(const QString &msg)
{
	QMessageBox::warning(this, tr("Instant Message"), msg);
}

/**	
	Export a pdf file. (Tested and worked.)
*/
void NN_Interface::ExportPDF()
{

#ifndef QT_NO_FILEDIALOG
	const QString fileName = QFileDialog::getSaveFileName(
	    this, "Export to a pdf file", QString(),
	    "PDF documents (*.pdf)");
#else
	const QString fileName = "bode.pdf";
#endif

	if ( !fileName.isEmpty() )
	{
		QPrinter printer;
		printer.setOutputFormat(QPrinter::PdfFormat);  // Or ::PostScriptFormat
		printer.setOrientation(QPrinter::Landscape);
		printer.setOutputFileName(fileName);

		printer.setCreator("Export PDF");
		
		//DotEditor1->print(printer);
	}
}

QString NN_Interface::GetSymbolNameFromItem(int row, int column)
{
	if (column != 0) {
		return tr("");
	} else {
		QTableWidgetItem *item_clicked = SymbolTable->item(row, 0);
		return item_clicked->text();
	}
}


void  NN_Interface::help()
{
	QMessageBox::about(this, tr("Help"),
					tr("<h2>Design Panel</h2>") );
}

void  NN_Interface::open()
{
	cout << LINE_INFO << "Not implemented" << endl;
}

void  NN_Interface::ResetMinFreq(double freq)
{
	UNUSED_VAR(freq);

//	cout << LINE_INFO << "New min freq = " << freq << endl;

	//FreqMin = freq;
}


void  NN_Interface::ResetMaxFreq(double freq)
{
	UNUSED_VAR(freq);
	
//	cout << LINE_INFO << "New max freq = " << freq << endl;

	//FreqMax = freq;
}


/**
	The symbol name has been saved in "QString SymbolClicked" when the value
	signal is emitted.
	
	This function updates the value entry in the symbol table instantly.
*/
void  NN_Interface::ResetSymbolValue(double value)
{
	UNUSED_VAR(value);
//	cout << LINE_INFO << "New value of \"" << SymbolClicked.toStdString() 
//		<< "\" = " << value << endl;

#if 0
	QString 	displaytext;
	QTableWidgetItem *valueItem;

	if (SymbolClicked.left(1) == "W")   // Device width 
	{
		//displaytext.setNum(value, 'e', 2);
		valueItem = new QTableWidgetItem(tr("%1u").arg(value) );
		//valueItem = new QTableWidgetItem(displaytext);
		SymbolTable->setItem(SymbolRow, 1, valueItem);
		RerunAnalysisForNewDeviceWidth(SymbolClicked, value * 1e-6);
	} 
	else   // Not a device width
	{
		displaytext.setNum(value, 'e', 2);
		valueItem = new QTableWidgetItem(displaytext);
		SymbolTable->setItem(SymbolRow, 1, valueItem);
		RerunAnalysisForNewSymbValue(SymbolClicked, value);
	}
#endif	
}

/**
	The device width is named as "W_M1", etc. This function passes the 
	device width name and the new value to GPDD, re-executes the AC analysis and replots.
*/
void  NN_Interface::RerunAnalysisForNewDeviceWidth(
					QString widthname, double newwidth)
{
	UNUSED_VAR(widthname);
	UNUSED_VAR(newwidth);
	
#if 0 // Temporarily disabled

	theGPDD->SetNewDeviceWidth(widthname.toStdString(), newwidth);

	sacAnalysis->Rerun_HGPDD();
	acPlotter->ClearAllCurves();
	Plot_AC_Response();

#endif	
}

void  NN_Interface::RerunAnalysisForNewSymbValue(
					QString symbname, double symbvalue)
{
	UNUSED_VAR(symbname);
	UNUSED_VAR(symbvalue);

#if 0  // Temprarily disabled	
	theGPDD->SetNewSymbolValue(symbname.toStdString(), symbvalue);

	sacAnalysis->Run_AC_Analysis();
	acPlotter->ClearAllCurves();
	
	Plot_AC_Response();
	
#endif	
}

void  NN_Interface::RerunAnalysisForNewFreqRange()
{
#if 0	
	sacAnalysis->Set_Start_Freq(FreqMin);
	sacAnalysis->Set_Stop_Freq(FreqMax);

	sacAnalysis->Rerun_HGPDD();  // Run symbolic AC analysis again !!
	
	acPlotter->ClearAllCurves();
	Plot_AC_Response();
#endif	
}

void  NN_Interface::Plot_AC_Response()
{
#if 0	
	double *data_x = sacAnalysis->GetFreqVector();
	double *data_y = sacAnalysis->GetMagVector();
	int 	length = sacAnalysis->GetNumFreqPoints();
	const char *axis_style_x = sacAnalysis->Get_Step_Type_Name().c_str();
	QString  	scale_x = tr(axis_style_x);

	// Convert data_y to [dB]
	for (int i = 0; i < length; i++) {
		data_y[i] = 20.0 * log10(data_y[i]);
	}

	// Plot the magnitude of AC response.
	acPlotter->PassCurveData(tr("Magnitude"), data_x, data_y, length, scale_x);

	// Plot the phase of AC response.
	data_y = sacAnalysis->GetPhaseVector();

	// Convert data_y from radian to [degree]
	for (int i = 0; i < length; i++) {
		data_y[i] = (180 / PI) * data_y[i];
	}
	
	acPlotter->PassCurveData(tr("Phase"), data_x, data_y, length, scale_x);
#endif	
}


void  NN_Interface::Plot_Sens_Response()
{
#if 0	
	double 	*data_x = sensAnalysis->GetFreqVector();
	double 	*data_y = sensAnalysis->GetRealSensVector();
	int 		length = sensAnalysis->GetNumFreqPoints();
	
	const char *axis_style_x = sacAnalysis->Get_Step_Type_Name().c_str();
	QString  	scale_x = tr(axis_style_x);

	// Convert data_y to [dB]
//	for (int i = 0; i < length; i++) 
//	{
//		data_y[i] = 20.0 * log10(data_y[i]);
//	}

	// Plot the magnitude of AC response.
	sensPlotter->PassCurveData(tr("Real Sens"), data_x, data_y, length, scale_x);

	// Plot the phase of AC response.
	data_y = sensAnalysis->GetImagSensVector();

	// Convert data_y from radian to [degree]
//	for (int i = 0; i < length; i++) 
//	{
//		data_y[i] = (180 / PI) * data_y[i];
//	}
	
	sensPlotter->PassCurveData(tr("Imag Sens"), data_x, data_y, length, scale_x);
#endif	
}

/**
	Print a plot to a ps file. (Tested and worked.)
*/
void NN_Interface::Print()
{
#if 0
	QPrinter printer;
#else
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFileName("./bode.ps");
#endif

#if 0
	QString docName = acPlotter->title().text();
	if ( !docName.isEmpty() )
	{
	    docName.replace (QRegExp (QString::fromLatin1 ("\n")), tr (" -- "));
	    printer.setDocName (docName);
	}

	printer.setCreator("Bode example");
	printer.setOrientation(QPrinter::Landscape);

	QPrintDialog dialog(&printer);
	if ( dialog.exec() )
	{
	    QwtPlotPrintFilter filter;
	    if ( printer.colorMode() == QPrinter::GrayScale )
	    {
	        filter.setOptions(QwtPlotPrintFilter::PrintAll 
	            & ~QwtPlotPrintFilter::PrintBackground);
	    }
	    acPlotter->print(printer, filter);
	}
#endif	
}


void  NN_Interface::RunSensitivity()
{
#if 0	
	string  sensvar = SensSymbol.toStdString();
	
	/*cout << LINE_INFO << endl;
	cout << "\n\tThe sens symbol is \"" << sensvar << "\"" << endl; */

	if (SensSymbol.isEmpty()) 
	{
		QString msg =  tr("The sensitivity parameter is not selected.\n") + \
			tr("Select a parameter by clicking a symbol name in the symbol table.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	if (!sensAnalysis) 
	{
		QString msg =  tr("The sensitivity is not initiated.\n") + \
			tr("Run AC analysis first.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

#if  0  // temporarily disabled!
	sensAnalysis->Run_Sensitivity(sensvar);
#endif

	Plot_Sens_Response();
#endif
	
}

void NN_Interface::SetSensitivitySymbol(int row, int column)
{
	UNUSED_VAR(row);
	UNUSED_VAR(column);
	
//	cout << LINE_INFO << endl;

#if 0
	string	symbol;
	QTableWidgetItem *itemclicked;
	
	if (column != 0) {
		return;
	} else {
		itemclicked = SymbolTable->item(row, 0);
		SensSymbol = itemclicked->text();
//		SymbolRow = row;
		symbol = SymbolClicked.toStdString();
//		cout << LINE_INFO << symbol << endl << flush; 
	}
#endif	
}

void  NN_Interface::ShowMinFreqDialog()
{
#if 0	
	SliderDialog 	*sliderDialog = new SliderDialog(this, tr("min freq"), tr("Hz"));

	sliderDialog->CreateSlider(1e-4, FreqMin, FreqMax, tr("DEC") );
	connect(sliderDialog, SIGNAL(SliderValueChanged(double)), this, SLOT(ResetMinFreq(double)));
	
	sliderDialog->setWindowTitle(tr("Adjust min frequency"));
	sliderDialog->show();
#endif	
}


void  NN_Interface::ShowMaxFreqDialog()
{
#if 0	
	SliderDialog 	*sliderDialog = new SliderDialog(this, tr("max freq"), tr("Hz"));

	sliderDialog->CreateSlider(FreqMin, FreqMax, FreqMax*1e4, tr("DEC") );
	connect(sliderDialog, SIGNAL(SliderValueChanged(double)), this, SLOT(ResetMaxFreq(double)));
	
	sliderDialog->setWindowTitle(tr("Adjust max frequency"));
	sliderDialog->show();
#endif
}

/**
	The Slot for the "symbol name clicked" signal.
	Catches the symbol clicked and display a slider dialog for
	parametric tuning.
*/
void NN_Interface::ShowSymbolSliderDialog(int row, int column)
{
	UNUSED_VAR(row);
	UNUSED_VAR(column);
	
#if 0	
	string	symbol;
	QTableWidgetItem *itemclicked;
	
	if (column != 0) {
		return;
	} else {
		itemclicked = SymbolTable->item(row, 0);
		SymbolClicked = itemclicked->text();
		SymbolRow = row;
		symbol = SymbolClicked.toStdString();
		//cout << LINE_INFO << symbol << endl; 
	}

	map<string, double*>::const_iterator  iter;

	iter = theParaToValueMap.find(symbol);
	double	symbvalue, lowerbound, upperbound;

	if (iter != theParaToValueMap.end()) {
		symbvalue = *(iter->second);
//		emit SymbolNameValueSignal(QString symbname, double symbvalue);
	} else {
		QString msg =  "Symbol not found.";
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	SliderDialog 	*sliderDialog;
	
	if (SymbolClicked.left(1) != "W") 
	{
		sliderDialog = new SliderDialog(this, SymbolClicked); 
		// SymbolClicked == "para name"

		lowerbound = symbvalue * 0.15;		// reduce by ---
		upperbound = symbvalue * 3.0;		// increase by ---

		sliderDialog->CreateSlider(lowerbound, symbvalue, upperbound, tr("LIN") );  // "scale type"
	} 
	else
	{
		sliderDialog = new SliderDialog(this, SymbolClicked, tr("u")); 
		// SymbolClicked == "para name"

		lowerbound = 1;		
		upperbound = symbvalue * 1e6 + 50;		

		sliderDialog->CreateWidthSlider(lowerbound, symbvalue * 1e6, upperbound);  // "scale type"
	}
	
	connect(sliderDialog, SIGNAL(SliderValueChanged(double)), this, SLOT(ResetSymbolValue(double)));
	
	sliderDialog->setWindowTitle(tr("Tune the parameter"));
	sliderDialog->show();
#endif	
}

