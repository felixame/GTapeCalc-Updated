/***************************************************************************
                          gtc_prefs.c  - Configuration for GTapeCalc
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
#include <gdk/gdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gtc_prefs.h"



/*static ConfigOption default_options[] = {
  {"win_x_size",        NULL,                DEFAULT_X_SIZE},
  {"win_y_size",        NULL,                DEFAULT_Y_SIZE},
  {"pane_width",        NULL,                PANE_WIDTH},
  {"entry_font",        DEFAULT_ENTRY_FONT,  0},
  {"clist_font",        DEFAULT_CLIST_FONT,  0},
  {"button_font",       DEFAULT_BUTTON_FONT, 0},
  {"column_title_show", NULL,                COLUMN_TITLE_SHOW},
  {"print_command",     PRINT_COMMAND,       0},
  {"page_width",        NULL,                PAGE_WIDTH},
  {"column_width",      NULL,                COLUMN_WIDTH},
  {"precision",         NULL,                8}
};*/

void config_load_options (ConfigVariable *vars, ConfigOption **opt, gint nopts);


ConfigVariable *config_load_defaults ()
{
  ConfigVariable *vars = (ConfigVariable *) malloc(sizeof(ConfigVariable));
  ConfigOption *options[1];
  ConfigOption opt;
  gint nopts;
  gchar *n = "none";

  opt.name = n;
  opt.str_val = n;
  opt.int_val = 0;
  nopts = 1;
  options[0] = &opt;

  memset (vars, '\0', sizeof(ConfigVariable));
  config_load_options (vars, options, nopts);

  return (vars);
}

void config_load_options (ConfigVariable *vars, ConfigOption **opt, gint nopts)
{
  gint i = 0;

  nopts--;
  while (i < nopts && strcmp(opt[i]->name, "win_x_size") != 0) i++;
  if (strcmp(opt[i]->name, "win_x_size") == 0)
    vars->win_x_size = opt[i]->int_val;
  else  vars->win_x_size = DEFAULT_X_SIZE;
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "win_y_size") != 0) i++;
  if (strcmp(opt[i]->name, "win_y_size") == 0)
    vars->win_y_size = opt[i]->int_val;
  else  vars->win_y_size = DEFAULT_Y_SIZE;
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "pane_width") != 0) i++;
  if (strcmp(opt[i]->name, "pane_width") == 0)
    vars->pane_width = opt[i]->int_val;
  else vars->pane_width = PANE_WIDTH;
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "entry_font") != 0) i++;
  if (strcmp(opt[i]->name, "entry_font") == 0)
    vars->entry_font_name = config_load_var(vars->entry_font_name,
	    opt[i]->str_val);
  else {
    if (DEFAULT_ENTRY_FONT != NULL)
      vars->entry_font_name = config_load_var (vars->entry_font_name,
        DEFAULT_ENTRY_FONT);
  }
  i = 0;


  while (i < nopts && strcmp(opt[i]->name, "clist_font") != 0) i++;
  if (strcmp(opt[i]->name, "clist_font") == 0)
    vars->clist_font_name = config_load_var(vars->clist_font_name,
	    opt[i]->str_val);
  else {
    if (DEFAULT_CLIST_FONT != NULL)
      vars->clist_font_name = config_load_var(vars->clist_font_name,
	      DEFAULT_CLIST_FONT);
	  }
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "button_font") != 0) i++;
  if (strcmp(opt[i]->name, "button_font") == 0)
    vars->button_font_name = config_load_var(vars->button_font_name,
	    opt[i]->str_val);
  else {
    if (DEFAULT_BUTTON_FONT != NULL)
      vars->button_font_name = config_load_var(vars->button_font_name,
	      DEFAULT_BUTTON_FONT);
	  }
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "column_title_show") != 0) i++;
  if (strcmp(opt[i]->name, "column_title_show") == 0)
    vars->column_title_show = opt[i]->int_val;
  else  vars->column_title_show = COLUMN_TITLE_SHOW;
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "print_command") != 0) i++;
  if (strcmp(opt[i]->name, "print_command") == 0)
    vars->print_command = config_load_var(vars->print_command,
	    opt[i]->str_val);
  else
    if (PRINT_COMMAND != NULL) {
      vars->print_command = config_load_var(vars->print_command,
	      PRINT_COMMAND);
    }
    else g_warning ("Invalid print command");
    i = 0;

  while (i < nopts && strcmp(opt[i]->name, "page_width") != 0) i++;
  if (strcmp(opt[i]->name, "page_width") == 0)
    vars->page_width = opt[i]->int_val;
  else vars->page_width = PAGE_WIDTH;
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "column_width") != 0) i++;
  if (strcmp(opt[i]->name, "column_width") == 0)
    vars->column_width = opt[i]->int_val;
  else vars->column_width = COLUMN_WIDTH;
  i = 0;

  while (i < nopts && strcmp(opt[i]->name, "precision") != 0) i++;
  if (strcmp(opt[i]->name, "precision") == 0)
    vars->precision = opt[i]->int_val;
  else vars->precision = PRECISION;
}

/* load config option into a variable */
char *config_load_var (char *var, char *option)
{
  /*if (var)
    free (var);*/

  var = (char *)realloc(var, strlen(option) + 1);
  if (var){
    strcpy (var, option);
    return (var);
  }
  else {
    fprintf(stderr, "couldn't allocate memory for %s\n", option);
    return (NULL);
  }
}

/* read in a config file and load the options into a ConfigVariable struct */
ConfigVariable *config_parse_rc_file ()
{
  ConfigVariable *vars;
  ConfigOption *option;
  ConfigOption *options[20];
  char buffer[512];
  char *rcname;
  FILE *fname;
  int nopts = 0;

  rcname = g_strdup_printf ("%s/.gtapecalcrc", g_get_home_dir()); /* free */

  fname = fopen (rcname, "r");
  g_free (rcname);
  if (!fname) {
    g_message ("no rc file");
    return (NULL);
  }
  else {
    while (fgets (buffer, 512, fname)) {
      char *index = NULL;
      int i = 0, x = 1, y = 0;

      if (buffer[i] == '#' || buffer[i] == '\n')
        continue;

      while (isspace (buffer[i]))
        i++;

      index = strchr(buffer, '\n');
      if (index)
        *index = '\0';

      index = strchr(buffer, '=');
      if (index) {
        *index = '\0';
        y = strlen(buffer);
        y--;
        while (buffer[y] == ' ') {
          buffer[y] = '\0';
          y--;
        }

        while (isspace(index[x])) {
          if (index[x] == '\n')
          break;
          x++;
        }
        if (index[x] != '\0') {
          option = g_new0(ConfigOption, 1);
          vars = g_new0(ConfigVariable, 1);

          if (isdigit(index[x]) || (index[x] == '-' && isdigit(index[x + 1])))
            option->int_val = atoi(index + 1);
          else option->str_val = g_strdup (index + 1);
            index = &buffer[i];
          option->name = g_strdup (index);
          options[nopts] = option;
          nopts++;
        }
      }
    }
    config_load_options (vars, options, nopts);

    nopts--;

    for (; nopts >= 0; nopts--) {
      if (options[nopts]->str_val)
      g_free (options[nopts]->str_val);
      g_free (options[nopts]);
    }
    return (vars);
  }
  return (NULL);
}

/* write a ~/ rc file ******************************************************/
void config_write_rc_file (ConfigVariable *vars)
{
  gchar line[512];
  gchar *rcname;
  FILE *fname;

  rcname = g_strdup_printf ("%s/.gtapecalcrc", g_get_home_dir());
  fname = fopen (rcname, "w");
  g_free (rcname);
  if (!fname) {
    perror ("gtapecalc-config_write_rc_file");
    g_warning ("Couldn't open an rc file for writing");
    return;
  }

  fputs("# config file for gtapecalc\n#it will be overwritten automatically\n", fname);

  sprintf (line, "win_x_size=%d\n", vars->win_x_size);
  fputs (line, fname);

  sprintf (line, "win_y_size=%d\n", vars->win_y_size);
  fputs (line, fname);

  sprintf (line, "pane_width=%d\n", vars->pane_width);
  fputs (line, fname);

  if (vars->entry_font_name == NULL)
    sprintf (line, "entry_font=\n");
  else
    sprintf (line, "entry_font=%s\n", vars->entry_font_name);
  fputs (line, fname);

  if (vars->clist_font_name == NULL)
    sprintf (line, "clist_font=\n");
  else
    sprintf (line, "clist_font=%s\n", vars->clist_font_name);
  fputs (line, fname);

  if (vars->button_font_name == NULL)
    sprintf (line, "button_font=\n");
  else
    sprintf (line, "button_font=%s\n", vars->button_font_name);
  fputs (line, fname);

  sprintf (line, "column_title_show=%d\n", vars->column_title_show);
  fputs (line, fname);

  sprintf (line, "print_command=%s\n", vars->print_command);
  fputs (line, fname);

  sprintf (line, "page_width=%d\n", vars->page_width);
  fputs (line, fname);

  sprintf (line, "column_width=%d\n", vars->column_width);
  fputs (line, fname);

  sprintf (line, "precision=%d\n", vars->precision);
  fputs (line, fname);

  fclose (fname);

  rcname = g_strdup_printf ("%s/.gtapecalcgtkrc", g_get_home_dir());
  fname = fopen (rcname, "w");
  g_free (rcname);
  if (!fname) {
    perror ("gtapecalc-config_write_rc_file");
    g_warning ("Couldn't open a gtkrc file");
    return;
  }
  fputs ("# gtapecalc rc file for theme related preferences\n", fname);
  fputs ("# it will be overwritten automatically by gtapecalc\n\n", fname);

  if (vars->clist_font_name == NULL)
    sprintf (line, "style \"output\"\n{\n  font =\"\"\n}\n\n");
  else
    sprintf (line, "style \"output\"\n{\n  font = \"%s\"\n}\n\n",
             vars->clist_font_name);
  fputs (line, fname);

  if (vars->button_font_name == NULL)
    sprintf (line, "style \"keybutton\"\n{\n  font =\"\"\n}\n\n");
  else
    sprintf (line, "style \"keybutton\"\n{\n  font = \"%s\"\n}\n\n",
             vars->button_font_name);
  fputs (line, fname);

  fputs ("widget \"*output_window*\" style \"output\"\n\n", fname);

  fputs ("widget \"*keypad_button*\" style \"keybutton\"\n\n", fname);

  fclose (fname);
}


