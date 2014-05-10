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

#ifndef _META_DB_SET_HPP_
#define _META_DB_SET_HPP_

#include <glibmm/ustring.h>


namespace gelide{

// Posibles tipos de set
enum SetType{
	SET_TYPE_UNKNOWN = 0,		/**< Tipo desconocido */
	SET_TYPE_ORIGINAL,			/**< Juego original */
	SET_TYPE_CLONE,				/**< Juego clon */
	SET_TYPE_BIOS				/**< Bios */
};

/**
 * Mantiene la información de un set de la bd de metadatos.
 */
struct MetaDbSet
{
	/**
	 * Constructor básico
	 */
	MetaDbSet(void):
		id(0),
		type(SET_TYPE_UNKNOWN),
		players(1)
	{
	}

	/**
	 * Constructor parametrizado
	 * @param p_name Nombre de set
	 * @param p_description Descripción o título del set
	 */
	MetaDbSet(const long long int p_id, const Glib::ustring& p_name, const Glib::ustring& p_title):
		id(p_id),
		name(p_name),
		type(SET_TYPE_UNKNOWN),
		title(p_title),
		players(1)
	{
	}

	long long int id;			/**< Identificador del elemento */
	Glib::ustring name;			/**< Nombre del set */
	Glib::ustring system;		/**< Nombre del sistema al que pertenece */

	SetType type;				/**< Indica el tipo del set (original, clon, bios) */
	Glib::ustring hash;			/**< Hash asignado al set */

	Glib::ustring title;		/**< Título del set */
	Glib::ustring manufacturer;	/**< Fabricante del set */
	Glib::ustring year;			/**< Año de lanzamiento */
	Glib::ustring genre;		/**< Género del juego que contiene el set */
	int players;				/**< Número de jugadores soportados por el juego */
};

} // namespace gelide

#endif // _META_DB_SET_HPP_
