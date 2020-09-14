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

/*
 * Mon Sep 14 2020
 * Fixed a crash when GTK attemps to interpret raw byte unicode
 * characters on button labels.
 * Fixed a crash when trying to add a note to an invalid/unselected row.
 */

#include <stdio.h>
#include <gtk/gtk.h>

#include "gtc_ui.h"
#include "gtc_callbacks.h"
#include "gtc_prefs.h"
#include "gtc_window.h"
#include "../config.h"

int main(int argc, char *argv[])
{
  CalcWindow *cwindow;
  gchar *title_string;

  gtk_init (&argc, &argv);

  cwindow = window_new();
  title_string = g_strdup_printf ("%s - %s", PACKAGE, VERSION);

  if (!title_string) {
    perror("gtapecalc");
    return (1);
  }

  gtk_window_set_title (GTK_WINDOW(cwindow->window), title_string);

  make_ui (cwindow);

  if (argc > 1) {
  	gchar *cmd_line;

  	cmd_line = g_strdup (argv[1]);
  	file_open (GTK_WIDGET(cwindow->window), cmd_line);
  	g_free (cmd_line);
  }

  gtk_widget_show (cwindow->window);
  g_free (title_string);

  gtk_main ();

  g_free (cwindow);

  return(0);
}

