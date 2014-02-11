## File created by the gnome-build tools

# Indicamos los subdirectorios a procesar
SUBDIRS = \
	icontheme

# Lugar de instalación de la configuración por defecto
gelide_cfgdir = $(datadir)/gelide
gelide_cfg_DATA = \
	gelide.xml

# Reglas para los ficheros .desktop
desktopdir = $(datadir)/applications
desktop_in_files = \
	gelide.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

# Incluimos los ficheros de datos por defecto y los .dektop in en la distribución
EXTRA_DIST = \
	$(gelide_cfg_DATA) \
	$(desktop_in_files)

# Indicamos que se deben eliminar los ficheros de datos con clean
CLEANFILES = \
	$(desktop_DATA)

# Limpieza de ficheros con "make maintainerclean"
MAINTAINERCLEANFILES = \
	Makefile \
	Makefile.in

