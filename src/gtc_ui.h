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
#include "gtc_window.h"

enum
{
  COLUMN_NOTE,
  COLUMN_NUMBER,
  COLUMN_OPERATOR,
  N_COLUMNS
};

gint make_ui (CalcWindow *);

void make_header_bar (GtkWidget *);
void make_buttons(GtkWidget *, CalcWindow *);

GtkWidget *show_popup_menu (GtkWidget *);

#endif /* __GTCALC_UI_H__ */
