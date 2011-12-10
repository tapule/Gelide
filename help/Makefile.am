## Process this file with automake to produce Makefile.in

# Incluimos las utilidades de gnome-doc
include $(top_srcdir)/gnome-doc-utils.make
dist-hook: doc-dist-hook

# Nombre del modulo de documentación
DOC_MODULE = gelide
DOC_ENTITIES = legal.xml
DOC_INCLUDES = 
# Listado de figuras incluidas a partir del subdirectorio C
DOC_FIGURES = \
             figures/imag0.png
DOC_LINGUAS =

# Páginas del manual
man_MANS = gelide.1

EXTRA_DIST = \
	$(man_MANS)

# Limpieza de ficheros con "make maintainerclean"
MAINTAINERCLEANFILES = \
	Makefile \
	Makefile.in
