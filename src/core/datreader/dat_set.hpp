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

#ifndef _DAT_SET_HPP_
#define _DAT_SET_HPP_

#include <glibmm/ustring.h>


namespace gelide{

/**
 * Mantiene la información de un set de un fichero dat.
 */
struct DatSet
{
	/**
	 * Constructor básico
	 */
	DatSet(void):
		is_bios(false),
		players(1)
	{
	}

	/**
	 * Constructor parametrizado
	 * @param p_name Nombre de set
	 * @param p_description Descripción o título del set
	 */
	DatSet(const Glib::ustring& p_name, const Glib::ustring& p_description):
		name(p_name),
		description(p_description),
		is_bios(false),
		players(1)
	{
	}

	Glib::ustring name;			/**< Nombre del set */
	Glib::ustring description;	/**< Descripción o título del set */
	Glib::ustring clone_of;		/**< Nombre del set padre */
	bool is_bios;				/**< Indica si el set es un set bios */
	Glib::ustring crc;			/**< CRC asignado al set (de su rom) */
	Glib::ustring manufacturer;	/**< Fabricante del set */
	Glib::ustring year;			/**< Año de lanzamiento */
	Glib::ustring genre;		/**< Género del juego que contiene el set */
	int players;				/**< Número de jugadores soportados por el juego */
};

} // namespace gelide

#endif // _DAT_SET_HPP_
