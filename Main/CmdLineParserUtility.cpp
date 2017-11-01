/*
	CmdLineParserUtility.cpp
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>

#include "Macro_Defs.h"
#include "CmdLineParserUtility.h"
#include "../NeuralNetwork_UI/NN_plotter.h"
//#include "../NeuralNetwork_UI/ImageViewSetting.h"

extern char _CommandLine[];

CmdLineParserUtility::CmdLineParserUtility()
{
	VarSymbolTable = NULL;
	ansEntry = NULL;

	//CmdLineParseResult = NULL;
	ResultCount = 0;

	AnsStrLen = 4000;  	// initial length of answer
	CmdWindowWidth = 90;  // width of command window in chars 

	Plotter2D = NULL;
	//PlotterSettings2D = NULL;
	CurveCount = 0;
}

CmdLineParserUtility::~CmdLineParserUtility()
{
#if 0	
	if (CmdLineParseResult) {
		free(CmdLineParseResult);
		CmdLineParseResult = NULL;
	}
#endif

	if (Plotter2D) {
		delete Plotter2D;
		Plotter2D = NULL;
	}
	/*
	if (PlotterSettings2D) {
		delete PlotterSettings2D;
		PlotterSettings2D = NULL;
	}
	*/
}

/* 
	This function clear all variables in the symbol table.
	This function responds to the command "clear all" in 
	the command window. While the variables are cleared, all plotted
	curves, if any, are cleared as well.
*/
void  CmdLineParserUtility::clearAllVariables()
{
#ifdef TRACE_
	cout << LINE_INFO << endl;
#endif

#if 0	
	if (CmdLineParseResult) {
		delete CmdLineParseResult;
		CmdLineParseResult = NULL;
	}
#endif

	if (! VarSymbolTable)  	return;

  	EXPR_ENTRY *entry, *tmp;

    /* Go through the VarSymbolTable list */
    for (entry = VarSymbolTable; entry; )
    {
		tmp = entry->next;
		if (entry)  deleteVarEntry(entry);
		entry = tmp;
	}

	for (int i = 0; i < CurveCount; i++) {
		Plotter2D->clearCurve( i );  // Curves are numbered with curve ids
	}
	
	cout << LINE_INFO << "... done" << endl;
	return;
}

void CmdLineParserUtility::deleteVarEntry(EXPR_ENTRY *entry)
{
	if (!entry)  return;

	cout << LINE_INFO;
	printf("Deleting entry \"%s\"\n", entry->name);
	
	if (entry->name)  delete entry->name;

	if (entry->val) {
		delete entry->val;
	}
}

/* 
	This function prints all variables in the symbol table.
*/
void  CmdLineParserUtility::dumpAllVariables()
{
	//cout << LINE_INFO << endl;
	
	if (! VarSymbolTable)  	return;

  	EXPR_ENTRY *entry;
	stringstream ss;

    /* Go through the VarSymbolTable list */
    for (entry = VarSymbolTable; entry; entry = entry->next)
    {
		ss << dumpVarEntry(entry);
	}

	dumpResultString(ss.str().c_str());
	return;
}

/* 
	Export the content of <str> to the <CmdLineParseResult> string. 
*/
void CmdLineParserUtility::dumpResultString(const char *str)
{
#ifdef TRACE_
	cout << LINE_INFO << endl;
#endif

	IntermediateResult += string(str);
	
#if 0	
	bool expand = false;

	// Initialize memory <CmdLineParseResult>
	if (! CmdLineParseResult) {
		CmdLineParseResult = (char*) malloc(AnsStrLen * sizeof(char)); 
		strcpy(CmdLineParseResult, str);
		return;
	}
	
	/* Make sure <CmdLineParseResult> has sufficient space. */
	while ( strlen(CmdLineParseResult) + strlen(str) > AnsStrLen ) 	{
		AnsStrLen *= 2;	// double the memory space (until sufficient)
		expand = true;
	}

	//fprintf(stderr, "\nlength of CmdLineParseResult = %d", AnsStrLen);

	if (expand) 
		CmdLineParseResult = (char *) realloc(CmdLineParseResult, AnsStrLen*sizeof(char));
	
	if (! CmdLineParseResult) {
		printf("\n\tNot enough memory.\n"); 	
		return;
	}

	strcpy(CmdLineParseResult, str);
	//strcat(CmdLineParseResult, str);

#endif

}

/*
	Dump the list of double values.
*/
string CmdLineParserUtility::dumpValueList(double *val, int vlen)
{
	int i, len = 0;
	char val_str[512];
	stringstream ss;

	if (vlen == 0)  return string("");

	for (i = 0; i < vlen; i++) {
		sprintf(val_str, "%g  ", val[i]);
		ss << string(val_str);
		len += strlen(val_str);
		if (len > CmdWindowWidth) {
			//strcat(val_str, "\n");  
			ss << "\n";  // Append a newline
			len = 0;
		}
	}
	//ss << "\n";  // End by newline
	return ss.str();
}

void CmdLineParserUtility::dumpValueListToResult(double *val, int vlen)
{
	string str = dumpValueList(val, vlen);
	if (! str.empty())
		dumpResultString(str.c_str());
}

string CmdLineParserUtility::dumpVarEntry(EXPR_ENTRY *entry)
{
	if (!entry)  return string("");

	stringstream ss;
	
	if (entry->name) {
		//printf("%s = ", entry->name);
		ss << "  " << string(entry->name) << " = ";
	}

	ss << dumpValueList(entry->val, entry->len);
	//ss << "\n";
	return ss.str();
}

void CmdLineParserUtility::dumpVarEntryToResult(EXPR_ENTRY *entry)
{
	string str = dumpVarEntry(entry);
	if (! str.empty())
		dumpResultString(str.c_str());
}

VECTOR_t CmdLineParserUtility::evaluateFunction(char *fname, VECTOR_t vin)
{
	int i;
	VECTOR_t var;
	char tmp_str[512];

	if (strcmp(fname, "abs") == 0 || strcmp(fname, "fabs") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = fabs(vin.val[i]); 
	}
	else if (strcmp(fname, "ceil") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = ceil(vin.val[i]); 
	}
	else if (strcmp(fname, "floor") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = floor(vin.val[i]); 
	}
	else if (strcmp(fname, "asin") == 0) { 
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] > 1.0 || vin.val[i] < -1.0) {
				sprintf(tmp_str, "\nOut of domain for asin().");
				dumpResultString(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = asin(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "acos") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] > 1.0 || vin.val[i] < -1.0) {
				sprintf(tmp_str, "\nOut of domain for asin().");
				dumpResultString(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = acos(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "atan") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = atan(vin.val[i]); 
	}
	else if (strcmp(fname, "sin") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = sin(vin.val[i]); 
	}
	else if (strcmp(fname, "cos") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = cos(vin.val[i]); 
	}
	else if (strcmp(fname, "tan") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = tan(vin.val[i]); 
	}
	else if (strcmp(fname, "ctan") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] == 0) {
				sprintf(tmp_str, "\nOut of domain for ctan().");
				dumpResultString(tmp_str); 
				var.val[i] = 0;
			} else
				var.val[i] = 1/tan(vin.val[i]);
		}
	}
	else if (strcmp(fname, "sinh") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = sinh(vin.val[i]); 
	}
	else if (strcmp(fname, "cosh") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = cosh(vin.val[i]); 
	}
	else if (strcmp(fname, "tanh") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = tanh(vin.val[i]); 
	}
	else if (strcmp(fname, "exp") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  
			var.val[i] = exp(vin.val[i]); 
	}
	else if (strcmp(fname, "ln") == 0 || strcmp(fname, "log") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] <= 0) {
				sprintf(tmp_str, "\nOut of domain for log().");
				dumpResultString(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = log(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "log10") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] <= 0) {
				sprintf(tmp_str, "\nOut of domain for log10().");
				dumpResultString(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = log10(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "sqrt") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++) { 
			if (vin.val[i] <= 0) {
				sprintf(tmp_str, "\nOut of domain for sqrt().");
				dumpResultString(tmp_str); 
				var.val[i] = 0;
			} else {
				var.val[i] = sqrt(vin.val[i]);
			}
		}
	}
	else if (strcmp(fname, "ones") == 0) {
		/* If passed in a vector, it returns a vector of 1's of the same length.
		   If passed in a scalar, it returns a vector of 1's of the length
		   given by floor(fabs(scalar)). */
		int len;
		if (vin.len > 1)	
			len = vin.len;
		else 				
			len = floor(fabs(vin.val[0])); 
		if (len == 0) {
			var.name = NULL;
			var.val = NULL;
			var.len = 0;
			return var;
		}
		var.val = (double *)malloc(len*sizeof(double));
		var.len = len;
		for (i = 0; i < len; i++)  var.val[i] = 1.0;
	}
	else if (strcmp(fname, "round") == 0) {
		var.val = (double *)malloc(vin.len*sizeof(double));
		var.len = vin.len;
		for (i = 0; i < vin.len; i++)  {
			var.val[i] = round(vin.val[i]); 
			// round() is a customized function, defined in this file.
		}
	}
	else {
		sprintf(tmp_str, "\nUnknown function name: %s ", fname);
		dumpResultString(tmp_str); 
		var.len = 0;
		var.val = NULL;
	}

	var.name = NULL;
	return  var;
}

/* 
	This function looks for a given symbol name. If not found, it creates
	a new entry structure and returns the pointer to it. 
*/
EXPR_ENTRY* CmdLineParserUtility::getVariable(const char *name)
{
#ifdef TRACE_
	cout << LINE_INFO << endl;
#endif

  	EXPR_ENTRY *entry, **entry_P;

    /* Scan the expression entry list */
    for (entry_P = &VarSymbolTable; *entry_P; entry_P = &((*entry_P)->next))
    {
	 	/* Is the symbol name already in the symbol table? */
		if (! (*entry_P) )  break;  // List is empty
        if (strcmp((*entry_P)->name, name) == 0)
			return *entry_P; 
	}
	
    /* Now we have reached the end of the VarSymbolTable list. */
#ifdef TRACE_
	cout << LINE_INFO << "Creating a new entry for \"" << string(name) << "\"" << endl;
#endif
	entry = (EXPR_ENTRY *) malloc(sizeof(EXPR_ENTRY));
	entry->name = strdup(name);  	/* copies and creates space for name */ 
	entry->val = NULL;    		/* empty value array */ 
	entry->len = 0;      
	entry->next = NULL;
	
	/*printf("\nVarSymbolTable <%s> created", entry->name); fflush(stdout);*/

	/* Link in the new entry at the end of entry list. */
	*entry_P = entry;		
	return entry;
}

/* 
	This function checks whether a given symbol name is in the list of
	VarSymbolTables. If found, it returns the pointer to it; otherwise, it
	returns NULL. 
*/
EXPR_ENTRY*  CmdLineParserUtility::lookupVariable(char *name)
{
#ifdef TRACE_
	cout << LINE_INFO << endl;
#endif

  	EXPR_ENTRY *entry;

	if (! VarSymbolTable) {
		//cout << LINE_INFO << "List is empty." << endl;
		return NULL;
	}

    /* Look through the VarSymbolTable list */
    for (entry = VarSymbolTable; entry; entry = entry->next)
    {
	 	/* Is the symbol name already in the symbol table? */
        if (strcmp(entry->name, name) == 0) {
#ifdef TRACE_
			cout << LINE_INFO << "Found the entry \"" 
				<< string(name) << "\"" << endl;
#endif
			return entry; 
        }
	}
#ifdef TRACE_
	cout << LINE_INFO << "Did not find the entry \"" << string(name) << "\"" << endl;
#endif
	return NULL;
}

void CmdLineParserUtility::new_plotting_window(int *num)
{
	UNUSED_VAR(num);
	cout << LINE_INFO << "Not implemented" << endl;
}


void CmdLineParserUtility::plotVector2D(VECTOR_t x, VECTOR_t y)
{
#ifdef TRACE_
	cout << LINE_INFO << endl;
#endif

	if (x.len <= 0 || y.len <= 0)
		return;

	if (! Plotter2D) {
	    Plotter2D = new NN_Plotter;
	}
    Plotter2D->setWindowTitle(QObject::tr("Curve Plotter"));

#if 0	
    int numPoints = 100;
    QVector<QPointF> points0;
    QVector<QPointF> points1;
/*
	qrand() in <QtGlobal> returns a value between 0 and RAND_MAX 
	(defined in <cstdlib> and <stdlib.h>).
*/
    for (int x = 0; x < numPoints; ++x) {
        points0.append(QPointF(x, uint(qrand()) % 100));
        points1.append(QPointF(x, uint(qrand()) % 100));
    }
#endif

    QVector<QPointF> points;

	qint64 length = qMin(x.len, y.len);
	double min_x, max_x, min_y, max_y;
	
	min_x = x.val[0];
	max_x = x.val[0];
	min_y = y.val[0];
	max_y = y.val[0];

	for (int i = 0; i < length; i++) {
        points.append(QPointF(x.val[i], y.val[i]));
		
		if (x.val[i] < min_x)  min_x = x.val[i];
		if (x.val[i] > max_x)  max_x = x.val[i];
		if (y.val[i] < min_y)  min_y = y.val[i];
		if (y.val[i] > max_y)  max_y = y.val[i];
	}
	
    Plotter2D->setCurveData(CurveCount++, points);
    //Plotter2D->setCurveData(1, points1);

    ImageViewSetting &settings = Plotter2D->getPlotSettings();

	if (min_x < settings.minX)
    	settings.minX = min_x;
	if (max_x > settings.maxX)
	    settings.maxX = max_x;
	if (min_y < settings.minY)
	    settings.minY = min_y;
	if (max_y > settings.maxX)
	    settings.maxY = max_y;
	
    Plotter2D->setPlotSettings(settings);

    Plotter2D->show();
}

/* 
	This function prints all variables in the symbol table.
*/
void  CmdLineParserUtility::printAllVariables()
{
	//cout << LINE_INFO << endl;
	
	if (! VarSymbolTable)  	return;

  	EXPR_ENTRY *entry;

    /* Go through the VarSymbolTable list */
    for (entry = VarSymbolTable; entry; entry = entry->next)
    {
		printVarEntry(entry);
	}
	return;
}


void CmdLineParserUtility::printParseError(
				const Cmd::location &loc, const string &msg)
{
	cout << LINE_INFO << "\n\t" << loc << ": " << msg << endl;
	
	/*
	printf("\n\tParse error near line %d\n", LineNum);
	stringstream ss;
	ss << "Syntax error near line " << LineNum;
	ErrorMessage = ss.str();
	SaveFirstError(ErrorMessage);
	ParseError = SPX_NETLIST_SYNTAX_ERROR;

	Abort_Scanner();
	*/
}

void CmdLineParserUtility::printVarEntry(EXPR_ENTRY *entry)
{
	if (!entry)  return;
	
	if (entry->name)  
		printf("%s = ", entry->name);

	printValueList(entry->val, entry->len);
	printf("\n");
}

/*
	Print the list of double values.
*/
void CmdLineParserUtility::printValueList(double *val, int vlen)
{
	cout << LINE_INFO << "Not implemented" << endl;

	int i, len = 0;
	char val_str[512];

	if (vlen == 0)  return;

	for (i = 0; i < vlen; i++) {
		sprintf(val_str, "%g  ", val[i]);
		//dumpResultString(val_str);
		printf("%s ", val_str);
		len += strlen(val_str);
		if (len > CmdWindowWidth) {
			//strcat(val_str, "\n");  
			//dumpResultString("\n");  // Append a newline
			printf("\n");
			len = 0;
		}
	}
	//dumpResultString("\n");  // End by newline
	printf("\n");
}

void CmdLineParserUtility::request_figure(int n)
{
	cout << LINE_INFO << "Not implemented" << endl;
	UNUSED_VAR(n);
}

double CmdLineParserUtility::round(double x)
{
	double 	t, y;
	int sign;

	if (x >= 0) 	sign = 1;
	else 			sign = -1;

	t = fabs(x);
	if (t - floor(t) < 0.5) 	
		y = sign * floor(t);
	else		
		y = sign * (floor(t)+1);
	return (y);
}


