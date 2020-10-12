/***************************************************************************
                          gtc_callbacks.h  -  callback functions
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


#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include "gtc_types.h"
#include "gtc_window.h"

void close_app(GtkWidget *widget);

void column_toggle (GtkWidget *widget);

void dec_select(GtkWidget *widget, char *n);

gint delete_event(GtkWidget *widget);

void fp_toggle (GtkWidget *widget, gpointer data);

void set_current_file (gchar *name);

void set_current_dir ();

void set_file_saved (CalcWindow *cw, gint state);

void de_fault(GtkWidget *widget);

void file_select(GtkWidget *widget, char *mode);

void file_open(GtkWidget *widget, gpointer data);

void file_save_event_cb (GtkWidget *widget, gchar *data);

void file_save_as (GtkWidget *widget);

int file_save_event(GtkWidget *widget, gchar *data);

void file_save_before_close (GtkWidget *widget);

void file_save_before_exit_cb (GtkWidget *widget);

void file_save_before_exit (GtkWidget *widget);

void print_file_cb (GtkWidget *widget, gpointer data);

void print_file(GtkWidget *widget, gpointer data);

void file_close_cb (GtkWidget *widget);

void entry_clear (GtkWidget *widget);

gint button_enter(GtkWidget *widget);

gint button_leave (GtkWidget *widget);

guint key_press_event (GtkWidget *widget, GdkEventKey *event);

guint key_release_event(GtkWidget *widget, GdkEventKey *event);

void key_press_button (GtkWidget *widget, ButtonType button, guint pressflg);

int key_event (GtkWidget *widget, gint keyval, gint pressflg);

void new_tape_event(GtkWidget *widget);

void edit_tape_event(GtkWidget *menuitem, gpointer data);

void select_row (GtkTreeView *tree, GtkTreePath *row);

void remove_row (GtkWidget *widget);

void insert_row (GtkWidget *widget);

void insert_comment(GtkWidget *widget);

void remove_comment (GtkWidget *widget);

void button_press(GtkWidget *widget, ButtonType *data);

void display_out(GtkWidget *widget, char *output, char *op);

void entry_show_total(GtkWidget *widget, double total);

void load_dialog_options (GtkWidget *widget);

guint mouse_click_check (GtkWidget *widget, GdkEventButton *event, gpointer data);

void show_help (GtkWidget *widget);

void statusbar_update(GtkWidget *widget, gchar *open_file, gchar *text);

/* wrappers for the list widget to make porting
*  to the TreeView widget and maintenance easier */

gint tlist_append (GtkWidget *clist, gchar **buffer);

void tlist_set_text (GtkWidget *tlist, gint row, gint column, const gchar *text);

void tlist_get_text (GtkWidget *tlist, gint row, gint column, gchar **buffer);

void tlist_clear (GtkWidget *tlist);

void tlist_remove_row (GtkWidget *tlist);

void tlist_insert_row (GtkWidget *tlist, gint row);

void tlist_scroll_to_row (GtkTreeView *tree, GtkTreeModel *model, GtkTreeIter *iter);

void cell_changed (gchar *arg1, gchar *arg2, gpointer data);
                         
void selection_changed (GtkTreeSelection *select, gpointer data);

#endif /* __CALLBACKS_H__ */
