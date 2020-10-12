#include <gtk/gtk.h>

#include "gtc_application.h"
#include "gtc_window.h"

#include "gtc_callbacks.h"
#include "gtc_prefs.h"

#include "../config.h"

G_DEFINE_TYPE (CalcApplication, calc_application, GTK_TYPE_APPLICATION)

static void
calc_application_startup (GApplication *calcApplication)
{
  GtkCssProvider *calcCssProvider;

	G_APPLICATION_CLASS (calc_application_parent_class)->startup (calcApplication);

  calcCssProvider = gtk_css_provider_new ();

  gtk_css_provider_load_from_file (calcCssProvider, g_file_new_for_uri ("resource:///com/github/felixame/gtapecalcmod/css/gtc_style.css"), NULL);
  gtk_style_context_add_provider_for_screen (gdk_screen_get_default (),
                                             GTK_STYLE_PROVIDER (calcCssProvider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void
calc_application_activate (GApplication *calcApplication)
{
	CalcWindow *calcWindow;

	calcWindow = g_object_new (
													CALC_WINDOW_TYPE,
		            					"application", calcApplication,
		                      NULL
													);

	gtk_window_present (GTK_WINDOW (calcWindow));

	// Begin new file session. Default filename is "Untitled.txt".
	// I don't think we need to expand the functionality of the files the
	// original GTapeCalc uses so we'll be keeping compatibility with previously
	// created tape files.

	//file_new ();

	gtk_header_bar_set_subtitle(calcWindow->headerBar, "Name of Current Open File Goes Here");

}

static void
calc_application_class_init (CalcApplicationClass *klass)
{
  G_APPLICATION_CLASS (klass)->startup = calc_application_startup;
  G_APPLICATION_CLASS (klass)->activate = calc_application_activate;
}

static void
calc_application_init (CalcApplication *self)
{
}

CalcApplication *
calc_application_new ()
{
  return g_object_new (CALC_APPLICATION_TYPE,
                       "application-id", APPLICATION_ID,
											 "flags", G_APPLICATION_FLAGS_NONE,
                       NULL);
}
