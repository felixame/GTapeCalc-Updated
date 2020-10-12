/***************************************************************************
    gtapecalc.c  - GTapeCalc - a tape calculator with gtk
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

#include <stdio.h>
#include <gtk/gtk.h>

#include "gtc_ui.h"
#include "gtc_callbacks.h"
#include "gtc_prefs.h"
#include "../config.h"

static CalcState state;

static void startup (GApplication *, gpointer *);
static void activate (GApplication *, gpointer *);

int main (int argc, char *argv[])
{
  GtkApplication *calcApp;
  int returnStatus;

  calcApp = gtk_application_new ("org.unknown.test", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (calcApp, "startup", G_CALLBACK(startup), NULL);
  g_signal_connect (calcApp, "activate", G_CALLBACK(activate), NULL);
  returnStatus = g_application_run (G_APPLICATION (calcApp), argc, argv);
  g_object_unref (calcApp);

  return returnStatus;
}

static void startup (GApplication *calcApp, gpointer *data)
{
  GtkCssProvider *calcCssProvider;

  calcCssProvider = gtk_css_provider_new ();

  gtk_css_provider_load_from_path (calcCssProvider, "resources/css/style.css", NULL);
  gtk_style_context_add_provider_for_screen (gdk_screen_get_default (),
                                             GTK_STYLE_PROVIDER (calcCssProvider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void activate (GApplication *calcApp, gpointer *data)
{
  CalcWindow *cwindow;

	g_assert (GTK_IS_APPLICATION (calcApp));

  cwindow = g_malloc0 (sizeof(CalcWindow));

	//cwindow->window = gtk_application_window_new (GTK_APPLICATION (calcApp));

  if (!cwindow) {
    g_warning ("Couldn't allocate memory for a window\n");
    perror ("gtapecalc-window_new");
    g_abort ();
  }

  cwindow->widget_vars = g_malloc0 (sizeof(ConfigVariable));
  g_message ("loading config from rc file");
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





  make_ui (cwindow);

  gtk_widget_show (cwindow->window);
}
