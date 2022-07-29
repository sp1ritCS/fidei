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
