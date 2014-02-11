## File created by the gnome-build tools

# Indicamos los subdirectorios a procesar
SUBDIRS = \
	systems

# Reglas para el icono del programa
gelide_icondir = $(datadir)/icons/hicolor/scalable/apps
gelide_icon_DATA = gelide.svg

# Tema de iconos para el configurator y el theme-editor
iconthemedir = $(datadir)/gelide/icontheme
icontheme_DATA = \
	gelide-all.png

# Incluimos los ficheros en la distribución
EXTRA_DIST = \
	$(gelide_icon_DATA) \
	$(icontheme_DATA)

# Limpieza de ficheros con "make maintainerclean"
MAINTAINERCLEANFILES = \
	Makefile \
	Makefile.in

