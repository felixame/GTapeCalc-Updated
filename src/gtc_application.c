#include <gtk/gtk.h>

#include "gtc_application.h"
#include "../config.h"

G_DEFINE_TYPE (GTapeCalcApplication, gtapecalc_application, GTK_TYPE_APPLICATION)

static void
gtapecalc_application_class_init (GTapeCalcApplicationClass *klass)
{
  G_APPLICATION_CLASS (klass)->startup = gtapecalc_application_startup;
  G_APPLICATION_CLASS (klass)->activate = gtapecalc_application_activate;
  G_APPLICATION_CLASS (klass)->open = gtapecalc_application_open;
}

static void
gtapecalc_application_example_app_init (GTapeCalcApplication *gTapeCalcApplication)
{

}

static void
gtapecalc_application_startup (GApplication *gTapeCalcApplication)
{
  GtkCssProvider *calcCssProvider;

  calcCssProvider = gtk_css_provider_new ();

  gtk_css_provider_load_from_file (calcCssProvider, g_file_new_for_uri ("resource:///com/github/felixame/gtapecalcmod/css/gtc_style.css"), NULL);
  gtk_style_context_add_provider_for_screen (gdk_screen_get_default (),
                                             GTK_STYLE_PROVIDER (calcCssProvider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void
gtapecalc_application_activate (GApplication *gTapeCalcApplication)
{
	CalcWindow *calcWindow;

	/* It's good practice to check your parameters at the beginning of the
	 * function. It helps catch errors early and in development instead of
	 * by your users.
	 */
	g_assert (GTK_IS_APPLICATION (gTapeCalcApplication));

	/* Get the current window or create one if necessary. */
	calcWindow = gtk_application_get_active_window (gTapeCalcApplication);
	if (!calcWindow)
		calcWindow = g_object_new (
													CALC_TYPE_WINDOW,
		            					"application", gTapeCalcApplication,
		                      NULL
													);

	/* Ask the window manager/compositor to present the window. */
	gtk_window_present (calcWindow);

	// Begin new file session. Default filename is "Untitled.txt".
	// I don't think we need to expand the functionality of the files the
	// original GTapeCalc uses so we'll be keeping compatibility with previously
	// created tape files.

	//file_session_new ();

	gtk_header_bar_set_subtitle(calcWindow->headerBar, "Name of Current Open File Goes Here");

}

GTapeCalcApplication *
gtapecalc_application_new (void)
{
  return g_object_new (EXAMPLE_APP_TYPE,
                       "application-id", APPLICATION_ID,
                       NULL);
}
