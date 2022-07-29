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

#ifndef __FIDEI_H__
#define __FIDEI_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <adwaita.h>

#include "bible.h"

G_BEGIN_DECLS

#define FIDEI_TYPE_APPWINDOW (fidei_appwindow_get_type())
G_DECLARE_FINAL_TYPE (FideiAppWindow, fidei_appwindow, FIDEI, APPWINDOW, AdwApplicationWindow)

GtkWidget* fidei_appwindow_new(GtkApplication* app, GListModel* bibles);

GListModel* fidei_appwindow_get_bibles(FideiAppWindow* self);
void fidei_appwindow_set_bibles(FideiAppWindow* self, GListModel* bibles);

FideiBible* fidei_appwindow_get_active_bible(FideiAppWindow* self);
void fidei_appwindow_set_active_bible(FideiAppWindow* self, FideiBible* bible);

void fidei_appwindow_open_chapter(FideiAppWindow* self, gint book, gint chapter);

G_END_DECLS

#endif // __FIDEI_H__
