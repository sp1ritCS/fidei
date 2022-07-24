if find_program('xgettext', required: get_option('gettext')).found()
	i18n.gettext(app_id, args: ['--keyword=_t'], preset: 'glib')
	devenv.set('FIDEI_DEBUG_LOCALEDIR', meson.current_build_dir())
endif
