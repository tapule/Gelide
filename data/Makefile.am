## File created by the gnome-build tools

# Reglas para el icono del programa
gelide_icondir = $(datadir)/icons/hicolor/scalable/apps
gelide_icon_DATA = gelide.svg

# Lugar de instalación de la configuración por defecto
gelide_cfgdir = $(datadir)/gelide
gelide_cfg_DATA = \
	defaultcfg.xml

# Reglas para el fichero .desktop
desktopdir = $(datadir)/applications
desktop_in_files = gelide.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

# Incluimos el icono y los ficheros in en la distribución
EXTRA_DIST = \
	$(gelide_icon_DATA) \
	$(gelide_cfg_DATA) \
	$(desktop_in_files)

# Indicamos que se deben eliminar los ficheros de datos con clean
CLEANFILES = \
	$(desktop_DATA)

# Limpieza de ficheros con "make maintainerclean"
MAINTAINERCLEANFILES = \
	Makefile \
	Makefile.in
