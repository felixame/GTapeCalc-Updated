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

#include "gtc_menu.h"
#include "gtc_types.h"

G_DEFINE_TYPE (CalcPopoverMenu, calc_popover_menu, GTK_TYPE_POPOVER_MENU)

static void
calc_window_class_init (CalcPopoverMenuClass *klass)
{
 /*  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass); */

 /*  gtk_widget_class_set_template_from_resource (widget_class, "/com/github/felixame/gtapecalcmod/ui/gtc_window.ui"); */

	/* gtk_widget_class_bind_template_child (widget_class, CalcWindow, headerBar); */
}

static void
calc_popover_menu_init (CalcPopoverMenu *self)
{

}
