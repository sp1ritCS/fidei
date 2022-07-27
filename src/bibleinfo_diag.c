#include "bibleinfo_diag.h"

#include <adwaita.h>

#include <libintl.h>
#define _t(String) gettext (String)


typedef struct {
	GtkWindow* parent;
	FideiBible* bible;

	AdwToastOverlay* toasts;
	GtkListBox* properties;
} FideiBibleInfoDiagPrivate;

struct _FideiBibleInfoDiag {
	GtkDialog parent_instance;
};

G_DEFINE_TYPE_WITH_PRIVATE(FideiBibleInfoDiag, fidei_bibleinfo_diag, GTK_TYPE_DIALOG)

enum {
	PROP_PARENT = 1,
	PROP_BIBLE,
	PROP_TITLE,
	PROP_PATH,
	PROP_PUBLISHER,
	PROP_LANG,
	PROP_IDENTIFIER,
	PROP_RIGHTS,
	N_PROPERTIES
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL, };

static void fidei_bibleinfo_diag_object_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
	FideiBibleInfoDiag* self = FIDEI_BIBLEINFO_DIAG(object);

	switch (prop_id) {
		case PROP_PARENT:
			g_value_set_object(value, fidei_bibleinfo_diag_get_parent(self));
			break;
		case PROP_BIBLE:
			g_value_set_object(value, fidei_bibleinfo_diag_get_bible(self));
			break;
		case PROP_TITLE:
			g_value_set_string(value, fidei_bibleinfo_diag_get_title(self));
			break;
		case PROP_PATH:
			g_value_set_string(value, fidei_bibleinfo_diag_get_path(self));
			break;
		case PROP_PUBLISHER:
			g_value_set_string(value, fidei_bibleinfo_diag_get_publisher(self));
			break;
		case PROP_LANG:
			g_value_set_string(value, fidei_bibleinfo_diag_get_lang(self));
			break;
		case PROP_IDENTIFIER:
			g_value_set_string(value, fidei_bibleinfo_diag_get_identifier(self));
			break;
		case PROP_RIGHTS:
			g_value_set_string(value, fidei_bibleinfo_diag_get_rights(self));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}
static void fidei_bibleinfo_diag_object_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
	FideiBibleInfoDiag* self = FIDEI_BIBLEINFO_DIAG(object);

	switch (prop_id) {
		case PROP_PARENT:
			fidei_bibleinfo_diag_set_parent(self, g_value_get_object(value));
			break;
		case PROP_BIBLE:
			fidei_bibleinfo_diag_set_bible(self, g_value_get_object(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}


static void fidei_bibleinfo_diag_class_init(FideiBibleInfoDiagClass* class) {
	GObjectClass* object_class = G_OBJECT_CLASS(class);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(class);

	object_class->get_property = fidei_bibleinfo_diag_object_get_property;
	object_class->set_property = fidei_bibleinfo_diag_object_set_property;

	obj_properties[PROP_PARENT] = g_param_spec_object("parent", "Parent", "Parent window this dialog attaches to", GTK_TYPE_WINDOW, G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_BIBLE] = g_param_spec_object("bible", "Bible", "Bible this dialog shows information about", FIDEI_TYPE_BIBLE, G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_TITLE] = g_param_spec_string("title", "Title", "Title of property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_PATH] = g_param_spec_string("path", "Path", "Location of property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_PUBLISHER] = g_param_spec_string("publisher", "Publisher", "Publisher of property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_LANG] = g_param_spec_string("lang", "Language", "Language of property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_IDENTIFIER] = g_param_spec_string("identifier", "Unique Identifier", "Unique Identifier of property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_RIGHTS] = g_param_spec_string("rights", "Rights", "Rights of property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

	gtk_widget_class_set_template_from_resource(widget_class, "/arpa/sp1rit/Fidei/ui/info.ui");
	gtk_widget_class_bind_template_child_private(widget_class, FideiBibleInfoDiag, toasts);
	gtk_widget_class_bind_template_child_private(widget_class, FideiBibleInfoDiag, properties);
}

static void fidei_bibleinfo_diag_row_clicked(GtkListBox*, GtkListBoxRow* clicked, FideiBibleInfoDiag* self) {
	g_return_if_fail(FIDEI_IS_BIBLEINFO_ROW(clicked));
	g_return_if_fail(FIDEI_IS_BIBLEINFO_DIAG(self));
	FideiBibleInfoDiagPrivate* priv = fidei_bibleinfo_diag_get_instance_private(self);

	const gchar* content = fidei_bibleinfo_row_get_property(FIDEI_BIBLEINFO_ROW(clicked));

	GdkDisplay* display = gtk_widget_get_display(GTK_WIDGET(clicked));
	GdkClipboard* clip = gdk_display_get_clipboard(display);
	gdk_clipboard_set(clip, G_TYPE_STRING, content);

	AdwToast* notif = adw_toast_new(_t("Copied to clipboard"));
	adw_toast_overlay_add_toast(priv->toasts, notif);
}

void fidei_bibleinfo_diag_init(FideiBibleInfoDiag* self) {
	FideiBibleInfoDiagPrivate* priv = fidei_bibleinfo_diag_get_instance_private(self);
	priv->parent = NULL;
	priv->bible = NULL;

	g_type_ensure(FIDEI_TYPE_BIBLEINFO_ROW);
	gtk_window_set_title(GTK_WINDOW(self), _t("Bible information"));

	gtk_widget_init_template(GTK_WIDGET(self));

	g_signal_connect(priv->properties, "row_activated", G_CALLBACK(fidei_bibleinfo_diag_row_clicked), self);
}

GtkWidget* fidei_bibleinfo_diag_new(GtkWindow* parent, FideiBible* bible) {
	return g_object_new(FIDEI_TYPE_BIBLEINFO_DIAG, "parent", parent, "bible", bible, NULL);
}


GtkWindow* fidei_bibleinfo_diag_get_parent(FideiBibleInfoDiag* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEINFO_DIAG(self), NULL);
	FideiBibleInfoDiagPrivate* priv = fidei_bibleinfo_diag_get_instance_private(self);

	return priv->parent;
}
void fidei_bibleinfo_diag_set_parent(FideiBibleInfoDiag* self, GtkWindow* parent) {
	g_return_if_fail(FIDEI_IS_BIBLEINFO_DIAG(self));
	g_return_if_fail(GTK_IS_WINDOW(parent));
	FideiBibleInfoDiagPrivate* priv = fidei_bibleinfo_diag_get_instance_private(self);

	if (priv->parent == parent)
		return;

	priv->parent = parent;

	gtk_window_set_transient_for(GTK_WINDOW(self), priv->parent);

	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_PARENT]);
}

FideiBible* fidei_bibleinfo_diag_get_bible(FideiBibleInfoDiag* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEINFO_DIAG(self), NULL);
	FideiBibleInfoDiagPrivate* priv = fidei_bibleinfo_diag_get_instance_private(self);

	return priv->bible;
}
void fidei_bibleinfo_diag_set_bible(FideiBibleInfoDiag* self, FideiBible* bible) {
	g_return_if_fail(FIDEI_IS_BIBLEINFO_DIAG(self));
	g_return_if_fail(FIDEI_IS_BIBLE(bible));
	FideiBibleInfoDiagPrivate* priv = fidei_bibleinfo_diag_get_instance_private(self);

	if (priv->bible == bible)
		return;

	priv->bible = bible;

	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_BIBLE]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_TITLE]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_PATH]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_PUBLISHER]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_LANG]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_IDENTIFIER]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_RIGHTS]);
}

static const gchar* fidei_bibleinfo_get_bible_property(FideiBibleInfoDiag* self, const gchar* (*bible_prop_fun) (FideiBible* bible)) {
	g_return_val_if_fail(FIDEI_IS_BIBLEINFO_DIAG(self), NULL);
	FideiBibleInfoDiagPrivate* priv = fidei_bibleinfo_diag_get_instance_private(self);

	if (!priv->bible)
		return NULL;

	return bible_prop_fun(priv->bible);
}
const gchar* fidei_bibleinfo_diag_get_title(FideiBibleInfoDiag* self) {
	return fidei_bibleinfo_get_bible_property(self, fidei_bible_get_title);
}
const gchar* fidei_bibleinfo_diag_get_path(FideiBibleInfoDiag* self) {
	return fidei_bibleinfo_get_bible_property(self, fidei_bible_get_path);
}
const gchar* fidei_bibleinfo_diag_get_publisher(FideiBibleInfoDiag* self) {
	return fidei_bibleinfo_get_bible_property(self, fidei_bible_get_publisher);
}
const gchar* fidei_bibleinfo_diag_get_lang(FideiBibleInfoDiag* self) {
	return fidei_bibleinfo_get_bible_property(self, fidei_bible_get_lang);
}
const gchar* fidei_bibleinfo_diag_get_identifier(FideiBibleInfoDiag* self) {
	return fidei_bibleinfo_get_bible_property(self, fidei_bible_get_identifier);
}
const gchar* fidei_bibleinfo_diag_get_rights(FideiBibleInfoDiag* self) {
	return fidei_bibleinfo_get_bible_property(self, fidei_bible_get_rights);
}
