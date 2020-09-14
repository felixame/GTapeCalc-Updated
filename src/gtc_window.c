/***************************************************************************
                          gtc_window.c  -  description
                             -------------------
    begin                : Sat Nov 20 1999
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
#include <stdlib.h>

#include "gtc_window.h"
#include "gtc_callbacks.h"
#include "gtc_prefs.h"
#include "pixmaps/gtapecalc.xpm"

static CalcState state;

/* set up a new window and initialize the widget variables ******************/
CalcWindow *window_new ()
{
  CalcWindow *cwindow;

  cwindow = g_malloc0 (sizeof(CalcWindow));
  if (!cwindow) {
    g_warning ("Couldn't allocate memory for a window\n");
    perror ("gtapecalc-window_new");
    exit (1);
  }
  cwindow->widget_vars = g_malloc0 (sizeof(ConfigVariable));
  /*g_message ("loading config from rc file");*/
  cwindow->widget_vars = config_parse_rc_file ();
  if (!cwindow->widget_vars) {
    g_message ("loading defaults");
    cwindow->widget_vars =  config_load_defaults();
  }

  cwindow->state = &state;
  cwindow->state->clear_entry = 1;
  cwindow->state->in_sub = 0;
  cwindow->state->have_sub = 0;
  cwindow->state->misc_state = 0;

  cwindow->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_policy (GTK_WINDOW(cwindow->window), 1, 1, 1);
  gtk_window_set_default_size (GTK_WINDOW(cwindow->window),
	  cwindow->widget_vars->win_x_size,
	  cwindow->widget_vars->win_y_size);
  g_signal_connect (GTK_OBJECT (cwindow->window), "delete_event",
	  G_CALLBACK(delete_event),
	  NULL);
  g_signal_connect_after (GTK_OBJECT(cwindow->window), "realize",
			    G_CALLBACK(window_set_icon),
			    NULL);
  return (cwindow);
}

/* set the window icon *****************************************************/
void window_set_icon (GtkWidget *widget, gpointer data)
{
  GdkPixmap      *pixmap;
  GdkBitmap      *mask;
  GtkIconFactory *factory;
  GtkIconSet     *icon_set;
  GdkPixbuf      *pixbuf;
  GtkStockItem   item;
  GError         *error = NULL;
  gchar          *id = "gtc_gtapecalc";

  pixmap = gdk_pixmap_create_from_xpm_d (widget->window, &mask,
	  &widget->style->bg[GTK_STATE_NORMAL], gtapecalc_xpm);
  gdk_window_set_icon (widget->window, NULL, pixmap, mask);
  gdk_window_set_icon_name (widget->window, "gtapecalc");

  pixbuf = gdk_pixbuf_new_from_file ("pixmaps/gtapecalc.xpm", &error);
  factory = gtk_icon_factory_new ();
  icon_set = gtk_icon_set_new_from_pixbuf (pixbuf);
  gtk_icon_factory_add (factory, "gtc_gtapecalc", icon_set);
  gtk_icon_factory_add_default (factory);
  item.stock_id = id;
  item.label = id;
  item.modifier = 0;
  item.keyval = 0;
  item.translation_domain = NULL;
  gtk_stock_add (&item, 1);
  if (error)
  	g_warning ("error was: %s", error->message);

  return;
}
