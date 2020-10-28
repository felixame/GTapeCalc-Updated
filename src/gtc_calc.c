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

static void
calc_values_class_init (CalcValuesClass *klass)
{

}

static void
calc_values_init (CalcValues *self)
{

}

CalcValues *
calc_values_new (double newTotal,
								 double newVal1,
								 double newVal2,
								 ButtonType newOperatorBuffer,
								 ButtonType operator,
								 unsigned int newVal1Flag,
								 char *newText)
{
	return g_object_new (CALC_TYPE_VALUES,
											 "total", newTotal,
											 "val1", newVal1,
											 "val2", newVal2,
											 "opbuf", newOperatorBuffer,
											 "op", operator,
											 "val1flag", newVal1Flag,
											 "text", newText,
											 NULL);
}

static void
calc_machine_class_init (CalcMachineClass *klass)
{
  /* GObjectClass *object_class = G_OBJECT_CLASS (klass); */

  /* object_class->constructed = viewer_file_constructed; */
  /* object_class->finalize = viewer_file_finalize; */
}

static void
calc_machine_init (CalcMachine *self)
{
	/* CalcValues newMainValue = { 0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0' }; */
	/* CalcValues newSubValue = { 0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0' }; */

	self->mainval = calc_values_new (0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0');
	self->subval = calc_values_new (0, 0, 0, BTN_NONE, BTN_NONE, 0, '\0');

  self->calcState.clear_entry = 1;
  self->calcState.in_sub = 0;
  self->calcState.have_sub = 0;
  self->calcState.misc_state = 0;
}

/* parse normal input from the keyboard ************************************/
void
parse_main_input (CalcMachine *self,
									ButtonType buttonType,
									gchar *calcEntryString)
{
  CalcType type;

  strip_commas (calcEntryString);

  if (buttonType == BTN_X_SQUARED || buttonType == BTN_NEGATE) {

    type = parse_sub_input (self->subval, buttonType, calcEntryString);

    /* output the last operator if it's not '=' for clarity in the list */
    if (self->mainval->opbuf != BTN_EQUAL && self->mainval->opbuf < BTN_NONE && !self->skip_output)
      calc_list_push ("", get_button_data (cw->button_list, mainval.opbuf));

    if (self->skip_output)    /* Don't skip if it's two in a row */
      self->skip_output = FALSE;

    parse_output (self->subval, type);
    self->skip_output = TRUE;
  }
  else {

    type = parse_input (self->mainval, buttonType, calcEntryString);
    parse_output (self->mainval, type);

    if (self->skip_output)
      self->skip_output = FALSE;
  }
}

/* set proper flags when a sub-expression is needed ***************************/
CalcType
parse_sub_input (CalcValues *values,
								 ButtonType buttonType,
								 gchar *calcEntryString)
{
  CalcType calcType;

  if (buttonType == BTN_X_SQUARED || buttonType == BTN_NEGATE) {

      values->op = values->opbuf;
      values->val1 = 0;
      values->val1flag = TRUE;
      calcType = parse_input (values, buttonType, calcEntryString);
    }

  return (calcType);
}

/* takes input from key_event and calculates *******************************/
CalcType
parse_input(CalcValues *values,
						ButtonType buttonType,
						gchar *calcEntryString)
{
  values->opbuf = buttonType;

	// If val1flag is not set,
  if (!values->val1flag) {

    if (isdigit(*calcEntryString) || *calcEntryString == '.' || *calcEntryString == '-') {

			// Convert entry data from a string to a double and store it in val1.
      values->val1 = strtod (calcEntryString, NULL);
      values->val1flag = TRUE;

      if (values->opbuf == BTN_EQUAL) {
        values->op = values->opbuf;
        values->total = values->val1;
    		values->val1 = 0;
        values->val1flag = FALSE;
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

  if (isdigit(*calcEntryString) || *calcEntryString == '.' || *calcEntryString == '-') {

		// Convert entry data from a string to a double and store it in val2.
    values->val2 = strtod (calcEntryString, NULL);

    if (values->opbuf == BTN_PERCENT)
      values->val2 = (values->val2 * .01);

		// This is the only place where calculate () is called.
    if (calculate(values)) { /* error */
      /*cw->state.clear_entry = 1;*/
      values->val1 = values->val2 = values->total = 0;
      return(OP_ERROR);
    }

    if ((values->opbuf == BTN_EQUAL) || (values->opbuf == BTN_PERCENT))
      return(EQUAL_TOTAL);
		else
      return(OP_TOTAL);
  }

  if (values->opbuf == BTN_EQUAL) {

    values->val1 = values->val2 = 0;
    values->val1flag = FALSE;
    values->op = values->opbuf;
    return (NUM_OP);
  }
  else {

    if (values->op == BTN_EQUAL || values->op == BTN_PERCENT)
			values->val1 = values->total;

    values->val1flag = TRUE;
    values->op = values->opbuf;
  }

  return(NO_OUTPUT);
}

/* parse a parentheses bracketed equation *******************************/
void
parse_grouped_input (CalcMachine *self,
										 ButtonType btn_type,
										 gchar *calcEntryString)
{
  CalcType type;
  char *temp_string;
  ButtonType btype;
  char num_string[MAX_ENTRY];
	gchar output[MAX_ENTRY];
  int x = 0, y = 0;
  CalcValues sub_values = {0, 0, 0, BTN_NONE, BTN_NONE, FALSE, '\0'};

  temp_string = (char *) malloc (strlen(calcEntryString) * sizeof(char) + 1);
  memset (temp_string, '\0', sizeof(temp_string));
  strcpy (temp_string, calcEntryString);

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

      while (btype < BTN_DIVIDE || btype > BTN_NEGATE) {
        op_str[z] = temp_string[x];
        op_str[z+1] = '\0';
        x++;
        z++;
        btype = get_button_type (cw, op_str);
      }

      y = 0;
      z = 0;

      if (btype >= BTN_X_SQUARED && btype <= BTN_NEGATE) {

				CalcValues cv = {0, 0, 0, BTN_NONE, BTN_NONE, FALSE, '\0'};    /* need to calculate */
                                                                   /* this seperatley   */
				parse_sub_input (&cv, btype, num_string);
				sprintf (num_string, "%f", cv.total);
				btype = BTN_NONE;

				while ((btype < BTN_DIVIDE || btype > BTN_NEGATE) && temp_string[x] != ')') {
	  			op_str[z] = temp_string[x];            /* get the next operator */
	  			op_str[z+1] = '\0';                    /* and set it up to plug */
          x++;                                   /* back into the loop    */
          z++;
          btype = get_button_type (cw, op_str);
        }

				if (temp_string[x] == ')')
	  			btype = BTN_EQUAL;
      	}
      	else
					btype = get_button_type (cw, op_str);
    }

    type = parse_input (&sub_values, btype, num_string);

    if (type == OP_TOTAL) {
      sub_values.val1 = sub_values.total;
      sub_values.val1flag = TRUE;
      sub_values.op = sub_values.opbuf;
      sub_values.val2 = 0;
    }
  }

  memset (num_string, '\0', sizeof(num_string));
  sprintf (num_string, "%f", sub_values.total);

  if (btn_type >= BTN_X_SQUARED && btn_type <= BTN_NEGATE) {

    type = parse_sub_input (&subval, btn_type, num_string);
    subval.text = temp_string;

    if (mainval.opbuf != BTN_EQUAL && mainval.opbuf < BTN_NONE && skip_output == 0) {

      gchar *buttonData = get_button_data (cw->button_list, mainval.opbuf);
      calc_list_push (widget, "", buttonData);
    }

    if (skip_output)
    	skip_output = FALSE;

    parse_output (widget, &subval, type);
		sprintf (output, "%s", subval.total);
    calc_entry_set_text (output);
    skip_output = TRUE;
  }
  else {

    type = parse_input (&mainval, btn_type, num_string);

    if (!skip_output) {
      mainval.text = temp_string;
      parse_output (widget, &mainval, type);
    }
    else
			skip_output = FALSE;
  }
}

/* take the output of parse_main_input() and calculates *************************/
int
calculate(CalcValues *vals)
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

			// In these kinds of error conditions, it may be useful to show a popup
			// with the error details.
	  	if (vals->val2 == 0) {
	    	vals->val1 = vals->val2 = 0;
	    	vals->val1flag = FALSE;
	    	printf("calculate () Error: Can't divide by zero!!\n");
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

    case BTN_NEGATE:
	  	vals->total = vals->val2 * -1;
	  	break;

    default:
	  	printf("calculate () Error: invalid operator with value \"%d\".\n", vals->op);
	  	return(1);
  }

  return(0);
}

/* parse the CalcType and display accordingly ******************************/
void
parse_output (CalcValues *values,
							CalcType type)
{
  char valstr[MAX_STRING];
  char output[MAX_STRING];
  char *op_str;

  switch (type) {
  	case NUM_ONE_EQUALS:

	  	if (values->text) {
	    	sprintf (output, "%s", values->text);
	    	calc_list_push (output, "");
	  	}

			format_num(values->total, valstr, cw->widget_vars->precision);

			// output = (string)values->total
	  	sprintf(output, "%s", valstr);               /* num1_equals */

	  	calc_entry_set_text (output);
	  	calc_list_push (output, " = ");
	  	calc_list_push ("", "");

	  	cw->state->clear_entry = 1;
	  	break;

    case NUM_ONE_OP:

	  	if (cw->state->have_sub) {
	    	//calc_entry_set_text (values->val1);
	    	sprintf (output, "%s", values->text);
				calc_entry_set_text (output);
	  	}
			else {
	    	format_num(values->val1, valstr, cw->widget_vars->precision);
	    	sprintf(output, "%s", valstr);               /* num1_op */
	  	}

			if (!skip_output)
	    	calc_list_push(output, "");

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

	  	calc_list_push(output, op_str);
	  	break;

    case EQUAL_TOTAL:

	  	op_str = get_button_data (cw->button_list, values->op);

	  	if (cw->state->have_sub)
	    	sprintf (output, "%s", values->text);
	  	else {

	    	if (values->opbuf == BTN_PERCENT)
	      	values->val2 = (values->val2 * 100);

	    	format_num (values->val2, valstr, cw->widget_vars->precision);
	    	sprintf (output, "%s", valstr);      /* num2_op */
	  	}

	  	if (!skip_output)
	    	calc_list_push(output, op_str);

	  	//calc_entry_set_text (values->total);
	  	format_num (values->total, valstr, cw->widget_vars->precision);
	  	op_str = get_button_data (cw->button_list, values->opbuf);

	  	sprintf (output, "%s", valstr);         /* total_equals */
			calc_entry_set_text (output);
	  	calc_list_push (output, op_str);
	  	calc_list_push ("", "");

	  	values->val1 = values->val2 = 0;
	  	values->val1flag = FALSE;
	  	values->op = values->opbuf;
	  	break;

    case NUM_OP:

	  	op_str = get_button_data (cw->button_list, values->opbuf);

	  	if (cw->state->have_sub)
	    	sprintf (output, "%s", values->text);
	  	else {
	    	format_num (values->total, valstr, cw->widget_vars->precision);
	    	sprintf (output, "%s", valstr);           /* num_op */
	  	}

	  	calc_list_push (output, op_str);
	  	calc_list_push ("", "");
	  	calc_entry_set_text (output);
	  	break;

    case OP_TOTAL:

	  	op_str = get_button_data (cw->button_list, values->op);

	  	if (values->text) {
	    	calc_entry_set_text (values->val2);
	    	sprintf (output, "%s", values->text);
			}
	  	else {

	    	if (values->opbuf == BTN_PERCENT)
	      	values->val2 = values->val2 * 100;

	    	format_num (values->val2, valstr, cw->widget_vars->precision);
	    	sprintf (output, "%s", valstr);      /* num2_op */
	    	calc_entry_set_text (output);
	  	}

	  	if (!skip_output)
	    	calc_list_push(output, op_str);

	  	values->val1 = values->total;
	  	values->val1flag = TRUE;
	  	values->op = values->opbuf;
	  	values->val2 = 0;
	  	cw->state->clear_entry = 1;
	  	break;

    case OP_ERROR:	/* op_error */

	  	mainval.val1 = mainval.val2 = mainval.total = 0;
	  	mainval.val1flag = FALSE;

	  	cw->state->clear_entry = 1;
	  	cw->state->have_sub = 0;
	  	cw->state->in_sub = 0;
	  	skip_output = FALSE;

			calc_entry_set_text ("Error");
	  	calc_list_push ("Error", "");
	  	calc_list_push ("", "");
	  	break;

    case NO_OUTPUT:
	  	break;

    default:
	  	printf ("parse_output () Error: invalid CalcType with value \"%s\".\n", type);
  }

  if (values->text) {
    free (values->text);
    values->text = '\0';
  }

}

/* clears all data out of the variables ************************************/
void
calc_machine_clear (CalcMachine *self)
{
  self->mainval.val1 = 0;
	self->mainval.val2 = 0;
	self->mainval.total = 0;
  self->mainval.val1flag = FALSE;

  self->calcState.clear_entry = 1;
  self->calcState.have_sub = 0;
  self->calcState.in_sub = 0;
  self->skip_output = FALSE;

	calc_list_push ("Clear", "");
  calc_list_push ("", "");

	calc_entry_clear ();
}
