/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gelide
 * Copyright (C) 2008 - 2014 Juan Ángel Moreno Fernández
 *
 * gelide is free software.
 *
 * You can redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * gelide is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gelide.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

#ifdef HAVE_CONFIG_H
	#include <config.h>
#else
	// Defines por defecto
	#define ENABLE_DEBUG_MODE
	#define PACKAGE_NAME			"Gelide"
	#define PACKAGE_VERSION			"0.2.0"
	#define PACKAGE_CODENAME		 "\"Theoden\""
	#define PACKAGE_WEBSITE			"http://gelide.sf.net"
	#define PACKAGE_DATA_DIR		"."
#endif

// CHECKME: Este defines solo se utiliza en wip, para release hay que quitarlo
// Activa el modo de ejecución local.
// El modo de ejecución local implica que todo el procesamiento se realizará
// desde el directorio donde reside el binario.
#define ENABLE_LOCAL_MODE

// En win siempre se usará el modo local
#ifdef OS_WIN
	#define ENABLE_LOCAL_MODE
#endif

#ifndef ENABLE_DEBUG_MODE
	#define NDEBUG
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "utils/i18n.hpp"
#include "utils/os_detect.hpp"
#include "utils/debug.hpp"
#include "utils/log.hpp"


// Directorios de trabajo en función del tipo de ejecución. La versión win
// siempre se compilará con modo local activado.
#ifdef ENABLE_LOCAL_MODE
	#ifdef OS_POSIX
		#define GELIDE_DATA_DIR				"data"
		#define GELIDE_ICON_DIR				GELIDE_DATA_DIR"/icontheme"
		#define GELIDE_ICONTHEME_DIR		GELIDE_DATA_DIR"/icontheme"
		#define GELIDE_COLLECTIONS_ICONS_DIR	GELIDE_DATA_DIR"/icontheme/collections"
		#define GELIDE_CHANGELOG_DIR		"."
		#define GELIDE_HELP_DIR				"help"
	#else
		#define GELIDE_DATA_DIR				"data"
		#define GELIDE_ICON_DIR				GELIDE_DATA_DIR"\\icontheme"
		#define GELIDE_ICONTHEME_DIR		GELIDE_DATA_DIR"\\icontheme"
		#define GELIDE_COLLECTIONS_ICONS_DIR	GELIDE_DATA_DIR"\\icontheme\\collections"
		#define GELIDE_CHANGELOG_DIR		"."
		#define GELIDE_HELP_DIR				"help"
	#endif
#else
	#define GELIDE_DATA_DIR					PACKAGE_DATA_DIR"/gelide"
	#define GELIDE_ICON_DIR					PACKAGE_DATA_DIR"/icons/hicolor/scalable/apps"
	#define GELIDE_ICONTHEME_DIR			GELIDE_DATA_DIR"/icontheme"
	#define GELIDE_COLLECTIONS_ICONS_DIR	GELIDE_DATA_DIR"/icontheme/collections"
	#define GELIDE_CHANGELOG_DIR			GELIDE_DATA_DIR
	#define GELIDE_HELP						"help" //?????
#endif

// Estructura de directorios para datos del usuario
// Toda la estructura se montará a partir del directory de trabajo del usuario
#define USER_LIBRARY_DIR				"library"
#define USER_COLLECTIONS_DIR			"collections"	// library/collections

// Estructura de directorios para las colecciones
// Toda la estructura se montará a partir del directorio de la colección
#define COLLECTION_ROMS_DIR				"roms"

// Directorio de trabajo por defecto
#define GELIDE_DEFAULT_DIR				".gelidewip"

// Ficheros manejados por Gelide
#ifdef OS_POSIX
	#define GELIDE_ICON_FILE			"gelide.svg"
#else
	#define GELIDE_ICON_FILE			"gelide.png"
#endif
#define GELIDE_DB_FILE					"gelide.sqlite"
#define GELIDE_CFG_FILE					"gelide.xml"
#define GELIDE_LOG_FILE					"gelide.log"
#define GELIDE_LASTRUN_LOG_FILE			"lastrun.log"
#define GELIDE_CHANGELOG_FILE			"ChangeLog"

// Ficheros manejados por el metadbmanager
#define METADBM_DB_FILE					"metadb.sqlite"
#define METADBM_LOG_FILE				"metadbm.log"

// Root, versión y grupos para el fichero de configuración de Gelide
#define GELIDE_CFG_ROOT					"gelidecfg"
#define GELIDE_CFG_VERSION				"0.2"
#define GELIDE_CFG_GLOBAL				"global"
#define GELIDE_CFG_UI					"ui"
#define GELIDE_CFG_DIALOGS				"dialogs"


#endif // _DEFINES_HPP_
