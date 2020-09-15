project('gtapecalc_port', 'c',
	version: '0.1.0',
	meson_version: '>= 0.50.0',
	default_options: ['warning_level=2', 'c_std=c18']
)

config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
configure_file(
  output: 'gtapecalc_port-config.h',
  configuration: config_h,
)
add_project_arguments([
  '-I' + meson.build_root(),
], language: 'c')

subdir('src')
