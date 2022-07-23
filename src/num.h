#ifndef __NUM_H__
#define __NUM_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define FIDEI_TYPE_LISTNUM (fidei_listnum_get_type())
G_DECLARE_FINAL_TYPE(FideiListNum, fidei_listnum, FIDEI, LISTNUM, GObject)

FideiListNum* fidei_listnum_new(gint num);
gint fidei_listnum_get(FideiListNum* self);

G_END_DECLS

#endif // __NUM_H__
