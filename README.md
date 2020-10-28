# GTapeCalc Updated

A fork of http://gtapecalc.sourceforge.net/, licensed under GPLv3.

Fixed a couple issues that kept GTapeCalc from being compiled on modern distros and brought it over to the Meson build system.

### Building

The project can be built by runing:

`meson bin && meson compile -C bin`

From there it can be run from `bin/src/gtapecalc`.

A proper installation script will be provided in the future.
