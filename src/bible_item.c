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

#include "bible_item.h"
#include "utils.h"

typedef struct {
	FideiBible* bible;

	GtkLabel* subtitle;
} FideiBiblePickerItemRowPrivate;

struct _FideiBiblePickerItemRow {
	GtkListBoxRow parent_instance;
};

G_DEFINE_TYPE_WITH_PRIVATE(FideiBiblePickerItemRow, fidei_biblepicker_item_row, GTK_TYPE_LIST_BOX_ROW)

enum {
	PROP_BIBLE = 1,
	PROP_TITLE,
	PROP_SUBTITLE,
	N_PROPERTIES
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL, };

static void fidei_biblepicker_item_row_object_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
	FideiBiblePickerItemRow* self = FIDEI_BIBLEPICKER_ITEM_ROW(object);

	switch (prop_id) {
		case PROP_BIBLE:
			g_value_set_object(value, fidei_biblepicker_item_row_get_bible(self));
			break;
		case PROP_TITLE:
			g_value_set_string(value, fidei_biblepicker_item_row_get_title(self));
			break;
		case PROP_SUBTITLE:
			g_value_take_string(value, fidei_biblepicker_item_row_get_subtitle(self));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}
static void fidei_biblepicker_item_row_object_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
	FideiBiblePickerItemRow* self = FIDEI_BIBLEPICKER_ITEM_ROW(object);

	switch (prop_id) {
		case PROP_BIBLE:
			fidei_biblepicker_item_row_set_bible(self, g_value_get_object(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}

void fidei_biblepicker_item_row_class_init(FideiBiblePickerItemRowClass* class) {
	GObjectClass* object_class = G_OBJECT_CLASS(class);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(class);

	object_class->get_property = fidei_biblepicker_item_row_object_get_property;
	object_class->set_property = fidei_biblepicker_item_row_object_set_property;
	//object_class->dispose = fidei_biblepicker_item_row_object_dispose;

	obj_properties[PROP_BIBLE] = g_param_spec_object("bible", "Bible", "Bible linked to this Row", FIDEI_TYPE_BIBLE, G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_TITLE] = g_param_spec_string("title", "Title", "Title of property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_SUBTITLE] = g_param_spec_string("subtitle", "Subtitle", "Subtitle describing property bible", NULL, G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

	gtk_widget_class_set_template_from_resource(widget_class, "/arpa/sp1rit/Fidei/ui/bible_item.ui");
	gtk_widget_class_bind_template_child_private(widget_class, FideiBiblePickerItemRow, subtitle);
}
void fidei_biblepicker_item_row_init(FideiBiblePickerItemRow* self) {
	gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget* fidei_biblepicker_item_row_new(FideiBible* bible) {
	return g_object_new(FIDEI_TYPE_BIBLEPICKER_ITEM_ROW, "bible", bible, NULL);
}

FideiBible* fidei_biblepicker_item_row_get_bible(FideiBiblePickerItemRow* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEPICKER_ITEM_ROW(self), NULL);
	FideiBiblePickerItemRowPrivate* priv = fidei_biblepicker_item_row_get_instance_private(self);
	return priv->bible;
}
void fidei_biblepicker_item_row_set_bible(FideiBiblePickerItemRow* self, FideiBible* bible) {
	g_return_if_fail(FIDEI_IS_BIBLEPICKER_ITEM_ROW(self));
	g_return_if_fail(FIDEI_IS_BIBLE(bible));
	FideiBiblePickerItemRowPrivate* priv = fidei_biblepicker_item_row_get_instance_private(self);

	if (priv->bible == bible)
		return;

	priv->bible = bible;

	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_BIBLE]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_TITLE]);
	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_SUBTITLE]);
}

const gchar* fidei_biblepicker_item_row_get_title(FideiBiblePickerItemRow* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEPICKER_ITEM_ROW(self), NULL);
	FideiBiblePickerItemRowPrivate* priv = fidei_biblepicker_item_row_get_instance_private(self);

	if (!priv->bible)
		return NULL;

	return fidei_bible_get_title(priv->bible);
}
gchar* fidei_biblepicker_item_row_get_subtitle(FideiBiblePickerItemRow* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEPICKER_ITEM_ROW(self), NULL);
	FideiBiblePickerItemRowPrivate* priv = fidei_biblepicker_item_row_get_instance_private(self);

	if (!priv->bible)
		return NULL;

	gchar* lang_ui = miso693_to_human_same(fidei_bible_get_lang(priv->bible));
	gchar* subtitle = g_strdup_printf("%s | %s", lang_ui, fidei_bible_get_publisher(priv->bible));
	g_free(lang_ui);

	return subtitle;
}
