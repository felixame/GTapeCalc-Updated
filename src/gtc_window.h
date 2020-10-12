/* gtc_window.h
 *
 * Copyright 2020 Felix
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <gtk/gtk.h>
#include "gtc_types.h"

G_BEGIN_DECLS

#define CALC_WINDOW_TYPE (calc_window_get_type())

struct _CalcWindow
{
  GtkApplicationWindow window;

  GtkHeaderBar *headerBar;
  GtkEntry *entry;
  GtkWidget *clist;
  GtkWidget *scrolled_window;
  gint selected_row;
  gchar *current_file;
  gint nbuttons;
  CalcButton *button_list;
  CalcState *state;
  ConfigVariable *widget_vars;
};

// static CalcButton button_list[] = {
//     {"(",    0, 0, NULL, NULL,       "(",   BTN_PAREN_LEFT},
//     {")",    1, 0, NULL, NULL,       ")",   BTN_PAREN_RIGHT},
//     {"÷",    2, 0, NULL, NULL,       "÷",   BTN_DIVIDE},
//     {"×",    3, 0, NULL, NULL,       "×",   BTN_MULTIPLY},
//     {"−",    4, 0, NULL, NULL,       "−",   BTN_MINUS},

//     {"%",    0, 1, NULL, NULL,       "%",   BTN_PERCENT},
//     {"7",    1, 1, NULL, NULL,       "7",   BTN_SEVEN},
//     {"8",    2, 1, NULL, NULL,       "8",   BTN_EIGHT},
//     {"9",    3, 1, NULL, NULL,       "9",   BTN_NINE},
//     {"+",    4, 1, NULL, NULL,       "+",   BTN_PLUS},

//     {"𝑥²",   0, 2, NULL, NULL,       "𝑥²", BTN_X_SQUARED},
//     {"4",    1, 2, NULL, NULL,       "4",   BTN_FOUR},
//     {"5",    2, 2, NULL, NULL,       "5",   BTN_FIVE},
//     {"6",    3, 2, NULL, NULL,       "6",   BTN_SIX},

//     {"(−)",  0, 3, NULL, NULL,       "(−)",  BTN_PLUS_MINUS},
//     {"1",    1, 3, NULL, NULL,        "1",  BTN_ONE},
//     {"2",    2, 3, NULL, NULL,        "2",  BTN_TWO},
//     {"3",    3, 3, NULL, NULL,        "3",  BTN_THREE},
//     {"=",    4, 3, NULL, NULL,        "=",  BTN_EQUAL},

//     {"AC",   0, 4, NULL, clear,       "AC", BTN_CLEAR_ALL},
//     {"CE",   1, 4, NULL, entry_clear, "CE", BTN_CLEAR_ENTRY},
//     {"0",    2, 4, NULL, NULL,        "0",  BTN_ZERO},
//     {".",    3, 4, NULL, NULL,        ".",  BTN_DEC_POINT}
//   };

G_DECLARE_FINAL_TYPE (CalcWindow, calc_window, CALC, WINDOW, GtkApplicationWindow)

G_END_DECLS
