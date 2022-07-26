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

#ifndef __BIBLEINFO_DIAG_H__
#define __BIBLEINFO_DIAG_H__

#include <glib-object.h>
#include <gtk/gtk.h>

#include "bible.h"

G_BEGIN_DECLS

#define FIDEI_TYPE_BIBLEINFO_ROW (fidei_bibleinfo_row_get_type())
G_DECLARE_FINAL_TYPE(FideiBibleInfoRow, fidei_bibleinfo_row, FIDEI, BIBLEINFO_ROW, GtkListBoxRow)

GtkWidget* fidei_bibleinfo_row_new(const gchar* label, const gchar* property);

const gchar* fidei_bibleinfo_row_get_label(FideiBibleInfoRow* self);
void fidei_bibleinfo_row_set_label(FideiBibleInfoRow* self, const gchar* label);

const gchar* fidei_bibleinfo_row_get_property(FideiBibleInfoRow* self);
void fidei_bibleinfo_row_set_property(FideiBibleInfoRow* self, const gchar* property);

// ---

#define FIDEI_TYPE_BIBLEINFO_DIAG (fidei_bibleinfo_diag_get_type())
G_DECLARE_FINAL_TYPE(FideiBibleInfoDiag, fidei_bibleinfo_diag, FIDEI, BIBLEINFO_DIAG, GtkDialog)

GtkWidget* fidei_bibleinfo_diag_new(GtkWindow* parent, FideiBible* bible);

GtkWindow* fidei_bibleinfo_diag_get_parent(FideiBibleInfoDiag* self);
void fidei_bibleinfo_diag_set_parent(FideiBibleInfoDiag* self, GtkWindow* parent);

FideiBible* fidei_bibleinfo_diag_get_bible(FideiBibleInfoDiag* self);
void fidei_bibleinfo_diag_set_bible(FideiBibleInfoDiag* self, FideiBible* bible);

const gchar* fidei_bibleinfo_diag_get_title(FideiBibleInfoDiag* self);
const gchar* fidei_bibleinfo_diag_get_path(FideiBibleInfoDiag* self);
const gchar* fidei_bibleinfo_diag_get_publisher(FideiBibleInfoDiag* self);
const gchar* fidei_bibleinfo_diag_get_lang(FideiBibleInfoDiag* self);
const gchar* fidei_bibleinfo_diag_get_identifier(FideiBibleInfoDiag* self);
const gchar* fidei_bibleinfo_diag_get_rights(FideiBibleInfoDiag* self);

G_END_DECLS

#endif // __BIBLEINFO_DIAG_H__
