/**	
 	gtk_wave_viewer.c 
 	Neural Network Designer project 
 	since 2017 - 2017 (initiated by Shi, Guoyong) 
 
 	Mixed-Signal Design Automation Lab (07-01-2017)	
 **/


/*	wave_viewer.c
 *	Provides a list of variables available for plotting after simulation.
 *	(c) Guoyong Shi, SJTU
 *	05-01-2008
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include "gui.h"

// Constants used in XSPICE "Circuit/Node.h"
#define SP_VOLTAGE 3
#define SP_CURRENT 4

enum
{
	COL_0 = 0,
	COL_1,
	COL_2,
	NUM_COLS
} ;

static int		width0 = 250;		/* reference window size */
static int 	anal_height = 80;  	/* analysis window size */
static int 	node_height = 300;  	/* wave window size */


// Get the analysis clicked.
void  select_analysis(GtkTreeView        *treeview,
                       GtkTreePath        *path,
                       GtkTreeViewColumn  *col,
                       gpointer            userdata)
{
	GtkTreeModel *model;
	GtkTreeIter   	iter;
	int			index;		// the analysis index
	int			type, num;	// the analysis type & num
	Anal_Info	*anal_list = (Anal_Info *) userdata;

	model = gtk_tree_view_get_model(treeview);

	if (gtk_tree_model_get_iter(model, &iter, path))
	{
		gtk_tree_model_get(model, &iter, COL_0, &index, -1);

		type = anal_list[index-1].analtype;
		num  = anal_list[index-1].analnum;
		
		run_analysis(type, num);
		
		//g_print ("\nSelected node [%d] with name \"%s\" to plot.", ndnum, name);
		//strcpy(variable_to_plot, name);
	}
}

// Get the node name clicked.
void  select_node(GtkTreeView        *treeview,
                       GtkTreePath        *path,
                       GtkTreeViewColumn  *col,
                       gpointer            userdata)
{
	GtkTreeModel *model;
	GtkTreeIter   	iter;
	int			node;	// the node number selected to plot

	model = gtk_tree_view_get_model(treeview);

	if (gtk_tree_model_get_iter(model, &iter, path))
	{
		gchar *name;

		gtk_tree_model_get(model, &iter, COL_0, &node, COL_1, &name, -1);

		plot_node(node);
		
		//g_print ("\nSelected node [%d] with name \"%s\" to plot.", ndnum, name);
		//strcpy(variable_to_plot, name);

		g_free(name);
	}
}


static GtkTreeModel* create_analysis_tree (Anal_Info *anal_list, int len)
{
	GtkListStore  *store;
	GtkTreeIter    iter;
	int			i;

	//store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
	store = gtk_list_store_new (2, G_TYPE_UINT, G_TYPE_STRING);

	/* Append a row and fill in some data */
	// Skip node 0 because it is GND.
	for (i = 0; i < len; i++) 
	{
		gtk_list_store_append (store, &iter);

		gtk_list_store_set (store, &iter, 
	                  COL_0, (i+1),
	                  COL_1, anal_list[i].analname, -1);
	}

	return GTK_TREE_MODEL (store);
}

static GtkTreeModel* create_node_tree (Node_Info *nodelist, int len)
{
	GtkListStore  *store;
	GtkTreeIter    iter;
	int			i;

	//store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
	store = gtk_list_store_new (NUM_COLS, G_TYPE_UINT, 
						G_TYPE_STRING, G_TYPE_STRING);

	/* Append a row and fill in some data */
	// Skip node 0 because it is GND.
	for (i = 1; i < len; i++) 
	{
		gtk_list_store_append (store, &iter);

		if (nodelist[i].nodetype == SP_VOLTAGE) {
			gtk_list_store_set (store, &iter, 
		                  COL_0, nodelist[i].nodenum,
		                  COL_1, nodelist[i].nodename, 
		                  COL_2, "VOLTAGE", -1);
		} else if (nodelist[i].nodetype == SP_CURRENT) {
			gtk_list_store_set (store, &iter, 
		                  COL_0, nodelist[i].nodenum,
		                  COL_1, nodelist[i].nodename, 
		                  COL_2, "CURRENT", -1);
		} else {
			gtk_list_store_set (store, &iter, 
		                  COL_0, nodelist[i].nodenum,
		                  COL_1, nodelist[i].nodename, 
		                  COL_2, NULL, -1);
		}
	}

	return GTK_TREE_MODEL (store);
}


static GtkWidget* create_analysis_view (Anal_Info *anal_list, int len)
{
	GtkTreeViewColumn   *col;
	GtkCellRenderer     *renderer;
	GtkTreeModel        *model;
	GtkWidget           *view;

	view = gtk_tree_view_new ();

	g_signal_connect(view, "row-activated", 
						(GCallback) select_analysis, (gpointer) anal_list);


	/* --- Column #0 --- */

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                           -1,      
	                                           "Analysis #",  
	                                           renderer,
	                                           "text", COL_0,
	                                           NULL);
	/* --- Column #1 --- */

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                           -1,      
	                                           "Analysis Type",  
	                                           renderer,
	                                           "text", COL_1,
	                                           NULL);

	model = create_analysis_tree (anal_list, len);

	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

	g_object_unref (model); /* destroy model automatically with view */

	return view;
}

static GtkWidget* create_node_view (Node_Info *nodelist, int len)
{
	GtkTreeViewColumn   *col;
	GtkCellRenderer     *renderer;
	GtkTreeModel        *model;
	GtkWidget           *view;

	view = gtk_tree_view_new ();

	g_signal_connect(view, "row-activated", 
						(GCallback) select_node, NULL);


	/* --- Column #0 --- */

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                           -1,      
	                                           "Node #",  
	                                           renderer,
	                                           "text", COL_0,
	                                           NULL);
	/* --- Column #1 --- */

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                           -1,      
	                                           "Node Name",  
	                                           renderer,
	                                           "text", COL_1,
	                                           NULL);

	/* --- Column #2 --- */

	//col = gtk_tree_view_column_new();
	gtk_tree_view_column_new();

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                           -1,      
	                                           "Node Type",  
	                                           renderer,
	                                           "text", COL_2,
	                                           NULL);

	model = create_node_tree (nodelist, len);

	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

	g_object_unref (model); /* destroy model automatically with view */

	return view;
}


int Wave_Viewer (Circuit_Info ckt_info)
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *frame;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *scrollwin;
	GtkWidget *view;

	int		entry_length = 200;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_title (GTK_WINDOW(window), "Wave Viewer");

	g_signal_connect(window, "delete_event", 
				G_CALLBACK(gtk_widget_destroy), NULL); 

	// Make a vbox container.
	vbox = gtk_vbox_new (FALSE, 0);
	/* (homogeneous, spacing) */
	gtk_container_add (GTK_CONTAINER (window), vbox);
	//gtk_widget_show (vbox);

	// Create a frame to hold a label and an entry.
	frame = gtk_frame_new(NULL);
	gtk_container_set_border_width(GTK_CONTAINER(frame), 1); 

	// Create an hbox to hold a label and an entry.
	hbox = gtk_hbox_new (FALSE, 0); /* spacing = 0 */
	gtk_container_set_border_width(GTK_CONTAINER(hbox), 0); 
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	
	/* create a label and an entry */
	label = gtk_label_new(" Circuit: ");
	entry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (entry), entry_length);

	printf("\n\n[%s : line %d] Circuit title is: %s", 
					__FILE__, __LINE__, ckt_info.ckttitle);
					
	gtk_entry_set_text (GTK_ENTRY (entry), ckt_info.ckttitle);
	gtk_editable_set_editable (GTK_EDITABLE (entry), FALSE);
	// Set the text entry not editable.

	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER (vbox), frame);

	// Create a srollwin for analysis list.
	scrollwin = gtk_scrolled_window_new(NULL, NULL);

	view = create_analysis_view(ckt_info.analysislist, ckt_info.numanalyses);

	gtk_widget_set_size_request(view, width0, anal_height);

	gtk_container_add(GTK_CONTAINER(scrollwin), view);
	gtk_container_add(GTK_CONTAINER (vbox), scrollwin);

	// Create a srollwin for analysis list.
	scrollwin = gtk_scrolled_window_new(NULL, NULL);

	view = create_node_view(ckt_info.nodelist, ckt_info.numnodes);

	gtk_widget_set_size_request(view, width0, node_height);

	gtk_container_add(GTK_CONTAINER(scrollwin), view);
	gtk_container_add(GTK_CONTAINER (vbox), scrollwin);

	gtk_widget_show_all(window);

	return 0;
}
