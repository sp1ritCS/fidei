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

#ifndef __BIBLE_H__
#define __BIBLE_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define FIDEI_TYPE_BIBLEBOOK (fidei_biblebook_get_type())
G_DECLARE_FINAL_TYPE (FideiBibleBook, fidei_biblebook, FIDEI, BIBLEBOOK, GObject)
const gchar* fidei_biblebook_get_bname(FideiBibleBook* self);
const gchar* fidei_biblebook_get_bsname(FideiBibleBook* self);
gint fidei_biblebook_get_booknum(FideiBibleBook* self);
gint fidei_biblebook_get_num_chapters(FideiBibleBook* self);

// ---

typedef struct {
	gchar* caption;
	gchar* content;
} FideiBibleVers;
void fidei_biblevers_free_inner(FideiBibleVers vers);

#define FIDEI_TYPE_BIBLE (fidei_bible_get_type())
G_DECLARE_FINAL_TYPE (FideiBible, fidei_bible, FIDEI, BIBLE, GObject)

const gchar* fidei_bible_get_path(FideiBible* self);
void fidei_bible_set_path(FideiBible* self, const gchar* path);

FideiBible* fidei_bible_new(const gchar* path);

const gchar* fidei_bible_get_title(FideiBible* self);
const gchar* fidei_bible_get_lang(FideiBible* self);
const gchar* fidei_bible_get_publisher(FideiBible* self);
const gchar* fidei_bible_get_identifier(FideiBible* self);
const gchar* fidei_bible_get_rights(FideiBible* self);

GListStore* fidei_bible_read_books(FideiBible* self);
guint fidei_bible_read_chapter(FideiBible* self, gint book, gint chapter, FideiBibleVers** verses);

G_END_DECLS

#endif //__BIBLE_H__
