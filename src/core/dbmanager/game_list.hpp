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

#ifndef _GAMELIST_HPP_
#define _GAMELIST_HPP_

#include <glibmm/ustring.h>
#include "item.hpp"


namespace gelide{

// Posibles tipos de listas de juegos
enum GamelistType
{
	GAMELIST_TYPE_STANDARD = 0,		/**< Lista estándar */
	GAMELIST_TYPE_SMART				/**< Lista inteligente */
};

// Posibles tipos operaciones entre reglas de una lista inteligente
enum GamelistOperation
{
	GAMELIST_OPERATION_AND = 0,		/**< Operación AND de las reglas de filtrado */
	GAMELIST_OPERATION_OR			/**< Operación OR de las reglas de filtrado */
};

// Posibles operaciones en las reglas de una lista inteligente
enum GamelistRuleOperation
{
	RULE_OPERATION_CONTAINS = 0,		/**< Operación contiene */
	RULE_OPERATION_NOTCONTAINS,			/**< Operación no contiene */
	RULE_OPERATION_EQUALS,				/**< Operación es */
	RULE_OPERATION_NOTEQUALS,			/**< Operación no es */
	RULE_OPERATION_STARTWITH,			/**< Operación comienza por */
	RULE_OPERATION_ENDWITH,				/**< Operación acaba en */
	RULE_OPERATION_LESSTHAN,			/**< Operación menos que */
	RULE_OPERATION_GREATERTHAN,			/**< Operación mas que */
	RULE_OPERATION_LESSTHANEQUALS,		/**< Operación como mucho */
	RULE_OPERATION_GREATERTHANEQUALS,	/**< Operación al menos */
	RULE_OPERATION_BEFORE,				/**< Operación anterior a */
	RULE_OPERATION_AFTER				/**< Operación posterior a */
};

// Posibles campos en las reglas de una lista inteligente
enum GamelistRuleField
{
	FIELD_COLLECTIONS_NAME = 0,		/**< Campo Coleccion */
	FIELD_GAMES_NAME,				/**< Campo Nombre de Set*/
	FIELD_GAMES_TITLE,				/**< Campo Título */
	FIELD_MANUFACTURERS_NAME,		/**< Campo Fabricante */
	FIELD_GENRES_NAME,				/**< Campo Género */
	FIELD_TAGS_NAME,					/**< Campo Etiqueta */
	FIELD_GAMES_STATUS,				/**< Campo Estado */
	FIELD_GAMES_TYPE,				/**< Campo Tipo */
	FIELD_YEARS_NAME,				/**< Campo Año */
	FIELD_GAMES_PLAYERS,				/**< Campo Jugadores */
	FIELD_GAMES_RATING,				/**< Campo Valoración */
	FIELD_GAMES_TIMESPLAYED,			/**< Campo Partidas jugadas */
	FIELD_GAMES_FAVORITE,			/**< Campo Favorito */
	FIELD_GAMES_LASTTIMEPLAYED,		/**< Campo Última partida */
	FIELD_GAMES_DATEADDED,			/**< Campo Fecha inserción */
	FIELD_COUNT						/**< Contador de campos */
};

/**
 * Mantiene la información de una lista de juegos
 *
 * Las listas de juegos son independientes de las colecciones, es decir,
 * pueden contener juegos de multiples colecciones al mismo tiempo.
 * Hay dos tipos de listas de juegos:
 * - Estándar: Es el usuario el que agrega juegos a la lista
 * - Inteligente: El usuario define unas reglas de filtrado para los juegos.
 */
struct GameList : public Item
{

	// Versión de las reglas de filtrado
	static const int RULES_VERSION = 1;		/**< Versión de las reglas de filtrado */

	/**
	 * Constructor básico
	 */
	GameList(void):
		Item(),
		type(GAMELIST_TYPE_STANDARD),
		operation(GAMELIST_OPERATION_AND),
		limited(false),
		limit_amount(25),
		use_custom_emulator(false),
		emulator_id(0)
	{
	}

	/**
	 * Constructor parametrizado
	 * @param p_id Nuevo identificador para el elemento
	 * @param p_name Nombre para el elemento
	 */
	GameList(const long long int p_id, const Glib::ustring& p_name):
		Item(p_id, p_name),
		type(GAMELIST_TYPE_STANDARD),
		operation(GAMELIST_OPERATION_AND),
		limited(false),
		limit_amount(25),
		use_custom_emulator(false),
		emulator_id(0)
	{
	}

	GamelistType type;				/**< Tipo de lista */
	GamelistOperation operation;	/**< Tipo de operación (And, Or) */
	Glib::ustring rules;			/**< XML con las reglas de filtrado */
	bool limited;					/**< Indica si aplicar limitador */
	int limit_amount;				/**< Tope del limitador */
	Glib::ustring query_cache;		/**< Caché de consulta */

	bool use_custom_emulator;		/**< Indica si se usa un emu custom */
	long long int emulator_id;		/**< Identificador del emulador custom */
};

} // namespace gelide

#endif // _GAMELIST_HPP_
