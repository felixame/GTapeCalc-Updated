/***************************************************************************
                          gtc_callbacks.c  -  callbacks and various others
                             -------------------
    begin                : Tue Oct 26 1999
    copyright            : (C) 1999 by Stephen Witkop
    email                : switk@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#include "gtc_calc.h"
#include "gtc_callbacks.h"
#include "gtc_ui.h"
#include "gtc_dialogs.h"
#include "gtc_prefs.h"
#include "gtc_utils.h"
#include "gtc_print.h"
#include "../config.h"

static GtkWidget *current_widget;
static gchar     *current_file = '\0';
static gchar     *current_dir = '\0';
static gint       last_row = 0;

void entry_backspace (GtkWidget *widget);
void entry_show_keypress(GtkWidget *widget, gchar *num);

/* basic app exit *********************************************************/
void close_app(GtkWidget *widget)
{
  if (!delete_event (widget))
  	gtk_main_quit();
}

/* toggles the visibility of the column titles *****************************/
void column_toggle (GtkWidget *widget)
{
  static gint state = 0;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  if (state) {
    g_object_set (G_OBJECT(cw->clist), "headers-visible", FALSE, NULL);
    state = 0;
  }
  else{
    g_object_set (G_OBJECT(cw->clist), "headers-visible", TRUE, NULL);
    state = 1;
  }
  cw->widget_vars->column_title_show = state;
}

/* sets the number of decimal places to output ******************************/
void dec_select(GtkWidget *widget, char *n)
{
  /*CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");
  gint i = atoi(n);

  if (GTK_CHECK_MENU_ITEM(widget)->active) {
    if (!cw->state->misc_state) {
      if (i < - 1) {
        dlg_preferences_show (cw->window);
      }
      else cw->widget_vars->precision = i;
    }
    else cw->state->misc_state = 0;
  }*/
}

/* quit function that checks for changed files ****************************/
gint delete_event(GtkWidget *widget)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  if (strcmp(current_file,"Untitled")) {
    if (!cw->state->file_saved) {
      file_save_before_exit_cb (widget);

      return (TRUE);
    }
  }

  g_free (cw->widget_vars);
  gtk_main_quit ();

  return (FALSE);
}

/* toggle the floating point option in the preferences dialog *************/
void fp_toggle (GtkWidget *widget, gpointer data)
{
  GtkWidget *window = (GtkWidget *)data;
  GtkWidget *spinb = g_object_get_data (window, "precision_spin");
  gboolean state;

  state = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget));
  if (state) {
      gtk_widget_set_sensitive (spinb, FALSE);
  }
  else gtk_widget_set_sensitive (spinb, TRUE);
}

/* set whether the current file is saved or not *****************************/
void set_file_saved (CalcWindow *cw, gint state)
{
  cw->state->file_saved = state;
}

/* sets current_file to "name" **********************************************/
void set_current_file (gchar *name)
{
  if (!current_file)
    current_file = (char *) malloc((strlen(name) + 1) * sizeof(char));
  else
    current_file = (char *) realloc(current_file,
				    (strlen(name) + 1) * sizeof(char));
  if (!current_file) {
    perror ("gtapecalc-set_current_file");
    exit (1);
  }
  strcpy(current_file, name);
}

/* keeps track of the current working dir */
void set_current_dir ()
{
  gchar *temp_dir;

  if (current_dir) {
    g_free (current_dir);
    temp_dir = g_path_get_dirname (current_file);
  }
  else
    temp_dir = g_get_current_dir ();

  current_dir = g_malloc0 (strlen(temp_dir) + 2);
  sprintf (current_dir, "%s/", temp_dir);
  /*g_message ("current directory is %s", current_dir);*/
  g_free (temp_dir);
}

/* open a file_selection dialog and calls file_event **********************/
void file_select(GtkWidget *widget, char *mode)
{
  /*GtkWidget *fs;
  GtkWidget *twindow = get_toplevel_widget (widget);

  if (!strcmp(mode, "r")) {
    fs = gtk_file_selection_new ("Open File");
    g_signal_connect_swapped (GTK_OBJECT(GTK_FILE_SELECTION(fs)->ok_button),
	    "clicked", G_CALLBACK(file_open),
	    GTK_OBJECT(fs));
    if (current_file) {
      gtk_file_selection_set_filename (GTK_FILE_SELECTION(fs), current_dir);
    }
  }
  else {
    fs = gtk_file_selection_new ("Save File As");
    g_signal_connect_swapped (GTK_OBJECT(GTK_FILE_SELECTION(fs)->ok_button),
	    "clicked", G_CALLBACK(file_save_as),
	    GTK_OBJECT(fs));

    if (current_file) {
      gtk_file_selection_set_filename(GTK_FILE_SELECTION(fs), current_dir);
      gtk_file_selection_set_filename(GTK_FILE_SELECTION(fs), current_file);
      gtk_entry_select_region (GTK_ENTRY(GTK_FILE_SELECTION(fs)->selection_entry),
                               0, strlen(current_file));
    }
  }

  gtk_window_set_modal (GTK_WINDOW(fs), TRUE);
  gtk_window_set_transient_for (GTK_WINDOW(fs),
                                GTK_WINDOW(twindow));

  g_signal_connect (GTK_OBJECT(fs), "destroy",
		      G_CALLBACK(gtk_widget_destroy),
		      GTK_OBJECT(fs));

  g_signal_connect_swapped (GTK_OBJECT(GTK_FILE_SELECTION(fs)->cancel_button),
			     "clicked", G_CALLBACK(gtk_widget_destroy),
			     GTK_OBJECT(fs));

  g_object_set_data (GTK_OBJECT(fs), "twindow", twindow);
  gtk_widget_show (fs);

  if (!strcmp(mode, "r")) {
    CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

    if ((!cw->state->file_saved) && (strcmp(current_file,"Untitled"))){

      ButtonItem file_save_buttons[] = {
        {NULL, GTK_STOCK_SAVE, G_CALLBACK(file_save_before_close), TRUE},
        {NULL, GTK_STOCK_NO,   G_CALLBACK(destroy_dialog),         FALSE},
      };
      DialogItem file_save_items = {
        NULL,
        300, 150,
        TRUE,
        "Save ?",
        GTK_STOCK_DIALOG_QUESTION,
        " File has changed since last save \n\n	Save Changes?",
        sizeof (file_save_buttons) / sizeof (ButtonItem)
      };

      dialog_new (widget, &file_save_items, file_save_buttons);
    }
  }*/
}


/* opens a file *************************************************************/
void file_open(GtkWidget *widget, gpointer data)
{
  /* const gchar *temp; */
  /* gchar *fn = 0; */
  /* FILE *fname; */
  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* if (GTK_IS_FILE_SELECTION(widget )) { */
  /*   temp = gtk_file_selection_get_filename(GTK_FILE_SELECTION(widget)); */
  /*   fn = g_strdup (temp); */
  /* } */
  /* else */
  /* 	fn = g_strdup((gchar *)data); */

  /* if (!fn) { */
  /*   g_warning ("No file name"); */
  /*   return; */
  /* } */

  /* fname = fopen(fn, "r"); */

  /* if (fname) { */

  /*   gint row = 0; */
  /*   char buff[3][256]; */
  /*   char *buffer[3]; */
  /*   gchar *index; */

  /*   new_tape_event(cw->clist); */

  /*   while (fgets (buff[0], 256, fname)) { */
  /*     buffer[0] = buff[0]; */
  /*     if ((index = strchr (buff[0], '\n'))) */
		/* 	*index = 0; */

  /*     fgets (buff[1], 256, fname); */
  /*     buffer[1] = buff[1]; */
  /*     if ((index = strchr (buff[1], '\n'))) */
		/* 	*index = 0; */

  /*     fgets (buff[2], 256, fname); */
  /*     buffer[2] = buff[2]; */
  /*     if ((index = strchr (buff[2], '\n'))) */
		/* 	*index = 0; */

  /*     row = tlist_append (cw->clist, buffer); */
  /*   } */

  /*   last_row = row; */
  /*   set_current_file(fn); */
  /*   set_current_dir (); */
  /*   statusbar_update(widget, current_file, ""); */
  /* } */
  /* else { */
  /*   gchar *err_msg; */

  /*   err_msg = malloc((strlen(fn) + 32) * sizeof(char)); */
  /*   sprintf (err_msg, "Couldn't open file -\n%s",fn); */
  /*   dlg_error_msg (widget, err_msg); */
  /*   free (err_msg); */

  /*   printf("Couldn't open file %s\n", fn); */
  /* } */

  /* set_file_saved (cw, TRUE); */

  /* if (fn) */
  /*   g_free (fn); */

  /* if (GTK_IS_FILE_SELECTION(widget )) */
  /*   gtk_widget_destroy (GTK_WIDGET(widget)); */
}

/* void callback for file save event ****************************************/
void file_save_event_cb (GtkWidget *widget, gchar *data)
{
  file_save_event (widget, data);
}

/* check to see if a file exists before saving to it ************************/
void file_save_as (GtkWidget *widget)
{
 /*  FILE *fname; */
 /*  const gchar *tname; */
 /*  gchar *tempname; */
 /*  GtkWidget *window = get_data_from_toplevel (widget, "twindow"); */

 /*  tname = gtk_file_selection_get_filename(GTK_FILE_SELECTION(widget)); */
 /*  tempname = g_strdup (tname); */
 /*  fname = fopen(tempname, "r"); */

 /*  if (fname) { */
 /*    GtkWidget *dialog; */
 /*    gchar *message; */

 /*    fclose (fname); */
 /*    g_warning ("File %s already exists", tempname); */
 /*    message = g_strdup_printf ("%s\n already exists\n\nOverwrite the existing file ?", */
	/*     tempname); */
 /*    if (message) { */
 /*      DialogItem dlg_items = { */
	/* NULL, */
	/* 300, 150, */
	/* TRUE, */
	/* "Warning!", */
	/* GTK_STOCK_DIALOG_WARNING, */
	/* message, */
	/* 2 */
 /*      }; */
 /*      ButtonItem dlg_buttons[] = { */
	/* {NULL, GTK_STOCK_OK, G_CALLBACK(file_save_event_cb), FALSE}, */
	/* {NULL, GTK_STOCK_CANCEL, G_CALLBACK(destroy_dialog), TRUE} */
 /*      }; */
 /*      dialog = dialog_new (widget, &dlg_items, dlg_buttons); */
 /*      g_object_set_data (dialog, "gfree_data", tempname); */
 /*      g_free (message); */
 /*      gtk_widget_destroy (widget); */
 /*      return; */
 /*    } */
 /*  } */
 /*  else { */
 /*    file_save_event (window, tempname); */
 /*  } */
 /*  g_free (tempname); */
 /*  gtk_widget_destroy (widget); */
}


/* saves an opened file *****************************************************/
int file_save_event(GtkWidget *widget, gchar *data)
{
  gchar *name = NULL;
  FILE *fname;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  if (GTK_IS_DIALOG(widget)) {
    name = g_object_get_data (widget, "gfree_data");
    if (!name)
      name = g_strdup (current_file);
    fname = fopen(name, "w");
    gtk_widget_destroy (GTK_WIDGET(widget));
    if (fname) {
      set_current_file(name);
      set_current_dir();
      /*g_free (name);*/
    }
  }
  else if (data){
    name = g_strdup (data);
    fname = fopen(name, "w");
    if (fname) {
      set_current_file(name);
      set_current_dir();
      /*g_free (name);*/
    }
  }
  else if (strcmp(current_file,"Untitled")) {
    fname = fopen(current_file, "w");
  }
  else {
    file_select(widget, "w");
    return(0);
  }

  if (!fname) {
    gchar *err_msg;
    char *str_err;
    int error;

    error = errno;
    perror ("gtapecalc-file_save_event");
    str_err = strerror (error);

    if (name) {
      err_msg = malloc((strlen(str_err) + strlen(name) + 32) * sizeof(char));
      if (err_msg) {
	sprintf (err_msg, "Couldn't save file \n%s\n\n(%s)",
		 name, str_err);
	dlg_error_msg (cw->window, err_msg);
  	free (err_msg);
      }
      else
  	perror ("gtapecalc-file_save_event/malloc");

    }
    else {
      err_msg = malloc((strlen(str_err) + strlen(current_file) + 32) * sizeof(char));
      if (err_msg) {
	sprintf (err_msg, "Couldn't save file \n%s\n\n(%s)",
		 current_file, str_err);
	dlg_error_msg (cw->window, err_msg);
  	free (err_msg);
      }
      else
  	perror ("gtapecalc-file_save_event/malloc");
    }

    if (name)
      g_free (name);

    return (1);
  }
  else {
    char *buffer;
    gint row = 0, x;

    while (row <= last_row) {
      for (x = 0; x < 3; x++) {
	tlist_get_text (cw->clist, row, x, &buffer);
	fputs (buffer, fname);
	fputc ('\n', fname);
  g_free (buffer);
      }
      row++;
    }

    fclose(fname);
    set_file_saved (cw, TRUE);
    statusbar_update (cw->clist, current_file, "saved");
    if (name)
      g_free (name);
  }
  return(0);
}

/* prompts for the user to save before closing ******************************/
void file_close_cb (GtkWidget *widget)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  if ((!cw->state->file_saved) && (strcmp(current_file,"Untitled"))){
    ButtonItem file_save_buttons[] = {
      {NULL, GTK_STOCK_SAVE,   G_CALLBACK(file_save_before_close), FALSE},
      {NULL, GTK_STOCK_NO,     G_CALLBACK(new_tape_event),         FALSE},
      {NULL, GTK_STOCK_CANCEL, G_CALLBACK(destroy_dialog),         TRUE},
    };

    DialogItem file_save_items = {
      NULL,
      300, 150,
      TRUE,
      "Save?",
      GTK_STOCK_DIALOG_QUESTION,
      " File has changed since last save \n\n Save Changes?  ",
      sizeof(file_save_buttons) / sizeof(ButtonItem)
    };

    dialog_new (widget, &file_save_items, file_save_buttons);
  }
  else new_tape_event (widget);
}

/* saves file, then clears the display **************************************/
void file_save_before_close (GtkWidget *widget)
{
  int ret_val;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  ret_val = file_save_event (widget, NULL);
  if (ret_val == 0) {
    set_current_file("Untitled");
    statusbar_update (cw->window, current_file, "");
    new_tape_event (cw->window);
  }
}

/* checks if file needs to be saved before exiting **************************/
void file_save_before_exit_cb (GtkWidget *widget)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  if (!cw->state->file_saved) {
    ButtonItem file_save_buttons[] = {
      {NULL, GTK_STOCK_SAVE,   G_CALLBACK(file_save_before_exit), FALSE},
      {NULL, GTK_STOCK_NO,     gtk_main_quit,             FALSE},
      {NULL, GTK_STOCK_CANCEL, G_CALLBACK(destroy_dialog),         TRUE},
    };

    DialogItem file_save_items = {
      NULL,
      300, 150,
      TRUE,
      "Save?",
      GTK_STOCK_DIALOG_QUESTION,
      " File has changed since last save \n\n Save Changes?  ",
      sizeof(file_save_buttons) / sizeof(ButtonItem)
    };

    dialog_new (widget, &file_save_items, file_save_buttons);
  }
  else
    gtk_main_quit ();
}
/* saves file, then exit ****************************************************/
void file_save_before_exit (GtkWidget *widget)
{
  gint ret_val = 0;

  if (GTK_IS_DIALOG(widget)) {
    GtkWidget *twindow;

    twindow = g_object_get_data (widget, "twindow");
    ret_val = file_save_event (twindow, NULL);
    gtk_widget_destroy (GTK_WIDGET(widget));
  }
  else
    ret_val = file_save_event (widget, NULL);
  if (ret_val == 0)
    gtk_main_quit ();
}

/* Query the print dialog for options **************************************/
void print_file_cb (GtkWidget *widget, gpointer data)
{
  PrintOption *options;
  GtkWidget *rbutton;
  GtkWidget *entry;
  GtkWidget *window = get_data_from_toplevel (widget, "twindow");

  options = g_new0 (PrintOption, 1);

  if (strcmp ((char *) data, "preview") == 0)
    options->preview = 1;
  else options->preview = 0;

  rbutton = g_object_get_data (window, "printer_button");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rbutton))) {
    entry = g_object_get_data (rbutton, "entry");
    options->print_command = g_strdup (gtk_entry_get_text (GTK_ENTRY(entry)));
    options->to_file = 0;
  }

  rbutton = g_object_get_data (window, "print_file_button");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rbutton))) {
    entry = g_object_get_data (rbutton, "entry");
    options->to_file_name = g_strdup (gtk_entry_get_text (GTK_ENTRY(entry)));
    options->to_file = 1;
  }

  rbutton = g_object_get_data (window, "no_header_button");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rbutton)))
    options->print_header = 0;
  else options->print_header = 1;

  if (options->print_header) {
    rbutton = g_object_get_data (window, "custom_header_button");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(rbutton))) {
      entry = g_object_get_data (rbutton, "entry");
      options->header = g_strdup (gtk_entry_get_text (GTK_ENTRY(entry)));
      options->custom_header = 1;
    }
    else
      options->header = g_strdup (g_path_get_basename (current_file));
  }
  entry = g_object_get_data (window, "page_width_entry");
  options->page_width = atoi (gtk_entry_get_text (GTK_ENTRY(entry)));
  entry = g_object_get_data (window, "column_width_entry");
  options->column_width = atoi (gtk_entry_get_text (GTK_ENTRY(entry)));

  print_file (window, options);
}

// TODO: Cleanup this function. It's hard to tell where each conditional ends.
/* print clist to a temp file, then to lpr *********************************/
void print_file(GtkWidget *widget, gpointer data)
{
  gchar *temp_file;
  FILE *fname;
  PrintOption *opts = data;

  gint column_width;
  gint page_width;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  page_width = opts->page_width * 2;     /* multiply the widths to   */
  column_width = opts->column_width * 2; /* to account for the fonts */
  if (column_width > (page_width * .75))
    column_width = (page_width * .75);

  if (opts->to_file)
    temp_file = opts->to_file_name;
  else {
    char temp_name[L_tmpnam];

    // Changed tmpnam() to g_mkdtemp().
    temp_file = g_mkdtemp(temp_name);
  }

  fname = fopen(temp_file, "w"); if (fname) {

    char *buffer = "";
    char command[strlen(temp_file) + 8];
    gchar padded_string[page_width + 1];
    gint padding;
    gint row = 0;
    gint lines = 0;
    gint page = 1;
    gint x, y;

    print_ps_header (fname);

    if (opts->print_header) {
      print_header (fname, opts->header, page);
    }
    else
      print_header (fname, "", page);

    while (row <= last_row) {

      if (lines > 40) {
        print_footer (fname, page);
        page++;

	      if (opts->print_header)
	        print_header (fname, opts->header, page);
	      else
	        print_header (fname, "", page);

        print_start_pos (fname);
        lines = 0;
      }

      /* write comment cells */
      tlist_get_text (cw->clist, row, 0, &buffer);

      x = y = 0;
      memset(padded_string, '\0', sizeof(padded_string));

      padding = (column_width) - strlen(buffer);

      /* multi-line comment */
      if (padding < 0){
	    gchar line[strlen(buffer) + 1];

	    strcpy(line, buffer);

	    while (x <= strlen(line)) {
	      strncpy(padded_string, &line[x], column_width);
	      x += column_width;

	      /* full line, wrap last word */
	      if (strlen(padded_string) == column_width) {

	        y = strlen(padded_string);

	        /* if there's a space at the end, don't bother */
	        while (padded_string[y] != ' ' &&
		            padded_string[y + 1] != ' ' &&
		            y > 0 &&
		            strchr(padded_string, ' ')) {
	          y--;
	          x--;
	        }

	        if (y < sizeof(padded_string) && y > 0)
	          padded_string[y + 1] = '\0';

	        if (y == 0)
	          x += column_width;

	        print_l_column (fname, padded_string);
	        fputs ("nline ", fname);
	        lines++;
	      }
	      else {
	        print_l_column (fname, padded_string);
	      }
      }
    }

    // Single line comment
    else {
	    print_l_column (fname, buffer);
    }
    g_free (buffer);

    /* write output cells */
    tlist_get_text (cw->clist, row, 1, &buffer);

    print_rs_column (fname, buffer);
    g_free (buffer);

    /* write operator cells */
    tlist_get_text (cw->clist, row, 2, &buffer);

    print_r_column (fname, buffer);
    g_free (buffer);

    row++;
    lines++;
  }

  print_footer (fname, page);
  fclose(fname);

  if (!opts->to_file && !opts->preview) {
    sprintf(command, "%s %s", opts->print_command, temp_file);
    system(command);
    g_free (opts->print_command);
  }
  if (opts->preview) {
#ifdef GSVIEWER
    sprintf (command, "%s %s", GSVIEWER , temp_file);
    system (command);
#endif
  }
  else
    statusbar_update (widget, current_file, " printed");
  }
  // TODO: What's going on here?
  else {
    gchar *err_msg;
    char *str_err;
    int error;

    error = errno;
    perror ("gtapecalc-print_file()");
    str_err = strerror (error);

    err_msg = g_strdup_printf ("I had a problem printing.\nThe error was - \n %s\n", str_err);
    dlg_error_msg (widget, err_msg);
    g_free (err_msg);
  }

  if (!opts->preview) {
    if (opts->print_header)
      g_free (opts->header);
    g_free (opts);
    gtk_widget_destroy (GTK_WIDGET(widget));
  }
}


/* updates the statusbar ****************************************************/
void statusbar_update(GtkWidget *widget, gchar *open_file, gchar *text)
{
  gint statusbar_id;
  gchar buff[256];
  static gint last_push;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  statusbar_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(cw->statusbar),
	  "Statusbar");


  if (last_push)
    gtk_statusbar_remove (GTK_STATUSBAR(cw->statusbar),
			  GPOINTER_TO_INT(statusbar_id), last_push);
  sprintf(buff, "<%s> %s", open_file, text);
  gtk_statusbar_push (GTK_STATUSBAR(cw->statusbar),
		      GPOINTER_TO_INT(statusbar_id), buff);
  last_push++;
}

/* clears the variables and the output window *******************************/
void new_tape_event(GtkWidget *widget)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  clear(widget, NULL);
  tlist_clear(cw->clist);

  set_current_file ("Untitled");
  statusbar_update(widget, current_file, "");

  set_file_saved (cw, TRUE);

  if (GTK_IS_DIALOG(widget))
    gtk_widget_destroy (GTK_WIDGET(widget));

}

/* callback for when a row is selected **************************************/
void select_row (GtkTreeView *tree, GtkTreePath *row)
{
  GtkTreeViewColumn *first_column;
  GList *list = gtk_tree_view_get_columns (tree);

  list = g_list_first (list);
  first_column = (GtkTreeViewColumn *)list->data;

  gtk_tree_view_set_cursor (tree, row, first_column, TRUE);

  g_list_free (list);
}

/* remove a row in the output window ****************************************/
void remove_row (GtkWidget *widget)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  tlist_remove_row (cw->clist);
  last_row--;

  statusbar_update (widget, current_file, " not saved");
  set_file_saved (cw, FALSE);

  if (GTK_IS_DIALOG(widget))
      gtk_widget_destroy (GTK_WIDGET(widget));
}

/* insert a blank row into the output window ********************************/
void insert_row (GtkWidget *widget)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  tlist_insert_row (cw->clist, cw->selected_row);
  last_row++;

  statusbar_update (widget, current_file, " not saved");
  set_file_saved (cw, FALSE);
}

/* inserts a new comment into the output window *****************************/
void insert_comment(GtkWidget *widget)
{
  const gchar *new_comment;
  GtkWidget *dlg_entry;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  dlg_entry = g_object_get_data (widget, "entry");

  new_comment = gtk_entry_get_text (GTK_ENTRY(dlg_entry));

  tlist_set_text (cw->clist, cw->selected_row, 0, new_comment);
  gtk_entry_set_text (GTK_ENTRY(dlg_entry), "");
  statusbar_update(widget, current_file, " not saved");
  gtk_widget_destroy (GTK_WIDGET(widget));
  set_file_saved (cw, FALSE);
}

/* remove comment from the output window ************************************/
void remove_comment (GtkWidget *widget)                                                          //
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  tlist_set_text (cw->clist, cw->selected_row, 0, "");
  statusbar_update(widget, current_file, " not saved");
  set_file_saved (cw, FALSE);
}

// TODO: Is this called anywhere?
/* dummy handler ************************************************************/
void de_fault(GtkWidget *widget)
{
  gchar *temp_str;

  printf("default event triggered\n");
  printf ("User name - %s\n", g_get_user_name());
  printf ("Real name - %s\n", g_get_real_name());
  printf ("Home dir - %s\n", g_get_home_dir());
  printf ("Temp dir - %s\n", g_get_tmp_dir());
  printf ("Program name - %s\n", g_get_prgname());

  temp_str = g_get_current_dir ();
  printf ("Current dir - %s\n", temp_str);
  g_free (temp_str);

  temp_str = g_path_get_dirname (current_file);
  printf ("CWD - %s\n", temp_str);
  g_free (temp_str);
}

/* register if mouse is over a button ***************************************/
gint button_enter(GtkWidget *widget)
{
  current_widget = widget;
  return(FALSE);
}

/* update current_button ****************************************************/
gint button_leave (GtkWidget *widget)
{
  current_widget = NULL;
  return(FALSE);
}

/* Check to see if the key pressed corresponds to a button ******************/
guint key_press_event (GtkWidget *widget, GdkEventKey *event)
{
  ButtonType btype;
  GtkWidget *focus_widget;

  focus_widget = gtk_window_get_focus (GTK_WINDOW(widget));

  if (GTK_IS_ENTRY(focus_widget)) {
    return FALSE;
  }

  btype =  key_event (widget, event->keyval, 1);

  if (btype >= BTN_ZERO && btype < BTN_NONE) {
    key_press_button (widget, btype, 1);
  }
  return (FALSE); /* Changed to FALSE for gtk2 */
}

/* Check the key that was released ******************************************/
guint key_release_event(GtkWidget *widget, GdkEventKey *event)
{
  ButtonType btype;
  GtkWidget *focus_widget;

  focus_widget = gtk_window_get_focus (GTK_WINDOW(widget));

  if (GTK_IS_ENTRY(focus_widget)) {
    return FALSE;
  }

  btype =  key_event (widget, event->keyval, 0);
  if (btype >= BTN_ZERO && btype < BTN_NONE) {
    key_press_button (widget, btype, 0);
  }

  return (FALSE);  /* Changed to FALSE for gtk2 */
}

/* Transfer a keypress into a button press **********************************/
void key_press_button (GtkWidget *widget, ButtonType button, guint is_pressed)
{
  CalcButton *button_list;
  gint nbuttons;
  gint index;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  button_list = cw->button_list;
  nbuttons = cw->nbuttons;

  for (index = 0; index < nbuttons; index++) {
    if (button == button_list[index].type) {

      if (is_pressed) {
	      if (button_list[index].widget != current_widget) {
	        gtk_button_enter (GTK_BUTTON(button_list[index].widget));
	        gtk_button_clicked (GTK_BUTTON(button_list[index].widget));
	      }
	      gtk_button_pressed (GTK_BUTTON(button_list[index].widget));
      }
      else {
	      gtk_button_released (GTK_BUTTON(button_list[index].widget));

	      if (button_list[index].widget != current_widget) {
	        gtk_button_leave (GTK_BUTTON(button_list[index].widget));
        }
      }
    }
  }
}

/* process keystrokes from the entry widget *********************************/
int key_event(GtkWidget *widget, gint keyval, gint is_pressed)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  switch (keyval) {
      case GDK_KEY_parenleft:
	  return (BTN_PAREN_LEFT);
    case GDK_KEY_parenright:
	  return (BTN_PAREN_RIGHT);
    case GDK_KEY_BackSpace:
	  if (is_pressed)
	    entry_backspace(widget);
	  return (-1);
    case GDK_KEY_Delete:
	  if (is_pressed)
	    gtk_label_set_text (GTK_LABEL(cw->entry), "");
	  return (-1);
    case GDK_KEY_KP_Decimal:
	  return(BTN_DEC_POINT);
    case GDK_KEY_KP_0:
	  return (BTN_ZERO);
    case GDK_KEY_KP_1:
	  return (BTN_ONE);
    case GDK_KEY_KP_2:
	  return (BTN_TWO);
    case GDK_KEY_KP_3:
	  return (BTN_THREE);
    case GDK_KEY_KP_4:
	  return (BTN_FOUR);
    case GDK_KEY_KP_5:
	  return (BTN_FIVE);
    case GDK_KEY_KP_6:
	  return (BTN_SIX);
    case GDK_KEY_KP_7:
	  return (BTN_SEVEN);
    case GDK_KEY_KP_8:
	  return (BTN_EIGHT);
    case GDK_KEY_KP_9:
	  return (BTN_NINE);
    case GDK_KEY_0:
	  return (BTN_ZERO);
    case GDK_KEY_1:
	  return (BTN_ONE);
    case GDK_KEY_2:
	  return (BTN_TWO);
    case GDK_KEY_3:
	  return (BTN_THREE);
    case GDK_KEY_4:
	  return (BTN_FOUR);
    case GDK_KEY_5:
	  return (BTN_FIVE);
    case GDK_KEY_6:
	  return (BTN_SIX);
    case GDK_KEY_7:
	  return (BTN_SEVEN);
    case GDK_KEY_8:
	  return (BTN_EIGHT);
    case GDK_KEY_9:
	  return (BTN_NINE);
    case GDK_KEY_period:
	  return (BTN_DEC_POINT);
    case GDK_KEY_KP_Enter:
	  return (BTN_EQUAL);
    case GDK_KEY_Return:
	  return (BTN_EQUAL);
    case GDK_KEY_equal:
	  return (BTN_EQUAL);
    case GDK_KEY_KP_Add:
	  return (BTN_PLUS);
    case GDK_KEY_plus:
	  return (BTN_PLUS);
    case GDK_KEY_KP_Subtract:
	  return (BTN_MINUS);
    case GDK_KEY_minus:
	  return (BTN_MINUS);
    case GDK_KEY_KP_Multiply:
	  return (BTN_MULTIPLY);
    case GDK_KEY_asterisk:
	  return (BTN_MULTIPLY);
    case GDK_KEY_KP_Divide:
	  return (BTN_DIVIDE);
    case GDK_KEY_slash:
	  return (BTN_DIVIDE);
    case GDK_KEY_percent:
	  return (BTN_PERCENT);
    case GDK_KEY_A:
	  return (BTN_CLEAR_ALL);
    case GDK_KEY_a:
	  return (BTN_CLEAR_ALL);
    case GDK_KEY_C:
	  return (BTN_CLEAR_ENTRY);
    case GDK_KEY_c:
	  return (BTN_CLEAR_ENTRY);
    default:
	  return (-1);
  }
}


/* outputs calculations to the clist ****************************************/
void display_out(GtkWidget *widget, char *output, char *op)
{
  gchar *text[3] = {"",output, op};
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  last_row = tlist_append (cw->clist, text);

  statusbar_update (widget, current_file, " not saved");
  set_file_saved (cw, FALSE);
}

/* deletes the last number from the entry label *****************************/
void entry_backspace (GtkWidget *widget)
{
  gchar *current_num;
  gchar *ret_num;
  gchar new_num[MAX_STRING];
  gint len;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  current_num = gtk_label_get_text (GTK_LABEL(cw->entry));
  strcpy(new_num, current_num);

  len = strlen(new_num);
  if (new_num[len-1] == '(')
    cw->state->in_sub = 0;
  if (new_num[len-1] == ')') {
    cw->state->in_sub = 1;
    cw->state->have_sub = 0;
    cw->state->clear_entry = 0;
  }
  if (new_num[len-1] == 32) {   /* check for an operand */
    int x = len - 2;

    while (new_num[x] != 32 && x > 0)
      x--;
    new_num[x] = '\0';
  }
  else
    new_num[len - 1] = '\0';

  strip_commas (new_num);
  ret_num = add_commas (new_num);

  gtk_label_set_text (GTK_LABEL(cw->entry), ret_num);
  free (ret_num);
}

/* clear the entry **********************************************************/
void entry_clear (GtkWidget *widget)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  gtk_label_set_text (GTK_LABEL(cw->entry), "");
  cw->state->in_sub = 0;
  cw->state->have_sub = 0;
}

/* puts the numeric keys pressed into the entry label ***********************/
void entry_show_keypress(GtkWidget *widget, gchar*num)
{
  gchar *current_num;
  gchar *index;
  gchar new_num[MAX_ENTRY];
  gchar *num_to_show;
  gint len;

  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  if (cw->state->clear_entry) {
    gtk_label_set_text(GTK_LABEL(cw->entry), "");
    cw->state->clear_entry = 0;
  }

  current_num = gtk_label_get_text (GTK_LABEL(cw->entry));
  if (strlen(current_num) < sizeof(new_num))
    strcpy(new_num, current_num);
  else {
    fprintf (stderr,
	     "error:(callbacks.c (1115)strlen of current num is - %d\n",
	     (int)strlen(current_num));
    return;
  }

  if (strchr(new_num, ',') && !cw->state->in_sub)
    strip_commas (new_num);

  len = strlen(new_num);
  index = &new_num[len];
  strcpy (index, num);

  if (!cw->state->in_sub)
    num_to_show = add_commas (new_num);

  gtk_label_set_text (GTK_LABEL(cw->entry), "");
  if (!cw->state->in_sub) {
    gtk_label_set_text (GTK_LABEL(cw->entry), num_to_show);
    free (num_to_show);
  }
  else gtk_label_set_text (GTK_LABEL(cw->entry), new_num);
}

void entry_show_total(GtkWidget *widget, double total)
{
  gchar totalstr[MAX_STRING];
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  format_num(total, totalstr, cw->widget_vars->precision);

  gtk_label_set_text (GTK_LABEL(cw->entry), "");
  gtk_label_set_text (GTK_LABEL(cw->entry), totalstr);
  cw->state->clear_entry = 1;
}

/* callback for buttons *****************************************************/

void button_press(GtkWidget *widget, ButtonType *data)
{
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  if (cw->state->have_sub) {
    parse_grouped_input (widget, *data);
    cw->state->have_sub = 0;

    return;
  }

  if (*data == BTN_PAREN_LEFT) {
  	gtk_label_set_text (GTK_LABEL(cw->entry), "");
    cw->state->in_sub = 1;
  }

  if ((*data >= BTN_ZERO && *data <= BTN_DEC_POINT) ||
       cw->state->in_sub == 1) {
    gchar *numb = get_button_data (cw->button_list, *data);

    entry_show_keypress (widget, numb);
  }
  else {
    parse_main_input(widget, *data);
  }

  if (*data == BTN_PAREN_RIGHT){
    cw->state->in_sub = 0;
    cw->state->have_sub = 1;
    cw->state->clear_entry = 1;
  }

  gtk_widget_grab_focus(GTK_WIDGET(cw->clist));
}

/* load the options from the preferences dialog ****************************/
void load_dialog_options (GtkWidget *widget)
{
  /*GList *toplevels;
  const gchar *tname;
  gchar *name;
  gint i;
  GList *container;
  GtkWidget *label;
  GtkWidget *temp_widget;
  GtkWidget *data_widget = get_data_from_toplevel (widget, "twindow");
  CalcWindow *cw = get_data_from_toplevel (data_widget, "cwindow");

  data_widget = g_object_get_data (GTK_OBJECT(widget), "precision_spin");
  i = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(data_widget));

  data_widget = g_object_get_data (GTK_OBJECT(widget), "fp_checkbutton");
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(data_widget))) {
    temp_widget = gtk_item_factory_get_widget (cw->item_factory,
	    "/View/Floating Point");
    gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(temp_widget),
	    TRUE);
  }
  else {
    temp_widget = gtk_item_factory_get_widget (cw->item_factory,
	    "/View/Custom");

    if (!GTK_CHECK_MENU_ITEM(temp_widget)->active)
      cw->state->misc_state = 1;

    gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(temp_widget),
	    TRUE);
    cw->widget_vars->precision = i;
  }

  data_widget = g_object_get_data (GTK_OBJECT(widget), "show_headers");
  temp_widget = gtk_item_factory_get_widget (cw->item_factory, "/View/Show Headers");
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data_widget))) {
    if (!cw->widget_vars->column_title_show) {
      gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(temp_widget),
	      TRUE);
    }
  }
  else {
    if (cw->widget_vars->column_title_show) {
      gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(temp_widget),
	      FALSE);
    }
  }

  data_widget = g_object_get_data (GTK_OBJECT(widget), "save_win_check");
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data_widget))) {
    gint x, y;

    gdk_window_get_size (cw->window->window, &x, &y);
    cw->widget_vars->win_x_size = x;
    cw->widget_vars->win_y_size = y;
    gdk_window_get_size (cw->entry->window, &x, &y);
    cw->widget_vars->pane_width = (x + 14);
  }

  data_widget = g_object_get_data (GTK_OBJECT(widget), "entry_font");
  tname = gtk_entry_get_text (GTK_ENTRY(data_widget));

  if (name[0] != '\0') {
  	name = g_strdup (tname);
    cw->widget_vars->entry_font_name =
      config_load_var (cw->widget_vars->entry_font_name, name);
    widget_font_load (cw->entry, cw->widget_vars->entry_font_name);

    g_free (name);
  }

  data_widget = g_object_get_data (GTK_OBJECT(widget), "tape_font");
  tname = gtk_entry_get_text (GTK_ENTRY(data_widget));
  if (tname[0] != '\0') {
    name = g_strdup (tname);
    cw->widget_vars->clist_font_name = config_load_var
      (cw->widget_vars->clist_font_name, name);
    widget_font_load (cw->clist, cw->widget_vars->clist_font_name);

    g_free (name);
  }
  else {
    if (cw->widget_vars->clist_font_name) {
      free (cw->widget_vars->clist_font_name);
      cw->widget_vars->clist_font_name = NULL;
      widget_font_load (cw->clist, NULL);
    }
  }

  data_widget = g_object_get_data (GTK_OBJECT(widget), "button_font");
  tname = gtk_entry_get_text (GTK_ENTRY(data_widget));
  if (tname[0] != '\0') {


  name = g_strdup (tname);
  cw->widget_vars->button_font_name = config_load_var
    (cw->widget_vars->button_font_name, name);
  for (i = 0; i < cw->nbuttons; i++) {
    container = gtk_container_children (GTK_CONTAINER(cw->button_list[i].widget));
    label = GTK_WIDGET(container->data);
    widget_font_load (label, cw->widget_vars->button_font_name);
    //style = gtk_style_copy (gtk_widget_get_style (cw->window));
    //style->font = font;
    //gtk_widget_set_style (GTK_WIDGET(label), style);

    gtk_widget_show (GTK_WIDGET(cw->button_list[i].widget));
  }
  //}
  //else g_warning ("Couldn't load button font %s\nusing default gtk font instead",
  //                cw->widget_vars->clist_font_name);
  g_free (name);
  }

  else {
    if (cw->widget_vars->button_font_name) {
      free (cw->widget_vars->button_font_name);
    }
  }

  data_widget = g_object_get_data (GTK_OBJECT(widget), "print_command");
  tname = gtk_entry_get_text (GTK_ENTRY(data_widget));
  if (tname) {
    name = g_strdup (tname);
    cw->widget_vars->print_command = config_load_var
      (cw->widget_vars->print_command, name);
    g_free (name);
  }
  else {
    g_warning ("Print Command was NULL");
    cw->widget_vars->print_command = PRINT_COMMAND;
  }

  data_widget = g_object_get_data (GTK_OBJECT(widget), "page_width");
  i = atoi (gtk_entry_get_text(GTK_ENTRY(data_widget)));
  cw->widget_vars->page_width = i;

  data_widget = g_object_get_data (GTK_OBJECT(widget), "column_width");
  i = atoi (gtk_entry_get_text(GTK_ENTRY(data_widget)));
  cw->widget_vars->column_width = i;

  config_write_rc_file (cw->widget_vars);

  if (gtk_rc_reparse_all ())
    {
      toplevels = gdk_window_get_toplevels();

      while (toplevels) {
	GtkWidget *widget;

	gdk_window_get_user_data (toplevels->data, (gpointer *)&widget);

	if (widget)
	  gtk_widget_reset_rc_styles (widget);

      toplevels = toplevels->next;
      }
      g_list_free (toplevels);
    }

  destroy_dialog (widget, NULL);*/
}

/* Determine in clist mouse clicks ******************************************/
guint mouse_click_check (GtkWidget *widget, GdkEventButton *event,
			gpointer data)
{
  if (event->button == 3) {
    gtk_menu_popup(GTK_MENU(data), NULL, NULL, NULL, NULL,
    event->button, event->time);
    return (FALSE);
  }
  return (FALSE);
}

//TODO: Get this working on Gnome 3.
void show_help (GtkWidget *widget)
{
#ifdef GNOME_HELP_BROWSER
  gchar *command;

  command = g_strdup_printf ("%s %sindex.html &", GNOME_HELP_BROWSER, HELP_DIR);
  system (command);
  g_free (command);
#endif

#ifdef NETSCAPE
  gchar *command;

  command = g_strdup_printf ("%s -remote 'OpenFile(%sindex.html,new_window)'",
			     NETSCAPE, HELP_DIR);
  if (system (command)) {
    g_free (command);
    command = g_strdup_printf ("%s -dont-save-geometry-prefs -geometry =500x500+1+1 %sindex.html &",
                               NETSCAPE, HELP_DIR);
    system (command);
  }
  g_free (command);
#endif

#ifdef LYNX
  gchar *command;

  command = g_strdup_printf ("xterm -geometry 100x34 -e %s %sindex.html &",
                             LYNX, HELP_DIR);
  system (command);
  g_free (command);
#endif

#ifdef NO_BROWSER
  g_warning ("No browser found on this system");
#endif
}

/* append a row to the end of the output list */
gint tlist_append (GtkWidget *tree, gchar **buffer)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gint count = 1; /* start at 1, initial row is #0, this is the next row */

  model = gtk_tree_view_get_model (GTK_TREE_VIEW(tree));
  if (gtk_tree_model_get_iter_first (model, &iter)) {

    while (gtk_tree_model_iter_next (model, &iter)) {
      count++;
    }

      gtk_list_store_insert (GTK_LIST_STORE(model), &iter, count);
      gtk_list_store_set (GTK_LIST_STORE(model), &iter,
                          COLUMN_NOTE, buffer[0],
                          COLUMN_NUMBER, buffer[1],
                          COLUMN_OPERATOR, buffer[2], -1);
    }
  else {
    gtk_list_store_append (GTK_LIST_STORE(model), &iter);
    gtk_list_store_set (GTK_LIST_STORE(model), &iter,
                        COLUMN_NOTE, buffer[0],
                        COLUMN_NUMBER, buffer[1],
                        COLUMN_OPERATOR, buffer[2], -1);
    count = 0;
  }
  tlist_scroll_to_row (GTK_TREE_VIEW(tree), model, &iter);

  /*g_message ("Add, last_row (count) is: %d", count);*/

  return count;
}
/* set the text in the specified cell *****************************************/
void tlist_set_text (GtkWidget *tlist, gint row, gint column, const gchar *text)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *row_str = g_strdup_printf ("%d", row);

  model = gtk_tree_view_get_model (GTK_TREE_VIEW(tlist));

  if (gtk_tree_model_get_iter_from_string (model, &iter, row_str)) {
    gtk_list_store_set (GTK_LIST_STORE(model), &iter,
                        column, text, -1);
    g_free (row_str);
  }
}

/* retrieve the text from the specified cell *****************************************/
/* TODO  make sure all pointers to '**buffer' are freed */
void tlist_get_text (GtkWidget *tlist, gint row, gint column, gchar **buffer)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *row_str = g_strdup_printf ("%d", row);

  model = gtk_tree_view_get_model (GTK_TREE_VIEW(tlist));

  if (gtk_tree_model_get_iter_from_string (model, &iter, row_str)) {
    gtk_tree_model_get (model, &iter,
                        column, buffer, -1);
    g_free (row_str);
  }
}

/* clear the list *********************************************/
void tlist_clear (GtkWidget *tlist)
{
  GtkTreeModel *model;

  model = gtk_tree_view_get_model (GTK_TREE_VIEW(tlist));
  gtk_list_store_clear (GTK_LIST_STORE(model));

  last_row = 0;
}

/* remove a row in the list ***********************************/
void tlist_remove_row (GtkWidget *tlist)
{
  GtkTreeModel *model;
  GtkTreeIter   iter;
  GtkTreeSelection *select;

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tlist));

  if (gtk_tree_selection_get_selected (select, &model, &iter)) {
    gtk_list_store_remove (GTK_LIST_STORE(model), &iter);
  }
}

/* insert a blank row in the list */
void tlist_insert_row (GtkWidget *tlist, gint row)
{
  GtkTreeModel *model;
  GtkTreeIter iter;

  model = gtk_tree_view_get_model (GTK_TREE_VIEW(tlist));

  gtk_list_store_insert (GTK_LIST_STORE(model), &iter, row);
  gtk_list_store_set (GTK_LIST_STORE(model), &iter,
                      COLUMN_NOTE, "",
		      COLUMN_NUMBER, "",
		      COLUMN_OPERATOR, "", -1);
}

/* scroll to the specified row */
void tlist_scroll_to_row (GtkTreeView *tree, GtkTreeModel *model, GtkTreeIter *iter)
{
  GtkTreePath *row_path;

  row_path = gtk_tree_model_get_path (model, iter);
  gtk_tree_view_scroll_to_cell (tree, row_path,
				NULL, TRUE, 1, 0);
  /*gtk_tree_view_set_cursor (tree, row_path, NULL, FALSE);*/
}

void
cell_changed (gchar *arg1, gchar *arg2, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeView *tree = data;

  model = gtk_tree_view_get_model (GTK_TREE_VIEW(tree));

  if (gtk_tree_model_get_iter_from_string (model, &iter, arg1)) {
    gtk_list_store_set (GTK_LIST_STORE(model), &iter,
			COLUMN_NOTE, arg2, -1);
  }
}

/* Keep track of the currently selected row **************************/
 void selection_changed (GtkTreeSelection *select, gpointer data)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gint count = 0;
  CalcWindow *cw = data;

  if (gtk_tree_selection_get_selected (select, &model, &iter))
    {
      while (gtk_tree_model_iter_next (model, &iter)) {
	      ++count;
      }
      cw->selected_row = last_row - count;

      /*g_print ("The selected row was: %d\n", cw->selected_row);*/
    }
}
