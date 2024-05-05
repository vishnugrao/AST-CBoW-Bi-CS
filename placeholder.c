static void
menuitem_activate(GntMenu *menu, GntMenuItem *item)
{
	if (!item)
		return;
	if (gnt_menuitem_activate(item)) {
		menu_hide_all(menu);
	} else {
		if (item->submenu) {
			GntMenu *sub = GNT_MENU(item->submenu);
			menu->submenu = sub;
			sub->type = GNT_MENU_POPUP;	/* Submenus are *never* toplevel */
			sub->parentmenu = menu;
			if (menu->type != GNT_MENU_TOPLEVEL) {
				GntWidget *widget = GNT_WIDGET(menu);
				item->priv.x = widget->priv.x + widget->priv.width - 1;
				item->priv.y = widget->priv.y + gnt_tree_get_selection_visible_line(GNT_TREE(menu));
			}
			gnt_widget_set_position(GNT_WIDGET(sub), item->priv.x, item->priv.y);
			GNT_WIDGET_UNSET_FLAGS(GNT_WIDGET(sub), GNT_WIDGET_INVISIBLE);
			gnt_widget_draw(GNT_WIDGET(sub));
		} else {
			menu_hide_all(menu);
		}
	}
}
