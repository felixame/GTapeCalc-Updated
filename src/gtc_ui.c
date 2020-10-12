/***************************************************************************
                          gtc_ui.c  -  ui code
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

/* #include <stdio.h> */
/* #include <gtk/gtk.h> */

/* #include "gtc_ui.h" */
/* #include "gtc_calc.h" */
/* #include "gtc_callbacks.h" */
/* #include "gtc_prefs.h" */
/* #include "gtc_utils.h" */
/* #include "gtc_window.h" */

/* static GtkTreeModel *make_model (void); */
/* static void make_columns (GtkTreeView *tree); */

/* static CalcButton button_list[] = { */
/*     {"(",    0, 0, NULL, NULL,       "(",   BTN_PAREN_LEFT}, */
/*     {")",    1, 0, NULL, NULL,       ")",   BTN_PAREN_RIGHT}, */
/*     {"÷",    2, 0, NULL, NULL,       "÷",   BTN_DIVIDE}, */
/*     {"×",    3, 0, NULL, NULL,       "×",   BTN_MULTIPLY}, */
/*     {"−",    4, 0, NULL, NULL,       "−",   BTN_MINUS}, */

/*     {"%",    0, 1, NULL, NULL,       "%",   BTN_PERCENT}, */
/*     {"7",    1, 1, NULL, NULL,       "7",   BTN_SEVEN}, */
/*     {"8",    2, 1, NULL, NULL,       "8",   BTN_EIGHT}, */
/*     {"9",    3, 1, NULL, NULL,       "9",   BTN_NINE}, */
/*     {"+",    4, 1, NULL, NULL,       "+",   BTN_PLUS}, */

/*     {"𝑥²",   0, 2, NULL, NULL,       "𝑥²", BTN_X_SQUARED}, */
/*     {"4",    1, 2, NULL, NULL,       "4",   BTN_FOUR}, */
/*     {"5",    2, 2, NULL, NULL,       "5",   BTN_FIVE}, */
/*     {"6",    3, 2, NULL, NULL,       "6",   BTN_SIX}, */

/*     {"(−)",  0, 3, NULL, NULL,       "(−)",  BTN_PLUS_MINUS}, */
/*     {"1",    1, 3, NULL, NULL,        "1",  BTN_ONE}, */
/*     {"2",    2, 3, NULL, NULL,        "2",  BTN_TWO}, */
/*     {"3",    3, 3, NULL, NULL,        "3",  BTN_THREE}, */
/*     {"=",    4, 3, NULL, NULL,        "=",  BTN_EQUAL}, */

/*     {"AC",   0, 4, NULL, clear,       "AC", BTN_CLEAR_ALL}, */
/*     {"CE",   1, 4, NULL, entry_clear, "CE", BTN_CLEAR_ENTRY}, */
/*     {"0",    2, 4, NULL, NULL,        "0",  BTN_ZERO}, */
/*     {".",    3, 4, NULL, NULL,        ".",  BTN_DEC_POINT} */
/*   }; */

/* static gint nbuttons = sizeof(button_list) / sizeof(CalcButton); */

/* const gint BOX_PADDING = 15; */
/* const gint CALC_BUTTON_GRID_COLUMN_PADDING = 5; */
/* const gint CALC_BUTTON_GRID_ROW_PADDING = 5; */
/* const gint NO_PADDING = 0; */

/* gint make_ui(CalcWindow *cwindow) */
/* { */

/*   GtkWidget *newHeaderBar; */


/*   GtkWidget *mainWindowVbox;    // GtkBox with vertical orientation that all */
/*                                 // window contents are packed into. */

/*   GtkWidget *mainWindowHpaned;  // GtkPaned with horizontal orientation that */
/*                                 // contents of mainWindowVbox are packed into. */

/*   GtkWidget *inputPaneVbox;     // GtkBox with vertical orientation that widgets */
/*                                 // on the calculator side are packed into. */

/*   GtkWidget *newCalcEntry;      // GtkEntry whose text represents current input */
/*                                 // on the calculator display. */


/*   GtkWidget *calcButtonGrid;    // Custom widget comprised of GtkGrid container */
/*                                 // and multiple GtkButton. */

/*   GtkWidget *outputPaneVbox; */
/*   GtkWidget *outputPaneScrolledWindow; */

/*   GtkWidget *popup_menu; */
/*   gchar *rcname; */
/*   GtkTreeModel *model; */
/* 	GtkTreeSelection *select; */

/*   ConfigVariable *prefs = cwindow->widget_vars; */

/*   // TODO: Move this to ~/.config/gtapecalc. */
/*   // No need to clutter up home dirs with dot files. */
/*   rcname = g_strdup_printf ("%s/.gtapecalcgtkrc", g_get_home_dir ()); */
/*   gtk_rc_parse (rcname); */
/*   g_free (rcname); */

/*   //config_load_defaults(); */

/*   // Header Bar. */
/*   newHeaderBar = gtk_header_bar_new(); */
/*   make_header_bar(newHeaderBar); // Handles creation and adding of */
/*                                  // buttons to the header bar as well */
/*                                  // as connecting them to associated */
/*                                  // popover menus. */

/*   gtk_window_set_titlebar (GTK_WINDOW(cwindow->window), newHeaderBar); */
/*   gtk_widget_show(newHeaderBar); */

/*   cwindow->headerBar = newHeaderBar; */

/*   // Main GtkBox with vertical orientation. */
/*   mainWindowVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, NO_PADDING); */
/*   gtk_widget_set_name (mainWindowVbox, "mainWindowVbox"); */
/*   gtk_container_add (GTK_CONTAINER (cwindow->window), mainWindowVbox); */
/*   gtk_widget_show (mainWindowVbox); */

/*   // Main GtkPanned with horizontal orientation. */
/*   mainWindowHpaned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL); */
/*   gtk_widget_set_name (mainWindowHpaned, "mainWindowHbox"); */
/*   //gtk_container_add (GTK_CONTAINER(mainWindowVbox), mainWindowHpaned); */
/*   gtk_container_add (GTK_CONTAINER(cwindow->window), mainWindowHpaned); */
/*   gtk_widget_show (mainWindowHpaned); */

/*   Vboxes for main window ************************************************ */

/*   // Input Pane GtkBox with vertical orientation. */
/*   // inputPaneVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10); */
/*   inputPaneVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, BOX_PADDING); */
/*   gtk_widget_set_name (inputPaneVbox, "inputPaneVbox"); */
/*   gtk_paned_pack1 (GTK_PANED(mainWindowHpaned), inputPaneVbox, FALSE, FALSE); */
/*   //gtk_box_pack_start (GTK_BOX(mainWindowHpaned), inputPaneVbox, FALSE, FALSE, NO_PADDING); */
/*   //gtk_widget_set_valign (inputPaneVbox, GTK_ALIGN_CENTER); */
/*   gtk_widget_show (inputPaneVbox); */

/*   Entry widget  *************************************************** */

/*   newCalcEntry = gtk_entry_new (); */
/*   gtk_widget_set_name (newCalcEntry, "calcEntry"); */
/*   gtk_widget_set_sensitive (newCalcEntry, FALSE); */
/*   gtk_editable_set_editable (GTK_EDITABLE (newCalcEntry), FALSE); */
/*   gtk_entry_set_alignment (GTK_ENTRY(newCalcEntry), 1); */
/*   gtk_box_pack_start (GTK_BOX(inputPaneVbox), newCalcEntry, FALSE, FALSE, NO_PADDING); */

/*   gtk_entry_set_text (GTK_ENTRY(newCalcEntry), "0"); */

/*   gtk_widget_show (newCalcEntry); */
/*   cwindow->entry = newCalcEntry; */

/*   GtkSeparator vertically separates the widgets in the input pane. */

/*   GtkWidget *inputSeparator; */

/*   inputSeparator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL); */
/*   gtk_widget_set_name (inputSeparator, "inputSeparator"); */
/*   gtk_box_pack_start (GTK_BOX(inputPaneVbox), inputSeparator, FALSE, FALSE, NO_PADDING); */
/*   gtk_widget_show(inputSeparator); */

/*   Grid for buttons  **************************************************** */

/*   cwindow->button_list = button_list; */
/*   cwindow->nbuttons = nbuttons; */

/*   calcButtonGrid = gtk_grid_new(); */
/*   gtk_widget_set_name (calcButtonGrid, "calcButtonGrid"); */
/*   make_buttons(calcButtonGrid, cwindow); */
/*   gtk_grid_set_column_spacing (GTK_GRID(calcButtonGrid), CALC_BUTTON_GRID_COLUMN_PADDING); */
/*   gtk_grid_set_row_spacing (GTK_GRID(calcButtonGrid), CALC_BUTTON_GRID_ROW_PADDING); */

/*   gtk_box_pack_start (GTK_BOX(inputPaneVbox), calcButtonGrid, TRUE, TRUE, NO_PADDING); */
/*   gtk_widget_show (calcButtonGrid); */

/*   GtkListBox displays output (replaces CList widget) */

/*   GtkWidget *outputListBox; */

/*   outputListBox = gtk_list_box_new (); */
/*   gtk_paned_add2 (GTK_PANED (mainWindowHpaned), outputListBox); */



/*   CList widget for output *********************************************** */
/*   // TODO: move delcaration of clist into this function and give it a */
/*   // more descriptive name. */

/*   outputPaneVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, BOX_PADDING); */
/*   gtk_widget_set_name (outputPaneVbox, "outputPaneVbox"); */
/*   gtk_paned_add2 (GTK_PANED(mainWindowHpaned), outputPaneVbox); */

/*   outputPaneScrolledWindow = gtk_scrolled_window_new (NULL, NULL); */
/*   gtk_box_pack_start (GTK_BOX(outputPaneVbox), outputPaneScrolledWindow, FALSE, FALSE, NO_PADDING); */
/*   //gtk_paned_pack2 (GTK_PANED(mainWindowHpaned), outputPaneScrolledWindow, TRUE, FALSE); */
/*   gtk_box_pack_start (GTK_BOX(mainWindowHpaned), outputPaneScrolledWindow, FALSE, FALSE, NO_PADDING); */

/*   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(outputPaneScrolledWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS); */
/*   //cwindow->clist = gtk_clist_new_with_titles (3, out_titles); */
/*   model = make_model (); */

/*   cwindow->clist = gtk_tree_view_new_with_model (model); */

/*   // v TODO: Figure out a HIG design equivalent to zebra striping rows. */
/*   //gtk_tree_view_set_rules_hint (GTK_TREE_VIEW(cwindow->clist), TRUE); */

/*   select = gtk_tree_view_get_selection (GTK_TREE_VIEW(cwindow->clist)); */
/* 	gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE); */
/* 	g_signal_connect (G_OBJECT(select), "changed", */
/* 	                               G_CALLBACK(selection_changed), */
/* 																 cwindow); */

/*   gtk_container_add (GTK_CONTAINER(outputPaneScrolledWindow), cwindow->clist); */

/*   popup_menu = show_popup_menu (cwindow->clist); */

/*   g_signal_connect (G_OBJECT(cwindow->clist), "row-activated", */
/* 	                  G_CALLBACK(select_row), */
/* 										cwindow); */

/*   g_signal_connect (cwindow->clist, "button-press-event", */
/* 		      G_CALLBACK(mouse_click_check), */
/* 		      popup_menu); */

/*   gtk_widget_show (outputPaneScrolledWindow); */
/*   gtk_widget_set_name (cwindow->clist, "output_window"); */
/*   gtk_widget_show (cwindow->clist); */

/*   make_columns (GTK_TREE_VIEW(cwindow->clist)); */

/*   cwindow->scrolled_window = outputPaneScrolledWindow; */

/*   Statusbar ************************************************************* */
/*   // TODO: All references to status bar functionality should be refitted for header bar. */

/*   Keypress mask ********************************************************* */

/*   gtk_widget_set_events(GTK_WIDGET(cwindow->window), */
/* 			GDK_EXPOSURE_MASK| */
/* 			GDK_KEY_PRESS_MASK| */
/* 			GDK_KEY_RELEASE_MASK); */

/*   g_signal_connect(cwindow->window, "key_press_event", */
/* 		     G_CALLBACK(key_press_event), cwindow->entry); */
/*   g_signal_connect(cwindow->window, "key_release_event", */
/* 		     G_CALLBACK(key_release_event), cwindow->entry); */

/*   Final initialization ************************************************** */

/*   gtk_widget_set_can_default (cwindow->button_list[BTN_EQUAL].widget, TRUE); */
/*   gtk_widget_grab_default (cwindow->button_list[BTN_EQUAL].widget); */

/*   g_object_set_data (cwindow->window, "cwindow", cwindow); */
/*   g_object_set_data (cwindow->window, "twindow", cwindow->window); */

/*   set_current_file ("Untitled"); */
/*   set_current_dir (); */

/*   set_file_saved (cwindow, TRUE); */
/*   //gtk_label_set_text (GTK_LABEL(cwindow->entry), "0"); */

/*   return(0); */
/* } */

/* // TODO: Allow header bar title to be defined in some more global location. */
/* Header Bar */
/* void make_header_bar (GtkWidget *headerBar) */
/* { */
/*   GtkWidget *newButton; */
/*   GtkWidget *saveButton; */
/*   GtkWidget *menuButton; */
/*   GtkWidget *editButton; */

/*   gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headerBar), TRUE); */
/*   gtk_header_bar_set_title(GTK_HEADER_BAR(headerBar), "gtapecalc"); */
/*   //gtk_header_bar_set_subtitle(GTK_HEADER_BAR(headerBar), "Name of Current Open File Goes Here"); */

/*   // Replacement for /file/new. */
/*   // Create new tape record. */
/*   newButton = gtk_button_new_from_icon_name("document-new-symbolic", GTK_ICON_SIZE_MENU); */

/*   // Replacement for /edit/preferences, /view, and /help. */
/*   // Popover holds checkbox to show/hide header, printing, floating point spinner, */
/*   // preferences, and help. */
/*   menuButton = gtk_button_new_from_icon_name("open-menu-symbolic", GTK_ICON_SIZE_MENU); */

/*   // Replacement for /edit. */
/*   // Popover holds add/delete note and insert/delete line. */
/*   editButton = gtk_button_new_from_icon_name("accessories-text-editor-symbolic", GTK_ICON_SIZE_MENU); */

/*   // Replacement for /file/save and /file/save as. */
/*   // Saves current tape record. */
/*   saveButton = gtk_button_new_from_icon_name("document-save-symbolic", GTK_ICON_SIZE_MENU); */

/*   gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), newButton); */
/*   gtk_header_bar_pack_end(GTK_HEADER_BAR(headerBar), menuButton); */
/*   gtk_header_bar_pack_end(GTK_HEADER_BAR(headerBar), editButton); */
/*   gtk_header_bar_pack_end(GTK_HEADER_BAR(headerBar), saveButton); */

/*   gtk_widget_show(newButton); */
/*   gtk_widget_show(saveButton); */
/*   gtk_widget_show(menuButton); */
/*   gtk_widget_show(editButton); */
/* } */

/* // TODO: Remove direct references to the window struct. */
/* Loop through the button array ******************************************* */
/* void make_buttons (GtkWidget *calcButtonGrid, CalcWindow *cw) */
/* { */
/*   PangoFontDescription *font_desc = NULL; */
/*   GList *button_container; */
/*   GtkWidget *button_label; */

/*   GtkStyleContext *temporaryStyleContext; */

/*   if (cw->widget_vars->button_font_name[0] != 0) { */
/*   	font_desc = pango_font_description_from_string (cw->widget_vars->button_font_name); */
/*   } */

/*   for (gint index = 0; index < cw->nbuttons; index++) { */

/*   	gchar *newLabel = g_locale_to_utf8 (cw->button_list[index].label, -1, NULL, NULL, NULL); */
/*     cw->button_list[index].widget = gtk_button_new_with_label(newLabel); */

/*     // Button signal connections. */
/*     // If the button doesn't have a callback function, */
/*     // just send the button type to button_press(). */
/*     if (!cw->button_list[index].sigfunc) */
/*       g_signal_connect (cw->button_list[index].widget, "clicked", */
/* 	      G_CALLBACK(button_press), */
/* 	      &cw->button_list[index].type); */
/*     else{ */
/*        gchar *data = g_locale_to_utf8 (cw->button_list[index].data, -1, NULL, NULL, NULL); */
/*       g_signal_connect (cw->button_list[index].widget, "clicked", */
/* 			  G_CALLBACK(cw->button_list[index].sigfunc), */
/* 			  data); */
/*     } */

/*     g_signal_connect (cw->button_list[index].widget, */
/* 	    "enter_notify_event", */
/* 	    G_CALLBACK(button_enter), */
/* 	    NULL); */

/*     g_signal_connect (cw->button_list[index].widget, */
/* 	    "leave_notify_event", */
/* 	    G_CALLBACK(button_leave), */
/* 	    NULL); */

/*     // Attach Parameters: */
/*     // gtk_grid_attach(<GtkGrid Object>, */
/*     // <widget to add>, */
/*     // <column>, */
/*     // <row>, */
/*     // <width spanned>, <height spanned>); */

/*     switch (cw->button_list[index].type) { */
/*     // Tall buttons (width: 1, height: 2) go here vvv. */
/*     case BTN_EQUAL: */
/*       temporaryStyleContext = gtk_widget_get_style_context (cw->button_list[index].widget); */
/*       gtk_style_context_add_class (temporaryStyleContext, "suggested-action"); */
/*     case BTN_PLUS: */
/*       // "+" and "=" buttons are double the height of regular calc buttons. */
/*       gtk_grid_attach (GTK_GRID(calcButtonGrid), */
/* 	      cw->button_list[index].widget, */
/* 	      cw->button_list[index].column, */
/* 	      cw->button_list[index].row, */
/* 	      1, 2); */

/*       break; */
/*     // Regular buttons (width: 1, height: 1) go here vvv. */
/*     case BTN_CLEAR_ALL: */
/*       temporaryStyleContext = gtk_widget_get_style_context (cw->button_list[index].widget); */
/*       gtk_style_context_add_class (temporaryStyleContext, "destructive-action"); */
/*       //gtk_style_context_add_class (temporaryStyleContext, "linked"); */
/*     default: */
/*       // Every other button spans one column unit and one row unit. */
/*       gtk_grid_attach (GTK_GRID(calcButtonGrid), */
/* 	      cw->button_list[index].widget, */
/* 	      cw->button_list[index].column, */
/* 	      cw->button_list[index].row, */
/* 	      1, 1); */
/*     } */

/*     //gtk_widget_set_name (cw->button_list[index].widget, "keypad_button"); */


/*     if (font_desc) { */
/*       button_container = gtk_container_get_children (GTK_CONTAINER(cw->button_list[index].widget)); */
/*       button_label = GTK_WIDGET(button_container->data); */

/*       gtk_widget_modify_font (GTK_WIDGET(button_label), font_desc); */
/*     } */

/*     gtk_widget_show (cw->button_list[index].widget); */

/*   } */

/*   //pango_font_description_free (font_desc); */
/* } */

/* Popup menu ************************************************************** */

/* GtkWidget *show_popup_menu (GtkWidget *clist) */
/* { */
/*   static PopupMenuEntry popup_menu_items[] = { */
/*     {"Add Note",    G_CALLBACK(edit_tape_dialog)}, */
/*     {"Delete Note", G_CALLBACK(remove_comment)}, */
/*     {NULL,          NULL}, */
/*     {"Insert Line", G_CALLBACK(insert_row)}, */
/*     {"Delete Line", G_CALLBACK(dlg_delete_line)} */
/*   }; */

/*   GtkWidget *p_menu; */
/*   GtkWidget *menu_item; */
/*   gint np_menu_items = sizeof (popup_menu_items) / sizeof (popup_menu_items[0]); */
/*   gint i; */

/*   p_menu = gtk_menu_new (); */

/*   for (i=0; i < np_menu_items ; i++){ */
/*     if (popup_menu_items[i].label) */
/*       menu_item = gtk_menu_item_new_with_label(popup_menu_items[i].label); */
/*     else */
/*       menu_item = gtk_menu_item_new (); */
/* 	if (popup_menu_items[i].sigfunc) { */
/*     	g_signal_connect_swapped (G_OBJECT(menu_item),"activate", */
/* 				G_CALLBACK(popup_menu_items[i].sigfunc), */
/* 				clist); */
/* 	} */

/*     gtk_widget_show (menu_item); */
/*     //gtk_menu_append (GTK_MENU(p_menu), menu_item); */
/*   } */
/*   gtk_widget_show (p_menu); */
/*   return (p_menu); */
/* } */

/* Setup the treeview model */
/* static GtkTreeModel *make_model (void) */
/* { */
/*   GtkListStore *store; */

/*   store = gtk_list_store_new (N_COLUMNS, */
/*                               G_TYPE_STRING, */
/*                               G_TYPE_STRING, */
/*                               G_TYPE_STRING); */

/*   return GTK_TREE_MODEL(store); */
/* } */

/* Setup the treeview columns */
/* // TODO: Move code defined UI spacing into CSS. */
/* // There's also a crash when trying to edit a non-editable form that needs */
/* // to be tracked down. */
/* static void make_columns (GtkTreeView *tree) */
/* { */
/*   GtkCellRenderer *renderer; */
/*   GtkTreeViewColumn *column; */

/*   renderer = gtk_cell_renderer_text_new (); */
/* 	g_object_set (G_OBJECT(renderer), "editable", TRUE, NULL); */
/* 	g_signal_connect (G_OBJECT(renderer), "edited", */
/* 	                  G_CALLBACK(cell_changed), tree); */

/*   column = gtk_tree_view_column_new_with_attributes ("Notes", renderer, */
/* 	                                                   "text", COLUMN_NOTE, */
/* 	                                                   NULL); */

/* 	g_object_set (G_OBJECT(column), "resizable", TRUE, NULL); */
/*   g_object_set (G_OBJECT(column), "sizing", GTK_TREE_VIEW_COLUMN_FIXED, NULL); */

/* 	//gtk_tree_view_column_set_min_width (column, 145); */
/*   gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column); */

/*   renderer = gtk_cell_renderer_text_new (); */
/*   column = gtk_tree_view_column_new_with_attributes ("Output", renderer, */
/* 	                                        "text", COLUMN_NUMBER, */
/* 																				  NULL); */

/* 	//g_object_set (G_OBJECT(renderer), "xalign", 1.0, NULL); */
/* 	//g_object_set (G_OBJECT(column), "resizable", TRUE, NULL); */
/* 	//gtk_tree_view_column_set_min_width (column, 120); */
/*   gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column); */


/*   renderer = gtk_cell_renderer_text_new (); */
/* 	//g_object_set (G_OBJECT(renderer), "xpad", 10, NULL); */

/*   column = gtk_tree_view_column_new_with_attributes ("~", renderer, */
/* 	                                                   "text", COLUMN_OPERATOR, */
/* 	                                                   NULL); */

/*   gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column); */
/* } */
