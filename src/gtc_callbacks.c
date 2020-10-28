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
#include "gtc_prefs.h"
#include "gtc_utils.h"
#include "gtc_print.h"
#include "gtc_window.h"
#include "../config.h"

static GtkWidget *current_widget;
static gchar     *current_file = '\0';
static gchar     *current_dir = '\0';
static gint       last_row = 0;

/* toggles the visibility of the column titles *****************************/
void column_toggle (GtkWidget *widget)
{
  /* static gint state = 0; */
  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* if (state) { */
  /*   g_object_set (G_OBJECT(cw->clist), "headers-visible", FALSE, NULL); */
  /*   state = 0; */
  /* } */
  /* else{ */
  /*   g_object_set (G_OBJECT(cw->clist), "headers-visible", TRUE, NULL); */
  /*   state = 1; */
  /* } */
  /* cw->widget_vars->column_title_show = state; */
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
  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* if (strcmp(current_file,"Untitled")) { */
  /*   if (!cw->state->file_saved) { */
  /*     file_save_before_exit_cb (widget); */

  /*     return (TRUE); */
  /*   } */
  /* } */

  /* g_free (cw->widget_vars); */
  /* gtk_main_quit (); */

  /* return (FALSE); */
}

/* toggle the floating point option in the preferences dialog *************/
void fp_toggle (GtkWidget *widget, gpointer data)
{

}

/* set whether the current file is saved or not *****************************/
void set_file_saved (CalcWindow *cw, gint state)
{
  /* cw->state->file_saved = state; */
}

/* sets current_file to "name" **********************************************/
void set_current_file (gchar *name)
{
  /* if (!current_file) */
  /*   current_file = (char *) malloc((strlen(name) + 1) * sizeof(char)); */
  /* else */
  /*   current_file = (char *) realloc(current_file, */
		/* 		    (strlen(name) + 1) * sizeof(char)); */
  /* if (!current_file) { */
  /*   perror ("gtapecalc-set_current_file"); */
  /*   exit (1); */
  /* } */
  /* strcpy(current_file, name); */
}

/* keeps track of the current working dir */
void set_current_dir ()
{
  /* gchar *temp_dir; */

  /* if (current_dir) { */
  /*   g_free (current_dir); */
  /*   temp_dir = g_path_get_dirname (current_file); */
  /* } */
  /* else */
  /*   temp_dir = g_get_current_dir (); */

  /* current_dir = g_malloc0 (strlen(temp_dir) + 2); */
  /* sprintf (current_dir, "%s/", temp_dir); */
  /*g_message ("current directory is %s", current_dir);*/
  /* g_free (temp_dir); */
}

/* open a file_selection dialog and calls file_event **********************/
void file_select(GtkWidget *widget, char *mode)
{

}


/* opens a file *************************************************************/
void file_open(GtkWidget *widget, gpointer data)
{

}

/* void callback for file save event ****************************************/
void file_save_event_cb (GtkWidget *widget, gchar *data)
{
  /* file_save_event (widget, data); */
}

/* check to see if a file exists before saving to it ************************/
void file_save_as (GtkWidget *widget)
{

}


/* saves an opened file *****************************************************/
int file_save_event(GtkWidget *widget, gchar *data)
{

}

/* prompts for the user to save before closing ******************************/
void file_close_cb (GtkWidget *widget)
{

}

/* saves file, then clears the display **************************************/
void file_save_before_close (GtkWidget *widget)
{

}

/* checks if file needs to be saved before exiting **************************/
void file_save_before_exit_cb (GtkWidget *widget)
{

}
/* saves file, then exit ****************************************************/
void file_save_before_exit (GtkWidget *widget)
{

}

/* Query the print dialog for options **************************************/
void print_file_cb (GtkWidget *widget, gpointer data)
{

}

// TODO: Cleanup this function. It's hard to tell where each conditional ends.
/* print clist to a temp file, then to lpr *********************************/
void print_file(GtkWidget *widget, gpointer data)
{
/*   gchar *temp_file; */
/*   FILE *fname; */
/*   PrintOption *opts = data; */

/*   gint column_width; */
/*   gint page_width; */
/*   CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

/*   page_width = opts->page_width * 2;      */ /* multiply the widths to   */
/*   column_width = opts->column_width * 2;  */ /* to account for the fonts */
/*   if (column_width > (page_width * .75)) */
/*     column_width = (page_width * .75); */

/*   if (opts->to_file) */
/*     temp_file = opts->to_file_name; */
/*   else { */
/*     char temp_name[L_tmpnam]; */

/*     // Changed tmpnam() to g_mkdtemp(). */
/*     temp_file = g_mkdtemp(temp_name); */
/*   } */

/*   fname = fopen(temp_file, "w"); if (fname) { */

/*     char *buffer = ""; */
/*     char command[strlen(temp_file) + 8]; */
/*     gchar padded_string[page_width + 1]; */
/*     gint padding; */
/*     gint row = 0; */
/*     gint lines = 0; */
/*     gint page = 1; */
/*     gint x, y; */

/*     print_ps_header (fname); */

/*     if (opts->print_header) { */
/*       print_header (fname, opts->header, page); */
/*     } */
/*     else */
/*       print_header (fname, "", page); */

/*     while (row <= last_row) { */

/*       if (lines > 40) { */
/*         print_footer (fname, page); */
/*         page++; */

/* 	      if (opts->print_header) */
/* 	        print_header (fname, opts->header, page); */
/* 	      else */
/* 	        print_header (fname, "", page); */

/*         print_start_pos (fname); */
/*         lines = 0; */
/*       } */

      /* write comment cells */
/*       tlist_get_text (cw->clist, row, 0, &buffer); */

/*       x = y = 0; */
/*       memset(padded_string, '\0', sizeof(padded_string)); */

/*       padding = (column_width) - strlen(buffer); */

      /* multi-line comment */
/*       if (padding < 0){ */
/* 	    gchar line[strlen(buffer) + 1]; */

/* 	    strcpy(line, buffer); */

/* 	    while (x <= strlen(line)) { */
/* 	      strncpy(padded_string, &line[x], column_width); */
/* 	      x += column_width; */

	      /* full line, wrap last word */
/* 	      if (strlen(padded_string) == column_width) { */

/* 	        y = strlen(padded_string); */

	        /* if there's a space at the end, don't bother */
/* 	        while (padded_string[y] != ' ' && */
/* 		            padded_string[y + 1] != ' ' && */
/* 		            y > 0 && */
/* 		            strchr(padded_string, ' ')) { */
/* 	          y--; */
/* 	          x--; */
/* 	        } */

/* 	        if (y < sizeof(padded_string) && y > 0) */
/* 	          padded_string[y + 1] = '\0'; */

/* 	        if (y == 0) */
/* 	          x += column_width; */

/* 	        print_l_column (fname, padded_string); */
/* 	        fputs ("nline ", fname); */
/* 	        lines++; */
/* 	      } */
/* 	      else { */
/* 	        print_l_column (fname, padded_string); */
/* 	      } */
/*       } */
/*     } */

/*     // Single line comment */
/*     else { */
/* 	    print_l_column (fname, buffer); */
/*     } */
/*     g_free (buffer); */

    /* write output cells */
/*     tlist_get_text (cw->clist, row, 1, &buffer); */

/*     print_rs_column (fname, buffer); */
/*     g_free (buffer); */

    /* write operator cells */
/*     tlist_get_text (cw->clist, row, 2, &buffer); */

/*     print_r_column (fname, buffer); */
/*     g_free (buffer); */

/*     row++; */
/*     lines++; */
/*   } */

/*   print_footer (fname, page); */
/*   fclose(fname); */

/*   if (!opts->to_file && !opts->preview) { */
/*     sprintf(command, "%s %s", opts->print_command, temp_file); */
/*     system(command); */
/*     g_free (opts->print_command); */
/*   } */
/*   if (opts->preview) { */
/* #ifdef GSVIEWER */
/*     sprintf (command, "%s %s", GSVIEWER , temp_file); */
/*     system (command); */
/* #endif */
/*   } */
/*   else */
/*     statusbar_update (widget, current_file, " printed"); */
/*   } */
/*   // TODO: What's going on here? */
/*   else { */
/*     gchar *err_msg; */
/*     char *str_err; */
/*     int error; */

/*     error = errno; */
/*     perror ("gtapecalc-print_file()"); */
/*     str_err = strerror (error); */

/*     err_msg = g_strdup_printf ("I had a problem printing.\nThe error was - \n %s\n", str_err); */
/* 		// FIXME: Fix error message dialog. */
/*     //dlg_error_msg (widget, err_msg); */
/*     g_free (err_msg); */
/*   } */

/*   if (!opts->preview) { */
/*     if (opts->print_header) */
/*       g_free (opts->header); */
/*     g_free (opts); */
/*     gtk_widget_destroy (GTK_WIDGET(widget)); */
/*   } */
}

/* updates the statusbar ****************************************************/
void statusbar_update(GtkWidget *widget, gchar *open_file, gchar *text)
{

}

/* clears the variables and the output window *******************************/
void new_tape_event(GtkWidget *widget)
{


}

/* callback for when a row is selected **************************************/
void select_row (GtkTreeView *tree, GtkTreePath *row)
{

}

/* remove a row in the output window ****************************************/
void remove_row (GtkWidget *widget)
{

}

/* insert a blank row into the output window ********************************/
void insert_row (GtkWidget *widget)
{

}

/* inserts a new comment into the output window *****************************/
void insert_comment(GtkWidget *widget)
{
  /* const gchar *new_comment; */
  /* GtkWidget *dlg_entry; */
  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* dlg_entry = g_object_get_data (widget, "entry"); */

  /* new_comment = gtk_entry_get_text (GTK_ENTRY(dlg_entry)); */

  /* tlist_set_text (cw->clist, cw->selected_row, 0, new_comment); */
  /* gtk_entry_set_text (GTK_ENTRY(dlg_entry), ""); */
  /* statusbar_update(widget, current_file, "•"); */
  /* gtk_widget_destroy (GTK_WIDGET(widget)); */
  /* set_file_saved (cw, FALSE); */
}

/* remove comment from the output window ************************************/
void remove_comment (GtkWidget *widget)                                                          //
{
  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* tlist_set_text (cw->clist, cw->selected_row, 0, ""); */
  /* statusbar_update(widget, current_file, "•"); */
  /* set_file_saved (cw, FALSE); */
}

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

// These next few functions handle the event where a key corresponding to a
// button action is pressed. None of the old code is relevant anymore as ui
// functionality which previously defined it is no longer being used.
//
// Key implementation concept: Instead of having separate event handlers for
// button presses and key presses, a key press should cause the button press
// event for the corresponding key.

/* Check to see if the key pressed corresponds to a button ******************/
guint key_press_event (GtkWidget *widget, GdkEventKey *event)
{

}

/* Check the key that was released ******************************************/
guint key_release_event(GtkWidget *widget, GdkEventKey *event)
{

}

/* Transfer a keypress into a button press **********************************/
void key_press_button (GtkWidget *widget, ButtonType button, guint is_pressed)
{

}

/* process keystrokes from the entry widget *********************************/
int key_event(GtkWidget *widget, gint keyval, gint is_pressed)
{
  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* switch (keyval) { */
  /*     case GDK_KEY_parenleft: */
	 /*  return (BTN_PAREN_LEFT); */
  /*   case GDK_KEY_parenright: */
	 /*  return (BTN_PAREN_RIGHT); */
  /*   case GDK_KEY_BackSpace: */
	 /*  if (is_pressed) */
	 /*    entry_backspace(widget); */
	 /*  return (-1); */
  /*   case GDK_KEY_Delete: */
	 /*  if (is_pressed) */
	 /*    gtk_entry_set_text (GTK_ENTRY(cw->entry), ""); */
	 /*  return (-1); */
  /*   case GDK_KEY_KP_Decimal: */
	 /*  return(BTN_DEC_POINT); */
  /*   case GDK_KEY_KP_0: */
	 /*  return (BTN_ZERO); */
  /*   case GDK_KEY_KP_1: */
	 /*  return (BTN_ONE); */
  /*   case GDK_KEY_KP_2: */
	 /*  return (BTN_TWO); */
  /*   case GDK_KEY_KP_3: */
	 /*  return (BTN_THREE); */
  /*   case GDK_KEY_KP_4: */
	 /*  return (BTN_FOUR); */
  /*   case GDK_KEY_KP_5: */
	 /*  return (BTN_FIVE); */
  /*   case GDK_KEY_KP_6: */
	 /*  return (BTN_SIX); */
  /*   case GDK_KEY_KP_7: */
	 /*  return (BTN_SEVEN); */
  /*   case GDK_KEY_KP_8: */
	 /*  return (BTN_EIGHT); */
  /*   case GDK_KEY_KP_9: */
	 /*  return (BTN_NINE); */
  /*   case GDK_KEY_0: */
	 /*  return (BTN_ZERO); */
  /*   case GDK_KEY_1: */
	 /*  return (BTN_ONE); */
  /*   case GDK_KEY_2: */
	 /*  return (BTN_TWO); */
  /*   case GDK_KEY_3: */
	 /*  return (BTN_THREE); */
  /*   case GDK_KEY_4: */
	 /*  return (BTN_FOUR); */
  /*   case GDK_KEY_5: */
	 /*  return (BTN_FIVE); */
  /*   case GDK_KEY_6: */
	 /*  return (BTN_SIX); */
  /*   case GDK_KEY_7: */
	 /*  return (BTN_SEVEN); */
  /*   case GDK_KEY_8: */
	 /*  return (BTN_EIGHT); */
  /*   case GDK_KEY_9: */
	 /*  return (BTN_NINE); */
  /*   case GDK_KEY_period: */
	 /*  return (BTN_DEC_POINT); */
  /*   case GDK_KEY_KP_Enter: */
	 /*  return (BTN_EQUAL); */
  /*   case GDK_KEY_Return: */
	 /*  return (BTN_EQUAL); */
  /*   case GDK_KEY_equal: */
	 /*  return (BTN_EQUAL); */
  /*   case GDK_KEY_KP_Add: */
	 /*  return (BTN_PLUS); */
  /*   case GDK_KEY_plus: */
	 /*  return (BTN_PLUS); */
  /*   case GDK_KEY_KP_Subtract: */
	 /*  return (BTN_MINUS); */
  /*   case GDK_KEY_minus: */
	 /*  return (BTN_MINUS); */
  /*   case GDK_KEY_KP_Multiply: */
	 /*  return (BTN_MULTIPLY); */
  /*   case GDK_KEY_asterisk: */
	 /*  return (BTN_MULTIPLY); */
  /*   case GDK_KEY_KP_Divide: */
	 /*  return (BTN_DIVIDE); */
  /*   case GDK_KEY_slash: */
	 /*  return (BTN_DIVIDE); */
  /*   case GDK_KEY_percent: */
	 /*  return (BTN_PERCENT); */
  /*   case GDK_KEY_A: */
	 /*  return (BTN_CLEAR_ALL); */
  /*   case GDK_KEY_a: */
	 /*  return (BTN_CLEAR_ALL); */
  /*   case GDK_KEY_C: */
	 /*  return (BTN_CLEAR_ENTRY); */
  /*   case GDK_KEY_c: */
	 /*  return (BTN_CLEAR_ENTRY); */
  /*   default: */
	 /*  return (-1); */
  /* } */
}

/* outputs calculations to the clist ****************************************/
void calc_list_push(char *output, char *op)
{
  /* gchar *text[3] = {"",output, op}; */
  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* last_row = tlist_append (cw->clist, text); */

  /* statusbar_update (widget, current_file, "•"); */
  /* set_file_saved (cw, FALSE); */
}

/* deletes the last number from the entry label *****************************/
void entry_backspace (GtkWidget *widget)
{
  const gchar *current_num;
  gchar *ret_num;
  gchar new_num[MAX_STRING];
  gint len;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  current_num = gtk_entry_get_text (cw->calcEntry);
  strcpy(new_num, current_num);

  len = strlen(new_num);
  if (new_num[len-1] == '(')
    cw->state->in_sub = 0;
  if (new_num[len-1] == ')') {
    cw->state->in_sub = 1;
    cw->state->have_sub = 0;
    cw->state->clear_entry = 0;
  }
  if (new_num[len-1] == 32) {    /* check for an operand */
    int x = len - 2;

    while (new_num[x] != 32 && x > 0)
      x--;
    new_num[x] = '\0';
  }
  else
    new_num[len - 1] = '\0';

  strip_commas (new_num);
  ret_num = add_commas (new_num);

  gtk_entry_set_text (cw->calcEntry, ret_num);
  free (ret_num);
}

// This function may no longer be relevant when key press -> button press
// functionality is implemented.
/* puts the numeric keys pressed into the entry label ***********************/
void entry_show_keypress(GtkWidget *widget, gchar*num)
{
  /* gchar *current_num; */
  /* gchar *index; */
  /* gchar new_num[MAX_ENTRY]; */
  /* gchar *num_to_show; */
  /* gint len; */

  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

  /* if (cw->state->clear_entry) { */
  /*   gtk_entry_set_text(GTK_ENTRY(cw->entry), ""); */
  /*   cw->state->clear_entry = 0; */
  /* } */

  /* current_num = gtk_entry_get_text (GTK_ENTRY(cw->entry)); */
  /* if (strlen(current_num) < sizeof(new_num)) */
  /*   strcpy(new_num, current_num); */
  /* else { */
  /*   fprintf (stderr, */
	 /*     "error:(callbacks.c (1115)strlen of current num is - %d\n", */
	 /*     (int)strlen(current_num)); */
  /*   return; */
  /* } */

  /* if (strchr(new_num, ',') && !cw->state->in_sub) */
  /*   strip_commas (new_num); */

  /* len = strlen(new_num); */
  /* index = &new_num[len]; */
  /* strcpy (index, num); */

  /* if (!cw->state->in_sub) */
  /*   num_to_show = add_commas (new_num); */

  /* gtk_entry_set_text (GTK_ENTRY(cw->entry), ""); */
  /* if (!cw->state->in_sub) { */
  /*   gtk_entry_set_text (GTK_ENTRY(cw->entry), num_to_show); */
  /*   free (num_to_show); */
  /* } */
  /* else gtk_entry_set_text (GTK_ENTRY(cw->entry), new_num); */
}

void
calc_entry_set_text (gchar *outputText)
{
/*   gchar totalstr[MAX_STRING]; */
/*   //gchar buff[MAX_STRING]; */
/*   CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */

/*   format_num(total, totalstr, cw->widget_vars->precision); */

/*   //sprintf(buff, "= %s", totalstr); */

/*   gtk_entry_set_text (GTK_ENTRY(cw->entry), ""); */
/*   gtk_entry_set_text (GTK_ENTRY(cw->entry), totalstr); */
/*   cw->state->clear_entry = 1; */
}

void
calc_entry_push_text (gchar *outputText)
{
	// Get reference to CalcWindow somehow.
	// Maybe move this to gtc_window like some kind of private function.
	//gtk_entry_set_text(CalcWindow->calcEntry, outputData);

}

void
calc_entry_clear ()
{
	// Get reference to CalcWindow somehow.
	// Maybe move this to gtc_window like some kind of private function.
	// somewhere #define CALC_ENTRY_STATE_CLEAR "0" or something.
	//gtk_entry_set_text(CalcWindow->calcEntry, CALC_ENTRY_STATE_CLEAR);
}

/* callback for buttons *****************************************************/

void
button_press (CalcMachine* calcMachine,
							ButtonType data)
{
	CalcWindow *calcWindow;
	gchar* calcEntryString;

	//gtk_entry_get_text(calcWidnow->entry, calcEntryString);

	if (data == BTN_CLEAR_ALL) {
		//clear (calcMachine);
		return;
	}

	if (data == BTN_CLEAR_ENTRY) {
		calc_machine_clear (calcMachine);
		return;
	}

	// gdk_screen_get_active_window() is depreciated. We'll need to find a proper
	// method for getting the window object globaly.
	//calcWindow = CALC_WINDOW(gdk_screen_get_active_window(gdk_screen_get_default()));

  /* CalcWindow *cw = get_data_from_toplevel (widget, "cwindow"); */
  /* const gchar *input = gtk_entry_get_text (GTK_ENTRY(cw->entry)); */

	// If there is a sub calculation that needs to be calculated, calculate it
	// and apply operation defined by data.
  if (cw->state->have_sub) {

    parse_grouped_input (data, calcEntryString);

    cw->state->have_sub = 0;
    return;
  }

	// If there is a left paren, we need to let gtc_calc know that the next
	// calculation is a sub calculation.
  if (data == BTN_PAREN_LEFT) {
  	//gtk_entry_set_text (GTK_ENTRY(cw->entry), "");
		calc_entry_clear ();
    cw->state->in_sub = 1;
  }

	// FIXME: There is an issue where in a grouped calculation, invalid characters
	// can be output to the CalcEntry. These characters are stripped out by the
	// parser so calculations remain correct, but they should be caught earlier
	// so they are not output.

  // If the button that is pressed is a numerical button or the
  // decimal point button call entry_show_keypress() to
  // append symbols onto the end of the current label in entry.
  // If the button is an operator, parse the text in entry.
  if ((data >= BTN_ZERO && data <= BTN_DEC_POINT) ||
       cw->state->in_sub == 1) {

    gchar *numb = get_button_data (cw->button_list, data);
		// entry_show_keypress () appends new characters onto the end of the old entry data.
    entry_show_keypress (widget, numb);
  }
  else {
    parse_main_input(data, calcEntryString);
  }

  // TODO: Fix condition where multiple ) button presses causes a crash.
	// If there is a right paren, the end of the sub calculation has been reached,
	// and there is a sub calculation that needs to be handled.
  if (data == BTN_PAREN_RIGHT){
    cw->state->in_sub = 0;
    cw->state->have_sub = 1;
    cw->state->clear_entry = 1;
  }

  /* gtk_widget_grab_focus(GTK_WIDGET(cw->clist)); */
}

/* load the options from the preferences dialog ****************************/
void load_dialog_options (GtkWidget *widget)
{

}

/* Determine in clist mouse clicks ******************************************/
guint mouse_click_check (GtkWidget *widget, GdkEventButton *event,
			gpointer data)
{

}

//TODO: Completely rewrite.
void show_help (GtkWidget *widget)
{

}

/* append a row to the end of the output list */
gint tlist_append (GtkWidget *tree, gchar **buffer)
{

}
/* set the text in the specified cell *****************************************/
void tlist_set_text (GtkWidget *tlist, gint row, gint column, const gchar *text)
{

}

/* retrieve the text from the specified cell *****************************************/
/* TODO  make sure all pointers to '**buffer' are freed */
void tlist_get_text (GtkWidget *tlist, gint row, gint column, gchar **buffer)
{

}

/* clear the list *********************************************/
void tlist_clear (GtkWidget *tlist)
{

}

/* remove a row in the list ***********************************/
void tlist_remove_row (GtkWidget *tlist)
{

}

/* insert a blank row in the list */
void tlist_insert_row (GtkWidget *tlist, gint row)
{

}

/* scroll to the specified row */
void tlist_scroll_to_row (GtkTreeView *tree, GtkTreeModel *model, GtkTreeIter *iter)
{

}

void
cell_changed (gchar *arg1, gchar *arg2, gpointer data)
{

}

/* Keep track of the currently selected row **************************/
 void selection_changed (GtkTreeSelection *select, gpointer data)
{

}
