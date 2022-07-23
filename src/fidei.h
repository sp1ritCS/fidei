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
