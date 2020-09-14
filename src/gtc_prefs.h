/***************************************************************************
                          gtc_prefs.h  - Configurable options for GTapeCalc
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


#ifndef __GTC_PREFS_H__
#define __GTC_PREFS_H__

#include "gtc_types.h"

/* Default Window size */
#define DEFAULT_X_SIZE 550
#define DEFAULT_Y_SIZE 300

/* Default Pane width */
#define PANE_WIDTH 200

/* Font for the calculator entry window */
#define DEFAULT_ENTRY_FONT "sans 16"

/* Font for the output window */
/*#define DEFAULT_CLIST_FONT "-adobe-courier-medium-r-normal-*-*-120-*-*-m-*-iso8859-1"*/
#define DEFAULT_CLIST_FONT ""

/* Font for the buttons */
/*#define DEFAULT_BUTTON_FONT "-adobe-helvetica-medium-r-normal-*-*-120-*-*-p-*-iso8859-1"*/
#define DEFAULT_BUTTON_FONT "sans 12"

/* Show or hide the column titles on the output window */
#define COLUMN_TITLE_SHOW TRUE

/* Command for printing */
#define PRINT_COMMAND "lpr"

/* Page width for printing */
#define PAGE_WIDTH 78

/* Column width for printing */
#define COLUMN_WIDTH PAGE_WIDTH / 2

/* Calculator precision -1 sets the precision to floating point */
#define PRECISION -1



/* Loads the defaults into a struct that should be freed
 * when no longer needed. */
ConfigVariable *config_load_defaults ();

/* load an option into a variable, allocating and freeing
   memory as needed */
char *config_load_var (char *var, char *option);

/* returns the pointer passed in loaded with variables
 * or NULL if unsuccessfull. The structure returned should be
 * g_freed when not needed anymore */
ConfigVariable *config_parse_rc_file ();

/* write a config file to the users $(HOME) dir overwriting
 * any existing .gtapecalcrc file */
void config_write_rc_file (ConfigVariable *vars);


#endif /* __GTC_PREFS_H__ */
