# Fidei - Take back your faith
# Copyright (C) 2022 Florian "sp1rit" <sp1rit@national.shitposting.agency>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

add_project_arguments(['-Wno-overlength-strings'], language: 'c')

src = [
	'app.c',
	'fidei.c',
	'bible.c',
	'about.c',
	'bibleinfo_diag.c',
	'bibleinfo_row.c',
	'bible_item.c',
	'num.c',
	'preferences.c',
	'utils.c'
]
deps = []

if get_option('sandboxed')
	openurl = gnome.gdbus_codegen('xdg_openurl', get_option('xdp_openuri_location'),
		interface_prefix: 'org.freedesktop.portal.',
		namespace: 'XdgDesktop',
		object_manager: true
	)

	src += openurl
	deps += dependency('gio-unix-2.0')

	add_project_arguments(['-DFIDEI_USE_PORTALS'], language: 'c')
endif

libicu = dependency('icu-uc', required: get_option('libicu'))
if libicu.found()
	add_project_arguments(['-DHAS_LIBICU'], language: 'c')
endif

fidei = executable('fidei', src, fidei_res,
	c_args: [
		'-DFIDEI_VERSION="@0@"'.format(meson.project_version()),
		'-DGETTEXT_PACKAGE="@0@"'.format(app_id),
		'-DPACKAGE_LOCALE_DIR="@0@"'.format(join_paths(get_option('prefix'), get_option('localedir'))),
	],
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
