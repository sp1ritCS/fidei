fidei = executable('fidei', [
		'app.c',
		'bible.c',
	], fidei_res,
	dependencies: [
		dependency('gtk4'),
		dependency('libadwaita-1'),
		dependency('libxml-2.0')
	],
	install : true
)

test('basic', fidei)
