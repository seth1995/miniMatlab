/*************************************************************** 
	NN_Interface.h

	QT4 GUI for AnalogDesigner (AD)
 
	SHI, Guoyong 		(shiguoyong@ic.sjtu.edu.cn)
	School of Microelectronics, Shanghai Jiao Tong University 
	(c) 2010 - 2012
******************************************************************/ 


#ifndef  NEURAL_NET_INTERFACE_H
#define  NEURAL_NET_INTERFACE_H

#include  <qmainwindow.h>
#include  <map>
#include  <string>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
//class QDoubleSpinBox;
class QToolBar;
class QTableWidget;

class QwtSlider;

class Plotter;
class DotMatrixEditor;

#if 0
class  GPDD;		// the Design Panel must talk to GPDD
class  Analysis_SAC;
class  Analysis_ACSens;
#endif

class NN_Interface : public QMainWindow
{
	Q_OBJECT

public:
	NN_Interface(QWidget *parent = 0);

	QString  GetSymbolNameFromItem(int row, int column);

signals:
	void  NN_InterfaceClosed();
	void  SymbolDoubleClicked(int row, int column);
	void  SymbolNameValueSignal(QString symbname, double symbvalue);
	
private slots:
	void  DisplayMessage(const QString &msg);
	void  help();
	void  open();
	void  ExportPDF();
	void  Print();

	void  Plot_AC_Response();
	void  Plot_Sens_Response();
	
	void  ShowMinFreqDialog();
	void  ShowMaxFreqDialog();
	void  ShowSymbolSliderDialog(int row, int column);
	
	void  ResetMinFreq(double freq);
	void  ResetMaxFreq(double freq);
	
	void  RerunAnalysisForNewDeviceWidth(QString symbname, double symbvalue);
	void  RerunAnalysisForNewSymbValue(QString symbname, double symbvalue);
	void  RerunAnalysisForNewFreqRange();
	
	void  ClearSensitivity();
	void  RunSensitivity();

	void  ResetSymbolValue(double value);
	void  SetSensitivitySymbol(int row, int column);

protected:
	void closeEvent(QCloseEvent *event);

private:
	void  CreateActions();
	void  CreateMenus();
	void  CreateToolBars();
	void  CreateStatusBar();
	
	QTableWidget*  CreateSymbolTable();
	void  DisplaySymbols();

	QMenu  	*AnalysisMenu;
	QMenu  	*FileMenu;
	QMenu  	*HelpMenu;
	
	QMenu  	*PlotMenu;
	QMenu 	*BkgColorMenu_ac;
	QMenu 	*BkgColorMenu_sens;

	QToolBar *ToolBar;

	QAction  *AboutAction;
	QAction  *ExitAction;
	QAction  *OpenAction;
	QAction  *plotACAction;		// plot AC response
	QAction  *plotSensAction;		// plot sensitivity response
	QAction  *clearSensAction;
	QAction 	*pdfExportAction;
	QAction 	*PrintAction;

	QAction  *fminAction;		// adjust the min frequency
	QAction  *fmaxAction;	// adjust the max frequency
	QAction	*RerunAction;	// Rerun the AC analysis

	QActionGroup *bkgColorGroup_ac;	
	QAction	*bkgDarkBuleAction_ac;
	QAction 	*bkgDarkGrayAction_ac;
	QAction 	*bkgWhiteAction_ac;

	QActionGroup *bkgColorGroup_sens;	
	QAction	*bkgDarkBuleAction_sens;
	QAction 	*bkgDarkGrayAction_sens;
	QAction 	*bkgWhiteAction_sens;

	QLabel  	*ReadyLabel;

	QTableWidget  	*SymbolTable;

	DotMatrixEditor *DotEditor1;
	
/*	
	Plotter	*acPlotter;		// Plots the ac response
	Plotter	*sensPlotter;	// Plots the sensitivity curves

	GPDD	*theGPDD;
	Analysis_SAC		*sacAnalysis;
	Analysis_ACSens	*sensAnalysis;
*/
#if 0	
	std::map<std::string, double*> 	theParaToValueMap;
	
	QString	SymbolClicked;	// the clicked symbol name
	QString	SensSymbol;		// the symbol selected for sensitivity analysis
	int		SymbolRow;		// the symbol table row where it is clicked

	double	FreqMin;	// The AC analysis frequency range
	double	FreqMax;
	QString	FreqStepName;	// "LIN" or "DEC"
#endif
};

#endif

