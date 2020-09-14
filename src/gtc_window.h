/***************************************************************************
                          gtc_window.h  -  description
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

#ifndef __GTC_WINDOW_H__
#define __GTC_WINDOW_H__

#include <errno.h>

#include <gtk/gtk.h>
#include "gtc_types.h"



CalcWindow *window_new ();

void window_set_icon (GtkWidget *widget, gpointer data);

#endif  /* __GTC_WINDOW_H__ */
