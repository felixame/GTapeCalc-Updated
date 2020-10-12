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
#include "gtc_types.h"

static CalcState state;

G_DEFINE_TYPE (CalcWindow, calc_window, GTK_TYPE_APPLICATION_WINDOW)

static void
calc_window_class_init (CalcWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/github/felixame/gtapecalcmod/ui/gtc_window.ui");

	gtk_widget_class_bind_template_child (widget_class, CalcWindow, headerBar);
  //gtk_widget_class_bind_template_child (widget_class, CalcWindow, entry);
}

static void
calc_window_init (CalcWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

	self->state = &state;
  self->state->clear_entry = 1;
  self->state->in_sub = 0;
  self->state->have_sub = 0;
  self->state->misc_state = 0;

	//self->button_list = button_list;
	self->nbuttons = 23;
}
