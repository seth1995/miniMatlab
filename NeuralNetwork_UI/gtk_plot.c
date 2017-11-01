/**	
 	gtk_plot.c 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/*	plot.c 
 *	For plotting math functions and data from file. 	
 *
 *	(c) Guoyong Shi, SJTU, 05-02-2007 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "plot.h"


// Axis step type
#define NO_TYPE		0
#define DECADE 		1		// multiply by 10
#define OCTAVE 		2		// multiply by 2
#define LINEAR 		3


/* A list of FIGUREs */
FIGURE 		*figure_head 	= NULL;		/* the head of the figure list */
FIGURE 		*figure_cur 		= NULL;		/* the current figure */
GdkPixmap 	*pixmap_cur 	= NULL;  		/* the currently active pixmap */

/* Now we have <figure_cur>. Don't use draw_area_cur! */
// GtkWidget 	*draw_area_cur = NULL;	/* the current draw_area */

/*================================================================*/

/* The "plot" command calls this function.
 * If figures exist, it plots to the latest active figure window.
 * If no figure exists, it creates a figure window first.
 *
 * Plot a pair of vectors as a curve in a plotting window. 
 * The two vectors passed over are supposed to have nonempty values and
 * the equal length. 
 */
void plot(DOUBLE_VECTOR xx, DOUBLE_VECTOR yy)
{
	double	xmin, xmax, ymin, ymax;
	GtkWidget *draw_area;	

  	//printf("\n<plot> : starting plot ... "); fflush(stdout);

	/* If no draw_area available, create a new plotting window. */
	/* While a plotting window is being created, it calls 
	 * the <configure_event> function to set up a figure. 
	 */
	if (!figure_cur) { 
		new_plotting_window(NULL);
	}

	/* Then plot everything to the current figure! */

	/* Get the bounding limits of the vectors */
	xmin = get_vector_min(xx);
	xmax = get_vector_max(xx);
	ymin = get_vector_min(yy);
	ymax = get_vector_max(yy);

	if (xmin == xmax) {	/* avoid a window without width */
		xmin -= 1.0;
		xmax += 1.0;
	}

	if (ymin == ymax) {	/* avoid a window without height */
		ymin -= 1.0;
		ymax += 1.0;
	}

	/* <figure_cur> already has been set up during window making. */
	/* The initial coordinates are determined by the first curve. */
	if (!figure_cur->curve_list) {
			figure_cur->xmin = xmin;
			figure_cur->xmax = xmax;
			figure_cur->ymin = ymin;
			figure_cur->ymax = ymax;
	} else {
		/* From the 2nd curve on, set the outmost coordinate system. */
		if (figure_cur->xmin > xmin)  
			figure_cur->xmin = xmin;
		if (figure_cur->xmax < xmax)  
			figure_cur->xmax = xmax;
		if (figure_cur->ymin > ymin)  
			figure_cur->ymin = ymin;
		if (figure_cur->ymax < ymax)  
			figure_cur->ymax = ymax;
	}

	/** Add the new curve to the curve list in <figure_cur> **/
	add_curve(figure_cur, xx.val, yy.val, xx.len);

	/* Issue an "expose" event to the callback function expose_event(). */
	draw_area = figure_cur->draw_area;

	gtk_widget_queue_draw_area(draw_area, 
					0, 0,   /* the origin */
					draw_area->allocation.width, 
					draw_area->allocation.height);

}

// Plot with coordinate axis step styles.
// style_x and style_y take one of the strings "DEC", "LIN", "OCT".
// plot_with_style() forces these strings to "uppercase".
//
void plot_with_style(DOUBLE_VECTOR x, DOUBLE_VECTOR y, 
					char *style_x, char *style_y)
{
	int		i;
	int		x_axis_step_type = NO_TYPE;
	int		y_axis_step_type = NO_TYPE;
	char		X_AXIS[4], Y_AXIS[4];	// Axis step type in lowercase
	DOUBLE_VECTOR  new_x, new_y;

#ifdef	TRACE
	printf("\n[%s : line %d] In %s() ... ",
				__FILE__, __LINE__, __FUNCTION__);
#endif

	strncpy(X_AXIS, style_x, 4);
	strncpy(Y_AXIS, style_y, 4);

	// Convert to uppercase
	for (i = 0; i < 3; i++) {
		X_AXIS[i] = toupper(X_AXIS[i]);
		Y_AXIS[i] = toupper(Y_AXIS[i]);
	}

#ifdef	TRACE
	printf("\n[%s : Line %d] X_AXIS = \"%s\"; Y_AXIS = \"%s\" ", 
					__FILE__, __LINE__, X_AXIS, Y_AXIS);
#endif

	// Check the legality of the style string for x-asix.
	if( strcmp(X_AXIS, "LIN") == 0 ) {
		x_axis_step_type = LINEAR;
	} else if ( strcmp(X_AXIS, "OCT") == 0 ) {
		x_axis_step_type = OCTAVE;
	} else if ( strcmp(X_AXIS, "DEC") == 0 ) {
		x_axis_step_type = DECADE;
	}

	if(x_axis_step_type == NO_TYPE) 
	{
		printf("\n%s() : Error: The X-axis style is illegal.", __FUNCTION__);
		return;
	}

	// Check the legality of the style string for y-asix.
	if( strcmp(Y_AXIS, "LIN") == 0 ) {
		y_axis_step_type = LINEAR;
	} else if ( strcmp(Y_AXIS, "OCT") == 0 ) {
		y_axis_step_type = OCTAVE;
	} else if ( strcmp(Y_AXIS, "DEC") == 0 ) {
		y_axis_step_type = DECADE;
	}

	if(y_axis_step_type == NO_TYPE) 
	{
		printf("\n%s() : Error: The Y-axis style is illegal.", __FUNCTION__);
		return;
	}

/**
	if(strcmp(style_x, "lin") == 0 && strcmp(style_y, "lin") == 0) {
		plot(x, y);	// do normal plot.
		return;
	}
**/

	// Convert the x-axis data and y-axis data according to
	// the axix step styles for plotting.
	
	new_x.val = (double *) malloc(x.len * sizeof(double));
	new_x.name = x.name;
	new_x.len = x.len;

	new_y.val = (double *) malloc(y.len * sizeof(double));
	new_y.name = y.name;
	new_y.len = y.len;

	switch (x_axis_step_type) {
		case LINEAR:
			for (i = 0; i < x.len; i++) {
				new_x.val[i] = x.val[i];
			}
			break;
		case DECADE:
			for (i = 0; i < x.len; i++) {
				new_x.val[i] = log10(x.val[i]);
			}
			break;
		case OCTAVE:
			for (i = 0; i < x.len; i++) {
				new_x.val[i] = log(x.val[i])/log(2);
			}
			break;
		default:
			break;
	}
	
	switch (y_axis_step_type) {
		case LINEAR:
			for (i = 0; i < y.len; i++) {
				new_y.val[i] = y.val[i];
			}
			break;
		case DECADE:
			for (i = 0; i < y.len; i++) {
				new_y.val[i] = log10(y.val[i]);
			}
			break;
		case OCTAVE:
			for (i = 0; i < y.len; i++) {
				new_y.val[i] = log(y.val[i])/log(2);
			}
			break;
		default:
			break;
	}
	
	plot(new_x, new_y);
	
	free((double*) new_x.val);
	free((double*) new_y.val);
	
}

/* Create a new FIGURE node given the figure number. 
 * In this function only the basic non-physical info is assigned.
 * Other fields related to the drawable will be assigned in the
 * caller function: new_plotting_window() after the <draw_area> is mapped.
 */
FIGURE*	create_figure(int num)
{
	FIGURE *fig_new;

	//printf("\n<create_figure> : start ..."); fflush(stdout);

	// fig_new = g_new0(FIGURE, 1);
	fig_new = (FIGURE *) malloc( sizeof(FIGURE) );
	if (!fig_new) {
		g_print("\nERROR: Failed to create a new <FIGUR> node.");
		exit(1);
	}

	fig_new->number = num;
	fig_new->curve_list = NULL;

	fig_new->xmin = 0.0; 	/* initialize the coordinate ranges */
	fig_new->ymin = 0.0;
	fig_new->xmax = 0.0;
	fig_new->ymax = 0.0;

	fig_new->next = NULL;

	/* The following fields will be assigned when the draw_area is
	 * realized (mapped). */
	fig_new->draw_area = NULL;
	fig_new->canvas = NULL;
	fig_new->pixmap = NULL;

	//printf("\n<create_figure> : created new figure node"); fflush(stdout);
	return  fig_new;
}

/* Insert a figure to the end of the figure list */
/* This function is called ONLY in the configure_event() callback! */
void insert_figure(FIGURE *fig_in)
{
	FIGURE **fig;

	//g_print("\n--In <insert_figure> ...");

	/* Move <fig> to the end of the figure list */
	for (fig = &figure_head; (*fig); fig = &((*fig)->next));
	// until fig->next == NULL !!

	*fig = fig_in;		// append to the list tail

	//g_print("\n--<insert_figure> Figure inserted (2)");
	
	//print_figure_list(figure_head);
}


/* Find the least available figure number. 
 * We use the quick sort algorithm.
 */
int  assign_figure_number()
{
	FIGURE 	*fig;
	int		cnt, k, a, b;
	int		*num;		// array of fig nums

	//g_print("\n<assign_figure_nubmer> : enter ...");

	if (!figure_head) {	
		//printf("\n<assign_figure_number> : assigned fig num <%d>", 1);  
		return 1;  // the 1st figure
	}

	// Count the number of figure nodes
	cnt = 0;
	for (fig = figure_head; fig; fig = fig->next)   ++cnt;

	// Make an array
	num = (int *) malloc( cnt*sizeof(int) );
	// Fill the number array
	k = 0;
	for (fig = figure_head; fig; fig = fig->next) {
		num[k++] = fig->number;
	}
	// Sort the number array. The returned <num> is sorted.
	quicksort(num, 0, cnt-1); 

	// Find the least availabel figure number. If the consecutive
	// two figure numbers have a gap greater than one, then
	// the smaller figure no added by 1 is the new fig no.
	a = 0;
	for (k = 0; k < cnt; k++) 
	{
		b = num[k];
		if ( b > a+1 ) 	break;
		a = b;		// step forward
	}

	free( (int *) num );

	//printf("\n<assign_figure_number> : assigned fig num <%d>", a+1);  
	return (a+1);
}

/* If the figure exists for a given figure number, returns the figure.
 * Otherwise, returns NULL.
 */
FIGURE* get_figure_by_number(int n)
{
	FIGURE *fig;

	g_print("\n--In <get_figure_by_nubmer> ...");

	if (!figure_head)	return NULL;  /* empty figure list */

	// Search the whole figure list
	for (fig = figure_head; fig; fig = fig->next) {
		if (fig->number == n)     return fig;	
	}
	return NULL;
}

/* If the figure exists for a given canvas, returns the figure.
 * Otherwise, returns NULL.
 */
FIGURE* get_figure_by_canvas(GdkDrawable *canvas)
{
	FIGURE *fig;

	//g_print("\n--In <get_figure_by_canvas> ...");

	if (!figure_head)	return NULL;  /* empty figure list */

	// Search the whole figure list
	for (fig = figure_head; fig; fig = fig->next) {
		if (fig->canvas == canvas)     return fig;	
	}
	return NULL;
}


/* request_figure() is called when the command "figure(n)" is typed.
 * It first checks whether figure "n" has been created.
 * If not, it creates a new figure numbered "n". Otherwise, it
 * activates it and assigns it to <figure_cur>.
 */
void request_figure(int n)
{
	FIGURE 	*fig;
	GtkWidget *toplevel;

	printf("\n<request_figure> : enter ..."); 
	fig = get_figure_by_number(n);

	if ( !fig ) {	/* figure not exists */
		printf("\n<request_figure> : figure (%d) not exist.", n); 
		fflush(stdout);

		new_plotting_window( &n );
	} else {		/* "fn" is the figure number */
		/* Activate the selected figure */
		figure_cur = fig;
		pixmap_cur = figure_cur->pixmap;

		printf("\n<request_figure> : Figure (%d) exists. Pop it up!!", n); 
		fflush(stdout);

		toplevel = gtk_widget_get_toplevel( fig->draw_area );

		if ( gtk_window_is_active( GTK_WINDOW(toplevel) ) )
		{
			printf("\nFigure (%d) is active", n);  fflush(stdout);
		} else 
		{
			printf("\nFigure (%d) is NOT active", n); fflush(stdout);
		}

		gtk_widget_hide(toplevel);
		gtk_window_set_position( GTK_WINDOW(toplevel), GTK_WIN_POS_CENTER );
		gtk_widget_show(toplevel);
		//gtk_window_move( GTK_WINDOW(toplevel), 10, 10 );

		//gtk_widget_queue_draw( toplevel );
		/* This should issue an "expose" event. */
	}
}


/* Delete the figure from the figure list given a canvas. */
gboolean delete_figure_by_canvas(GdkDrawable *canvas)
{
	FIGURE **fig, *tmp;

	//g_print("\n--In <delete_figure> ...");

	/* In case the figure list is empty */
	if (figure_head == NULL) {
		g_print("\n--<delete_figure_by_canvas> : Figure list is empty");
		return 0;
	}

	/* Look up from the whole figure list then delete */
	for (fig = &figure_head; (*fig); fig = &((*fig)->next))
	{
		if ( (*fig)->canvas == canvas ) 
		{
			tmp = *fig;
			*fig = tmp->next;					// reconnect 
			remove_curves( tmp->curve_list ); 	// delete the curves
			g_object_unref( tmp->pixmap ); 		// release the pixmap 
			free( (FIGURE *) tmp );

			g_print("\n--<delete_figure_by_canvas> : Figure deleted");
			return 1;	/* in list */
		}
	}

	g_print("\n--<delete_figure_by_canvas> : Figure not in the list");
	return 0;
}

/* When a plot_win is destroyed, its associated canvas is left null.
 * I have to remove that <FIGURE> node and its contents associated
 * to the canvas. 
 */
gboolean delete_figure_no_canvas()
{
	FIGURE **fig, *tmp;

	printf("\n[%s : line %d] delete_figure_no_canvas()", __FILE__, __LINE__);

	/* In case the figure list is empty */
	if (figure_head == NULL) {
		g_print("\n--<delete_figure_no_canvas> : Figure list empty");
		return 0;
	}

	/* Look up from the list then delete */
	for (fig = &figure_head; (*fig); fig = &((*fig)->next))
	{
		if ( (*fig)->pixmap == pixmap_cur ) 
		{			
			tmp = *fig;
			*fig = tmp->next;					// reconnect 

			remove_curves( tmp->curve_list ); 	// delete the curves

			g_object_unref( tmp->pixmap ); 		// release the pixmap 

			/*g_print("\ndelete_figure_no_canvas(): Figure (%d) deleted",
							tmp->number);*/

			free( (FIGURE *) tmp );
			return 1;	/* in list */
		}
	}

	printf("\n[%s : line %d] delete_figure_no_canvas() : Figure not in the list",
						__FILE__, __LINE__);
	return 0;
}

/* This function is for the purpose of debugging. */
void print_figure_list(FIGURE *fig)
{
	printf("\nprint_figure_list() ");
	while (fig) 
	{
		printf("-> fig (%d) ", fig->number);
		fig = fig->next;
	}
	fflush(stdout);
}

/* Remove all space allocated to the curves in the figure
 * to be destroyed. 
 */
void remove_curves(CURVE_NODE *curve_list)
{
	CURVE_NODE 	*cn;
	CURVE 		*curve;

	printf("\n[%s : line %d] remove_curves(): removing all curves",
					__FILE__, __LINE__); 

	// Remove all curves first
	cn = curve_list; 
	while ( curve_list )
	{ 
		/* move to next before the current node is deleted */
		curve_list = curve_list->next; 
		curve = cn->curve;

		ASSERT2(curve)

		free( (double *) curve->vecx );
		free( (double *) curve->vecy );
		free( (CURVE_NODE *) cn);
		cn = curve_list;
	}

	printf("\n[%s : line %d] remove_curves(): done", __FILE__, __LINE__);
}


/* Add a curve to the specified figure. 
 */
void add_curve(FIGURE *fig, double *vecx, double *vecy, int len)
{
	CURVE_NODE *cn, *new_cn;
	CURVE 	*curve;
	int		i;

	//printf("\n<add_curve> : adding a curve ...");  fflush(stdout);

	/* Make a new curve first */
	curve = (CURVE *) malloc(sizeof(CURVE));
	curve->vecx = (double *) malloc(len*sizeof(double));
	curve->vecy = (double *) malloc(len*sizeof(double));
	curve->len = len;

	/* copy the vectors */
	for (i = 0; i < len; i++) {
		curve->vecx[i] = vecx[i];
		curve->vecy[i] = vecy[i];
	}

	/* Then make a new curve node */
	new_cn = (CURVE_NODE *) malloc(sizeof(CURVE_NODE));
	new_cn->curve = curve;
	new_cn->next = NULL;

	/* Link the new curve node to the end of the curve_list */
	if (!fig->curve_list) {
		fig->curve_list = new_cn;	
		//printf("\n<add_curve> : added a curve at head");  fflush(stdout);
		return;
	}
	/* Move <cn> to the end of the curve list */
	for (cn = fig->curve_list; cn->next; cn = cn->next);
	
	cn->next = new_cn;

	//printf("\n<add_curve> : added a curve at tail");  fflush(stdout);
}


/* Plot curves to the specified figure <fig>
 * This function is called under the following situations:
 * (1) A new curve is to be plotted.
 * (2) The plot window is resized.
 * (3) The axes are reset to new ranges.
 */

void plot_curves(FIGURE *fig)
{
	gint		PW, PH;		/* current canvas width-height in pixels */
	int		px, py, px1, py1, px2, py2, dotsize = 6;
	int		i;
	double	xx_min, xx_max;  // range of x-coordinates
	double	yy_min, yy_max;  // range of x-coordinates
	double	WW, HH;		  // width and height for the real window 

	
	GtkWidget *draw_area;

	GdkGC		*gc;
	CURVE 		*curve;
	CURVE_NODE 	*cn;

	ASSERT2(fig)
	// ASSERT2 defined in "GRASS_GUI/src/gui.h"

	if (!fig->curve_list)  return;  /* nothing to plot */

#if 0
	printf("\n<plot_curves> : plotting all curves to Figure (%d)",
					fig->number);
	fflush(stdout);
#endif

	draw_area = fig->draw_area;
	ASSERT2(draw_area)

	draw_coordinates(fig, &xx_min, &xx_max, &yy_min, &yy_max);

	fig->xmax = xx_max;	fig->xmin = xx_min;
	fig->ymax = yy_max;	fig->ymin = yy_min;

	/* Get the width/height of the current canvas */
  	PW = get_canvas_width (draw_area);
  	PH = get_canvas_height(draw_area);

	/* set the real window max width & max height */
	//WW = fig->xmax - fig->xmin;	
	//HH = fig->ymax - fig->ymin; 	
	WW = xx_max - xx_min;	
	HH = yy_max - yy_min; 	

	gc = draw_area->style->black_gc;

	/* Plot all curves in the current figure */
	ASSERT2(pixmap_cur)

	for (cn = fig->curve_list; cn; cn = cn->next)
	{ 
		curve = cn->curve;
		ASSERT2(curve);

		// Plot line segments
		px1 = transform_x(curve->vecx[0], xx_min, PW, WW);
		py1 = transform_y(curve->vecy[0], yy_max, PH, HH);

		for (i = 1; i < curve->len; i++) {
			px2 = transform_x(curve->vecx[i], xx_min, PW, WW);
			py2 = transform_y(curve->vecy[i], yy_max, PH, HH);

			gdk_draw_line(pixmap_cur, gc, px1, py1, px2, py2);
			px1 = px2;	py1 = py2;
		}
	}

	//printf("\n<plot_curves> : done");	fflush(stdout);
}

/* Destroy the active plotting window. 
 * To be called by the [Close] command in the [File] menu.
 */

void destroy_plotting_window(FIGURE **figure)
{
	GtkWidget	*toplevel;
  	GdkDrawable *canvas = (*figure)->canvas;

	//g_print("\n--<destroy_plotting_window> : canvas = %p", canvas);

	toplevel = gtk_widget_get_toplevel( (*figure)->draw_area );
	gtk_widget_destroy ( GTK_WIDGET (toplevel) );

	delete_figure_by_canvas( canvas );

	/* Assign the 1st figure in the figure list to be the current figure.*/
	figure_cur = figure_head;
}

/* To be called when the plotting window is closed by
 * clicking "X" or choosing "Close" from the top window edge.
 * This function is needed to make sure that the <FIGURE> node
 * created for the destroyed plotting window is deleted.
 */

void destroy_plotting_window2()
{
	/* Delete the currently active window without knowing the canvas. */
	delete_figure_no_canvas();

	print_figure_list(figure_head);

	/* Assign the 1st figure in the figure list to be the current figure.*/
	figure_cur = figure_head;
}

/*==== Below are functions for processing the plotting window geometry ===*/

int	get_canvas_width(GtkWidget *widget)
{
	return widget->allocation.width;
}


int	get_canvas_height(GtkWidget *widget)
{
	return widget->allocation.height;
}


/* Get the minimum of the given vector. */
double	get_vector_min(DOUBLE_VECTOR vv)
{
	double	min;
	int		i;

	min = vv.val[0];
	for (i = 1; i < vv.len; i++) {
		if (min > vv.val[i])	min = vv.val[i];
	}
	return min;
}

/* Get the maximum of the given vector. */
double	get_vector_max(DOUBLE_VECTOR vv)
{
	double	max;
	int		i;

	max = vv.val[0];
	for (i = 1; i < vv.len; i++) {
		if (max < vv.val[i])	max = vv.val[i];
	}
	return max;
}

/* Transform double coordinate to pixel, given 
 * 		dW = real window width
 * 		pW = pixel window width
 */
int	 transform_x(double xx, double xmin, int pW, double dW)
{
	int		px;

	px = (int) ((xx - xmin) * (double) pW / dW);
	return 	px;
}

/* Transform double coordinate to pixel, given 
 * 		dH = real window height
 * 		pH = pixel window height
 */
int	 transform_y(double yy, double ymax, int pH, double dH)
{
	int		py;

	py = (int) ((ymax - yy) * (double) pH / dH);
	return 	py;
}

/**** A quick sort function ********************************/


#define SWAP(a, m, n) { int t; t = a[m]; a[m] = a[n]; a[n] = t;}

void quicksort( int *a, int low, int high )
{
  	int pivot;
  	
  	if ( high > low )
    {
    	pivot = partition( a, low, high );
    	quicksort( a, low, pivot-1 );
    	quicksort( a, pivot+1, high );
    }
}

int partition( int *a, int low, int high )
{
  	int left, right, pivot;
  	int pivot_item;

  	pivot_item = a[low];
  	pivot = left = low;
  	right = high;
  	while ( left < right ) {
    	while( a[left]  <= pivot_item && left < high)    left++;
    	while( a[right] >  pivot_item && low  < right)   right--;
    	if ( left < right ) 	SWAP(a, left, right);
    }
  	a[low] = a[right];
  	a[right] = pivot_item;

  	return right;
}

