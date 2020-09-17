/***************************************************************************
                          gtc_dialogs.c  -  dialogs
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

#include "gtc_dialogs.h"
#include "gtc_utils.h"
#include "gtc_callbacks.h"

/* dialog for adding comments to the output window *************************/
void edit_tape_dialog (GtkWidget *widget)
{
  /*extern gint selected_row;*/
  GtkWidget *twindow;

  GtkWidget *dialog;
  GtkWidget *ok_button;
  GtkWidget *cancel_button;
  GtkWidget *dlg_entry;
  GtkWidget *frame;
  gchar *comment;
  gint x, y;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  // You can't put a label on a null entry, so we need to check if there is a
  // valid entry before allowing the user to label it.
  if (!cw->selected_row) {
    dlg_error_msg (cw->window, "You must select a row before\nadding a note.");
    return;
  }

  twindow = cw->window;

  gdk_window_get_deskrelative_origin (twindow->window, &x, &y);

  dialog = gtk_dialog_new ();
  gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);
  gtk_widget_set_usize (GTK_WIDGET(dialog), 300, 150);
  gtk_widget_set_uposition (GTK_WIDGET(dialog), x, y);

  gtk_window_set_transient_for (GTK_WINDOW(dialog),
                                GTK_WINDOW(twindow));

  cancel_button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->action_area),
		      cancel_button, TRUE, TRUE, 0);

  g_signal_connect_swapped (GTK_OBJECT(cancel_button), "clicked",
			     G_CALLBACK(destroy_dialog),
			     GTK_OBJECT(dialog));

  gtk_widget_show (cancel_button);

  ok_button = gtk_button_new_from_stock (GTK_STOCK_OK);
  GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->action_area),
                             ok_button, TRUE, TRUE, 0);

  gtk_widget_grab_default (GTK_WIDGET(ok_button));
    gtk_widget_show (ok_button);

  frame = gtk_frame_new ("Add A Note");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox),
		      frame, TRUE, TRUE, 10);
  gtk_widget_show (frame);

  dlg_entry = gtk_entry_new ();

  g_signal_connect_swapped (GTK_OBJECT (dlg_entry),
			     "focus_in_event",
			     G_CALLBACK( gtk_widget_grab_default),
			     GTK_OBJECT (ok_button));
  g_signal_connect_swapped (GTK_OBJECT (dlg_entry), "activate",
                             G_CALLBACK( gtk_button_clicked),
                             GTK_OBJECT (ok_button));
  g_signal_connect_swapped (GTK_OBJECT(ok_button), "clicked",
			     G_CALLBACK(insert_comment),
			     (gpointer)dialog);

  gtk_container_add (GTK_CONTAINER(frame), dlg_entry);
  gtk_widget_show (dlg_entry);

  gtk_object_set_data (GTK_OBJECT(dialog), "entry", dlg_entry);

  tlist_get_text (cw->clist, cw->selected_row, 0, &comment);

  gtk_entry_set_text (GTK_ENTRY(dlg_entry), comment);
  gtk_entry_select_region (GTK_ENTRY(dlg_entry), 0,
			   strlen(comment));

  g_free (comment);

  gtk_object_set_data (GTK_OBJECT(dialog), "twindow", twindow);

  gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);
  /*gtk_grab_add (dialog);*/

  gtk_widget_show (dialog);
  gtk_widget_grab_focus (GTK_WIDGET(dlg_entry));
}

/* shows a dialog with parameters passed to it ******************************/
GtkWidget* dialog_new (GtkWidget *widget, DialogItem *dlg, ButtonItem *btns)
{
  GtkWidget *twindow;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *ndialog;
  gint i;
  GtkStyle  *style;


  twindow = get_data_from_toplevel(widget, "twindow");

  dlg->dialog = gtk_dialog_new();

  ndialog = dlg->dialog;

  if (dlg->is_modal)
    gtk_window_set_modal (GTK_WINDOW(ndialog), TRUE);

  gtk_window_set_transient_for (GTK_WINDOW(ndialog),
                                GTK_WINDOW(twindow));

  style = gtk_widget_get_style (ndialog);

  gtk_widget_set_usize (ndialog, dlg->size_x, dlg->size_y);
  gtk_window_set_title (GTK_WINDOW(ndialog), dlg->title);
  gtk_container_border_width (GTK_CONTAINER (ndialog), 10);

  g_signal_connect (GTK_OBJECT (ndialog), "destroy",
                      G_CALLBACK(gtk_widget_destroy), ndialog);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(GTK_DIALOG(ndialog)->vbox), hbox);
  gtk_widget_show (hbox);

  if (dlg->pixmap) {
    GtkWidget *pixwid;
    GtkStockItem item;

    if (gtk_stock_lookup (dlg->pixmap, &item))
      {
	pixwid = gtk_image_new_from_stock (dlg->pixmap,
					   GTK_ICON_SIZE_DIALOG);
	gtk_container_add (GTK_CONTAINER(hbox), pixwid);
	gtk_widget_show (pixwid);
      }
    else
      g_warning ("Stock dialog ID doesn't exist?");
  }

  label = gtk_label_new (dlg->message);
  gtk_box_pack_start (GTK_BOX(hbox),
		      label, TRUE, TRUE, 10);
  gtk_widget_show (label);

  for (i = 0; i< dlg->nbuttons; i++) {

    btns[i].button = gtk_button_new_from_stock (btns[i].label);
    GTK_WIDGET_SET_FLAGS (btns[i].button, GTK_CAN_DEFAULT);

    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (ndialog)->action_area),
			btns[i].button,
			FALSE, TRUE, 5);
    g_signal_connect_swapped (GTK_OBJECT (btns[i].button), "clicked",
			       G_CALLBACK (btns[i].sigfunc),
			       GTK_OBJECT(ndialog));
    /* gtk_widget_show (btns[i].button);*/

    if (btns[i].default_btn)
      gtk_widget_grab_default (btns[i].button);

    gtk_widget_show (btns[i].button);
  }
  /*gtk_grab_add (ndialog);*/
  gtk_widget_show (ndialog);

  gtk_object_set_data (GTK_OBJECT(ndialog), "twindow", twindow);
  return (ndialog);
}

/* dialog for handling errors ***********************************************/
void dlg_error_msg (GtkWidget *widget, gchar *err_str)
{
  gchar *err_msg = g_strdup(err_str);

  ButtonItem err_buttons[] = {
    {NULL, GTK_STOCK_OK, G_CALLBACK(destroy_dialog), TRUE}
  };

  DialogItem dlg_items = {
    NULL,
    350, 150,
    TRUE,
    "Error !",
    GTK_STOCK_DIALOG_ERROR,
    err_msg,
    sizeof(err_buttons) /sizeof(ButtonItem)
  };
  dialog_new (widget, &dlg_items, err_buttons);

  g_free (err_msg);
}

/* delete line dialog *******************************************************/
void dlg_delete_line (GtkWidget *widget)
{
  DialogItem dlg_items = {
    NULL,
    350, 150,
    TRUE,
    "Warning!",
    GTK_STOCK_DIALOG_WARNING,
    "Deleting a line will also remove the\ncalculation associated with it!\n\nDelete line ?",
    2
  };
  ButtonItem dlg_buttons[] = {
    {NULL, GTK_STOCK_DELETE, G_CALLBACK(remove_row), FALSE},
    {NULL, GTK_STOCK_CANCEL, G_CALLBACK(destroy_dialog), TRUE}
  };

  dialog_new (widget, &dlg_items, dlg_buttons);
}

/* shows the about dialog ***************************************************/
void show_about(GtkWidget *widget)
{
  gchar *title_string = g_malloc0 (256 * sizeof(gchar));

  sprintf (title_string, "  %s \n version %s \n\n http://gtapecalc.sourceforge.net \n\n (c)1999 by Stephen Witkop \n ",
	  PACKAGE, VERSION);

  if (title_string) {
    DialogItem about_items = {
      NULL,
      320, 200,
      FALSE,
      "About ?",
      "gtc_gtapecalc",
      title_string,
      1
    };
    ButtonItem about_buttons[] = {
      {NULL, GTK_STOCK_OK, G_CALLBACK(destroy_dialog), TRUE},
    };

    dialog_new (widget, &about_items, about_buttons);
    g_free (title_string);
  }
}

/* preferences dialog ******************************************************/
void dlg_preferences_show (GtkWidget *widget)
{
  /*static guint init = 0;*/
  GtkWidget *window;
  GtkWidget *notebook;
  GtkWidget *frame;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *vbox1;
  GtkWidget *vbox2;
  GtkWidget *vbox3;
  GtkWidget *hbox;
  GtkWidget *button;
  GtkWidget *checkbutton;
  GtkWidget *checkbutton2;
  GtkWidget *spinbutton;
  GtkWidget *entry;
  GtkAdjustment *adj;
  gchar buffer[16];
  gint i;
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  //window = gtk_window_new (GTK_WINDOW_DIALOG);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  // 2.0 Change
  gtk_window_set_title (GTK_WINDOW(window), "Preferences");
  /*gtk_window_set_default_size (GTK_WINDOW(window), 300, 400);*/
  gtk_window_set_modal (GTK_WINDOW(window), TRUE);
  gtk_window_set_transient_for (GTK_WINDOW(window),
                                GTK_WINDOW(cw->window));
  gtk_container_set_border_width (GTK_CONTAINER(window), 10);
  g_signal_connect (GTK_OBJECT(window), "delete_event",
	  G_CALLBACK(destroy_dialog), NULL);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(window), vbox);
  gtk_widget_show (vbox);

  notebook = gtk_notebook_new();
  gtk_notebook_set_show_border (GTK_NOTEBOOK(notebook),TRUE);
  gtk_notebook_set_tab_hborder (GTK_NOTEBOOK(notebook), 5);
  gtk_notebook_set_homogeneous_tabs (GTK_NOTEBOOK(notebook), TRUE);
  gtk_box_pack_start (GTK_BOX(vbox), notebook, FALSE, TRUE, 10);
  gtk_widget_show (notebook);

  /** View page **/
  vbox2 = gtk_vbox_new (FALSE, 0);
  frame = gtk_frame_new ("Precision");
  gtk_container_set_border_width (GTK_CONTAINER(frame), 10);
  /*gtk_widget_set_usize (GTK_WIDGET(frame), 75, 100);*/
  gtk_box_pack_start (GTK_BOX(vbox2), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);
  vbox3 = gtk_vbox_new (FALSE, 30);
  gtk_container_add (GTK_CONTAINER(frame), vbox3);
  gtk_widget_show (vbox3);
  hbox = gtk_hbox_new (FALSE, 10);
  /*gtk_container_add (GTK_CONTAINER(frame), hbox);*/
  gtk_box_pack_start (GTK_BOX(vbox3), hbox, FALSE, FALSE, 10);
  gtk_widget_show (hbox);

  i = cw->widget_vars->precision;
  if (i < 0)
    i = 2;
  adj = (GtkAdjustment *)gtk_adjustment_new(i, -1.0, 8.0,
	                                    1.0, 5.0, 0.0);
  spinbutton = gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON(spinbutton), TRUE);
  gtk_widget_set_usize (spinbutton, 45, -1);
  gtk_box_pack_start (GTK_BOX(hbox), spinbutton, FALSE, FALSE, 10);


  checkbutton = gtk_check_button_new_with_label ("Floating Point");
  gtk_box_pack_start (GTK_BOX(hbox), checkbutton, FALSE, FALSE, 0);
  g_signal_connect (GTK_OBJECT(checkbutton), "toggled",
	  G_CALLBACK(fp_toggle),
	  GTK_OBJECT(window));
  gtk_widget_show (spinbutton);
  gtk_widget_show (checkbutton);

  gtk_object_set_data (GTK_OBJECT(window), "precision_spin", spinbutton);
  gtk_object_set_data (GTK_OBJECT(window), "fp_checkbutton", checkbutton);

  frame = gtk_frame_new ("Window");
  gtk_container_set_border_width (GTK_CONTAINER(frame), 10);
  gtk_box_pack_start (GTK_BOX(vbox2), frame, FALSE, TRUE, 10);
  gtk_widget_show (frame);
  gtk_widget_show (vbox2);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(frame), vbox3);
  gtk_widget_show (vbox3);

  checkbutton2 = gtk_check_button_new_with_label ("Show Column Headers");
  gtk_box_pack_start (GTK_BOX(vbox3), checkbutton2, FALSE, TRUE, 10);
  if (cw->widget_vars->column_title_show)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(checkbutton2), TRUE);
  gtk_widget_show (checkbutton2);
  gtk_object_set_data (GTK_OBJECT(window), "show_headers", checkbutton2);

  checkbutton2 = gtk_check_button_new_with_label ("Save Current Window Size");
  gtk_box_pack_start (GTK_BOX(vbox3), checkbutton2, FALSE, TRUE, 10);
  gtk_widget_show (checkbutton2);
  gtk_object_set_data (GTK_OBJECT(window), "save_win_check", checkbutton2);

  gtk_widget_show (vbox2);
  label = gtk_label_new ("View");
  gtk_notebook_append_page (GTK_NOTEBOOK(notebook), vbox2, label);

  /** Fonts page **/
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox1), vbox2, TRUE, FALSE, 0);
  gtk_widget_show (vbox2);

  label = gtk_label_new ("Calculator Font");
  gtk_box_pack_start (GTK_BOX(vbox2), label, FALSE, FALSE, 0);
  gtk_widget_show (label);
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);
  entry = gtk_entry_new ();
  /*gtk_entry_set_editable (GTK_ENTRY(entry), FALSE);*/
  gtk_box_pack_start (GTK_BOX(hbox), entry, FALSE, FALSE, 0);
  gtk_widget_set_usize (entry, 250, -1);
  gtk_widget_show (entry);
  if (cw->widget_vars->entry_font_name)
    gtk_entry_set_text (GTK_ENTRY(entry),
	    cw->widget_vars->entry_font_name);
  gtk_editable_set_position (GTK_EDITABLE(entry), 0);
  button = gtk_button_new_with_label ("Font");
  g_signal_connect (GTK_OBJECT(button), "clicked",
	  G_CALLBACK(dlg_font_select), entry);
  gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, FALSE, 5);
  gtk_widget_show (button);
  gtk_object_set_data (GTK_OBJECT(window), "entry_font", entry);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox1), vbox2, TRUE, FALSE, 0);
  gtk_widget_show (vbox2);

  label = gtk_label_new ("Tape Font");
  gtk_box_pack_start (GTK_BOX(vbox2), label, FALSE, FALSE, 0);
  gtk_widget_show (label);
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);
  entry = gtk_entry_new ();
  /*gtk_entry_set_editable (GTK_ENTRY(entry), FALSE);*/
  gtk_box_pack_start (GTK_BOX(hbox), entry, FALSE, FALSE, 0);
  gtk_widget_set_usize (entry, 250, -1);
  gtk_widget_show (entry);
  if (cw->widget_vars->clist_font_name)
    gtk_entry_set_text (GTK_ENTRY(entry),
	      cw->widget_vars->clist_font_name);
	else gtk_entry_set_text (GTK_ENTRY(entry), "");
  gtk_editable_set_position (GTK_EDITABLE(entry), 0);
  button = gtk_button_new_with_label ("Font");
  g_signal_connect (GTK_OBJECT(button), "clicked",
	    G_CALLBACK(dlg_font_select), entry);
  gtk_box_pack_start (GTK_BOX(hbox), button, TRUE, FALSE, 0);
  gtk_widget_show (button);
  gtk_object_set_data (GTK_OBJECT(window), "tape_font", entry);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox1), vbox2, TRUE, FALSE, 0);
  gtk_widget_show (vbox2);

  label = gtk_label_new ("Button Font");
  gtk_box_pack_start (GTK_BOX(vbox2), label, FALSE, FALSE, 0);
  gtk_widget_show (label);
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);
  entry = gtk_entry_new ();
  /*gtk_entry_set_editable (GTK_ENTRY(entry), FALSE);*/
  gtk_box_pack_start (GTK_BOX(hbox), entry, FALSE, FALSE, 0);
  gtk_widget_set_usize (entry, 250, -1);
  gtk_widget_show (entry);
  if (cw->widget_vars->button_font_name)
    gtk_entry_set_text (GTK_ENTRY(entry),
	      cw->widget_vars->button_font_name);
  gtk_editable_set_position (GTK_EDITABLE(entry), 0);
  button = gtk_button_new_with_label ("Font");
  g_signal_connect (GTK_OBJECT(button), "clicked",
	    G_CALLBACK(dlg_font_select), entry);
  gtk_box_pack_start (GTK_BOX(hbox), button,TRUE, FALSE, 5);
  gtk_widget_show (button);
  gtk_object_set_data (GTK_OBJECT(window), "button_font", entry);

  label = gtk_label_new ("Fonts");
  gtk_notebook_append_page (GTK_NOTEBOOK(notebook), vbox1, label);

  /** Printing page **/
  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);

  frame = gtk_frame_new ("Print Command");
  gtk_container_set_border_width (GTK_CONTAINER(frame), 10);
  gtk_widget_set_usize (GTK_WIDGET(frame), 200, 100);
  gtk_box_pack_start (GTK_BOX(vbox2), frame, FALSE, TRUE, 10);
  gtk_widget_show (frame);
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(frame), hbox);
  gtk_widget_show (hbox);
  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(hbox), vbox3, FALSE, FALSE, 5);
  gtk_widget_show (vbox3);
  entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX(vbox3), entry, TRUE, FALSE, 5);
  gtk_entry_set_text (GTK_ENTRY(entry),
	  cw->widget_vars->print_command);
  gtk_widget_show (entry);
  gtk_object_set_data (GTK_OBJECT(window), "print_command", entry);

  frame = gtk_frame_new (" Page ");
  gtk_container_set_border_width (GTK_CONTAINER(frame), 10);
  gtk_box_pack_start (GTK_BOX(vbox2), frame, FALSE, TRUE, 10);
  gtk_widget_show (frame);
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(frame), hbox);
  gtk_widget_show (hbox);
  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(hbox), vbox3, FALSE, FALSE, 5);
  gtk_widget_show (vbox3);
  label = gtk_label_new ("Page Width");
  gtk_box_pack_start (GTK_BOX(vbox3), label, TRUE, FALSE, 5);
  gtk_widget_show (label);
  entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX(vbox3), entry, TRUE, FALSE, 5);
  sprintf (buffer, "%i", cw->widget_vars->page_width);
  gtk_entry_set_text (GTK_ENTRY(entry), buffer);
  gtk_widget_show (entry);
  gtk_object_set_data (GTK_OBJECT(window), "page_width", entry);

  label = gtk_label_new ("Column Width");
  gtk_box_pack_start (GTK_BOX(vbox3), label, TRUE, FALSE, 5);
  gtk_widget_show (label);

  entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX(vbox3), entry, FALSE, TRUE, 5);
  sprintf (buffer, "%i", cw->widget_vars->column_width);
  gtk_entry_set_text (GTK_ENTRY(entry), buffer);
  gtk_widget_show (entry);
  gtk_object_set_data (GTK_OBJECT(window), "column_width", entry);


  label = gtk_label_new ("    Printing    ");
  gtk_notebook_append_page (GTK_NOTEBOOK(notebook), vbox2, label);

  /** buttons **/
  hbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 10);
  gtk_widget_show (hbox);

  button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  g_signal_connect_swapped (GTK_OBJECT(button), "clicked",
	  G_CALLBACK(destroy_dialog),
	  GTK_OBJECT(window));
  gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 10);
  gtk_widget_show (button);

  button = gtk_button_new_from_stock (GTK_STOCK_OK);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  g_signal_connect_swapped (GTK_OBJECT(button), "clicked",
	  G_CALLBACK(load_dialog_options),
	  GTK_OBJECT(window));
  gtk_box_pack_start (GTK_BOX(hbox), button,FALSE, TRUE, 10);
  gtk_widget_grab_default (button);
  gtk_widget_show (button);

  gtk_object_set_data (GTK_OBJECT(window), "twindow", cw->window);
  gtk_widget_show (window);

  if (cw->widget_vars->precision < 0)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(checkbutton), TRUE);
}

/* Show the print dialog ***************************************************/
void dlg_print (GtkWidget *widget)
{
  GtkWidget *window;
  GtkWidget *vbox, *vbox2, *hbox, *hbox2;
  GtkWidget *frame;
  GtkWidget *rbutton1, *rbutton2;
  GtkWidget *entry1, *entry2;
  GtkWidget *label;
  GtkWidget *hsep;
  GtkWidget *bbox;
  GtkWidget *button;
  gchar *str;
  /*PrintOption *options;*/
  CalcWindow *cw = get_data_from_toplevel (widget, "cwindow");

  /* options = g_new0 (PrintOption, 1);
  options->print_command = g_strdup (cw->widget_vars->print_command);
  options->to_file_name = g_strdup ("output.ps");
  options->page_width = cw->widget_vars->page_width;
  options->column_width = cw->widget_vars->column_width;
  */

  //window = gtk_window_new (GTK_WINDOW_DIALOG);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  // 2.0 Change
  gtk_window_set_title (GTK_WINDOW(window), "Print");
  /*gtk_window_set_default_size (GTK_WINDOW(window), 200, 200);*/
  gtk_window_set_modal (GTK_WINDOW(window), TRUE);
  gtk_window_set_transient_for (GTK_WINDOW(window),
                                GTK_WINDOW(cw->window));
  gtk_container_set_border_width (GTK_CONTAINER(window), 10);
  g_signal_connect (GTK_OBJECT(window), "delete_event",
	  G_CALLBACK(destroy_dialog), NULL);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(window), vbox);

  /* Select Printer frame */
  hbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  frame = gtk_frame_new ("Select Printer");
  gtk_container_set_border_width (GTK_CONTAINER(frame), 20);
  gtk_box_pack_start (GTK_BOX(hbox), frame, FALSE, FALSE, 0);

  vbox2 = gtk_vbox_new (FALSE, 10);
  gtk_container_add (GTK_CONTAINER(frame), vbox2);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  rbutton1 = gtk_radio_button_new_with_label (NULL, "Printer");
  g_signal_connect (GTK_OBJECT(rbutton1), "toggled",
		      G_CALLBACK(dlg_set_radio_entry_insensitive),
		      NULL);
  gtk_box_pack_start (GTK_BOX(hbox), rbutton1, FALSE, FALSE, 0);

  entry1 = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY(entry1), cw->widget_vars->print_command);
  gtk_box_pack_start (GTK_BOX(hbox), entry1, FALSE, FALSE, 0);

  gtk_object_set_data (GTK_OBJECT(rbutton1), "entry", entry1);
  gtk_object_set_data (GTK_OBJECT(window), "printer_button", rbutton1);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  rbutton2 = gtk_radio_button_new_with_label (
	       gtk_radio_button_group (GTK_RADIO_BUTTON(rbutton1)),
	       "File    ");
  g_signal_connect (GTK_OBJECT(rbutton2), "toggled",
		      G_CALLBACK(dlg_set_radio_entry_insensitive),
		      NULL);
  gtk_box_pack_start (GTK_BOX(hbox), rbutton2, FALSE, FALSE, 0);
  entry2 = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY(entry2), "output.ps");
  gtk_box_pack_start (GTK_BOX(hbox), entry2, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (entry2, FALSE);

  gtk_object_set_data (GTK_OBJECT(rbutton2), "entry", entry2);
  gtk_object_set_data (GTK_OBJECT(window), "print_file_button", rbutton2);

  /* Select Header frame */
  hbox2 = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);

  frame = gtk_frame_new ("Header");
  gtk_container_set_border_width (GTK_CONTAINER(frame), 0);
  gtk_box_pack_start (GTK_BOX(hbox2), frame, FALSE, FALSE, 0);

  vbox2 = gtk_vbox_new (FALSE, 10);
  gtk_container_add (GTK_CONTAINER(frame), vbox2);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  rbutton1 = gtk_radio_button_new_with_label (NULL, "Use Filename as Header");
  gtk_box_pack_start (GTK_BOX(hbox), rbutton1, FALSE, FALSE, 0);
  gtk_object_set_data (GTK_OBJECT(window), "fname_header_button", rbutton1);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  rbutton2 = gtk_radio_button_new_with_label (
	       gtk_radio_button_group (GTK_RADIO_BUTTON(rbutton1)),
	       "Don't Print a Header");
  gtk_box_pack_start (GTK_BOX(hbox), rbutton2, FALSE, FALSE, 0);
  gtk_object_set_data (GTK_OBJECT(window), "no_header_button", rbutton2);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  rbutton2 = gtk_radio_button_new_with_label (
	       gtk_radio_button_group (GTK_RADIO_BUTTON(rbutton1)),
	       "Custom Header");
  g_signal_connect (GTK_OBJECT(rbutton2), "toggled",
		      G_CALLBACK(dlg_set_radio_entry_insensitive),
		      NULL);
  gtk_box_pack_start (GTK_BOX(hbox), rbutton2, FALSE, FALSE, 0);
  gtk_object_set_data (GTK_OBJECT(window), "custom_header_button", rbutton2);

  entry2 = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX(hbox), entry2, FALSE, FALSE, 0);

  gtk_object_set_data (GTK_OBJECT(rbutton2), "entry", entry2);

  /* Page Format frame */

  frame = gtk_frame_new ("Page Format");
  gtk_container_set_border_width (GTK_CONTAINER(frame), 0);
  gtk_box_pack_start (GTK_BOX(hbox2), frame, FALSE, FALSE, 0);

  vbox2 = gtk_vbox_new (FALSE, 10);
  gtk_container_add (GTK_CONTAINER(frame), vbox2);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Page Width   ");
  gtk_box_pack_start (GTK_BOX(hbox), label, FALSE, FALSE, 0);

  entry1 = gtk_entry_new ();
  str = g_strdup_printf ("%d", cw->widget_vars->page_width);
  gtk_entry_set_text (GTK_ENTRY(entry1), str);
  gtk_widget_set_usize (GTK_WIDGET(entry1), 30, -1);
  gtk_box_pack_start (GTK_BOX(hbox), entry1, FALSE, FALSE, 0);
  g_free (str);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Column Width");
  gtk_box_pack_start (GTK_BOX(hbox), label, FALSE, FALSE, 0);

  entry2 = gtk_entry_new ();
  str = g_strdup_printf ("%d", cw->widget_vars->column_width);
  gtk_entry_set_text (GTK_ENTRY(entry2), str);
  gtk_widget_set_usize (GTK_WIDGET(entry2), 30, -1);
  gtk_box_pack_start (GTK_BOX(hbox), entry2, FALSE, FALSE, 0);
  g_free (str);

  gtk_object_set_data (GTK_OBJECT(window), "column_width_entry", entry1);
  gtk_object_set_data (GTK_OBJECT(window), "page_width_entry", entry2);
  gtk_object_set_data (GTK_OBJECT(window), "cwindow", cw);
  gtk_object_set_data (GTK_OBJECT(window), "twindow", window);

  /* Horizontal separator to start the button box */
  hsep = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(vbox), hsep, FALSE, FALSE, 20);

  bbox = gtk_hbutton_box_new ();
  gtk_container_set_border_width (GTK_CONTAINER(bbox), 5);
  gtk_container_add (GTK_CONTAINER(vbox), bbox);

  gtk_button_box_set_layout (GTK_BUTTON_BOX(bbox),
			     GTK_BUTTONBOX_END);

  button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_container_add (GTK_CONTAINER(bbox), button);
  g_signal_connect_swapped (GTK_OBJECT(button), "clicked",
			     G_CALLBACK(destroy_dialog),
			     GTK_OBJECT(window));

  button = gtk_button_new_from_stock (GTK_STOCK_PRINT_PREVIEW);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_container_add (GTK_CONTAINER(bbox), button);
  g_signal_connect (GTK_OBJECT(button), "clicked",
		      G_CALLBACK(print_file_cb),
		      "preview");

  button = gtk_button_new_from_stock (GTK_STOCK_PRINT);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_container_add (GTK_CONTAINER(bbox), button);
  gtk_widget_grab_default (GTK_WIDGET(button));
  g_signal_connect (GTK_OBJECT(button), "clicked",
		     G_CALLBACK(print_file_cb),
		     "print");

  gtk_widget_show_all (window);
}

/* Sets the associated entry of a radio button insensitive *****************/
void dlg_set_radio_entry_insensitive (GtkWidget *widget)
{
  GtkWidget *entry;

  entry = gtk_object_get_data (GTK_OBJECT(widget), "entry");
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
    gtk_widget_set_sensitive (entry, TRUE);
    gtk_widget_grab_focus (entry);
  }
  else {
    gtk_widget_set_sensitive (entry, FALSE);
  }
}

/* Shows the Font Selection dialog *****************************************/
void dlg_font_select (GtkWidget *widget, gpointer data)
{
  const gchar *current_font;
  GtkWidget *fs;
  GtkWidget *entry = (GtkWidget *)data;

  current_font = gtk_entry_get_text (GTK_ENTRY(entry));

  fs = gtk_font_selection_dialog_new ("Calculator Font");

  gtk_window_set_modal (GTK_WINDOW (fs), TRUE);
  gtk_window_set_transient_for (GTK_WINDOW(fs),
	  GTK_WINDOW(get_toplevel_widget(widget)));

  g_print (current_font);
  if (strcmp(current_font, "") != 0)
  gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(fs),
	  current_font);
  g_signal_connect_swapped (GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(fs)->ok_button),
	  "clicked", G_CALLBACK(dlg_get_font), GTK_OBJECT(fs));

  g_signal_connect_swapped(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(fs)->cancel_button),
	      "clicked", G_CALLBACK(destroy_dialog), GTK_OBJECT(fs));
  gtk_object_set_data (GTK_OBJECT(fs), "entry", entry);

  gtk_widget_show (fs);
}

/* get the selection from the font dialog and put in the preferences entry */
void dlg_get_font (GtkWidget *widget, gpointer data)
{
  gchar *font;
  GtkWidget *entry = gtk_object_get_data (GTK_OBJECT(widget), "entry");

  font = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(widget));
  gtk_entry_set_text (GTK_ENTRY(entry), font);

  g_free (font);
  gtk_widget_destroy (GTK_WIDGET(widget));
}

/* destroys the dialog passed to it ****************************************/
void destroy_dialog(GtkWidget *widget, gpointer data)
{
  gpointer data_to_free = NULL;

  data_to_free = gtk_object_get_data (GTK_OBJECT(widget), "gfree_data");
  if (data_to_free)
    g_free ((gchar *)data_to_free);
  gtk_widget_destroy (widget);
}
