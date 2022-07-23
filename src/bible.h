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

#define FIDEI_TYPE_BIBLE (fidei_bible_get_type())
G_DECLARE_FINAL_TYPE (FideiBible, fidei_bible, FIDEI, BIBLE, GObject)

const gchar* fidei_bible_get_path(FideiBible* self);
void fidei_bible_set_path(FideiBible* self, const gchar* path);

FideiBible* fidei_bible_new(const gchar* path);

const gchar* fidei_bible_get_title(FideiBible* self);
const gchar* fidei_bible_get_lang(FideiBible* self);
const gchar* fidei_bible_get_publisher(FideiBible* self);
const gchar* fidei_bible_get_identifier(FideiBible* self);

GListStore* fidei_bible_read_books(FideiBible* self);
gchar** fidei_bible_read_chapter(FideiBible* self, gint book, gint chapter);

G_END_DECLS

#endif //__BIBLE_H__
