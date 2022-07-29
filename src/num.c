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

