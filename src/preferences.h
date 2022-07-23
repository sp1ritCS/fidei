#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <glib-object.h>
#include <adwaita.h>

G_BEGIN_DECLS

#define FIDEI_TYPE_PREFERENCES (fidei_preferences_get_type())
G_DECLARE_FINAL_TYPE(FideiPreferences, fidei_preferences, FIDEI, PREFERENCES, AdwPreferencesWindow)

GtkWidget* fidei_preferences_new(GSettings* settings);

GSettings* fidei_preferences_get_settings(FideiPreferences* self);
void fidei_preferences_set_settings(FideiPreferences* self, GSettings* settings);

G_END_DECLS

#endif // __PREFERENCES_H__
