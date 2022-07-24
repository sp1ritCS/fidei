#include "fidei.h"
#include "bible.h"
#include "fidei_res.h"

#include <locale.h>
#include <libintl.h>
#define _t(String) gettext (String)

#include <gtk/gtk.h>
#include <adwaita.h>

static GListStore* create_bible_list() {
	GListStore* bibles = g_list_store_new(FIDEI_TYPE_BIBLE);

	const gchar* datadir = g_get_user_data_dir();

	GFile* bibledir = g_file_new_build_filename(datadir, "arpa.sp1rit.Fidei", NULL);

	GError* err = NULL;
	GFileEnumerator* files = g_file_enumerate_children(bibledir, "standard::*,owner::user", G_FILE_QUERY_INFO_NONE, NULL, &err);
	if (err) {
		if (err->code == G_IO_ERROR_NOT_FOUND) {
			GError* create_error = NULL;
			if (!g_file_make_directory_with_parents(bibledir, NULL, &create_error)) {
				g_critical(_t("Failure creating directory: %s\n"), create_error->message);
				g_error_free(create_error);
				return NULL;
			}
			// create and push empty statemachine
			goto out;
		} else {
			g_critical(_t("Failure opening directory: %s\n"), err->message);
			g_error_free(err);
			return NULL;
		}
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


		gchar* path = g_build_path("/", g_file_get_path(bibledir), g_file_info_get_name(file), NULL);
		FideiBible* obj = fidei_bible_new(path);
		g_free(path);
		g_list_store_append(bibles, obj);
		g_object_unref(obj);


		g_object_unref(file);
	} while (TRUE);

out:
	g_object_unref(files);
	g_object_unref(bibledir);

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
