project('fidei', 'c',
	version: '0.1',
	default_options: ['warning_level=3', 'c_std=gnu2x']
)

app_id = 'arpa.sp1rit.Fidei'

i18n = import('i18n')

devenv = environment()

subdir('data')
subdir('src')
subdir('po')

meson.add_devenv(devenv)
