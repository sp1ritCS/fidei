#include "bible.h"
#include <libxml/xmlreader.h>


typedef struct {
	xmlChar* bname;
	xmlChar* bsname;
	gint booknum;
	gint num_chapters;
} FideiBibleBookPrivate;
struct _FideiBibleBook {
	GObject parent_instance;
};
G_DEFINE_TYPE_WITH_PRIVATE (FideiBibleBook, fidei_biblebook, G_TYPE_OBJECT)
void fidei_biblebook_object_dispose(GObject* object) {
	FideiBibleBookPrivate* priv = fidei_biblebook_get_instance_private(FIDEI_BIBLEBOOK(object));

	g_free(g_steal_pointer(&priv->bname));
	g_free(g_steal_pointer(&priv->bsname));

	G_OBJECT_CLASS(fidei_biblebook_parent_class)->dispose(object);
}
void fidei_biblebook_class_init(FideiBibleBookClass* class) {
	G_OBJECT_CLASS(class)->dispose = fidei_biblebook_object_dispose;
}
void fidei_biblebook_init(FideiBibleBook* self) {
	FideiBibleBookPrivate* priv = fidei_biblebook_get_instance_private(self);
	priv->bname = NULL;
	priv->bsname = NULL;
	priv->booknum = -1;
	priv->num_chapters = -1;
}

// takes ownership of strings
FideiBibleBook* fidei_biblebook_new(xmlChar* bname, xmlChar* bsname, gint booknum, gint num_chapters) {
	FideiBibleBook* self = g_object_new(FIDEI_TYPE_BIBLEBOOK, NULL);
	FideiBibleBookPrivate* priv = fidei_biblebook_get_instance_private(self);
	priv->bname = bname;
	priv->bsname = bsname;
	priv->booknum = booknum;
	priv->num_chapters = num_chapters;

	return self;
}

const gchar* fidei_biblebook_get_bname(FideiBibleBook* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEBOOK(self), NULL);
	FideiBibleBookPrivate* priv = fidei_biblebook_get_instance_private(self);
	return (gchar*)priv->bname;
}
const gchar* fidei_biblebook_get_bsname(FideiBibleBook* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEBOOK(self), NULL);
	FideiBibleBookPrivate* priv = fidei_biblebook_get_instance_private(self);
	return (gchar*)priv->bsname;
}
gint fidei_biblebook_get_booknum(FideiBibleBook* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEBOOK(self), -1);
	FideiBibleBookPrivate* priv = fidei_biblebook_get_instance_private(self);
	return priv->booknum;
}
gint fidei_biblebook_get_num_chapters(FideiBibleBook* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLEBOOK(self), -1);
	FideiBibleBookPrivate* priv = fidei_biblebook_get_instance_private(self);
	return priv->num_chapters;
}

// ---

void fidei_biblevers_free_inner(FideiBibleVers vers) {
	g_free(g_steal_pointer(&vers.caption));
	g_free(g_steal_pointer(&vers.content));
}


typedef struct {
	gchar* path;

	xmlChar* title;
	xmlChar* publisher;
	xmlChar* lang;
	xmlChar* identifier;
	xmlChar* rights;

	xmlTextReaderPtr reader;
	GListStore* books;
} FideiBiblePrivate;

struct _FideiBible {
	GObject parent_instance;

	/* Other members, including private data. */
};

G_DEFINE_TYPE_WITH_PRIVATE (FideiBible, fidei_bible, G_TYPE_OBJECT)

enum {
	PROP_PATH = 1,
	N_PROPERTIES
};

static GParamSpec* obj_properties[N_PROPERTIES] = { NULL, };

static void fidei_bible_object_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec) {
	FideiBible* self = FIDEI_BIBLE(object);

	switch (prop_id) {
		case PROP_PATH:
			g_value_set_string(value, fidei_bible_get_path(self));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}
static void fidei_bible_object_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec) {
	FideiBible* self = FIDEI_BIBLE(object);

	switch (prop_id) {
		case PROP_PATH:
			fidei_bible_set_path(self, g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}

static void fidei_bible_object_dispose(GObject* object) {
	FideiBiblePrivate* priv = fidei_bible_get_instance_private(FIDEI_BIBLE(object));

	g_free(g_steal_pointer(&priv->path));
	g_free(g_steal_pointer(&priv->title));
	g_free(g_steal_pointer(&priv->publisher));
	g_free(g_steal_pointer(&priv->lang));
	g_free(g_steal_pointer(&priv->identifier));
	g_free(g_steal_pointer(&priv->rights));

	if (priv->reader)
		xmlFreeTextReader(g_steal_pointer(&priv->reader));

	g_clear_object(&priv->books);

	G_OBJECT_CLASS(fidei_bible_parent_class)->dispose(object);
}

static void fidei_bible_class_init(FideiBibleClass* class) {
	GObjectClass* object_class = G_OBJECT_CLASS(class);

	object_class->get_property = fidei_bible_object_get_property;
	object_class->set_property = fidei_bible_object_set_property;
	object_class->dispose = fidei_bible_object_dispose;

	obj_properties[PROP_PATH] = g_param_spec_string("path", "Path", "Path of the file to load and display from.", NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
	g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
}

static void fidei_bible_init(FideiBible* self) {
	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);
	priv->path = NULL;
	priv->title = NULL;
	priv->publisher = NULL;
	priv->lang = NULL;
	priv->identifier = NULL;
	priv->rights = NULL;
	priv->reader = NULL;
}

FideiBible* fidei_bible_new(const gchar* path) {
	return g_object_new(FIDEI_TYPE_BIBLE, "path", path, NULL);
}

static gboolean fidei_bible_navigate_to_xmlbible(xmlTextReaderPtr reader) {
	gint ret;

	ret = xmlTextReaderRead(reader);
	while (ret == 1) {
		const xmlChar* name = xmlTextReaderConstName(reader);
		if (g_strcmp0((gchar*)name, "XMLBIBLE") == 0) {
			ret = xmlTextReaderRead(reader);
			return TRUE;
		}
		ret = xmlTextReaderRead(reader);
	}
	return FALSE;
}

static void fidei_bible_parse_info(FideiBible* self) {
	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);

	xmlTextReaderPtr reader = xmlReaderForFile(priv->path, NULL, 0);
	gint ret = 1;

	if (fidei_bible_navigate_to_xmlbible(reader))
		goto found_xmlbible;
	else
		goto out;

found_xmlbible:
	while (ret == 1) {
		const xmlChar* name = xmlTextReaderConstName(reader);
		if (g_strcmp0((gchar*)name, "INFORMATION") == 0)
			goto found_information;
		else if (g_strcmp0((gchar*)name, "BIBLEBOOK") == 0)
			goto out;

		ret = xmlTextReaderRead(reader);
	}

found_information: {
	xmlNodePtr info = xmlTextReaderExpand(reader);

	xmlNodePtr cur = info->children;
	while (cur) {
		switch (g_str_hash(cur->name)) {
			case 0x106daa27:
				priv->title = xmlNodeGetContent(cur);
				break;
			case 0x5e099013:
				priv->publisher = xmlNodeGetContent(cur);
				break;
			case 0x5a6b41c9:
				priv->lang = xmlNodeGetContent(cur);
				break;
			case 0xbe5ad288:
				priv->identifier = xmlNodeGetContent(cur);
				break;
			case 0x19716e36:
				priv->rights = xmlNodeGetContent(cur);
			default:
				(void)0;
		}

		cur = cur->next;
	}

	ret = xmlTextReaderNext(priv->reader);


/**/}

out:
	priv->reader = reader;
}

const gchar* fidei_bible_get_path(FideiBible* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLE(self), NULL);

	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);
	return priv->path;
}
void fidei_bible_set_path(FideiBible* self, const gchar* path) {
	g_return_if_fail(FIDEI_IS_BIBLE(self));

	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);

	if (priv->path)
		g_free(priv->path);
	priv->path = g_strdup(path);

	if (!priv->path)
		return;

	fidei_bible_parse_info(self);
}

const gchar* fidei_bible_get_title(FideiBible* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLE(self), NULL);

	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);
	return (gchar*)priv->title;
}
const gchar* fidei_bible_get_lang(FideiBible* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLE(self), NULL);

	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);
	return (gchar*)priv->lang;
}
const gchar* fidei_bible_get_publisher(FideiBible* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLE(self), NULL);

	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);
	return (gchar*)priv->publisher;
}
const gchar* fidei_bible_get_identifier(FideiBible* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLE(self), NULL);

	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);
	return (gchar*)priv->identifier;
}

GListStore* fidei_bible_read_books(FideiBible* self) {
	g_return_val_if_fail(FIDEI_IS_BIBLE(self), NULL);
	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);

	if (priv->reader) {
		GListStore* books = g_list_store_new(FIDEI_TYPE_BIBLEBOOK);

		gint booknum = 0;
		gint ret = 1;
		while (ret == 1) {
			const xmlChar* name = xmlTextReaderConstName(priv->reader);
			if (g_strcmp0((gchar*)name, "BIBLEBOOK") == 0) {
				xmlChar* bname = xmlTextReaderGetAttribute(priv->reader, (xmlChar*)"bname");
				xmlChar* bsname = xmlTextReaderGetAttribute(priv->reader, (xmlChar*)"bsname");
				gint num_chapters = 0;

				gint cret;
				cret = xmlTextReaderRead(priv->reader);
				while (cret == 1) {
					const xmlChar* name = xmlTextReaderConstName(priv->reader);
					if (g_strcmp0((gchar*)name, "CHAPTER") == 0)
						num_chapters++;
					else if (g_strcmp0((gchar*)name, "#text") != 0)
						break;

					cret = xmlTextReaderNext(priv->reader);
				}

				FideiBibleBook* book = fidei_biblebook_new(bname, bsname, booknum, num_chapters);
				g_list_store_append(books, book);
				g_object_unref(book);

				booknum++;
			}

			ret = xmlTextReaderNext(priv->reader);
		};

		priv->books = books;
		xmlFreeTextReader(g_steal_pointer(&priv->reader));

		return priv->books;
	} else {
		return priv->books;
	}
}

guint fidei_bible_read_chapter(FideiBible* self, gint book, gint chapter, FideiBibleVers** verses) {
	g_return_val_if_fail(FIDEI_IS_BIBLE(self), -1);
	g_return_val_if_fail((book | chapter) >= 0, -1);
	FideiBiblePrivate* priv = fidei_bible_get_instance_private(self);

	FideiBibleVers* verses_i = NULL;
	guint num_verses = 0;

	gint seen_books = -1;
	gint seen_chapters = -1;

	xmlTextReaderPtr reader = xmlReaderForFile(priv->path, NULL, 0);
	gint ret = 1;

	if (fidei_bible_navigate_to_xmlbible(reader))
		goto found_xmlbible;
	else
		goto out;

found_xmlbible:
	while (ret == 1) {
		const xmlChar* name = xmlTextReaderConstName(reader);
		if (g_strcmp0((gchar*)name, "BIBLEBOOK") == 0)
			seen_books++;

		if (seen_books == book) {
			ret = xmlTextReaderRead(reader);
			goto found_book;
		}

		ret = xmlTextReaderNext(reader);
	}
	goto out;

found_book:
	while (ret == 1) {
		const xmlChar* name = xmlTextReaderConstName(reader);
		if (g_strcmp0((gchar*)name, "CHAPTER") == 0)
				seen_chapters++;

		if (seen_chapters == chapter)
			goto found_chapter;

		ret = xmlTextReaderNext(reader);
	}

found_chapter: {
	xmlNodePtr chapternode = xmlTextReaderExpand(reader);
	xmlNodePtr cur = chapternode->children;
	while (cur) {
		if (g_strcmp0((gchar*)cur->name, "VERS") == 0)
			num_verses++;
		cur = cur->next;
	}

	verses_i = g_new0(FideiBibleVers, num_verses);
	cur = chapternode->children;
	gint i = 0;
	while (cur) {
		if (g_strcmp0((gchar*)cur->name, "CAPTION") == 0) {
			xmlChar* vers = xmlGetProp(cur, (const xmlChar*)"vref");
			if (vers) {
				gint64 versref = g_ascii_strtoull((const gchar*)vers, NULL, 10);
				if (versref && versref < num_verses)
					verses_i[i].caption = (gchar*)xmlNodeGetContent(cur);
				else
					g_warning("Invalid vref found on caption: %s", vers);
				xmlFree(vers);
			}
		}
		if (g_strcmp0((gchar*)cur->name, "VERS") == 0) {
			verses_i[i].content = (gchar*)xmlNodeGetContent(cur);
			i++;
		}
		cur = cur->next;
	}
/**/}

out:
	xmlFreeTextReader(reader);
	*verses = verses_i;
	return num_verses;
}
