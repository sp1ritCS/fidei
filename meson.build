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

project('fidei', 'c',
	version: '0.1',
	default_options: ['warning_level=3', 'c_std=gnu2x']
)

app_id = 'arpa.sp1rit.Fidei'

i18n = import('i18n')

gtk_dep = dependency('gtk4', version: '>= 4.10.0')
adw_dep = dependency('libadwaita-1', version: '>= 1.2.0')
xml_dep = dependency('libxml-2.0')
icu_dep = dependency('icu-uc', required: get_option('libicu'))

devenv = environment()

subdir('data')
subdir('src')
subdir('po')

install_subdir(app_id, install_dir: get_option('datadir'))

meson.add_devenv(devenv)
