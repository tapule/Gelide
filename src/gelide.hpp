/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gelide
 * Copyright (C) 2008 - 2011 Juan Ángel Moreno Fernández
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

#ifndef _GELIDE_HPP_
#define _GELIDE_HPP_

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gelide_i18n.hpp"

//#define LOCAL_MODE
//#define ENABLE_DEBUG_MODE


#ifndef HAVE_CONFIG_H
	#define PACKAGE_NAME			"Gelide"
	#define PACKAGE_VERSION			"0.1.5"
	#define PACKAGE_CODENAME		 "\"\""
	#define PACKAGE_WEBSITE			"http://gelide.sourceforge.net"
#endif

#ifdef ENABLE_DEBUG_MODE
	#define GELIDE_DEBUG(str) std::cout << "Debug** " << str << std::endl
	#define GELIDE_WARNING(str) std::cout << "Warning** " << __FILE__ << "[" << __LINE__ << "]: " << str << std::endl
#else
	#define GELIDE_DEBUG(str)
	#define GELIDE_WARNING(str)
	#define NDEBUG
#endif

#define GELIDE_ERROR(str) std::cerr << "Error** " << __FILE__ << "[" << __LINE__ << "]: " << str << std::endl; exit(1)

#include <cassert>

#ifndef LOCAL_MODE
	#define GELIDE_ICON				PACKAGE_DATA_DIR"/icons/hicolor/scalable/apps/gelide.svg"
	#define GELIDE_PIXMAPS			PACKAGE_DATA_DIR"/gelide/pixmaps/"
	#define GELIDE_UI_ICONS			PACKAGE_DATA_DIR"/gelide/ui/icons/"
	#define GELIDE_CFG_DEFAULT		PACKAGE_DATA_DIR"/gelide/defaultcfg.xml"
	#define GELIDE_DOCS				PACKAGE_DOC_DIR"/gelide/"
#else
	#define GELIDE_ICON				"../data/gelide.svg"
	#define GELIDE_PIXMAPS			"../pixmap/"
	#define GELIDE_UI_ICONS			"../ui/icons/"
	#define GELIDE_CFG_DEFAULT		"../data/defaultcfg.xml"
	#define GELIDE_DOCS				"../"
#endif


#define GELIDE_DIR					".gelide"
#define GELIDE_CHANGELOG			GELIDE_DOCS"ChangeLog"
#define USER_SYSTEMS_LIST			"systems.gsl"

// Fichero de configuración y grupos en el fichero de configuración
#define GELIDE_CFG_FILE				"gelidecfg.xml"
#define GELIDE_CFG_DIALOGS			"dialogs"
#define GELIDE_CFG_PREFERENCES		"preferences"
#define GELIDE_CFG_UI				"ui"

#define SYSTEMS_IMAGES				"images"
#define SYSTEMS_IMAGES_SNAP			SYSTEMS_IMAGES"/snap"
#define SYSTEMS_IMAGES_TITLE		SYSTEMS_IMAGES"/title"
#define SYSTEMS_IMAGES_FRONT		SYSTEMS_IMAGES"/front"
#define SYSTEMS_IMAGES_BACK			SYSTEMS_IMAGES"/back"
#define SYSTEMS_IMAGES_MEDIA		SYSTEMS_IMAGES"/media"
#define SYSTEMS_ROMS				"roms"
#define SYSTEMS_GAMES_LIST			"games.ggl"
#define SYSTEMS_EMULATORS_LIST		"emulators.gel"
#define SYSTEMS_EXPORT_ICONS_DIR	"icons"
#define SYSTEMS_EXPORT_DATS_DIR		"dats"


#define GELIDE_VERSION				PACKAGE_VERSION
#define GELIDE_WEBSITE				PACKAGE_WEBSITE
#define GELIDE_WIDTH				1500
#define GELIDE_HEIGHT				800

#define FILTER_TYPE_NOFILTER		-1
#define FILTER_TYPE_ALL				0
#define FILTER_TYPE_FAVORITE		1
#define FILTER_TYPE_PLAYED			2
#define FILTER_TYPE_WORKING			3
#define FILTER_TYPE_AVAILABLE		4
#define FILTER_TYPE_UNKNOWN			5
#define FILTER_TYPE_RANK			6
#define FILTER_TYPE_YEAR			7

#define CSV_SEPARATOR				"\x3B"	// ";"

#define PARAMS_TAG_SIZE				3
#define PARAMS_TAG_GAME_NAME		"$gn"		// Nombre del set
#define PARAMS_TAG_GAME_FILE		"$gf"		// Path + fichero del juego
#define PARAMS_TAG_GAME_PATH		"$gp"		// Path del juego
#define PARAMS_TAG_BIOS_FILE		"$bf"		// Path + fichero bios
#define PARAMS_TAG_BIOS_PATH		"$bp"		// Path del fichero bios
#define PARAMS_TAG_EMULATOR_FILE	"$ef"		// Path + binario del emulador
#define PARAMS_TAG_EMULATOR_PATH	"$ep"		// Path del binario del emulador
/* ESTA ETIQUETA SE ELIMINARÁ EN LA PRÓXIMA VERSIÓN */
#define PARAMS_TAG_ROMS_PATH		"$rp"



#endif // _GELIDE_HPP_
