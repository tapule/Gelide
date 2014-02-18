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

#ifndef _ITEM_HPP_
#define _ITEM_HPP_

#include <glibmm/ustring.h>


namespace gelide{

/**
 * Estructura base para los diferentes objetos del sistema. Encapsula de forma
 * genérica los datos estandar de diferentes objetos.
 *
 * Estos objetos son los manejados por el DbManager (colecciones, listas,
 * juegos, etc).
 * La idea detrás de esta estructura, es facilitar el paso de datos entre el
 * DBManager y los diferentes consumidores de datos, así como sevir de base
 * para los diferentes tipos de objetos manejados.
 */
struct Item
{
	/**
	 * Constructor básico
	 */
	Item(void):
		id(0)
	{
	}

	Item(const long long int p_id, const Glib::ustring& p_name, const Glib::ustring& p_title):
		id(p_id),
		name(p_name),
		title(p_title)
	{
	}

	Item(const long long int p_id, const Glib::ustring& p_name):
		id(p_id),
		name(p_name),
		title(p_name)
	{
	}

	long long int id;			/**< Identificador del elemento */
	Glib::ustring name;		/**< Nombre del elemento */
	Glib::ustring title;		/**< Título del elemento */
};

} // namespace gelide

#endif // _ITEM_HPP_
