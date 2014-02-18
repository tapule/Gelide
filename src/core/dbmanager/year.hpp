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

#ifndef _YEAR_HPP_
#define _YEAR_HPP_

#include <glibmm/ustring.h>
#include "item.hpp"


namespace gelide{

/**
 * Mantiene la información de un año para los juegos.
 */
struct Year : public Item
{
	/**
	 * Constructor básico
	 */
	Year(void):
		Item()
	{
	}

	/**
	 * Constructor parametrizado
	 * @param p_id Nuevo identificador para el elemento
	 * @param p_name Nombre para el elemento
	 */
	Year(const long long int p_id, const Glib::ustring& p_name):
		Item(p_id, p_name)
	{
	}
};

} // namespace gelide

#endif // _YEAR_HPP_
