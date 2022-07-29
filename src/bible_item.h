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
