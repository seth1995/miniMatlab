/**	
 	gtk_plot.h 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/*	plot.h 
 *	The OASIS Project
 *	(c) Guoyong Shi, Shanghai Jiao Tong University
 *	06-03-2007
 */


#include 	<stdio.h>
#include 	<gtk/gtk.h>

#include "gui.h"

// Define a CURVE structure
// The CURVE structure is for storing two equal-length vectors.
typedef struct _curve 	CURVE;

struct _curve {
		double  *vecx;
		double  *vecy;
		int		len;
};

// Define a list of curves for storing past plotted curves.
// Will be used for redrawing when needed.

typedef struct _curvenode 	CURVE_NODE;

struct _curvenode {
	CURVE 	*curve;
	CURVE_NODE	*next;
};

CURVE_NODE *curve_list;


/* Define structure for handling multiple pixmaps */
typedef struct _figure 	FIGURE;

struct _figure
{
	int			number;		/* figure number (1, 2, ...) */
	GtkWidget	*draw_area;	/* canvas = draw_area->window */
	GdkDrawable	*canvas;	/* one canvas for one pixmap */
	GdkPixmap	*pixmap;	/* has to be released if FIGURE destroyed */
	CURVE_NODE	*curve_list;  /* a list of curves plotted */
	double		xmin;
	double		ymin;		/* coordinate limits of the figure */
	double		xmax;
	double		ymax;
	FIGURE		*next;
};


/* A list of FIGUREs */
extern FIGURE *figure_head;
extern FIGURE *figure_cur;
extern GdkPixmap *pixmap_cur;  /* the active pixmap */

/* Functions defined in "draw_util.c" are related to 
 * the drawing device (here GTK), including interfaces
 * between <GdkDrawable> and <FIGURE>. 
 */

void new_plotting_window(int *num);

gint configure_event( GtkWidget *widget, GdkEventConfigure *event, 
				gpointer data);
gint expose_event( GtkWidget *widget, GdkEventExpose *event, 
				gpointer data);
gint button_press_event( GtkWidget *widget, GdkEventButton *event);
gint motion_notify_event( GtkWidget *widget, GdkEventMotion *event);

void clear_canvas(FIGURE **figure);

void draw_brush( GtkWidget *widget, gdouble x, gdouble y);
void clear_drawing_canvas(GtkWidget *widget);
void destroy_drawing_window(GtkWidget *widget);

/* Functions defined in "plot.c" are related to plotting and
 * figure management. 
 */

void plot(DOUBLE_VECTOR x, DOUBLE_VECTOR y);

FIGURE*		create_figure(int num);
FIGURE* 		get_figure_by_canvas(GdkDrawable *canvas);
FIGURE* 		get_figure_by_number(int n);
int 			assign_figure_number();
void 		insert_figure(FIGURE *fig_in);
gboolean 	delete_figure_by_canvas(GdkDrawable *canvas);
gboolean 	delete_figure_no_canvas();

void print_figure_list(FIGURE *fig);
void request_figure(int n);
void destroy_plotting_window(FIGURE **figure);
void destroy_plotting_window2();

void add_curve(FIGURE *fig, double *vecx, double *vecy, int len);
void plot_curves(FIGURE *fig);
void remove_curves(CURVE_NODE *curve_list);

int	 get_canvas_width(GtkWidget *widget);
int	 get_canvas_height(GtkWidget *widget);
int	 transform_x(double xx, double xmin, int pW, double dW);
int	 transform_y(double yy, double ymax, int pH, double dH);

double	get_vector_min(DOUBLE_VECTOR vv);
double	get_vector_max(DOUBLE_VECTOR vv);

void quicksort( int *a, int low, int high );
int   partition( int *a, int low, int high );

/*---- Functions defined in "coordinates.c" ----------------*/

void  draw_coordinates(FIGURE *figure, double *xx_min, double *xx_max,
			double *yy_min, double *yy_max);
void  write_text_at(int x, int y, GdkGC *gc, const char *text);

