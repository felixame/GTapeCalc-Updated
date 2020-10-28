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

#pragma once
#ifndef __CALC_H__
#define __CALC_H__

#include "gtc_types.h"

#define MAX_ENTRY 64
#define MAX_STRING (MAX_ENTRY + (MAX_ENTRY / 3))

G_BEGIN_DECLS

#define CALC_TYPE_VALUES calc_values_get_type ()
#define CALC_TYPE_MACHINE calc_machine_get_type ()
G_DECLARE_FINAL_TYPE (CalcValues, calc_values, CALC, VALUES, GObject)
G_DECLARE_FINAL_TYPE (CalcMachine, calc_machine, CALC, MACHINE, GObject)

struct _CalcValues
{
  GObject parent;

  double total;
  double val1;
  double val2;
  ButtonType opbuf;
  ButtonType op;
  unsigned int val1flag;
  char  *text;
};

struct _CalcMachine
{
  GObject parent;

  CalcValues *mainval;
  CalcValues *subval;
  unsigned int skip_output;
  CalcState calcState;
};

G_DEFINE_TYPE (CalcValues, calc_values, G_TYPE_OBJECT)
G_DEFINE_TYPE (CalcMachine, calc_machine, G_TYPE_OBJECT)

G_END_DECLS

void
parse_main_input (CalcMachine *self,
                  ButtonType btn_type,
									char *calcEntryText);
CalcType
parse_sub_input (CalcValues *values,
                 ButtonType btype,
                 gchar *entry_data);

CalcType
parse_input(CalcValues *values,
            ButtonType btype,
            gchar *entry_data);

void
parse_grouped_input (CalcMachine *self,
                     ButtonType btn_type,
										 gchar *calcEntryString);

int
calculate (CalcValues *vals);

void
parse_output (CalcValues *values,
              CalcType type);

void
calc_machine_clear (self *CalcMachine);

#endif /* __CALC_H__ */

