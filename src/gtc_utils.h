/***************************************************************************
                          gtc_utils.h  -  utility functions
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


#ifndef __UTIL_H__
#define __UTIL_H__

#include "gtc_types.h"


gpointer get_widget_data (GtkWidget *widget, gchar *key);

gpointer get_data_from_toplevel (GtkWidget *widget, gchar *key);

GtkWidget *get_toplevel_widget (GtkWidget *widget);

ConfigVariable *get_widget_variables (GtkWidget *widget);

gchar *get_button_data (CalcButton list[], ButtonType type);

gchar *get_button_label (CalcButton list[], ButtonType type);

ButtonType get_button_type (CalcWindow *cw, char *op_str);

gchar *strip_commas (gchar *strip_str);

gchar *strip_end_chars (gchar *strip_str, gchar strip_char);

/* be sure to free() returned string when done with it ***************** */
gchar *add_commas (gchar *num_string);

char *format_num(double total_to_format, char val_str[], int d_places);

void widget_font_load (GtkWidget *widget, gchar *font_name);


#endif /* __UTIL_H__ */
