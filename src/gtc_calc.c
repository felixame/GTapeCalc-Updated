/***************************************************************************
                          gtc_calc.c  -  calculation functions
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gtc_calc.h"
#include "gtc_callbacks.h"
#include "gtc_utils.h"


int calculate(CalcValues *vals);

static int skip_output = 0;

CalcValues mainval = {0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0'};
CalcValues subval  = {0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0'};

/* parse normal input from the keyboard ************************************/
void parse_main_input (GtkWidget *widget, ButtonType btn_type)
{
  CalcType type;
  gchar *entry_data;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  entry_data = gtk_entry_get_text (GTK_ENTRY(cw->entry));
  strip_commas (entry_data);

  if (btn_type >= BTN_X_SQUARED && btn_type <= BTN_PLUS_MINUS) {
    type = parse_sub_input (&subval, btn_type, entry_data);

    /* output the last operator if it's not '=' for clarity in the list */
    if (mainval.opbuf != BTN_EQUAL &&
    	    mainval.opbuf < BTN_NONE && skip_output == 0) {
      gchar *bdata = get_button_data (cw->button_list, mainval.opbuf);

      display_out (widget, "", bdata);
    }

    if (skip_output == 1)    /* Don't skip if it's two in a row */
      skip_output = 0;

    parse_output (widget, &subval, type);
    skip_output = 1;
  }
  else {
    type = parse_input (&mainval, btn_type, entry_data);
    parse_output (widget, &mainval, type);
    if (skip_output == 1)
      skip_output = 0;
  }
}

/* calculate a sub-expression **********************************************/
CalcType parse_sub_input (CalcValues *values, ButtonType btype, gchar *entry_data)
{
  CalcType type;

  if (btype >= BTN_X_SQUARED && btype <= BTN_PLUS_MINUS) {
    if (btype >= BTN_X_SQUARED && btype <= BTN_PLUS_MINUS) {
      values->opbuf = btype;
      values->op = values->opbuf;
      values->val1 = 0;
      values->val1flag = 1;
      type = parse_input (values, btype, entry_data);
    }
    /*skip_output = 1;*/
  }
  return (type);
}

/* takes input from key_event and calculates *******************************/
CalcType parse_input(CalcValues *values, ButtonType btype, gchar *entry_data)
{
  values->opbuf = btype;

  if (values->val1flag == 0) {
    if (isdigit(*entry_data) || *entry_data == '.' ||
    	    *entry_data == '-') {
      values->val1 = strtod (entry_data, NULL);
      values->val1flag = 1;
      if (values->opbuf == BTN_EQUAL) {
        values->op = values->opbuf;
        values->total = values->val1;
    	values->val1 = 0;
        values->val1flag = 0;
	/*cw->state.clear_entry = 1;*/
	return(NUM_ONE_EQUALS);
      }

      else {
	values->op = values->opbuf;
	/*cw->state.clear_entry = 1;*/
	return(NUM_ONE_OP);
      }
    }
  }
  if ((isdigit(*entry_data)) || (*entry_data == '.') ||
    	  *entry_data == '-') {
    values->val2 = atof(entry_data);
    if (values->opbuf == BTN_PERCENT)
      values->val2 = (values->val2 * .01);

    if (calculate(values)) { /* error */
      /*cw->state.clear_entry = 1;*/
      values->val1 = values->val2 = values->total = 0;
      return(OP_ERROR);
    }

    if ((values->opbuf == BTN_EQUAL) || (values->opbuf == BTN_PERCENT)) {

      return(EQUAL_TOTAL);
    }
    else {

      return(OP_TOTAL);
    }
  }
  if (values->opbuf == BTN_EQUAL) {
    values->val1 = values->val2 = 0;
    values->val1flag = 0;
    values->op = values->opbuf;
    return (NUM_OP);
  }
  else {
    if (values->op == BTN_EQUAL || values->op == BTN_PERCENT)
      values->val1 = values->total;
    values->val1flag = 1;
    values->op = values->opbuf;
  }
  return(NO_OUTPUT);
}

/* parse the CalcType and display accordingly ******************************/
void parse_output (GtkWidget *widget, CalcValues *values, CalcType type)
{
  char valstr[MAX_STRING];
  char output[MAX_STRING];
  char *op_str;

  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  switch (type) {
      case NUM_ONE_EQUALS:
	  if (values->text) {
	    sprintf (output, "%s", values->text);
	    display_out (widget, output, "");
	  }
	  format_num(values->total, valstr, cw->widget_vars->precision);
	  sprintf(output, "%s", valstr);               /* num1_equals */

	  entry_show_total (widget, values->total);
	  display_out(widget, output, " = ");
	  display_out(widget, "", "");
	  cw->state->clear_entry = 1;
	  break;
      case NUM_ONE_OP:
	  if (cw->state->have_sub) {
	    entry_show_total (widget, values->val1);
	    sprintf (output, "%s", values->text);
	  }
	  else {
	    format_num(values->val1, valstr, cw->widget_vars->precision);
	    sprintf(output, "%s", valstr);               /* num1_op */
	  }
	  if (skip_output == 0)
	    display_out(widget, output, "");

	  cw->state->clear_entry = 1;
	  break;
      case NUM_TWO_OP:
	  op_str = get_button_data (cw->button_list, values->op);
	  if (cw->state->have_sub) {
	    sprintf (output, "%s", values->text);
	  }
	  else {
	    if (values->opbuf == BTN_PERCENT)
	      values->val2 = (values->val2 * 100);
	    format_num(values->val2, valstr, cw->widget_vars->precision);
	    sprintf(output, "%s", valstr);      /* num2_op */
	  }
	  display_out(widget, output, op_str);
	  break;
      case EQUAL_TOTAL:
	  op_str = get_button_data (cw->button_list, values->op);
	  if (cw->state->have_sub)
	    sprintf (output, "%s", values->text);
	  else {
	    if (values->opbuf == BTN_PERCENT)
	      values->val2 = (values->val2 * 100);
	    format_num(values->val2, valstr, cw->widget_vars->precision);
	    sprintf(output, "%s", valstr);      /* num2_op */
	  }
	  if (skip_output == 0)
	    display_out(widget, output, op_str);

	  entry_show_total (widget, values->total);
	  format_num(values->total, valstr, cw->widget_vars->precision);
	  op_str = get_button_data (cw->button_list, values->opbuf);
	  sprintf(output, "%s", valstr);         /* total_equals */
	  display_out(widget, output, op_str);
	  display_out(widget, "", "");
	  values->val1 = values->val2 = 0;
	  values->val1flag = 0;
	  values->op = values->opbuf;
	  break;
      case NUM_OP:
	  op_str = get_button_data (cw->button_list, values->opbuf);
	  if (cw->state->have_sub)
	    sprintf (output, "%s", values->text);
	  else {
	    format_num(values->total, valstr, cw->widget_vars->precision);
	    sprintf(output, "%s", valstr);           /* num_op */
	  }
	  display_out(widget, output, op_str);
	  display_out(widget, "", "");
	  entry_show_total(widget, values->total);
	  break;
      case OP_TOTAL:
	  op_str = get_button_data (cw->button_list, values->op);
	  if (values->text) {
	    entry_show_total (widget, values->val2);
	    sprintf (output, "%s", values->text);
	  }
	  else {
	    if (values->opbuf == BTN_PERCENT)
	      values->val2 = (values->val2 * 100);
	    format_num(values->val2, valstr, cw->widget_vars->precision);
	    sprintf(output, "%s", valstr);      /* num2_op */
	    entry_show_total (widget, values->total);
	  }
	  if (skip_output == 0)
	    display_out(widget, output, op_str);
	  values->val1 = values->total;
	  values->val1flag = 1;
	  values->op = values->opbuf;
	  values->val2 = 0;
	  cw->state->clear_entry = 1;
	  break;
      case OP_ERROR:
	  mainval.val1 = mainval.val2 = mainval.total = 0;
	  mainval.val1flag = 0;

	  cw->state->clear_entry = 1;
	  cw->state->have_sub = 0;
	  cw->state->in_sub = 0;
	  skip_output = 0;

	  gtk_entry_set_text (GTK_ENTRY(cw->entry), "Error");
	  display_out (widget, "Error", "");                    /* op_error */
	  display_out (widget, "", "");
	  break;
      case NO_OUTPUT:
	  break;
      default:
	  printf ("error");
  }
  if (values->text) {
    free (values->text);
    values->text = '\0';
  }
}

/* take the output of parse_main_input() and calculates *************************/
int calculate(CalcValues *vals)
{

  switch (vals->op) {
      case BTN_PLUS:
	  vals->total = vals->val1 + vals->val2;
	  break;
      case BTN_MINUS:
	  vals->total = vals->val1 - vals->val2;
      	  break;
      case BTN_MULTIPLY:
	  vals->total = (vals->val1) * (vals->val2);
	  break;
      case BTN_DIVIDE:
	  if (vals->val2 == 0) {
	    vals->val1 = vals->val2 = 0;
	    vals->val1flag = 0;
	    printf("Can't divide by zero!!\n");
	    return(1);
	  }
	  vals->total = vals->val1 / vals->val2;
	  break;
      case BTN_EQUAL:
	  break;
      case BTN_PERCENT:
	  vals->total = vals->val1 * (vals->val2 * .01);
	  break;
      case BTN_X_SQUARED:
	  vals->total = vals->val2 * vals->val2;
	  break;
      case BTN_PLUS_MINUS:
	  vals->total = vals->val2 * -1;
	  break;
      default:
	  printf("invalid operator '%d'\n", vals->op);
	  return(1);
  }
  return(0);
}

/* calcutate a parentheses bracketed equation *******************************/
void parse_grouped_input (GtkWidget *widget, ButtonType btn_type)
{
  CalcType type;
  char *input;
  char *temp_string;
  ButtonType btype;
  char num_string[MAX_ENTRY];
  int x = 0, y = 0;
  CalcValues sub_values = {0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0'};
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");


  input = gtk_entry_get_text (GTK_ENTRY(cw->entry));

  temp_string = (char *) malloc (strlen(input) * sizeof(char) + 1);
  memset (temp_string, '\0', sizeof(temp_string));
  strcpy (temp_string, input);

  while (temp_string[x] == '(' && temp_string[x] != '\0')
    x++;
  while (temp_string[x] != ')') {
    memset (num_string, '\0', sizeof(num_string));
    while (isdigit (temp_string[x]) || temp_string[x] == '.') {
      num_string[y] = temp_string[x];
      x++;                                               /* get a number */
      y++;
    }

    if (temp_string[x] == ')') {
      btype = BTN_EQUAL;
    }
    else {                                     /* find the next operator */
      char op_str[16];
      int z = 0;

      btype = BTN_NONE;
      while (btype < BTN_DIVIDE || btype > BTN_PLUS_MINUS) {
        op_str[z] = temp_string[x];
        op_str[z+1] = '\0';
        x++;
        z++;
        btype = get_button_type (cw, op_str);
      }
      y = 0;
      z = 0;

      if (btype >= BTN_X_SQUARED && btype <= BTN_PLUS_MINUS) {
	CalcValues cv = {0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0'};    /* need to calculate */
                                                                   /* this seperatley   */
	parse_sub_input (&cv, btype, num_string);
	sprintf (num_string, "%f", cv.total);
	btype = BTN_NONE;
	while ((btype < BTN_DIVIDE || btype > BTN_PLUS_MINUS) &&
		temp_string[x] != ')') {
	  op_str[z] = temp_string[x];            /* get the next operator */
	  op_str[z+1] = '\0';                    /* and set it up to plug */
          x++;                                   /* back into the loop    */
          z++;
          btype = get_button_type (cw, op_str);
        }
	if (temp_string[x] == ')')
	  btype = BTN_EQUAL;
      }
      else btype = get_button_type (cw, op_str);
    }
    type = parse_input (&sub_values, btype, num_string);
    if (type == OP_TOTAL) {
      sub_values.val1 = sub_values.total;
      sub_values.val1flag = 1;
      sub_values.op = sub_values.opbuf;
      sub_values.val2 = 0;
    }
  }
  memset (num_string, '\0', sizeof(num_string));
  sprintf (num_string, "%f", sub_values.total);

  if (btn_type >= BTN_X_SQUARED && btn_type <= BTN_PLUS_MINUS) {
    type = parse_sub_input (&subval, btn_type, num_string);
    subval.text = temp_string;

    if (mainval.opbuf != BTN_EQUAL &&               /* check this code out */
    	  mainval.opbuf < BTN_NONE && skip_output == 0) {
      gchar *bdata = get_button_data (cw->button_list, mainval.opbuf);

      display_out (widget, "", bdata);
    }

    if (skip_output == 1)
    	skip_output = 0;

    parse_output (widget, &subval, type);
    entry_show_total (widget, subval.total);
    skip_output = 1;
  }
  else {
    type = parse_input (&mainval, btn_type, num_string);
    if (skip_output == 0) {
      mainval.text = temp_string;
      parse_output (widget, &mainval, type);
    }
    else skip_output = 0;
  }
}


/* clears all data out of the variables ************************************/
void clear(GtkWidget *widget, gpointer data)
{
  char *clear = "Clear   ";
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  display_out (widget, clear, "");
  display_out (widget, "", "");
  mainval.val1 = mainval.val2 = mainval.total = 0;
  mainval.val1flag = 0;

  gtk_entry_set_text (GTK_ENTRY(cw->entry), "0");
  cw->state->clear_entry = 1;
  cw->state->have_sub = 0;
  cw->state->in_sub = 0;
  skip_output = 0;
}
