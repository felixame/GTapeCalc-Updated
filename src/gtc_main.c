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
#include "gtc_window.h"
#include "../config.h"

static void
startup (GApplication *);

static void
activate (GApplication *);

int
main (int argc,
			char *argv[])
{
  GtkApplication *gTapeCalcApplication;
  int returnStatus;



  gTapeCalcApplication = gtk_application_new (APPLICATION_ID, G_APPLICATION_FLAGS_NONE);
  g_signal_connect (gTapeCalcApplication, "startup", G_CALLBACK(startup), NULL);
  g_signal_connect (gTapeCalcApplication, "activate", G_CALLBACK(activate), NULL);
  returnStatus = g_application_run (G_APPLICATION (gTapeCalcApplication), argc, argv);
  g_object_unref (gTapeCalcApplication);

  return returnStatus;
}

static void
startup (GApplication *gTapeCalcApplication)
{
  GtkCssProvider *calcCssProvider;

  calcCssProvider = gtk_css_provider_new ();

  gtk_css_provider_load_from_file (calcCssProvider, g_file_new_for_uri ("resource:///com/github/felixame/gtapecalcmod/css/style.css"), NULL);
  gtk_style_context_add_provider_for_screen (gdk_screen_get_default (),
                                             GTK_STYLE_PROVIDER (calcCssProvider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void
activate (GApplication *gTapeCalcApplication)
{
	GtkWindow *window;

	/* It's good practice to check your parameters at the beginning of the
	 * function. It helps catch errors early and in development instead of
	 * by your users.
	 */
	g_assert (GTK_IS_APPLICATION (gTapeCalcApplication));

	/* Get the current window or create one if necessary. */
	window = gtk_application_get_active_window (gTapeCalcApplication);
	if (!window)
		window = g_object_new (CALC_TYPE_WINDOW,
		                       "application", gTapeCalcApplication,
		                       NULL);

	/* Ask the window manager/compositor to present the window. */
	gtk_window_present (window);




 /*  CalcWindow *cwindow; */

	/* g_assert (GTK_IS_APPLICATION (gTapeCalcApplication)); */

 /*  cwindow = g_malloc0 (sizeof(CalcWindow)); */

	/* cwindow->window = gtk_application_window_new (GTK_APPLICATION (gTapeCalcApplication)); */

 /*  if (!cwindow) { */
 /*    g_warning ("Couldn't allocate memory for a window\n"); */
 /*    perror ("gtapecalc-window_new"); */
 /*    g_abort (); */
 /*  } */

 /*  cwindow->widget_vars = g_malloc0 (sizeof(ConfigVariable)); */
 /*  g_message ("loading config from rc file"); */
 /*  cwindow->widget_vars = config_parse_rc_file (); */
 /*  if (!cwindow->widget_vars) { */
 /*    g_message ("loading defaults"); */
 /*    cwindow->widget_vars =  config_load_defaults(); */
 /*  } */
 /*  cwindow->state = &state; */
 /*  cwindow->state->clear_entry = 1; */
 /*  cwindow->state->in_sub = 0; */
 /*  cwindow->state->have_sub = 0; */
 /*  cwindow->state->misc_state = 0; */


 /*  make_ui (cwindow); */

 /*  gtk_widget_show (cwindow->window); */
}
