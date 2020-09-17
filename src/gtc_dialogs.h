/***************************************************************************
                          gtc_dialogs.h  -  dialog implementations
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


#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include <gtk/gtk.h>
#include "../config.h"

/*extern gint selected_row;*/
/*extern gint file_saved;*/

typedef struct _DialogItem DialogItem;
 typedef struct _ButtonItem ButtonItem;

struct _ButtonItem
{
  GtkWidget    *button;
  const gchar  *label;
  GCallback     sigfunc;
  gint                default_btn;
};

struct _DialogItem
{
  GtkWidget  *dialog;
  gint        size_x;
  gint        size_y;
  gint        is_modal;
  gchar      *title;
  gchar      *pixmap;
  gchar      *message;
  gint        nbuttons;
};

#define QUESTION_PIX "question.xpm"

void edit_tape_dialog (GtkWidget *widget);
GtkWidget* dialog_new (GtkWidget *widget, DialogItem *dlg, ButtonItem *btns);
void dlg_error_msg (GtkWidget *widget, gchar *err_str);
void dlg_delete_line (GtkWidget *widget);
void show_about(GtkWidget *widget);
void dlg_preferences_show (GtkWidget *widget);
void dlg_print (GtkWidget *widget);
void dlg_set_radio_entry_insensitive (GtkWidget *widget);
void dlg_font_select (GtkWidget *widget, gpointer entry);
void dlg_get_font (GtkWidget *widget, gpointer data);
void destroy_dialog(GtkWidget *widget, gpointer dlg);



#endif /* __DIALOGS_H__ */
