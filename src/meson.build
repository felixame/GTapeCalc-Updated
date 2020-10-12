gnome = import('gnome')

gtk_dependency = dependency('gtk+-3.0', version : '>=3.22')
gio_dependency = dependency('gio-2.0', version: '>= 2.50')

sources = files(
	'gtc_main.c',
	'gtc_application.c',
	'gtc_calc.c',
	'gtc_callbacks.c',
	'gtc_prefs.c',
	'gtc_print.c',
	'gtc_ui.c',
	'gtc_utils.c',
	'gtc_window.c'
	)

sources += gnome.compile_resources(
	'gtc_resources',
  	join_paths(
	'resources', 'gtc.gresource.xml'
	),
	source_dir: 'resources',
	c_name: 'gtapecalc'
)


executable('gtapecalc', sources, dependencies : [gtk_dependency, gio_dependency])
