/***************************************************************************
                          gtc_print.h  -  description
                             -------------------
    begin                : Sun Dec 31 2000
    copyright            : (C) 2000 by Stephen Witkop
    email                : switk@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

int print_ps_header (FILE *fn);

void print_start_pos (FILE *fn);

void print_header (FILE *fn, char *file_name, int page);

void print_l_column (FILE *fn, char *text);

void print_rs_column (FILE *fn, char *text);

void print_r_column (FILE *fn, char *text);

void print_footer (FILE *fn, int page);

void print_showpage (FILE *fn);

