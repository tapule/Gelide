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

#ifndef _COLLECTION_HPP_
#define _COLLECTION_HPP_

#include <glibmm/ustring.h>
#include "item.hpp"


namespace gelide{

/**
 * Mantiene la información de una colección.
 */
struct Collection : public Item
{
	/**
	 * Constructor básico
	 */
	Collection(void):
		Item(),
		enabled(true),
		sort_order(0),
		emulator_id(0)
	{
	}

	/**
	 * Constructor parametrizado
	 * @param p_id Nuevo identificador para el sistema
	 * @param p_name Nombre para el sistema
	 */
	Collection(const long long int p_id, const Glib::ustring& p_name):
		Item(p_id, p_name),
		enabled(true),
		sort_order(0),
		emulator_id(0)
	{
	}

	bool enabled;				/**< Indica si está activado o no */
	int sort_order;				/**< Orden ocupado en la lista de sistemas */

	Glib::ustring manufacturer;	/**< Fabricante de la colección */
	Glib::ustring year;			/**< Año de lanzamiento */

	Glib::ustring Icon16;		/**< Path al icono de 16x16 */
	Glib::ustring Icon32;		/**< Path al icono de 32x32 */

	long long int emulator_id;	/**< Identificador del emulador */
};

} // namespace gelide

#endif // _COLLECTION_HPP_
