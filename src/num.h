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
