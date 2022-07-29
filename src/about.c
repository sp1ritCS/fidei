/*
 Fidei - Take back your faith
 Copyright (C) 2022 Florian "sp1rit" <sp1rit@national.shitposting.agency>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.

 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "about.h"

GtkWidget* fidei_get_aboutdiag(GtkWindow* parent) {
	g_return_val_if_fail(GTK_IS_WINDOW(parent), NULL);
	GtkBuilder* bld = gtk_builder_new_from_resource("/arpa/sp1rit/Fidei/ui/about.ui");
	GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(bld, "diag"));

	GdkTexture* icon = gdk_texture_new_from_resource("/arpa/sp1rit/Fidei/icons/scalable/apps/arpa.sp1rit.Fidei.svg");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), GDK_PAINTABLE(icon));
	g_object_unref(icon);

#ifdef FIDEI_VERSION
		gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), FIDEI_VERSION);
#endif

	gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

	g_object_unref(bld);
	return dialog;
}

