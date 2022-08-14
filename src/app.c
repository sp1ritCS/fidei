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

#include "fidei.h"
#include "bible.h"
#include "fidei_res.h"

#include <locale.h>
#include <libintl.h>
#define _t(String) gettext (String)

#include <gtk/gtk.h>
#include <adwaita.h>

static void add_dir_to_bible_list(GListStore* bibles, GFile* dir, gboolean create_dir) {
	GError* err = NULL;
	GFileEnumerator* files = g_file_enumerate_children(dir, "standard::*,owner::user", G_FILE_QUERY_INFO_NONE, NULL, &err);
	if (err) {
		if (err->code == G_IO_ERROR_NOT_FOUND) {
			if (create_dir) {
				GError* create_error = NULL;
				if (!g_file_make_directory_with_parents(dir, NULL, &create_error)) {
					g_critical(_t("Failure creating directory: %s\n"), create_error->message);
					g_error_free(create_error);
				}
			}
		} else {
			g_critical(_t("Failure opening directory: %s\n"), err->message);
		}
		g_error_free(err);
		return;
	}

	GFileInfo* file;
	do {
		GError* err = NULL;
		file = g_file_enumerator_next_file(files, NULL, &err);
		if (err) {
			g_critical(_t("Failure acessing file: %s\n"), err->message);
			g_error_free(err);
			continue;
		}
		if (!file)
			break;


		gchar* path = g_build_path("/", g_file_get_path(dir), g_file_info_get_name(file), NULL);
		FideiBible* obj = fidei_bible_new(path);
		g_free(path);
		g_list_store_append(bibles, obj);
		g_object_unref(obj);


		g_object_unref(file);
	} while (TRUE);

	g_object_unref(files);
}

static GListStore* create_bible_list() {
	GListStore* bibles = g_list_store_new(FIDEI_TYPE_BIBLE);

	const gchar* datadir = g_get_user_data_dir();
	GFile* bibledir = g_file_new_build_filename(datadir, "arpa.sp1rit.Fidei", NULL);
	add_dir_to_bible_list(bibles, bibledir, TRUE);
	g_object_unref(bibledir);

	const gchar* const* datadirs = g_get_system_data_dirs();
	for (guint i = 0; i < g_strv_length((gchar**)datadirs); i++) {
		GFile* bibledir = g_file_new_build_filename(datadirs[i], "arpa.sp1rit.Fidei", NULL);
		add_dir_to_bible_list(bibles, bibledir, FALSE);
		g_object_unref(bibledir);
	}

	return bibles;
}


static void activate(GtkApplication* app, GListStore* store) {
	GtkWidget* win = fidei_appwindow_new(app, G_LIST_MODEL(store));
	gtk_window_set_title(GTK_WINDOW(win), "Fidei");
	gtk_widget_show(win);
}

int main(int argc, char** argv) {
	AdwApplication* app;
	int status;

	setlocale(LC_ALL, "");
	const gchar* locale_dir = g_getenv("FIDEI_DEBUG_LOCALEDIR");
	if (locale_dir)
		bindtextdomain(GETTEXT_PACKAGE, locale_dir);
	else
		bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);


	GListStore* bibles = create_bible_list();

	g_resources_register(fidei_resource_get_resource());
	app = adw_application_new("arpa.sp1rit.Fidei", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), bibles);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	g_object_unref(bibles);

	return status;
}
