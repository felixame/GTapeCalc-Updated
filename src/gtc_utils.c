/***************************************************************************
                          gtc_utils.c  - utility functions
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gtc_utils.h"
#include "gtc_calc.h"
#include "gtc_window.h"


/* get a pointer to a widget from the toplevel window data 
   set with gtk_object_set_data including dialogs which have a
   pointer to the toplevel window set in their data
*/
gpointer get_data_from_toplevel (GtkWidget *widget, gchar *key)
{
  gpointer data;
 
  if (GTK_IS_DIALOG(widget) || GTK_IS_FILE_SELECTION(widget)) {
    GtkWidget *twindow;
    
    twindow = get_widget_data (widget, "twindow");
    data = get_widget_data (twindow, key);
  }
  else
    data = get_widget_data (widget, key);
  return (data);
}
    
/* get the toplevel window of the widget (ripped from Glade)******************/
GtkWidget *get_toplevel_widget (GtkWidget *widget)
{
  GtkWidget *parent;

  for (;;)
  {
    if (GTK_IS_MENU (widget))
      parent = gtk_menu_get_attach_widget (GTK_MENU (widget));
    else
      parent = widget->parent;
    if (parent == NULL)
      break;
    widget = parent;
  }
  return (widget);
}

/* get data associated with key from the toplevel widget *******************/
gpointer get_widget_data (GtkWidget *widget, gchar *key)
{
  GtkWidget *toplevel;
  gpointer data;

  toplevel = get_toplevel_widget (widget);
  data = gtk_object_get_data (GTK_OBJECT(toplevel), key);

  return (data);
}

/* get the widget properties from the toplevel window data ******************/
ConfigVariable *get_widget_variables (GtkWidget *widget)
{
  ConfigVariable *vars;
  GtkWidget *twindow;

  twindow = get_toplevel_widget (widget);

  vars = gtk_object_get_data (GTK_OBJECT(twindow), "vars");
  return (vars);
}

/* get's the data associated with a CalcButton ******************************/
gchar *get_button_data (CalcButton list[], ButtonType type)
{
  int i;

  for (i = 0; i < (int)BTN_NONE; i++) {
    if (list[i].type == type)
      return (g_locale_to_utf8 (list[i].data, -1, NULL, NULL, NULL));
  }
  return (NULL);
}

/* get's the label associated with a CalcButton ******************************/
gchar *get_button_label (CalcButton list[], ButtonType type)
{
  int i;

  for (i = 0; i < (int)BTN_NONE; i++) {
    if (list[i].type == type)
      return (list[i].label);
  }
  return (NULL);
}

/* get's the type of button from the string passed in ***********************/
ButtonType get_button_type (CalcWindow *cw, char *op_str)
{
  int x;

  for (x = 0; x < cw->nbuttons; x++) {
    if (strcmp (op_str, cw->button_list[x].data) == 0)
      return (cw->button_list[x].type);
  }
  return (BTN_NONE);
}

/* strip commas from a string ***********************************************/
gchar *strip_commas (gchar *strip_str)
{
  gint x = 0;
  gint y = 0;
  
  while (strip_str[x]) {   
    if (strip_str[y] == ',')
      ++y;
    strip_str[x] = strip_str[y];
    x++;
    y++;
  }
  return (strip_str);
}

/* strip chars from the end of a string ********************* ***************/
gchar *strip_end_chars (gchar *strip_str, gchar strip_char)
{
  gchar *index;

  index = strrchr(strip_str, '\0') - 1;
  while (*index == strip_char) {
    strcpy(index, "");
    index--;
  }

  return (strip_str);
}

/* add commas to a string of numbers ****************************************/
gchar *add_commas (gchar *num_string)
{
  int x, y, z, prefix_len, new_len;
  char *index;
  char temp_string[MAX_STRING];
  char *new_string;

  index = strrchr(num_string, '.');
  if (index)
      ;
  else index = strrchr(num_string, '\0');
  
  prefix_len = strlen(num_string) - strlen(index);
  if (prefix_len > 3) {
    x = y = z = 0;
    new_len = prefix_len + (prefix_len / 3);

    if (prefix_len % 3 == 0)
      new_len--;

    if (num_string[0] == '-')
      if ((prefix_len - 1) % 3 == 0)
	new_len--;
    strcpy(temp_string + new_len, index);
    
    while (prefix_len > y) {
      ++x;
      ++y;
      ++z;
      memcpy(temp_string + (new_len - x), num_string + (prefix_len - y), 1);
      if (z == 3) 
	if (prefix_len > y &&
		num_string[prefix_len - (y + 1)] != '-') { /* negative no. */
	  ++x;
	  memcpy(temp_string + (new_len - x), ",", 1);
	  z = 0;
	}
    }
    new_string = (char *) malloc(strlen(temp_string) + 1);
    if (new_string)
      strcpy(new_string, temp_string);
    else return (0);
  }
  else {
    new_string = (char *) malloc(strlen(num_string) + 1);
    if (new_string)
      strcpy(new_string, num_string);
    else return (0);
  }
  return (new_string);
}

/* formats the output of a number and returns it **************************/
char *format_num(double total_to_format, char valstr[], int d_places)
{
  char temp_string[MAX_STRING];
  char *comma_string;
  int i = d_places;

  if (d_places < 0) {
    i = 8;
  }
  sprintf(temp_string, "%.*f", i, total_to_format);

  comma_string = add_commas (temp_string);
  if (comma_string) {
    strcpy(valstr, comma_string);
    free (comma_string);
  }else return (0);
  
  if (d_places < 0) {

    strip_end_chars (valstr, '0');
    strip_end_chars (valstr, '.');
  }

  return(valstr);    
}
  
void widget_font_load (GtkWidget *widget, gchar *font_name)
{
  PangoFontDescription *font_desc;

  if (font_name) {
    font_desc = pango_font_description_from_string (font_name);
    if (font_desc) {
      gtk_widget_modify_font (GTK_WIDGET(widget), font_desc);
      pango_font_description_free (font_desc);
    }
    else g_warning ("Not a valid font");
  }
  else {
    font_desc = pango_font_description_from_string ("Sans 10");
    gtk_widget_modify_font (widget, font_desc);
    pango_font_description_free (font_desc);
  }
}



