#include "num.h"

struct _FideiListNum {
	GObject parent_class;
	gint num;
};

G_DEFINE_TYPE(FideiListNum, fidei_listnum, G_TYPE_OBJECT)

void fidei_listnum_class_init(FideiListNumClass*) {}
void fidei_listnum_init(FideiListNum* self) {
	self->num = -1;
}

FideiListNum* fidei_listnum_new(gint num) {
	FideiListNum* self = g_object_new(FIDEI_TYPE_LISTNUM, NULL);
	self->num = num;
	return self;
}

gint fidei_listnum_get(FideiListNum* self) {
	g_return_val_if_fail(FIDEI_IS_LISTNUM(self), -1);
	return self->num;
}

