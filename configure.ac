# Process this file with autoconf to produce a configure script.

# Inicializamos autoconf
AC_INIT(gelide, 0.1.5, gelide.prj@gmail.com)

# Inicializamos automake e indicamos que se cree config.h con autoheader
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

# Documentación de gnome en help
AC_CONFIG_MACRO_DIR([m4])
GNOME_DOC_INIT


# Comprobaciones varias
AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC


# Generamos un define en config.h con el nombre de la release
#PACKAGE_CODENAME=
#AC_SUBST(PACKAGE_CODENAME)
#AC_DEFINE_UNQUOTED(PACKAGE_CODENAME,"$PACKAGE_CODENAME", [Gelide codename])

# Generamos un define en config.h con la web del programa
PACKAGE_WEBSITE=http://gelide.sourceforge.net
AC_SUBST(PACKAGE_WEBSITE)
AC_DEFINE_UNQUOTED(PACKAGE_WEBSITE,"$PACKAGE_WEBSITE", [Gelide website])

# Internacionalización
GETTEXT_PACKAGE=gelide
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])
AM_PROG_LIBTOOL


# Comprobamos las librerías necesarias y exportamos los flags oportunos
PKG_CHECK_MODULES(GELIDE, [gtkmm-2.4 >= 2.8 libxml-2.0 giomm-2.4])
AC_SUBST(GELIDE_CFLAGS)
AC_SUBST(GELIDE_LIBS)


# Comprobamos parámetros
#    Activar modo debug --enable-debug
AC_ARG_ENABLE(debug, [AC_HELP_STRING([--enable-debug],[build with debug support [default=no]])],,[enable_debug=no])
if test "x$enable_debug" = "xyes"; then
	AC_DEFINE(ENABLE_DEBUG_MODE,1, [Activate debug mode])
fi

#    Activar modo de carga progresivo --enable-progresive
#AC_ARG_ENABLE(progresive, [AC_HELP_STRING([--enable-progresive],[build with progresive game lists support [default=yes]])],,[enable_progresive=yes])
#if test $enable_progresive; then
#	AC_DEFINE(PROGRESIVE_MODE,1, [Enable progresive mode])
#fi

# Indicamos los ficheros que debe generar autoconf a partir de ficheros in
AC_CONFIG_FILES([
	Makefile
	data/Makefile
	doc/Makefile
	help/Makefile
	pixmap/Makefile
	po/Makefile.in
	src/Makefile
	ui/Makefile
	ui/icons/Makefile
])

AC_OUTPUT

# Mensaje de configuración

echo ""
echo "======================================================================"
echo ""
echo "$PACKAGE_NAME configuration:"
echo "  C compiler              : $CC"
echo "  C++ compiler            : $CXX"
echo "  Prefix                  : $prefix"
echo ""
echo "$PACKAGE_NAME features:"
echo "  Debug support           : $enable_debug"
#echo "  Progresive game list    : $enable_progresive"
echo ""
echo "The following options will be passed to the compiler:"
echo "  CFLAGS: $CFLAGS"
echo "  CXXFLAGS: $CXXFLAGS"
echo ""
echo "  GELIDE_CFLAGS: $GELIDE_CFLAGS"
echo ""
echo "  GELIDE_LIBS: $GELIDE_LIBS"
echo ""
echo "======================================================================"
echo ""

