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

#ifndef _EMULATOR_HPP_
#define _EMULATOR_HPP_

#include <glibmm/ustring.h>


namespace gelide{

enum EmulatorParentType{
	EMULATOR_PARENT_COLLECTION = 0,	/**< Tipo de padre colección */
	EMULATOR_PARENT_GAMELIST,		/**< Tipo de padre lista */
	EMULATOR_PARENT_GAME			/**< Tipo de padre juego */
};

/**
 * Encapsula las propiedades de un emulador
 */
struct Emulator
{
	/**
	 * Constructor básico
	 */
	Emulator(void):
		id(0),
		parent_id(0),
		parent_type(EMULATOR_PARENT_COLLECTION)
	{
	}

	long long int id;					/**< Identificador del emulador */
	long long int parent_id;			/**< Identificador del padre del emu */
	EmulatorParentType parent_type;		/**< Tipo de padre del emulador */
	Glib::ustring binary;				/**< Path del ejecutable del emulador */
	Glib::ustring params;				/**< Parámetros para la llamada al emulador */
	Glib::ustring precommand;			/**< Comandos previos a la emulación */
	Glib::ustring postcommad;			/**< Comandos posteriores a la emulación */
	Glib::ustring rundir;				/**< Directorio donde ejecutar la emulación */
};

} // namespace gelide

#endif // _EMULATOR_HPP_
