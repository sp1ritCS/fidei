#include "preferences.h"
#include "utils.h"

#include <libintl.h>
#define _t(String) gettext (String)

typedef struct {
	GSettings* settings;

	GtkSwitch* caption_btn;
	GtkFontButton* font_btn;
	AdwBin* smallcaps_bin;

	GtkEntryBuffer* active_addlang_buf;
} FideiPreferencesPrivate;

struct _FideiPreferences {
	AdwPreferencesWindow parent_instance;
};

G_DEFINE_TYPE_WITH_PRIVATE (FideiPreferences, fidei_preferences, ADW_TYPE_PREFERENCES_WINDOW)

enum {
	PROP_SETTINGS = 1,
	N_PROPERTIES
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL, };

static void fidei_preferences_object_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
	FideiPreferences* self = FIDEI_PREFERENCES(object);

	switch (prop_id) {
		case PROP_SETTINGS:
			g_value_set_object(value, fidei_preferences_get_settings(self));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}
static void fidei_preferences_object_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
	FideiPreferences* self = FIDEI_PREFERENCES(object);

	switch (prop_id) {
		case PROP_SETTINGS:
			fidei_preferences_set_settings(self, g_value_get_object(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}
static void fidei_preferences_object_dispose(GObject* object) {
	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(FIDEI_PREFERENCES(object));

	g_clear_object(&priv->settings);

	G_OBJECT_CLASS(fidei_preferences_parent_class)->dispose(object);
}


static void fidei_preferences_class_init(FideiPreferencesClass* class) {
	GObjectClass* object_class = G_OBJECT_CLASS(class);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(class);

	object_class->get_property = fidei_preferences_object_get_property;
	object_class->set_property = fidei_preferences_object_set_property;
	object_class->dispose = fidei_preferences_object_dispose;

	obj_properties[PROP_SETTINGS] = g_param_spec_object("settings", "Settings", "GSettings configuration", G_TYPE_SETTINGS, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

	gtk_widget_class_set_template_from_resource(widget_class, "/arpa/sp1rit/Fidei/ui/prefs.ui");
	gtk_widget_class_bind_template_child_private(widget_class, FideiPreferences, caption_btn);
	gtk_widget_class_bind_template_child_private(widget_class, FideiPreferences, font_btn);
	gtk_widget_class_bind_template_child_private(widget_class, FideiPreferences, smallcaps_bin);
}

static void fidei_preferences_init(FideiPreferences* self) {
	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);
	priv->settings = NULL;
	priv->active_addlang_buf = NULL;

	gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget* fidei_preferences_new(GSettings* settings) {
	return g_object_new(FIDEI_TYPE_PREFERENCES, "settings", settings, NULL);
}

static void fidei_preferences_regex_entry_changed(GtkEntryBuffer* buf, GParamSpec*, FideiPreferences* self) {
	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);

	const gchar* entry_lang = g_object_get_data(G_OBJECT(buf), "language");

	GVariantBuilder builder;
	g_variant_builder_init(&builder, G_VARIANT_TYPE ("a{ss}"));

	GVariantIter* iter;
	g_settings_get(priv->settings, "small-caps", "a{ss}", &iter);
	gchar *lang,*regex;
	while (g_variant_iter_loop(iter, "{ss}", &lang, &regex)) {
		if (g_strcmp0(entry_lang, lang) == 0)
			g_variant_builder_add(&builder, "{ss}", entry_lang, gtk_entry_buffer_get_text(buf));
		else
			g_variant_builder_add(&builder, "{ss}", lang, regex);
	}
	g_variant_iter_free(iter);

	GVariant* variant = g_variant_builder_end(&builder);
	g_settings_set_value(priv->settings, "small-caps", variant);
}

static void fidei_preferences_build_smbox(FideiPreferences* self);
static void fidei_preferences_langdelbtn_clicked(GtkButton* btn, FideiPreferences* self) {
	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);

	const gchar* entry_lang = g_object_get_data(G_OBJECT(btn), "language");

	GVariantBuilder builder;
	g_variant_builder_init(&builder, G_VARIANT_TYPE ("a{ss}"));

	GVariantIter* iter;
	g_settings_get(priv->settings, "small-caps", "a{ss}", &iter);
	gchar *lang,*regex;
	while (g_variant_iter_loop(iter, "{ss}", &lang, &regex)) {
		if (g_strcmp0(entry_lang, lang) != 0)
			g_variant_builder_add(&builder, "{ss}", lang, regex);
	}
	g_variant_iter_free(iter);

	GVariant* variant = g_variant_builder_end(&builder);
	g_settings_set_value(priv->settings, "small-caps", variant);

	fidei_preferences_build_smbox(self);
}


static void fidei_preferences_langaddbtn_pressed(GtkButton*, FideiPreferences* self) {
	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);
	g_return_if_fail(GTK_IS_ENTRY_BUFFER(priv->active_addlang_buf));

	GVariantBuilder builder;
	g_variant_builder_init(&builder, G_VARIANT_TYPE ("a{ss}"));

	GVariantIter* iter;
	g_settings_get(priv->settings, "small-caps", "a{ss}", &iter);
	gchar *lang,*regex;
	while (g_variant_iter_loop(iter, "{ss}", &lang, &regex))
			g_variant_builder_add(&builder, "{ss}", lang, regex);
	g_variant_iter_free(iter);
	g_variant_builder_add(&builder, "{ss}", gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(priv->active_addlang_buf)), "");

	GVariant* variant = g_variant_builder_end(&builder);
	g_settings_set_value(priv->settings, "small-caps", variant);

	fidei_preferences_build_smbox(self);
}

static GtkWidget* fidei_preferences_build_addlangbox(FideiPreferences* self) {
	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);

	GtkWidget* row = adw_action_row_new();
	adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), _t("Add language"));

	GtkWidget* lang = gtk_entry_new();
	gtk_widget_set_hexpand(lang, TRUE);
	gtk_widget_set_valign(lang, GTK_ALIGN_CENTER);
	GtkEntryBuffer* buf = gtk_entry_get_buffer(GTK_ENTRY(lang));

	GtkWidget* addbtn = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(addbtn), "list-add-symbolic");
	gtk_widget_set_valign(addbtn, GTK_ALIGN_CENTER);
	gtk_widget_add_css_class(addbtn, "suggested-action");

	adw_action_row_add_suffix(ADW_ACTION_ROW(row), lang);
	adw_action_row_add_suffix(ADW_ACTION_ROW(row), addbtn);
	adw_action_row_set_activatable_widget(ADW_ACTION_ROW(row), lang);

	priv->active_addlang_buf = buf;

	g_signal_connect(addbtn, "clicked", G_CALLBACK(fidei_preferences_langaddbtn_pressed), self);

	return row;
}

static void fidei_preferences_build_smbox(FideiPreferences* self) {
	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);

	GtkWidget* box = gtk_list_box_new();
	gtk_list_box_set_selection_mode(GTK_LIST_BOX(box), GTK_SELECTION_NONE);
	gtk_widget_add_css_class(box, "boxed-list");

	GVariantIter* iter;
	g_settings_get(priv->settings, "small-caps", "a{ss}", &iter);
	gchar *lang,*regex;
	while (g_variant_iter_loop(iter, "{ss}", &lang, &regex)) {
		GtkWidget* row = adw_expander_row_new();

		gchar* lang_ui = miso693_to_human_same(lang);
		adw_preferences_row_set_title(ADW_PREFERENCES_ROW(row), lang_ui);
		g_free(lang_ui);

		GtkWidget* actionbox = adw_action_row_new();
		adw_preferences_row_set_title(ADW_PREFERENCES_ROW(actionbox), _t("Regular Expression"));
		GtkWidget* action = gtk_entry_new();
		gtk_widget_set_hexpand(action, TRUE);
		gtk_widget_set_valign(action, GTK_ALIGN_CENTER);
		GtkEntryBuffer* buf = gtk_entry_get_buffer(GTK_ENTRY(action));
		gtk_entry_buffer_set_text(buf, regex, -1);
		g_object_set_data_full(G_OBJECT(buf), "language", g_strdup(lang), g_free);
		g_signal_connect(buf, "notify::text", G_CALLBACK(fidei_preferences_regex_entry_changed), self);
		adw_action_row_add_suffix(ADW_ACTION_ROW(actionbox), action);
		adw_action_row_set_activatable_widget(ADW_ACTION_ROW(actionbox), action);
		adw_expander_row_add_row(ADW_EXPANDER_ROW(row), actionbox);

		GtkWidget* btnbox = adw_action_row_new();
		adw_preferences_row_set_title(ADW_PREFERENCES_ROW(btnbox), _t("Remove language"));
		GtkWidget* delete_lang_btn = gtk_button_new();
		gtk_button_set_icon_name(GTK_BUTTON(delete_lang_btn), "user-trash-symbolic");
		gtk_widget_set_valign(delete_lang_btn, GTK_ALIGN_CENTER);
		gtk_widget_add_css_class(delete_lang_btn, "destructive-action");
		g_object_set_data_full(G_OBJECT(delete_lang_btn), "language", g_strdup(lang), g_free);
		g_signal_connect(delete_lang_btn, "clicked", G_CALLBACK(fidei_preferences_langdelbtn_clicked), self);
		adw_action_row_add_suffix(ADW_ACTION_ROW(btnbox), delete_lang_btn);
		adw_action_row_set_activatable_widget(ADW_ACTION_ROW(btnbox), delete_lang_btn);
		adw_expander_row_add_row(ADW_EXPANDER_ROW(row), btnbox);

		g_object_bind_property(buf, "text", row, "subtitle", G_BINDING_SYNC_CREATE);

		gtk_list_box_append(GTK_LIST_BOX(box), row);
	}
	g_variant_iter_free(iter);

	gtk_list_box_append(GTK_LIST_BOX(box), fidei_preferences_build_addlangbox(self));
	adw_bin_set_child(priv->smallcaps_bin, box);
}

GSettings* fidei_preferences_get_settings(FideiPreferences* self) {
	g_return_val_if_fail(FIDEI_IS_PREFERENCES(self), NULL);

	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);
	return priv->settings;
}
void fidei_preferences_set_settings(FideiPreferences* self, GSettings* settings) {
	g_return_if_fail(FIDEI_IS_PREFERENCES(self));

	FideiPreferencesPrivate* priv = fidei_preferences_get_instance_private(self);
	if (priv->settings) {
		g_critical(_t("Property settings of Fidei.Preferences can only be set once"));
		return;
	}

	priv->settings = g_object_ref(settings);

	g_settings_bind(priv->settings, "captions", priv->caption_btn, "active", G_SETTINGS_BIND_DEFAULT);
	g_settings_bind(priv->settings, "font", priv->font_btn, "font", G_SETTINGS_BIND_DEFAULT);
	fidei_preferences_build_smbox(self);

	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_SETTINGS]);
}
