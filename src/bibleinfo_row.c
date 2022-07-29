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

#include "bibleinfo_diag.h"

typedef struct {
	gchar* label;
	gchar* property;
} FideiBibleInfoRowPrivate;

struct _FideiBibleInfoRow {
	GtkListBoxRow parent_instance;
};

G_DEFINE_TYPE_WITH_PRIVATE(FideiBibleInfoRow, fidei_bibleinfo_row, GTK_TYPE_LIST_BOX_ROW)

enum {
	PROP_LABEL = 1,
	PROP_PROPERTY,
	N_PROPERTIES
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL, };

static void fidei_bibleinfo_row_object_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
	FideiBibleInfoRow* self = FIDEI_BIBLEINFO_ROW(object);

	switch (prop_id) {
		case PROP_LABEL:
			g_value_set_string(value, fidei_bibleinfo_row_get_label(self));
			break;
		case PROP_PROPERTY:
			g_value_set_string(value, fidei_bibleinfo_row_get_property(self));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}
static void fidei_bibleinfo_row_object_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
	FideiBibleInfoRow* self = FIDEI_BIBLEINFO_ROW(object);

	switch (prop_id) {
		case PROP_LABEL:
			fidei_bibleinfo_row_set_label(self, g_value_get_string(value));
			break;
		case PROP_PROPERTY:
			fidei_bibleinfo_row_set_property(self, g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}
static void fidei_bibleinfo_row_object_dispose(GObject* object) {
	FideiBibleInfoRowPrivate* priv = fidei_bibleinfo_row_get_instance_private(FIDEI_BIBLEINFO_ROW(object));

	g_free(g_steal_pointer(&priv->label));
	g_free(g_steal_pointer(&priv->property));

	G_OBJECT_CLASS(fidei_bibleinfo_row_parent_class)->dispose(object);
}

static void fidei_bibleinfo_row_class_init(FideiBibleInfoRowClass* class) {
	GObjectClass* object_class = G_OBJECT_CLASS(class);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(class);

	object_class->get_property = fidei_bibleinfo_row_object_get_property;
	object_class->set_property = fidei_bibleinfo_row_object_set_property;
	object_class->dispose = fidei_bibleinfo_row_object_dispose;

	obj_properties[PROP_LABEL] = g_param_spec_string("label", "Label", "Label relevant to this item", NULL, G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);
	obj_properties[PROP_PROPERTY] = g_param_spec_string("property", "Property", "Proprty relevant to this item", NULL, G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

	gtk_widget_class_set_template_from_resource(widget_class, "/arpa/sp1rit/Fidei/ui/info_row.ui");
}

void fidei_bibleinfo_row_init(FideiBibleInfoRow* self) {
	FideiBibleInfoRowPrivate* priv = fidei_bibleinfo_row_get_instance_private(self);
	priv->label = NULL;
	priv->property = NULL;

	gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget* fidei_bibleinfo_row_new(const gchar* label, const gchar* property) {
	return g_object_new(FIDEI_TYPE_BIBLEINFO_ROW, "label", label, "poperty", property, NULL);
}


const gchar* fidei_bibleinfo_row_get_label(FideiBibleInfoRow* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEINFO_ROW(self), NULL);
	FideiBibleInfoRowPrivate* priv = fidei_bibleinfo_row_get_instance_private(self);

	return priv->label;
}
void fidei_bibleinfo_row_set_label(FideiBibleInfoRow* self, const gchar* label) {
	g_return_if_fail(FIDEI_IS_BIBLEINFO_ROW(self));
	FideiBibleInfoRowPrivate* priv = fidei_bibleinfo_row_get_instance_private(self);

	if (priv->label)
		g_free(priv->label);

	priv->label = g_strdup(label);

	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_LABEL]);
}

const gchar* fidei_bibleinfo_row_get_property(FideiBibleInfoRow* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEINFO_ROW(self), NULL);
	FideiBibleInfoRowPrivate* priv = fidei_bibleinfo_row_get_instance_private(self);

	return priv->property;
}
void fidei_bibleinfo_row_set_property(FideiBibleInfoRow* self, const gchar* property) {
	g_return_if_fail(FIDEI_IS_BIBLEINFO_ROW(self));
	FideiBibleInfoRowPrivate* priv = fidei_bibleinfo_row_get_instance_private(self);

	if (priv->property)
		g_free(priv->property);

	priv->property = g_strdup(property);

	g_object_notify_by_pspec(G_OBJECT(self), obj_properties[PROP_PROPERTY]);
}
