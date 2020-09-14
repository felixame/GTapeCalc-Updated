/***************************************************************************
                          gtc_ui.h  -  default preferences and settings
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


#ifndef __GTCALC_UI_H__
#define __GTCALC_UI_H__

#include "gtc_types.h"

enum
{
  COLUMN_NOTE,
  COLUMN_NUMBER,
  COLUMN_OPERATOR,
  N_COLUMNS
};

/*extern gint   pane_width;
extern char *entry_font_name;
extern char *clist_font_name;
extern char *button_font_name;
extern gint   column_title_show;
*/

gint make_ui (CalcWindow *cwindow);

void make_buttons(GtkWidget *table, CalcWindow *cw);

GtkWidget *show_popup_menu (GtkWidget *clist);

#endif /* __GTCALC_UI_H__ */
