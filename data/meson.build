gnome = import('gnome')
fidei_res = gnome.compile_resources('fidei_res', 'fidei.gresource.xml',
	c_name: 'fidei_resource'
)

install_data('@0@.svg'.format(app_id), install_dir: get_option('datadir') / 'icons'  / 'hicolor'  / 'scalable' / 'apps')
install_data('@0@.gschema.xml'.format(app_id), install_dir: get_option('datadir') / 'glib-2.0' / 'schemas')

if find_program('xgettext', required: get_option('gettext')).found()
	i18n.merge_file(
		input: '@0@.desktop'.format(app_id),
		output: 'arpa.sp1rit.Fidei.desktop',
		type: 'desktop',
		po_dir: meson.source_root() / 'po',
		install: true,
		install_dir: get_option('datadir') / 'applications'
	)
	i18n.merge_file(
		input: '@0@.metainfo.xml'.format(app_id),
		output: 'arpa.sp1rit.Fidei.metainfo.xml',
		type: 'xml',
		po_dir: meson.source_root() / 'po',
		install: true,
		install_dir: get_option('datadir') / 'metainfo'
	)
else
install_data('@0@.desktop'.format(app_id), install_dir: get_option('datadir') / 'applications')
install_data('@0@.metainfo.xml'.format(app_id), install_dir: get_option('datadir') / 'metainfo')
endif

gnome.compile_schemas(build_by_default: true, depend_files: 'arpa.sp1rit.Fidei.gschema.xml')
devenv.set('GSETTINGS_SCHEMA_DIR', meson.current_build_dir())

gnome.post_install(
	glib_compile_schemas: true,
	gtk_update_icon_cache: true,
	update_desktop_database: true
)
