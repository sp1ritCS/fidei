add_project_arguments(['-Wno-overlength-strings'], language: 'c')

fidei = executable('fidei', [
		'app.c',
		'fidei.c',
		'bible.c',
		'num.c',
		'preferences.c'
	], fidei_res,
	c_args: ['-DFIDEI_VERSION="@0@"'.format(meson.project_version())],
	dependencies: [
		dependency('gtk4'),
		dependency('libadwaita-1'),
		dependency('libxml-2.0'),
		dependency('icu-uc')
	],
	export_dynamic: true,
	install : true
)

test('basic', fidei)
