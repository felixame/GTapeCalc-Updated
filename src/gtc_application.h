/* gtc_application.h
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

#define GTAPECALC_TYPE_APPLICATION (gtapecalc_application_get_type())

struct _GTapeCalcApplication
{
  GtkApplicationWindow gTapeCalcInstance;

};

G_DECLARE_FINAL_TYPE (GTapeCalcApplication, gtapecalc_application, GTAPECALC, APPLICATION, GtkApplication)

G_END_DECLS
