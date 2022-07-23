gnome = import('gnome')
fidei_res = gnome.compile_resources('fidei_res', 'fidei.gresource.xml',
	c_name: 'fidei_resource'
)

install_data('arpa.sp1rit.Fidei.gschema.xml', install_dir: get_option('datadir') / 'glib-2.0' / 'schemas')

gnome.compile_schemas(build_by_default: true, depend_files: 'arpa.sp1rit.Fidei.gschema.xml')
devenv.set('GSETTINGS_SCHEMA_DIR', meson.current_build_dir())

gnome.post_install(
	glib_compile_schemas: true,
#	update_desktop_database: true
)
