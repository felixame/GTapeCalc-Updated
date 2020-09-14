/***************************************************************************
                          gtc_print.c  -  description
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

#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>

void print_h_line (FILE *fn);
void print_showpage (FILE *fn);

int print_ps_header (FILE *fn)
{
  char *ps_header = "%!PS-Adobe-3.0 %sCreator: gtapecalc-0.3.10 %CreationDate: Tue Jul 27 13:40:34\n"
      "%1999 %DocumentNeededResources: font Times-Bold %+ font Times-Italic %+ font\n"
      "%Times-Roman %+ font Courier %DocumentSuppliedResources: procset grops 1.11 0\n"
      "%%Pages:  %PageOrder: Ascend %Orientation: Portrait %EndComments\n"
      "/inch {72 mul} def "
      "/pwidth {8.5 inch} def "
      "/margin {1.25 inch} def"
      "/startpos {1.25 inch 10 inch moveto} def "
      "/startfooter {margin 1.2 inch moveto} def"
      "/sw {stringwidth pop} def "
      "/xdef {/str exch def} def "
      "/calign {pwidth str sw sub 2 div} def "
      "/lalign {margin} def "
      "/ralign {pwidth margin sub str sw sub} def "
      "/lcenter {pwidth margin sub margin sub str sw sub 2 div} def "
      "/linepos {10 inch} def "
      "/nlinepos {/linepos 10 inch def} def"
      "/nline {/linepos linepos 15 sub def} def "
      "/snline {/linepos linepos 10 sub def} def"
      "/bnline {/linepos linepos 18 sub def} def"
      "/lshow {lalign linepos moveto str show} def "
      "/lcshow {lcenter linepos moveto str show} def "
      "/rshow {ralign linepos moveto str show} def "
      "/rsshow {ralign 18 sub linepos moveto str show} def "
      "/hline {margin linepos 10 add moveto 6 inch 0 inch rlineto stroke} def "
      "/Time10 {/Times findfont 10 scalefont setfont} def "
      "/Time14 {/Times findfont 14 scalefont setfont} def "
      "/Time16 {/Times findfont 16 scalefont setfont} def "
      "/Fix10 {/Helvetica findfont 10 scalefont setfont} def \n";

  if (fn)
    fputs (ps_header, fn);
  else {
    fprintf (stderr, "print_ps_header (): invalid filename");
    return (1);
  }
  return (0);
}

void print_start_pos (FILE *fn)
{
  fputs ("startpos \n", fn);
}

void print_header (FILE *fn, char *file_name, int page)
{
  char buffer[64];
  gchar *basefn;

  basefn = g_path_get_basename (file_name);

  sprintf (buffer, "%%%%Page: %d %d\n", page, page);
  fputs (buffer, fn);
  fputs ("nlinepos \n", fn);
  fputs ("Time16 startpos (", fn);
  fputs (basefn, fn);
  fputs (") xdef calign linepos moveto str show nline nline \n", fn);
  print_h_line (fn);
  fputs ("Time14 (Notes) xdef margin .75 inch add linepos 3 sub moveto str show \n", fn);
  fputs ("(Output) xdef ralign .5 inch sub linepos 3 sub moveto str show bnline \n", fn);
  print_h_line (fn);
  fputs ("snline Time10 \n", fn);

  g_free (basefn);
}

void print_h_line (FILE *fn)
{
  fputs ("hline ", fn);
}

void print_l_column (FILE *fn, char *text)
{
  char buffer[strlen(text) + 16];

  sprintf (buffer, "(%s) xdef lshow \n", text);
  fputs (buffer, fn);
}

void print_rs_column (FILE *fn, char *text)
{
  char buffer[strlen (text) + 16];

  sprintf (buffer, "(%s) xdef rsshow \n", text);
  fputs (buffer, fn);
}

void print_r_column (FILE *fn, char *text)
{
  char buffer[strlen (text) + 16];

  sprintf (buffer, "(%s) xdef rshow nline \n", text);
  fputs (buffer, fn);
}

void print_footer (FILE *fn, int page)
{
  char buffer[64];

  /*fputs ("margin .65 inch moveto 6 inch 0 inch rlineto stroke ", fn);*/

  sprintf (buffer, "(page %d) xdef calign .5 inch moveto str show \n", page);
  fputs (buffer, fn);
  print_showpage (fn);
  /*fputs ("nlinepos \n", fn);*/
}

void print_showpage (FILE *fn)
{
  fputs ("showpage \n", fn);
}
