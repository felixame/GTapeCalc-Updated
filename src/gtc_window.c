/* myo-window.c
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

#include "gtc_window.h"
//#include "gtc_types.h"
//#include "gtc_callbacks.h"
//#include "gtc_calc.h"

G_DEFINE_TYPE (CalcWindow, calc_window, GTK_TYPE_APPLICATION_WINDOW)

static void
calc_window_class_init (CalcWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/github/felixame/gtapecalcmod/ui/gtc_window.ui");

	gtk_widget_class_bind_template_child (widget_class, CalcWindow, calcHeaderBar);
  gtk_widget_class_bind_template_child (widget_class, CalcWindow, calcHeaderBar_menuButton);
	gtk_widget_class_bind_template_child (widget_class, CalcWindow, calcEntry);

	//gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (klass), button_press);




	/* GtkBuilder *builder; */
	/* GtkButton *sevenButton; */

	/* sevenButton = gtk_builder_get_object (builder, "BTN_SEVEN"); */

	/* g_signal_connect (sevenButton, "pressed", G_CALLBACK(button_press), &cw->button_list[index].type); */

	   /*  // Button signal connections. */
    /* // If the button doesn't have a callback function, */
    /* // just send the button type to button_press(). */
    /* if (!cw->button_list[index].sigfunc) */
    /*   g_signal_connect (cw->button_list[index].widget, "clicked", */
	   /*    G_CALLBACK(button_press), */
	   /*    &cw->button_list[index].type); */
    /* else{ */
    /*    gchar *data = g_locale_to_utf8 (cw->button_list[index].data, -1, NULL, NULL, NULL); */
    /*   g_signal_connect (cw->button_list[index].widget, "clicked", */
			 /*  G_CALLBACK(cw->button_list[index].sigfunc), */
			 /*  data); */
    /* } */
}

static void
calc_window_init (CalcWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

	/*GtkBuilder *builder;

	// Numerical buttons.
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_zero"),
										"pressed", G_CALLBACK(button_press), BTN_ZERO);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_one"),
										"pressed", G_CALLBACK(button_press), BTN_ONE);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_two"),
										"pressed", G_CALLBACK(button_press), BTN_TWO);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_three"),
										"pressed", G_CALLBACK(button_press), BTN_THREE);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_four"),
										"pressed", G_CALLBACK(button_press), BTN_FOUR);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_five"),
										"pressed", G_CALLBACK(button_press), BTN_FIVE);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_six"),
										"pressed", G_CALLBACK(button_press), BTN_SIX);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_seven"),
										"pressed", G_CALLBACK(button_press), BTN_SEVEN);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_eight"),
										"pressed", G_CALLBACK(button_press), BTN_EIGHT);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_nine"),
										"pressed", G_CALLBACK(button_press), BTN_NINE);

	// Operation buttons.
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_leftParen"),
										"pressed", G_CALLBACK(button_press), BTN_PAREN_LEFT);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_rightParen"),
										"pressed", G_CALLBACK(button_press), BTN_PAREN_RIGHT);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_multiply"),
										"pressed", G_CALLBACK(button_press), BTN_MULTIPLY);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_divide"),
										"pressed", G_CALLBACK(button_press), BTN_DIVIDE);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_plus"),
										"pressed", G_CALLBACK(button_press), BTN_PLUS);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_minus"),
										"pressed", G_CALLBACK(button_press), BTN_MINUS);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_equal"),
										"pressed", G_CALLBACK(button_press), BTN_EQUAL);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_percent"),
										"pressed", G_CALLBACK(button_press), BTN_PERCENT);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_squared"),
										"pressed", G_CALLBACK(button_press), BTN_X_SQUARED);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_negate"),
										"pressed", G_CALLBACK(button_press), BTN_NEGATE);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_decPoint"),
										"pressed", G_CALLBACK(button_press), BTN_DEC_POINT);

	// Clear buttons.
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_clearAll"),
										"pressed", G_CALLBACK(button_press), BTN_CLEAR_ALL);
	g_signal_connect (gtk_builder_get_object (builder, "CalcButton_clearEntry"),
										"pressed", G_CALLBACK(button_press), BTN_CLEAR_ENTRY);
	 */
}

CalcWindow *
calc_window_new (GApplication *calcApplication)
{
	return g_object_new (CALC_TYPE_WINDOW,
		            				"application", calcApplication,
		                  	NULL);
}
