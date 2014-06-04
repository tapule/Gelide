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

#ifndef _FILTER_HPP_
#define _FILTER_HPP_


namespace gelide{

// Posibles filtros para la BD
enum FilterType{
	FILTER_NONE = 0,		/**< Tipo de filtro ninguno */
	FILTER_FAVORITE,		/**< Tipo de filtro por favorito */
	FILTER_GENRE,			/**< Tipo de filtro por género */
	FILTER_LETTER,			/**< Tipo de filtro por letra inicial */
	FILTER_MANUFACTURER,	/**< Tipo de filtro por fabricante */
	FILTER_PLAYERS,			/**< Tipo de filtro por núemro de juegadores */
	FILTER_RATING,			/**< Tipo de filtro por puntuación */
	FILTER_STATE,			/**< Tipo de filtro por estado */
	FILTER_TAG,				/**< Tipo de filtro por etiqueta */
	FILTER_TIMES_PLAYED,	/**< Tipo de filtro por partidas jugadas */
	FILTER_GAME_TYPE,		/**< Tipo de filtro por tipo de juego (original, clon, bios) */
	FILTER_YEAR				/**< Tipo de filtro por año */
};

/**
 * Mantiene la información de un filtro y su valor.
 */
struct Filter
{
	/**
	 * Constructor básico
	 */
	Filter(void):
		type(FILTER_NONE),
		value(0)
	{
	}

	/**
	 * Constructor parametrizado
	 * @param p_type Nuevo tipo de filtro
	 * @param p_value Valor para el filtro
	 */
	Filter(const FilterType p_type, const long long int p_value):
		type(p_type),
		value(p_value)
	{
	}

	FilterType type;		/**< Tipo de filtro */
	long long int value;	/**< Valor del filtro */
};

} // namespace gelide

#endif // _GAME_FILTER_HPP_
