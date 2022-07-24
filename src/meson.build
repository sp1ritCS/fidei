add_project_arguments(['-Wno-overlength-strings'], language: 'c')

libicu = dependency('icu-uc', required: get_option('libicu'))
if libicu.found()
	add_project_arguments(['-DHAS_LIBICU'], language: 'c')
endif

fidei = executable('fidei', [
		'app.c',
		'fidei.c',
		'bible.c',
		'num.c',
		'preferences.c',
		'utils.c'
	], fidei_res,
	c_args: ['-DFIDEI_VERSION="@0@"'.format(meson.project_version())],
	dependencies: [
		dependency('gtk4'),
		dependency('libadwaita-1'),
		dependency('libxml-2.0'),
		libicu
	],
	export_dynamic: true,
	install : true
)

test('basic', fidei)
