/***************************************************************************
                          gtc_calc.h  -  calculation functions and types
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


#ifndef __CALC_H__
#define __CALC_H__

#include "gtc_types.h"


#define MAX_ENTRY 64
#define MAX_STRING (MAX_ENTRY + (MAX_ENTRY / 3))



void parse_main_input (GtkWidget *widget, ButtonType btn_type);

CalcType parse_input(CalcValues *values, ButtonType btype, gchar *entry_data);

CalcType parse_sub_input (CalcValues *values, ButtonType btype, gchar *entry_data);

void parse_grouped_input (GtkWidget *widget, ButtonType btn_type);

void parse_output (GtkWidget *widget, CalcValues *values, CalcType type);

void clear(GtkWidget *widget, gpointer data);

#endif /* __CALC_H__ */



