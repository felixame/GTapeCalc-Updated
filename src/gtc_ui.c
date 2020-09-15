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


#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gtc_ui.h"
#include "gtc_calc.h"
#include "gtc_callbacks.h"
#include "gtc_dialogs.h"
#include "gtc_window.h"
#include "gtc_prefs.h"
#include "gtc_utils.h"

static GtkTreeModel *
make_model (void);

static void
make_columns (GtkTreeView *tree);

static GtkItemFactoryEntry menu_items[] = {
  {"/_File",                 NULL,         0,                 0, "<Branch>" },
  {"/File/_New",             "<control>N", file_close_cb,     0, NULL },
  {"/File/_Open",            "<control>O", file_select,       0, NULL },
  {"/File/_Save",            "<control>S", file_save_event_cb, 0, NULL },
  {"/File/Save _As...",      NULL,         file_select,       0, NULL },
  {"/File/sep1",             NULL,         0,                 0, "<Separator>" },
  {"/File/_Print",           "<control>P", dlg_print,        0, NULL },
  {"/File/_Close",           NULL        , file_close_cb,     0, NULL },
  {"/File/sep2",             NULL,         0,                 0, "<Separator>" },
  {"/File/_Quit",            "<control>Q", close_app,      0, NULL },
  {"/File/sep3",             NULL,         0,                 0, "<Separator>" },
  {"/_Edit",                 NULL,         0,                 0, "<Branch>" },
  {"/Edit/_Add Note",     "<control>A", edit_tape_dialog,  0, NULL },
  {"/Edit/_Delete Note",  "<control>D", remove_comment,    0, NULL },
  {"/Edit/sep4",             NULL,         0,                 0, "<Separator>" },
  {"/Edit/_Insert Line",     "<control>I", insert_row,        0, NULL },
  {"/Edit/Delete _Line",     "<control>L", dlg_delete_line,   0, NULL },
  {"/Edit/sep5",             NULL,         0,                 0, "<Separator>" },
  {"/Edit/_Preferences",     NULL,         dlg_preferences_show, 0, NULL},
  {"/_View",                 NULL,         0,                 0, "<Branch>" },
  {"/View/_Show Headers",    NULL,         column_toggle,     0, "<CheckItem>" },
  {"/View/sep6",             NULL,         0,                 0, "<Separator>" },
  {"/View/_Floating Point",  NULL,        dec_select,        0, "<RadioItem>" },
  {"/View/_0.00",            NULL,        dec_select,        0, "/View/Floating Point" },
  {"/View/0_.0000",          NULL,        dec_select,        0, "/View/Floating Point" },
  {"/View/_Custom",          NULL,         dec_select,        0, "/View/Floating Point" },
  /*{"/_Test",                 NULL,         0,                 0, "<Branch>" },
  {"/Test/_Test Size",       NULL,         get_pane_size,     0, NULL },
  {"/Test/_Default",         NULL,         de_fault,           0, NULL },*/
  {"/_Help",                 NULL,         0,                 0, "<LastBranch>" },
  {"/Help/_Contents",        NULL,         show_help,         0, NULL},
  {"/Help/_About",           NULL,         show_about,        0, NULL }
};

static gchar *callback_data[] = {
  0,    /* /File                   */
  0,    /* /File/New               */
  "r",  /* /File/Open              */
  0,    /* /File/Save              */
  "w",  /* /File/Save_As           */
  0,    /* /File/sep1              */
  0,    /* /File/Print             */
  0,    /* /File/Close             */
  0,    /* /File/sep2              */
  0,    /* /File/Quit              */
  0,    /* /File/sep3              */
  0,    /* /Edit                   */
  0,    /* /Edit/Add Comment       */
  0,    /* /Edit/Remove Comment    */
  0,    /* /Edit/sep4              */
  0,    /* /Edit/Insert Line       */
  0,    /* /Edit/Remove Line       */
  0,    /* /Edit/sep5              */
  0,    /* /Edit/Preferences       */
  0,    /* /View                   */
  0,    /* /View/Show Columns      */
  0,    /* /View/sep               */
  "-1",  /* /View/Floating Point    */
  "2",  /* /View/0.00              */
  "4",  /* /View/0.0000            */
  "-2", /* /View/Custom            */
  /*0,*/    /* /Test               */
  /*0,*/    /* /Test/Test Size     */
  /*0,*/    /* /Test/Default       */
  0,    /* /Help                   */
  0,    /* /Help/Contents          */
  0     /* /Help/About             */
};
//static char squared[] = {32, 120, 178, 32, '\0'};
//static char p_minus[] = {32, 177, 32, '\0'};


gint make_ui(CalcWindow *cwindow)
{
  static CalcButton button_list[] = {
    {"(",    0, 0, NULL, NULL,       "(",   BTN_PAREN_LEFT},
    {")",    1, 0, NULL, NULL,       ")",   BTN_PAREN_RIGHT},
    {"/",    2, 0, NULL, NULL,       " / ",   BTN_DIVIDE},
    {"x",    3, 0, NULL, NULL,       " x ",   BTN_MULTIPLY},
    {"-",    4, 0, NULL, NULL,       " - ",   BTN_MINUS},

    {"%",    0, 1, NULL, NULL,       " % ",   BTN_PERCENT},
    {"7",    1, 1, NULL, NULL,       "7",   BTN_SEVEN},
    {"8",    2, 1, NULL, NULL,       "8",   BTN_EIGHT},
    {"9",    3, 1, NULL, NULL,       "9",   BTN_NINE},
    {"+",    4, 1, NULL, NULL,       " + ",   BTN_PLUS},

    // In 2020 GTK seems to be confused by byte defined unicode chars.
    // Easier to just copy the character into the IDE and let the compiler
    // figure it out.
    //{squared,0, 2, NULL, NULL,       squared, BTN_X_SQUARED},
    {"x²",   0, 2, NULL, NULL,       "x²", BTN_X_SQUARED},
    {"4",    1, 2, NULL, NULL,       "4",   BTN_FOUR},
    {"5",    2, 2, NULL, NULL,       "5",   BTN_FIVE},
    {"6",    3, 2, NULL, NULL,       "6",   BTN_SIX},

    //{"+/-",  0, 3, NULL, NULL,        p_minus,  BTN_PLUS_MINUS},
    {"±",    0, 3, NULL, NULL,        "±",  BTN_PLUS_MINUS},
    {"1",    1, 3, NULL, NULL,        "1",  BTN_ONE},
    {"2",    2, 3, NULL, NULL,        "2",  BTN_TWO},
    {"3",    3, 3, NULL, NULL,        "3",  BTN_THREE},
    {"=",    4, 3, NULL, NULL,        " = ",  BTN_EQUAL},

    {"AC",   0, 4, NULL, clear,       "AC", BTN_CLEAR_ALL},
    {"CE",   1, 4, NULL, entry_clear, "CE", BTN_CLEAR_ENTRY},
    {"0",    2, 4, NULL, NULL,        "0",  BTN_ZERO},
    {".",    3, 4, NULL, NULL,        ".",  BTN_DEC_POINT}
  };

  static gint nbuttons = sizeof(button_list) / sizeof(CalcButton);

  gint nrows    = 5;
  gint ncolumns = 5;

  GtkWidget *vbox;
  GtkWidget *vbox1;

  GtkWidget *hpaned;
  GtkWidget *table;
  GtkWidget *frame;
  GtkWidget *event_box;
  GtkWidget *check_item;
  GtkWidget *menu_bar;
  GtkWidget *popup_menu;
  GdkColor   event_box_bg_color;
  GdkColor   entry_text_color;
  GdkColormap *cmap;
  GtkStyle  *event_box_style;
  GtkStyle  *entry_style;
  GtkAccelGroup *accel_group;
  gchar *rcname;
  GtkTreeModel *model;
	GtkTreeSelection *select;

  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);
  gint i;
  GtkItemFactoryEntry *menu_entry;

  ConfigVariable *prefs = cwindow->widget_vars;

  cwindow->button_list = button_list;
  cwindow->nbuttons = nbuttons;

  rcname = g_strdup_printf ("%s/.gtapecalcgtkrc", g_get_home_dir ());
  gtk_rc_parse (rcname);
  g_free (rcname);

  /*config_load_defaults();*/
  /* Main vbox **************************************************************/

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (cwindow->window), vbox);
  gtk_widget_show (vbox);

  /* Menu bar  **************************************************************/

  accel_group = gtk_accel_group_new ();
  cwindow->item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>",
				       accel_group); // 2.0 Change

  for (i = 0; i < nmenu_items; i++) {
    menu_entry = menu_items + i;

    /*printf ("menu is %s, callback_data is %s\n", menu_entry->path,
	    *(call_data + i));*/

    gtk_item_factory_create_item (cwindow->item_factory, menu_entry,
				  *(callback_data + i), 2);
  }
  /*gtk_item_factory_create_items (item_factory, nmenu_items,
				 menu_items, NULL);*/
  //gtk_accel_group_attach (accel_group, GTK_OBJECT (cwindow->window));
  // 2.0 Change

  menu_bar = gtk_item_factory_get_widget (cwindow->item_factory, "<main>");

  gtk_box_pack_start (GTK_BOX(vbox), menu_bar,FALSE, TRUE, 0);
  gtk_widget_show (menu_bar);

  gtk_window_add_accel_group (GTK_WINDOW(cwindow->window), accel_group);

  /* End of menus  **********************************************************/

  hpaned = gtk_hpaned_new ();
  gtk_container_add (GTK_CONTAINER(vbox), hpaned);
  //gtk_paned_set_handle_size (GTK_PANED(hpaned),
	//		     10);
  // 2.0 Change
  gtk_paned_set_gutter_size (GTK_PANED(hpaned),
			     15);
  gtk_widget_show (hpaned);

  /* Vboxes for main window *************************************************/

  vbox1 = gtk_vbox_new (FALSE, 10);
  gtk_container_border_width (GTK_CONTAINER (vbox1), 10);
  gtk_paned_add1 (GTK_PANED(hpaned), vbox1);
  gtk_widget_show (vbox1);

  /* Entry label widget  ****************************************************/

  frame = gtk_frame_new (NULL);
  gtk_widget_set_usize (frame, prefs->pane_width, 36);
  gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX(vbox1), frame, FALSE, TRUE, 0);
  gtk_widget_show (frame);

  // Event Box
  event_box = gtk_event_box_new ();
  gtk_container_add (GTK_CONTAINER(frame), event_box);

  cmap = gdk_colormap_get_system();

  event_box_bg_color.red = 0xcf80;
  event_box_bg_color.green = 0xeff0;
  event_box_bg_color.blue = 0xc090;

  if (!gdk_color_alloc(cmap, &event_box_bg_color)) {
    g_error("couldn't allocate event box background color");
  }
  else {
    event_box_style = gtk_style_copy (gtk_widget_get_style (event_box));
    event_box_style->bg[GTK_STATE_NORMAL] = event_box_bg_color;

    gtk_widget_set_style (GTK_WIDGET(event_box), event_box_style);
    gtk_style_unref (event_box_style);
  }

  gtk_widget_show (event_box);

  // Entry label
  cwindow->entry = gtk_label_new ("0");
  gtk_misc_set_alignment (GTK_MISC(cwindow->entry), 1, .5);
  gtk_container_add (GTK_CONTAINER(event_box), cwindow->entry);

  // GTK 2 doesn't have a good way of checking whether or not you're using
  // the dark theme so we'll have to manually set the entry text color to
  // black so that we have some contrast with the forced light event box
  // regardless of theme.
  entry_text_color.red = 0x0000;
  entry_text_color.green = 0x0000;
  entry_text_color.blue = 0x0000;

  if (!gdk_color_alloc(cmap, &entry_text_color)) {
    g_error("couldn't allocate entry text color");
  }
  else {
    entry_style = gtk_style_copy (gtk_widget_get_style (cwindow->entry));
    entry_style->text[GTK_STATE_NORMAL] = entry_text_color;

    gtk_widget_set_style (GTK_WIDGET(cwindow->entry), entry_style);
    gtk_style_unref (entry_style);
  }

  gtk_widget_show (cwindow->entry);

  if (prefs->entry_font_name[0] != '\0') {
   widget_font_load (cwindow->entry, prefs->entry_font_name);
    //entry_font = gdk_font_load (prefs->entry_font_name);
    /*style = gtk_style_new ();*/
    //if (entry_font) {
      //style = gtk_style_copy (gtk_widget_get_style (GTK_WIDGET(cwindow->entry)));
      //style->font = entry_font;
      //gtk_widget_set_style (GTK_WIDGET(cwindow->entry), style);
      //gtk_style_unref (style);
    //}
    //else
      //g_warning ("Couldn't load font %s,\nusing default gtk font instead",
      //           prefs->entry_font_name);
  }
  // 2.0 Change

  /* Table for buttons  *****************************************************/

  table = gtk_table_new(nrows, ncolumns, TRUE);
  gtk_table_set_row_spacings (GTK_TABLE(table), 5);
  gtk_table_set_col_spacings (GTK_TABLE(table), 5);

  gtk_box_pack_start (GTK_BOX(vbox1), table, TRUE, TRUE, 0);

  gtk_widget_show (table);

  make_buttons(table, cwindow);

  /* CList widget for output ************************************************/

  cwindow->scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_paned_add2 (GTK_PANED(hpaned), cwindow->scrolled_window);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(cwindow->scrolled_window),
	  GTK_POLICY_AUTOMATIC,
	  GTK_POLICY_ALWAYS);
  //cwindow->clist = gtk_clist_new_with_titles (3, out_titles);
  model = make_model ();

  cwindow->clist = gtk_tree_view_new_with_model (model);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW(cwindow->clist), TRUE);

  select = gtk_tree_view_get_selection (GTK_TREE_VIEW(cwindow->clist));
	gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
	g_signal_connect (G_OBJECT(select), "changed",
	                               G_CALLBACK(selection_changed),
																 cwindow);

  gtk_container_add (GTK_CONTAINER(cwindow->scrolled_window), cwindow->clist);

  popup_menu = show_popup_menu (cwindow->clist);

  g_signal_connect (G_OBJECT(cwindow->clist), "row-activated",
	                  G_CALLBACK(select_row),
										cwindow);

  g_signal_connect (GTK_OBJECT(cwindow->clist), "button-press-event",
		      G_CALLBACK(mouse_click_check),
		      GTK_OBJECT(popup_menu));

  gtk_widget_show (cwindow->scrolled_window);
  gtk_widget_set_name (cwindow->clist, "output_window");
  gtk_widget_show (cwindow->clist);

  make_columns (GTK_TREE_VIEW(cwindow->clist));

  if (prefs->clist_font_name[0] != '\0') {
   widget_font_load (cwindow->clist, prefs->clist_font_name);
  }

  /* Statusbar **************************************************************/

  cwindow->statusbar = gtk_statusbar_new ();
  gtk_box_pack_start (GTK_BOX(vbox),cwindow->statusbar, FALSE, TRUE, 0);
  gtk_widget_show (cwindow->statusbar);
  cwindow->statusbar_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(cwindow->statusbar),
					     "Statusbar");


  /* Keypress mask **********************************************************/

  gtk_widget_set_events(GTK_WIDGET(cwindow->window),
			GDK_EXPOSURE_MASK|
			GDK_KEY_PRESS_MASK|
			GDK_KEY_RELEASE_MASK);

  g_signal_connect(GTK_OBJECT (cwindow->window), "key_press_event",
		     G_CALLBACK(key_press_event), cwindow->entry);
  g_signal_connect(GTK_OBJECT (cwindow->window), "key_release_event",
		     G_CALLBACK(key_release_event), cwindow->entry);

  /* Final initialization ***************************************************/

  gtk_object_set_data (GTK_OBJECT(cwindow->window), "cwindow", cwindow);
  gtk_object_set_data (GTK_OBJECT(cwindow->window), "twindow", cwindow->window);

  set_current_file ("Untitled");
  set_current_dir ();

  set_file_saved (cwindow, TRUE);
  gtk_label_set (GTK_LABEL(cwindow->entry), "0");

  check_item = gtk_item_factory_get_widget(cwindow->item_factory,
					   "/View/Show Headers");
  gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(check_item),
				 prefs->column_title_show);
  if (!prefs->column_title_show)
    g_object_set (G_OBJECT(cwindow->clist), "headers-visible", FALSE, NULL);


  switch (prefs->precision) {
      case 2:
	  check_item = gtk_item_factory_get_widget (cwindow->item_factory,
		  "/View/0.00");
	  gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(check_item),
		  TRUE);
	  break;
      case 4:
	  check_item = gtk_item_factory_get_widget (cwindow->item_factory,
		  "/View/0.0000");
	  gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(check_item),
		  TRUE);
	  break;
      case -1:
	  check_item = gtk_item_factory_get_widget (cwindow->item_factory,
		  "/View/Floating Point");
	  gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(check_item),
		  TRUE);
	  break;
      default:
      	  cwindow->state->misc_state = 1; /* set this flag to tell the callback
	      				     not to show the preferences dialog */
	  check_item = gtk_item_factory_get_widget (cwindow->item_factory,
		  "/View/Custom");
	  gtk_check_menu_item_set_state (GTK_CHECK_MENU_ITEM(check_item),
		  TRUE);
	  break;
  }

  return(0);
}

/* Loop through the button array ********************************************/

void make_buttons (GtkWidget *table, CalcWindow *cw)
{
  //GdkFont *button_font;
  PangoFontDescription *font_desc = NULL;
  gint index;
  GList *button_container;
  GtkWidget *button_label;
  //GtkStyle *button_style;
  // 2.0 Change
  /*GdkColor button_color;
    GdkColormap *cmap;*/

  if (cw->widget_vars->button_font_name[0] != 0) {
  	font_desc = pango_font_description_from_string (cw->widget_vars->button_font_name);
  }

  for (index = 0; index < cw->nbuttons; index++) {

  	gchar *label = g_locale_to_utf8 (cw->button_list[index].label, -1, NULL, NULL, NULL);
    cw->button_list[index].widget = gtk_button_new_with_label(label);


    if (!cw->button_list[index].sigfunc)
      g_signal_connect (GTK_OBJECT(cw->button_list[index].widget), "clicked",
	      G_CALLBACK(button_press),
	      &cw->button_list[index].type);
    else{
       gchar *data = g_locale_to_utf8 (cw->button_list[index].data, -1, NULL, NULL, NULL);
      g_signal_connect (GTK_OBJECT(cw->button_list[index].widget), "clicked",
			  G_CALLBACK(cw->button_list[index].sigfunc),
			  data);
    }
    g_signal_connect (GTK_OBJECT(cw->button_list[index].widget),
	    "enter_notify_event",
	    G_CALLBACK(button_enter),
	    NULL);

    g_signal_connect (GTK_OBJECT(cw->button_list[index].widget),
	    "leave_notify_event",
	    G_CALLBACK(button_leave),
	    NULL);

    if (strcmp (cw->button_list[index].label, "+") != 0 &&
        strcmp (cw->button_list[index].label, "=") != 0) {

      gtk_table_attach_defaults (GTK_TABLE(table),
	      cw->button_list[index].widget,
	      cw->button_list[index].column,
	      cw->button_list[index].column + 1,
	      cw->button_list[index].row,
	      cw->button_list[index].row + 1);
    }
    else {

      gtk_table_attach_defaults (GTK_TABLE(table),
	      cw->button_list[index].widget,
	      cw->button_list[index].column,
	      cw->button_list[index].column + 1,
	      cw->button_list[index].row,
	      cw->button_list[index].row + 2);
    }

    gtk_widget_set_name (cw->button_list[index].widget, "keypad_button");


    if (font_desc) {
      button_container = gtk_container_children (GTK_CONTAINER(cw->button_list[index].widget));
      button_label = GTK_WIDGET(button_container->data);

      gtk_widget_modify_font (GTK_WIDGET(button_label), font_desc);
    }

    gtk_widget_show (cw->button_list[index].widget);

  }

  pango_font_description_free (font_desc);
}

/* Popup menu ***************************************************************/

GtkWidget *show_popup_menu (GtkWidget *clist)
{
  static PopupMenuEntry popup_menu_items[] = {
    {"Add Note",    G_CALLBACK(edit_tape_dialog)},
    {"Delete Note", G_CALLBACK(remove_comment)},
    {NULL,          NULL},
    {"Insert Line", G_CALLBACK(insert_row)},
    {"Delete Line", G_CALLBACK(dlg_delete_line)}
  };

  GtkWidget *p_menu;
  GtkWidget *menu_item;
  gint np_menu_items = sizeof (popup_menu_items) / sizeof (popup_menu_items[0]);
  gint i;

  p_menu = gtk_menu_new ();

  for (i=0; i < np_menu_items ; i++){
    if (popup_menu_items[i].label)
      menu_item = gtk_menu_item_new_with_label(popup_menu_items[i].label);
    else
      menu_item = gtk_menu_item_new ();
	if (popup_menu_items[i].sigfunc) {
    	g_signal_connect_swapped (G_OBJECT(menu_item),"activate",
				G_CALLBACK(popup_menu_items[i].sigfunc),
				GTK_OBJECT(clist));
	}

    gtk_widget_show (menu_item);
    gtk_menu_append (GTK_MENU(p_menu), menu_item);
  }
  gtk_widget_show (p_menu);
  return (p_menu);
}

/* Setup the treeview model */
static GtkTreeModel *
make_model (void)
{
  GtkListStore *store;

  store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

  return GTK_TREE_MODEL(store);
}

/* Setup the treeview columns */
static void
make_columns (GtkTreeView *tree)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new ();
	g_object_set (G_OBJECT(renderer), "editable", TRUE, NULL);
	g_signal_connect (G_OBJECT(renderer), "edited",
	                  G_CALLBACK(cell_changed), tree);

  column = gtk_tree_view_column_new_with_attributes ("               Notes", renderer,
	                                                   "text", COLUMN_NOTE,
	                                                   NULL);

	g_object_set (G_OBJECT(column), "resizable", TRUE, NULL);
  /*g_object_set (G_OBJECT(column), "sizing",
                  GTK_TREE_VIEW_COLUMN_FIXED, NULL);*/

	gtk_tree_view_column_set_min_width (column, 145);
  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("          Output", renderer,
	                                        "text", COLUMN_NUMBER,
																				  NULL);

	g_object_set (G_OBJECT(renderer), "xalign", 1.0, NULL);
	g_object_set (G_OBJECT(column), "resizable", TRUE, NULL);
	gtk_tree_view_column_set_min_width (column, 120);
  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);


  renderer = gtk_cell_renderer_text_new ();
	g_object_set (G_OBJECT(renderer), "xpad", 10, NULL);

  column = gtk_tree_view_column_new_with_attributes ("", renderer,
	                                                   "text", COLUMN_OPERATOR,
	                                                   NULL);

  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);
}
