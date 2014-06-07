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

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <glibmm/ustring.h>
#include "item.hpp"


namespace gelide{

// Posibles tipos de juegos
enum GameType
{
	GAME_TYPE_UNKNOWN = 0,		/**< Tipo desconocido */
	GAME_TYPE_ORIGINAL,			/**< Juego original */
	GAME_TYPE_CLONE,			/**< Juego clon */
	GAME_TYPE_BIOS				/**< Bios */
};

// Posibles estados de un juego
enum GameState
{
	GAME_STATE_UNKNOWN = 0,		/**< Estado desconocido (no comprobado) */
	GAME_STATE_CORRECT,			/**< Estado correcto (comprobado y localizado) */
	GAME_STATE_INCORRECT		/**< Estado incorrecto (comprobado y no localizado) */
};

/**
 * Mantiene la información de un juego.
 */
struct Game : public Item
{
	/**
	 * Constructor básico
	 */
	Game(void):
		Item(),
		collection_id(0),
		state(GAME_STATE_UNKNOWN),
		type(GAME_TYPE_UNKNOWN),
		manufacturer_id(1),
		year_id(1),
		genre_id(1),
		players(1),
		rating(0),
		times_played(0),
		favorite(false),
		use_custom_emulator(0),
		emulator_id(0)
	{
	}

	/**
	 * Constructor parametrizado
	 * @param p_id Nuevo identificador para el elemento
	 * @param p_name Nombre para el elemento
	 */
	Game(const long long int p_id, const Glib::ustring& p_name, const Glib::ustring& p_title):
		Item(p_id, p_name, p_title),
		collection_id(0),
		state(GAME_STATE_UNKNOWN),
		type(GAME_TYPE_UNKNOWN),
		manufacturer_id(1),
		year_id(1),
		genre_id(1),
		players(1),
		rating(0),
		times_played(0),
		favorite(false),
		use_custom_emulator(0),
		emulator_id(0)
	{
	}

	long long int collection_id;	/**< Identificador de la colección a la que pertenece*/
	GameState state;				/**< Estado del juego (0 desconocido, 1 lcorrecto, 2 incorrecto) */
	Glib::ustring file;				/**< Path al fichero del juego */

	GameType type;					/**< Indica el tipo del set (original, clon, bios) */
	Glib::ustring crc;				/**< CRC asignado al juego (de su rom) */

	long long int manufacturer_id;	/**< Identificador del fabricante */
	Glib::ustring manufacturer;		/**< Fabricante del juego */
	long long int year_id;			/**< Identificador del año */
	Glib::ustring year;				/**< Año de lanzamiento */
	long long int genre_id;			/**< Identificador del género del juego */
	Glib::ustring genre;			/**< Género del juego */
	int players;					/**< Número de jugadores del juego*/

	int rating;						/**< Puntuación del juego*/
	unsigned int times_played;		/**< Contador de veces jugado */
	Glib::ustring last_time_played;	/**< Fecha de la última partida */
	Glib::ustring date_added;		/**< Fecha de inserción en la BD */
	bool favorite;					/**< Indica si el juego está marcado como favorito */

	bool use_custom_emulator;		/**< Indica si se usa un emu custom */
	long long int emulator_id;		/**< Identificador del emulador custom */
};

} // namespace gelide

#endif // _GAME_HPP_
