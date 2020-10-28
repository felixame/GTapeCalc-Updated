/***************************************************************************
                          gtc_types.h  -  description
                             -------------------
    begin                : Sat Nov 27 1999
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

#ifndef __GTC_TYPES_H__
#define __GTC_TYPES_H__

#include <gtk/gtk.h>

#include "gtc_application.h"
#include "gtc_window.h"

typedef enum
{
  NO_OUTPUT,
  NUM_OP,
  NUM_ONE_EQUALS,
  NUM_ONE_OP,
  NUM_TWO_OP,
  EQUAL_TOTAL,
  OP_TOTAL,
  SUB_TOTAL,
  OP_ERROR
} CalcType;

// typedef struct
// {
//   GtkWidget      *window;
//   GtkWidget      *headerBar;
//   GtkWidget      *entry;
//   GtkWidget      *clist;
//   GtkWidget      *scrolled_window;
//   gint           nbuttons;
//   gint           selected_row;
//   gchar          *current_file;
//   CalcButton     *button_list;
//   CalcState      *state;
//   ConfigVariable *widget_vars;
// } CalcWindow;

// typedef struct
// {
//   gchar *label;
//   gint column;
//   gint row;
//   GtkWidget *widget;
//   gpointer*  sigfunc; // Pointer to function that handles button action.
//   gchar *data;
//   ButtonType type;
// } CalcButton;

typedef enum
{
  BTN_ZERO,
  BTN_ONE,
  BTN_TWO,
  BTN_THREE,
  BTN_FOUR,
  BTN_FIVE,
  BTN_SIX,
  BTN_SEVEN,
  BTN_EIGHT,
  BTN_NINE,
  BTN_DEC_POINT,
  BTN_PAREN_LEFT,
  BTN_PAREN_RIGHT,
  BTN_DIVIDE,
  BTN_MULTIPLY,
  BTN_PLUS,
  BTN_MINUS,
  BTN_PERCENT,
  BTN_EQUAL,
  BTN_X_SQUARED,
  BTN_NEGATE,
  BTN_CLEAR_ALL,
  BTN_CLEAR_ENTRY,
  BTN_NONE
} ButtonType;

// struct _PopupMenuEntry
// {
//   const gchar *label;
//   GCallback sigfunc;
// };

typedef struct
{
  gint   win_x_size;
  gint   win_y_size;
  gint   pane_width;
  char  *entry_font_name;
  char  *clist_font_name;
  char  *button_font_name;
  gint   column_title_show;
  char  *print_command;
  gint   page_width;
  gint   column_width;
  gint   precision;
} ConfigVariable;

typedef struct
{
  gchar *name;
  gchar *str_val;
  gint   int_val;
} ConfigOption;

typedef struct
{
  unsigned int file_saved :1;
  unsigned int clear_entry :1;
  unsigned int in_sub :1;
  unsigned int have_sub :1;
  unsigned int misc_state :1;
} CalcState;

typedef struct
{
  char *print_command;
  char *to_file_name;
  char *header;
  int page_width;
  int column_width;
  unsigned int to_file :1;
  unsigned int print_header :1;
  unsigned int custom_header :1;
  unsigned int preview :1;
} PrintOption;


#endif /* __GTC_TYPES_H__ */
