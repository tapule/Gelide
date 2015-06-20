## Process this file with automake to produce Makefile.in

#ACLOCAL_AMFLAGS = -I m4

# Indicamos los subdirectorios a procesar
SUBDIRS = \
	data \
	doc \
	help \
	pixmap \
	po \
	src \
	ui 

# Lugar de instalación de la documentación estandar
gelidedocdir = ${prefix}/share/doc/gelide
# Archivos de documentación estandar a instalar
gelidedoc_DATA = \
	AUTHORS \
	ChangeLog \
	COPYING \
	INSTALL \
	NEWS \
	README

# Incluir los archivos de datos estandar en la distribución
EXTRA_DIST = \
	$(gelidedoc_DATA) \
	gnome-doc-utils.make \
	po/gelide.pot \
	m4

DISCLEAN = \
	gnome-doc-utils.make

# Limpieza de ficheros con "make maintainerclean"
MAINTAINERCLEANFILES = \
	aclocal.m4 \
	config.guess \
	config.h \
	config.h.in \
	config.log \
	config.status \
	config.sub \
	configure \
	depcomp \
	gelide*.tar.gz \
	gnome-doc-utils.make \
	install-sh \
	libtool \
	ltmain.sh \
	Makefile \
	Makefile.in \
	missing \
	mkinstalldirs \
	stamp-h1 \
	m4/*.m4


#DISTCHECK_CONFIGURE_FLAGS = --disable-scrollkeeper --enable-gtk-doc
DISTCHECK_CONFIGURE_FLAGS = --disable-scrollkeeper

