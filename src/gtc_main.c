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

#include <gtk/gtk.h>

#include "gtc_application.h"

int
main (int argc,
			char *argv[])
{
	CalcApplication *gTapeCalcInstance;

	gTapeCalcInstance = calc_application_new ();
	return g_application_run (G_APPLICATION (gTapeCalcInstance), argc, argv);
}
