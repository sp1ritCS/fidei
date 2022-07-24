#include "utils.h"

#ifdef HAS_LIBICU
#include <libintl.h>
#include <unicode/uloc.h>

gchar* miso693_to_human(const gchar* iso693) {
	if (!iso693)
		return NULL;

	UChar wide_language_name[0x7F];
	UErrorCode status = U_ZERO_ERROR;
	gint32 len = uloc_getDisplayName(iso693, NULL, wide_language_name, 0x7F, &status);
	if (len <= 0)
		return NULL;

	GError* err = NULL;
	gchar* language_name = g_utf16_to_utf8(wide_language_name, len, NULL, NULL, &err);
	if (err) {
		g_warning("Failed converting wide string to UTF-8: %s\n", err->message);
		g_error_free(err);
		return NULL;
	}

	if (g_ascii_strcasecmp(language_name, iso693) == 0) {
		g_free(language_name);
		return NULL;
	}

	gchar* human = g_strdup_printf("%s (%s)", language_name, iso693);
	g_free(language_name);
	return human;
}

gchar* miso693_to_human_same(const gchar* iso693) {
	if (!iso693)
		return NULL;

	gchar* human = miso693_to_human(iso693);
	if (!human)
		return g_strdup(iso693);
	return human;
}
#else
gchar* miso693_to_human(const gchar* iso693) {
	return g_strdup(iso693);
}
gchar* miso693_to_human_same(const gchar* iso693) {
	return g_strdup(iso693);
}
#endif
