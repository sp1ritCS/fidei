#ifndef __BIBLE_ITEM_H__
#define __BIBLE_ITEM_H__

#include <glib-object.h>
#include <gtk/gtk.h>

#include "bible.h"

G_BEGIN_DECLS

#define FIDEI_TYPE_BIBLEPICKER_ITEM_ROW (fidei_biblepicker_item_row_get_type())
G_DECLARE_FINAL_TYPE(FideiBiblePickerItemRow, fidei_biblepicker_item_row, FIDEI, BIBLEPICKER_ITEM_ROW, GtkListBoxRow)

GtkWidget* fidei_biblepicker_item_row_new(FideiBible* bible);

FideiBible* fidei_biblepicker_item_row_get_bible(FideiBiblePickerItemRow* self);
void fidei_biblepicker_item_row_set_bible(FideiBiblePickerItemRow* self, FideiBible* bible);

const gchar* fidei_biblepicker_item_row_get_title(FideiBiblePickerItemRow* self);
gchar* fidei_biblepicker_item_row_get_subtitle(FideiBiblePickerItemRow* self);

G_END_DECLS

#endif // __BIBLE_ITEM_H__
