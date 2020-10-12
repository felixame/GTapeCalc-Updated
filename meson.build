project('gtapecalcmod', 'c',
	version: '0.1.0',
	meson_version: '>= 0.50.0',
	default_options: [ 'warning_level=2',
			   'c_std=gnu11'
			 ]
)

add_project_arguments([
	'-I' + meson.build_root(),
], language: 'c')

subdir('src')

configure_file(output : 'config.h',
	configuration : {
	# Application info
	'APPLICATION_ID' : '"com.github.felixame.gtapecalcmod"',
	'APPLICATION_NAME' : '"GTapeCalc"',
	'APPLICATION_VERSION' : '"3.0.0"',

	# External resource locations
	'PIXMAP_DIR' : '"/usr/local/share/gtapecalc/pixmaps/"',
	'HELP_DIR' : '"/usr/local/share/gtapecalc/doc/help/"'
	})

