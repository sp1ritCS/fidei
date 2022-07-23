#include "bible.h"
#include "fidei_res.h"

#include <gtk/gtk.h>
#include <adwaita.h>

struct _ListNumber {
	GObject parent_class;
	gint num;
};

#define LIST_TYPE_NUMBER (list_number_get_type())
G_DECLARE_FINAL_TYPE(ListNumber, list_number, LIST, NUMBER, GObject)
G_DEFINE_TYPE(ListNumber, list_number, G_TYPE_OBJECT)

void list_number_class_init(ListNumberClass*) {}
void list_number_init(ListNumber* self) {
	self->num = -1;
}
ListNumber* list_number_new(gint num) {
	ListNumber* self = g_object_new(LIST_TYPE_NUMBER, NULL);
	self->num = num;
	return self;
}
gint list_number_get(ListNumber* self) {
	g_return_val_if_fail(LIST_IS_NUMBER(self), -1);
	return self->num;
}

// --

static GtkWidget* create_picker_item(const gchar* title, const gchar* lang, const gchar* publisher) {
	GtkWidget* item = gtk_list_box_row_new();

	GtkWidget* child = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
	gtk_widget_set_valign(child, GTK_ALIGN_CENTER);
	gtk_widget_add_css_class(child, "header");

	GtkWidget* titlebox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_valign(titlebox, GTK_ALIGN_CENTER);
	gtk_widget_set_hexpand(titlebox, TRUE);
	gtk_widget_add_css_class(titlebox, "title");

	GtkWidget* titlew = gtk_label_new(title);
	gtk_widget_add_css_class(titlew, "title");
	gtk_label_set_xalign(GTK_LABEL(titlew), 0.f);

	gchar* subtitle = g_strdup_printf("%s | %s", lang, publisher);
	GtkWidget* subtitlew = gtk_label_new(subtitle);
	g_free(subtitle);
	gtk_widget_add_css_class(subtitlew, "subtitle");
	gtk_label_set_xalign(GTK_LABEL(subtitlew), 0.f);

	gtk_box_append(GTK_BOX(titlebox), titlew);
	gtk_box_append(GTK_BOX(titlebox), subtitlew);

	GtkWidget* arrow = gtk_image_new_from_icon_name("go-next-symbolic");

	gtk_box_append(GTK_BOX(child), titlebox);
	gtk_box_append(GTK_BOX(child), arrow);

	gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(item), child);

	return item;
}

static void bible_dir_open(GtkButton*, gpointer) {
	const gchar* datadir = g_get_user_data_dir();
	GFile* bibledir = g_file_new_build_filename(datadir, "arpa.sp1rit.bible", NULL);

	GError* err = NULL;
	GAppInfo* app = g_file_query_default_handler(bibledir, NULL, &err);
	if (err) {
		g_critical("Failed querying launch application: %s\n", err->message);
		g_error_free(err);
		g_object_unref(bibledir);
		return;
	}

	GList* files = g_list_alloc();
	files->prev = NULL;
	files->next = NULL;
	files->data = bibledir;

	g_app_info_launch(app, files, NULL, &err);
	if (err) {
		g_critical("Failed launching application: %s\n", err->message);
		g_error_free(err);
	}

	g_list_free(files);
	g_object_unref(app);
	g_object_unref(bibledir);
}

static void setup_bookitem(GtkListItemFactory*, GtkListItem* item) {
	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_add_css_class(box, "card");
	gtk_widget_set_margin_top(box, 4);
	gtk_widget_set_margin_bottom(box, 4);
	gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
	gtk_widget_add_css_class(box, "book");
	GtkWidget* bname = gtk_label_new(NULL);
	gtk_widget_add_css_class(bname, "title");
	gtk_widget_set_halign(bname, GTK_ALIGN_CENTER);
	gtk_widget_set_margin_start(bname, 4);
	gtk_widget_set_margin_end(bname, 4);
	//gtk_label_set_xalign(GTK_LABEL(bname), 0.f);
	GtkWidget* bsname = gtk_label_new(NULL);
	gtk_widget_add_css_class(bsname, "subtitle");
	//gtk_label_set_xalign(GTK_LABEL(bsname), 0.f);

	gtk_box_append(GTK_BOX(box), bname);
	gtk_box_append(GTK_BOX(box), bsname);

	gtk_list_item_set_child(item, box);
}
static void bind_bookitem(GtkListItemFactory*, GtkListItem* item) {
	GtkWidget* box = gtk_list_item_get_child(item);
	FideiBibleBook* book = gtk_list_item_get_item(item);

	GtkWidget* bname = gtk_widget_get_first_child(box);
	GtkWidget* bsname = gtk_widget_get_last_child(box);

	gtk_label_set_text(GTK_LABEL(bname), fidei_biblebook_get_bname(book));
	gtk_label_set_text(GTK_LABEL(bsname), fidei_biblebook_get_bsname(book));
}

typedef struct {
	GtkStack* stack;
	GtkGridView* view;
	gint* booknum;
} BookClickedUd;
static void book_clicked(GtkListView* view, guint pos, BookClickedUd* user_data) {
	GtkSelectionModel* model = gtk_list_view_get_model(view);
	GListModel* books = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(model));
	FideiBibleBook* book = g_list_model_get_item(books, pos);
	*user_data->booknum = (gint)pos;

	GListStore* store = g_list_store_new(LIST_TYPE_NUMBER);
	for (gint i = 0; i < fidei_biblebook_get_num_chapters(book); i++) {
		ListNumber* num = list_number_new(i);
		g_list_store_append(store, num);
		g_object_unref(num);
	}

	GtkSingleSelection* chaptermodel = gtk_single_selection_new(G_LIST_MODEL(store));
	gtk_grid_view_set_model(user_data->view, GTK_SELECTION_MODEL(chaptermodel));
	g_object_unref(chaptermodel);

	gtk_stack_set_visible_child_name(user_data->stack, "chapter");
}

static void backbookbtn_clicked(GtkButton*, GtkStack* stack) {
	gtk_stack_set_visible_child_name(stack, "book");
}

static void setup_chapteritem(GtkListItemFactory*, GtkListItem* item) {
	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_size_request(box, 48, 48);
	gtk_widget_add_css_class(box, "card");
	GtkWidget* chapter = gtk_label_new(NULL);
	gtk_widget_set_hexpand(chapter, TRUE);

	gtk_box_append(GTK_BOX(box), chapter);

	gtk_list_item_set_child(item, box);
}
static void bind_chapteritem(GtkListItemFactory*, GtkListItem* item) {
	GtkWidget* box = gtk_list_item_get_child(item);
	ListNumber* num = gtk_list_item_get_item(item);

	GtkWidget* chapter = gtk_widget_get_first_child(box);

	gchar* numstr = g_strdup_printf("%d", list_number_get(num)+1);
	gtk_label_set_text(GTK_LABEL(chapter), numstr);
	g_free(numstr);
}

static GtkWidget* create_chapter_content_view(gchar** verses) {
	GtkWidget* view = gtk_text_view_new();
	gtk_widget_set_hexpand(view, TRUE);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
	gtk_widget_add_css_class(view, "content");
	GtkTextBuffer* buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

	GtkTextIter end;
	for (gsize i = 0; verses[i]; i++) {
		gtk_text_buffer_get_end_iter(buf, &end);
		gchar* verslabel = g_strdup_printf(" <span font_family=\"sans-serif\" font_size=\"8pt\" rise=\"4pt\">%ld </span>", i+1);
		gtk_text_buffer_insert_markup(buf, &end, &verslabel[(gsize)!i], -1);
		g_free(verslabel);
		gtk_text_buffer_get_end_iter(buf, &end);
		gtk_text_buffer_insert(buf, &end, verses[i], -1);
	}

	return view;
}

typedef struct {
	FideiBible** bible;
	gint* booknum;
	AdwLeaflet* leaflet;
	GtkScrolledWindow* content;
} ChapterClickedUd;
static void chapter_clicked(GtkListView*, guint pos, ChapterClickedUd* user_data) {
	gchar** verses = fidei_bible_read_chapter(*user_data->bible, *user_data->booknum, pos);
	GtkWidget* chapter = create_chapter_content_view(verses);
	g_strfreev(verses);

	gtk_scrolled_window_set_child(user_data->content, chapter);
	adw_leaflet_navigate(user_data->leaflet, ADW_NAVIGATION_DIRECTION_FORWARD);
}

typedef struct {
	GtkStack* stack;
	GtkListView* view;
	FideiBible** bible;
} PickerClickedUd;
static void picker_clicked(GtkListBox*, GtkListBoxRow* row, PickerClickedUd* user_data) {
	*user_data->bible = g_object_get_data(G_OBJECT(row), "bible");
	gtk_stack_set_visible_child_name(user_data->stack, "bible");

	GListStore* books = fidei_bible_read_books(*user_data->bible);
	GtkSingleSelection* model = gtk_single_selection_new(G_LIST_MODEL(books));
	gtk_list_view_set_model(user_data->view, GTK_SELECTION_MODEL(model));
}

static GtkWidget* create_picker_view(GListModel* states, GtkStack* view_stack, GtkListView* list_view, FideiBible** active) {
	GtkWidget* view = gtk_scrolled_window_new();
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(view), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	GtkWidget* clamp = adw_clamp_new();
	gtk_widget_set_valign(clamp, GTK_ALIGN_CENTER);
	gtk_widget_set_margin_start(clamp, 12);
	gtk_widget_set_margin_end(clamp, 12);

	GtkWidget* inner = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

	GtkWidget* bi_items = gtk_list_box_new();
	gtk_list_box_set_selection_mode(GTK_LIST_BOX(bi_items), GTK_SELECTION_NONE);
	gtk_widget_add_css_class(bi_items, "boxed-list");
	PickerClickedUd* picker_clicked_ud = g_new(PickerClickedUd, 1); // TODO: free
	picker_clicked_ud->stack = view_stack;
	picker_clicked_ud->view = list_view;
	picker_clicked_ud->bible = active;
	g_signal_connect(bi_items, "row-activated", G_CALLBACK(picker_clicked), picker_clicked_ud);

	/* begin auto */
	for (guint i = 0; i < g_list_model_get_n_items(states); i++) {
		FideiBible* state = FIDEI_BIBLE(g_list_model_get_object(states, i));
		GtkWidget* item = create_picker_item(
			fidei_bible_get_title(state),
			fidei_bible_get_lang(state),
			fidei_bible_get_publisher(state)
		);
		g_object_set_data(G_OBJECT(item), "bible", state);
		gtk_list_box_append(GTK_LIST_BOX(bi_items), item);
	}
	/* end auto */

	GtkWidget* actionrow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

	GtkWidget* download = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(download), "<a href=\"https://bible4u.app/download.html\">Download more</a>");
	gtk_widget_set_hexpand(download, TRUE);
	gtk_label_set_xalign(GTK_LABEL(download), 0.f);
	gtk_widget_set_margin_start(download, 12);

	GtkWidget* folderbtn = gtk_button_new_with_label("Open directory");
	g_signal_connect(folderbtn, "clicked", G_CALLBACK(bible_dir_open), NULL);
	gtk_widget_add_css_class(folderbtn, "suggested-action");

	gtk_box_append(GTK_BOX(actionrow), download);
	gtk_box_append(GTK_BOX(actionrow), folderbtn);

	gtk_box_append(GTK_BOX(inner), bi_items);
	gtk_box_append(GTK_BOX(inner), actionrow);

	adw_clamp_set_child(ADW_CLAMP(clamp), inner);

	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(view), clamp);
	return view;
}

static GtkWidget* create_selector_view(GtkWidget** bookview, FideiBible** active) {
	GtkWidget* leaflet = adw_leaflet_new();

	GtkWidget* selectors = gtk_stack_new();
	gtk_stack_set_transition_type(GTK_STACK(selectors), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);

	GtkWidget* bookscroll = gtk_scrolled_window_new();
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(bookscroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	GtkWidget* bookclamp = adw_clamp_scrollable_new();
	gtk_widget_set_margin_end(bookclamp, 8);

	GtkListItemFactory* bookfactory = gtk_signal_list_item_factory_new();
	g_signal_connect(bookfactory, "setup", G_CALLBACK(setup_bookitem), NULL);
	g_signal_connect(bookfactory, "bind", G_CALLBACK(bind_bookitem), NULL);
	*bookview = gtk_list_view_new(NULL, bookfactory);
	gtk_widget_add_css_class(*bookview, "navigation-sidebar");
	gtk_list_view_set_single_click_activate(GTK_LIST_VIEW(*bookview), TRUE);
	adw_clamp_scrollable_set_child(ADW_CLAMP_SCROLLABLE(bookclamp), *bookview);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(bookscroll), bookclamp);
	gtk_stack_add_named(GTK_STACK(selectors), bookscroll, "book");

	GtkWidget* chapterbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget* backbookbtn = gtk_button_new();
	gtk_button_set_icon_name(GTK_BUTTON(backbookbtn), "go-previous-symbolic");
	gtk_widget_add_css_class(backbookbtn, "pill");
	gtk_widget_set_margin_top(backbookbtn, 8);
	gtk_widget_set_margin_start(backbookbtn, 8);
	gtk_widget_set_margin_end(backbookbtn, 12);
	gtk_widget_set_margin_bottom(backbookbtn, 8);
	g_signal_connect(backbookbtn, "clicked", G_CALLBACK(backbookbtn_clicked), selectors);
	gtk_box_append(GTK_BOX(chapterbox), backbookbtn);

	GtkWidget* chapterboxsep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_margin_end(chapterboxsep, 4);
	gtk_box_append(GTK_BOX(chapterbox), chapterboxsep);

	GtkWidget* chapterscroll = gtk_scrolled_window_new();
	gtk_widget_set_size_request(chapterscroll, 187, -1);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(chapterscroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_widget_add_css_class(chapterscroll, "view");
	gtk_widget_set_vexpand(chapterscroll, TRUE);

	GtkWidget* chapterclamp = adw_clamp_scrollable_new();
	gtk_widget_set_margin_top(chapterclamp, 8);
	gtk_widget_set_margin_start(chapterclamp, 8);
	gtk_widget_set_margin_end(chapterclamp, 12);
	gtk_widget_set_margin_bottom(chapterclamp, 8);

	GtkListItemFactory* chapterfactory = gtk_signal_list_item_factory_new();
	g_signal_connect(chapterfactory, "setup", G_CALLBACK(setup_chapteritem), NULL);
	g_signal_connect(chapterfactory, "bind", G_CALLBACK(bind_chapteritem), NULL);
	GtkWidget* chapterview = gtk_grid_view_new(NULL, chapterfactory);
	gtk_grid_view_set_single_click_activate(GTK_GRID_VIEW(chapterview), TRUE);
	adw_clamp_scrollable_set_child(ADW_CLAMP_SCROLLABLE(chapterclamp), chapterview);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(chapterscroll), chapterclamp);
	gtk_box_append(GTK_BOX(chapterbox), chapterscroll);
	gtk_stack_add_named(GTK_STACK(selectors), chapterbox, "chapter");
	adw_leaflet_append(ADW_LEAFLET(leaflet), selectors);

	GtkWidget* contentscroll = gtk_scrolled_window_new();
	gtk_widget_set_size_request(contentscroll, 384, -1);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(contentscroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	adw_leaflet_append(ADW_LEAFLET(leaflet), contentscroll);

	gint* booknum = g_new(gint, 1); // TODO: free
	*booknum = -1;

	BookClickedUd* book_clicked_ud = g_new(BookClickedUd, 1); // TODO: free
	book_clicked_ud->stack = GTK_STACK(selectors);
	book_clicked_ud->view = GTK_GRID_VIEW(chapterview);
	book_clicked_ud->booknum = booknum;
	g_signal_connect(*bookview, "activate", G_CALLBACK(book_clicked), book_clicked_ud);

	ChapterClickedUd* chapter_clicked_ud = g_new(ChapterClickedUd, 1); // TODO: free
	chapter_clicked_ud->bible = active;
	chapter_clicked_ud->booknum = booknum;
	chapter_clicked_ud->content = GTK_SCROLLED_WINDOW(contentscroll);
	chapter_clicked_ud->leaflet = ADW_LEAFLET(leaflet);
	g_signal_connect(chapterview, "activate", G_CALLBACK(chapter_clicked), chapter_clicked_ud);

	return leaflet;
}

static void activate(GtkApplication* app, GListStore* states) {
	FideiBible** active = g_new(FideiBible*, 1); // TODO: free

	GtkWidget* window = adw_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Fidei");

	GtkWidget* outb = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget* titlebar = adw_header_bar_new();
	gtk_box_append(GTK_BOX(outb), titlebar);

	GtkWidget* stack = gtk_stack_new();
	gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	gtk_widget_set_hexpand(stack, TRUE);
	gtk_widget_set_vexpand(stack, TRUE);

	GtkWidget* bookview;
	GtkWidget* selector_view = create_selector_view(&bookview, active);
	GtkWidget* picker_view = create_picker_view(G_LIST_MODEL(states), GTK_STACK(stack), GTK_LIST_VIEW(bookview), active);

	gtk_stack_add_named(GTK_STACK(stack), picker_view, "picker");
	gtk_stack_add_named(GTK_STACK(stack), selector_view, "bible");

	gtk_box_append(GTK_BOX(outb), stack);

	adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), outb);
	gtk_widget_show(window);
}

static GListStore* create_bible_statemachine() {
	GListStore* states = g_list_store_new(FIDEI_TYPE_BIBLE);

	const gchar* datadir = g_get_user_data_dir();

	GFile* bibledir = g_file_new_build_filename(datadir, "arpa.sp1rit.Fidei", NULL);

	GError* err = NULL;
	GFileEnumerator* files = g_file_enumerate_children(bibledir, "standard::*,owner::user", G_FILE_QUERY_INFO_NONE, NULL, &err);
	if (err) {
		if (err->code == G_IO_ERROR_NOT_FOUND) {
			GError* create_error = NULL;
			if (!g_file_make_directory_with_parents(bibledir, NULL, &create_error)) {
				g_critical("Failure creating directory: %s\n", create_error->message);
				g_error_free(create_error);
				return NULL;
			}
			// create and push empty statemachine
			goto out;
		} else {
			g_critical("Failure opening directory: %s\n", err->message);
			g_error_free(err);
			return NULL;
		}
	}

	GFileInfo* file;
	do {
		GError* err = NULL;
		file = g_file_enumerator_next_file(files, NULL, &err);
		if (err) {
			g_critical("Failure acessing file: %s\n", err->message);
			g_error_free(err);
			continue;
		}
		if (!file)
			break;


		gchar* path = g_build_path("/", g_file_get_path(bibledir), g_file_info_get_name(file), NULL);
		FideiBible* obj = fidei_bible_new(path);
		g_free(path);
		g_list_store_append(states, obj);
		g_object_unref(obj);


		g_object_unref(file);
	} while (TRUE);

out:
	g_object_unref(files);
	g_object_unref(bibledir);

	return states;
}


int main(int argc, char** argv) {
	AdwApplication* app;
	int status;

	GListStore* states = create_bible_statemachine();

	g_resources_register(fidei_resource_get_resource());
	app = adw_application_new("arpa.sp1rit.Fidei", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), states);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	g_object_unref(states);

	return status;
}
