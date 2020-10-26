/* gtc_menu.h
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

G_BEGIN_DECLS

#define CALC_TYPE_POPOVER_MENU (calc_popover_menu_get_type())

struct _CalcPopoverMenu
{
  GtkPopoverMenu calcPopoverMenu;
  GtkSpinButton *calcPrecision;
};

G_DECLARE_FINAL_TYPE (CalcPopoverMenu, calc_popover_menu, CALC, POPOVER_MENU, GtkPopoverMenu)

CalcPopoverMenu *
calc_popover_menu_new ();

G_END_DECLS
