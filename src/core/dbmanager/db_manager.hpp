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

#ifndef _DB_MANAGER_HPP_
#define _DB_MANAGER_HPP_

#include <vector>
#include <map>
#include <glibmm/ustring.h>
#include "../../defines.hpp"
#include "../../utils/sqlite.hpp"
#include "filter.hpp"
#include "item.hpp"
#include "collection.hpp"
#include "emulator.hpp"
#include "game_list.hpp"
#include "game.hpp"
#include "tag.hpp"
#include "resource.hpp"
#include "manufacturer.hpp"
#include "genre.hpp"
#include "year.hpp"
#include "players.hpp"
#include "rating.hpp"
#include "letter.hpp"
#include "times_played.hpp"
#include "type.hpp"
#include "state.hpp"


namespace gelide{

/**
 * Gestor de la base de datos de Gelide.
 *
 * Se encarga de manejar la bd sqlite3 con los datos de las colecciones, listas
 * juegos etc. que se usarán en el frontend.
 */
class DbManager
{
public:
	// Version de la base de datos
	static const int DATA_BASE_VERSION = 1;	/**< Versión de la base de datos */

	/**
	 * Constructor de la clase
	 */
	DbManager(void);

	/**
	 * Destructor de la clase
	 */
	virtual ~DbManager(void);

	/**
	 * Carga un fichero de base de datos
	 * @param file Fichero del que cargar la base de datos
	 * @return true si se pudo cargar con exito, false en otro caso.
	 */
	bool loadDataBase(const Glib::ustring& file);

	/**
	 * Crea una nueva base de datos
	 * @param file Fichero donde crear la base de datos
	 * @return true si se pudo crear, false en otro caso
	 * @note: Si la base de datos ya existe, será sobreescrita
	 */
	bool createDataBase(const Glib::ustring& file);

	/**
	 * Reinicia la base de datos a su estado original
	 * @return true si se pudo resetear, false en otro caso
	 */
	bool resetDataBase(void);

	/**
	 * Cierra la base de datos
	 * @return true si se pudo cerrar con exito, false en otro caso
	 */
	bool closeDataBase(void);

	/**
	 * Obtiene la información de la cabecera de la base de datos
	 * @param version Lugar donde se guardará la versión de la bd
	 * @param gelide_version Lugar donde se guardará la cadena de versión de
	 * gelide utilizado para su creación
	 * @return true si se pudo obtener los datos, false en otro caso
	 */
	bool getDataBaseHeader(int& version, Glib::ustring& gelide_version);


	/********************************************************
		Gestión de transacciones para operaciones en bloque
	*********************************************************/

	/**
	 * Indica a la base de datos el comienzo de una transacción
	 * @return true si se pudo comenzar la transacción, false en otro caso
	 */
	bool transactionBegin(void);

	/**
	 * Indica a la base de datos la finalización o commit de una transacción
	 * @return true si se pudo finalizar la transacción, false en otro caso
	 */
	bool transactionCommit(void);

	/**
	 * Indica a la base de datos que anule la última transacción comenzada
	 * @return true si se pudo hacer el rollback, false en otro caso.
	 */
	bool transactionRollBack(void);


	/***************************************
		Gestión de las colecciones
	****************************************/

	/**
	 * Obtiene la colección especificada por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Colección búscado o null si no existe
	 */
	Collection* collectionGet(const long long int id = 0);

	/**
	 * Añade una nueva colección a la base de datos
	 * @param collection Nueva colección a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool collectionAdd(Collection* collection);

	/**
	 * Actualiza la entrada en la base de datos de la colección pasada
	 * @param collection Colección a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 */
	bool collectionUpdate(Collection* collection);

	/**
	 * Elimina la colección indicada de la base de datos
	 * @param id Identificador de la colección a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool collectionDelete(const long long int id);

	/**
	 * Mueve una colección a la posición de otra, modificando el orden de la
	 * lista de elementos.
	 * @param id_orig Identificador del elemento origen
	 * @param id_dest Identificador del elemento destino
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool collectionMove(const long long int orig_id, const long long int dest_id);

	/**
	 * Obtiene un listado de las colecciones en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool collectionGetAll(std::vector<Collection* >& list);

	/**
	 * Obtiene un listado de las colecciones en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool collectionGetAll(std::vector<Item* >& list);

	/**
	 * Cuenta el número total de colecciones presentes en el sistema
	 * @return Número de colecciones en la base de datos
	 */
	unsigned int collectionCount(void);

	/**
	 * Obtiene la lista de juegos de una colección dada
	 * @param id Identificador de la colección
	 * @param list Vector donde guardar los juegos
	 * @param filters Filtros a aplicar a la consulta
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector destino será vaciado antes de comenzar con la operación
	 */
	bool collectionGetGames(const long long int id, std::vector<Game* >& list, std::vector<Filter >& filters);

	/**
	 * Obtiene la lista de juegos de una colección dada
	 * @param id Identificador de la colección
	 * @param list Vector donde almacenar los juegos
	 * @param filters Filtros a aplicar a la consulta
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado antes de comenzar con la operación
	 */
	bool collectionGetGames(const long long int id, std::vector<Item* >& list, std::vector<Filter >& filters);

	/**
	 * Cuenta el número de juegos de una colección
	 * @param id Identificador de la colección de la que contar sus juegos
	 * @return Numero de juegos de la colección
	 */
	unsigned int collectionCountGames(const long long int id);

	/**
	 * Obtiene la ruta completa de un fichero de recurso
	 * @param id Identificador de la colección de la que obtener el fichero
	 * @param resource_id Identificador del recurso solicitado
	 * @return Path completo al fichero si existe el recurso o "" si no existe
	 */
	Glib::ustring collectionGetResourceFile(const long long int id, const long long int resource_id);

	/**
	 * Obtiene todos los ficheros de recursos vinculados a una colección
	 * @param id Identificador de la colección de la que obtener sus fichero
	 * @param list Almacen para los paths de los ficheros
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool collectionGetResourceFiles(const long long int id, std::vector<Glib::ustring >& list);

	/**
	 * Añade un fichero a un recurso de la colección
	 * @param id Identificador de la colección a la que añadir el fichero
	 * @param resource_id Identificador del recurso de destino
	 * @param file Path del fichero que se añadirá
	 * @return true si se pudo realizar la acción, false en otro caso
	 */
	bool collectionAddResourceFile(const long long int id, const long long int resource_id, const Glib::ustring& file);

	/**
	 * Elimina la entrada de un fichero de recurso para una colección
	 * @param id Identificador de la colección de la que eliminar el fichero
	 * @param resource_id Identificador del recurso donde está el fichero
	 * @return
	 */
	bool collectionRemoveResourceFile(const long long int id, const long long int resource_id);


	/***************************************
		Gestión de los emuladores
	****************************************/

	/**
	 * Obtiene el emulador especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Emulador búscado o null si no existe
	 */
	Emulator* emulatorGet(const long long int id = 0);

	/**
	 * Añade un nuevo emulador a la base de datos
	 * @param emulator Nuevo emulador a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool emulatorAdd(Emulator* emulator);

	/**
	 * Actualiza la entrada en la base de datos del emulador pasado
	 * @param emulator Emulador a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool emulatorUpdate(Emulator* emulator);

	/**
	 * Elimina el emulador indicado de la base de datos
	 * @param id Identificador del emulador a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool emulatorDelete(const long long int id);

	/**
	 * Cuenta el número total de emuladores presentes en el sistema
	 * @return Número de emuladores en la base de datos
	 */
	unsigned int emulatorCount(void);


	/***************************************
		Gestión de las listas de juegos
	****************************************/

	/**
	 * Obtiene la lista de juegos especificada por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Lista de juegos búscada o null si no existe
	 */
	GameList* gameListGet(const long long int id = 0);

	/**
	 * Añade una nueva lista de juegos a la base de datos
	 * @param list Nueva lista a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool gameListAdd(GameList* list);

	/**
	 * Actualiza la entrada en la base de datos de la lista pasada
	 * @param list Lista a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 */
	bool gameListUpdate(GameList* list);

	/**
	 * Elimina la lista indicada de la base de datos
	 * @param id Identificador de la lista a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameListDelete(const long long int id);

	/**
	 * Obtiene un listado de las listas de juegos en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool gamelistGetAll(std::vector<GameList* >& list);

	/**
	 * Obtiene un listado de las listas de juegos en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool gamelistGetAll(std::vector<Item* >& list);

	/**
	 * Cuenta el número de listas de juegos totales en la base de datos
	 * @return Número de listas de juegos totales en la base de datos
	 */
	unsigned int gameListCount(void);

	/**
	 * Obtiene los de juegos de una lista dada
	 * @param id Identificador de la lista
	 * @param list Vector donde almacenar los juegos
	 * @param filters Filtros a aplicar a la consulta
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado antes de comenzar con la operación
	 */
	bool gameListGetGames(const long long int id, std::vector<Game* >& list, std::vector<Filter >& filters);

	/**
	 * Obtiene los de juegos de una lista dada
	 * @param id Identificador de la lista
	 * @param list Vector donde almacenar los juegos
	 * @param filters Filtros a aplicar a la consulta
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado antes de comenzar con la operación
	 */
	bool gameListGetGames(const long long int id, std::vector<Item* >& list, std::vector<Filter >& filters);

	/**
	 * Añade un juego a una lista de juegos
	 * @param id Identificador de la lista de juegos destino
	 * @param game_id Identificador del juego a añadir
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameListAddGame(const long long int id, const long long int game_id);

	/**
	 * Quita un juego de una lista de juegos
	 * @param id Identificador de la lista de juegos destino
	 * @param game_id Identificador del juego a quitar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameListRemoveGame(const long long int id, const long long int game_id);

	/**
	 * Añade un grupo de juegos a una lista de juegos
	 * @param id Identificador de la lista de juegos destino
	 * @param list Lista de identificadores de los juegos a añadir
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameListAddGameGroup(const long long int id, const std::vector<long long int >& list);

	/**
	 * Quita un grupo de juegos de una lista de juegos
	 * @param id Identificador de la lista de juegos destino
	 * @param list Lista de identificadores de los juegos a quitar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameListRemoveGameGroup(const long long int id, const std::vector<long long int >& list);

	/**
	 * Cuenta el número de juegos presentes en la lista de juegos
	 * @param id Identificador de la lista
	 * @return Número de juegos en la lista solicitada
	 */
	unsigned int gameListCountGames(const long long int id);

private:
	/**
	 * Actualiza el campo QueryCache de una lista de juegos a partir de sus reglas
	 * @param list Lista de la que actualizar su consulta
	 * @return true si se pudo realizar la tarea, false en otro caso
	 */
	bool gameListUpdateQueryCache(GameList* list);

	/**
	 * Parsea una regla de lista inteligente a partir de sus campos
	 * @param field Campo field de una regla de lista inteligente
	 * @param operation Campo operation de una regla de lista inteligente
	 * @param value Campo value de una regla de lista inteligente
	 * @return Cadena SQL con la regla generada
	 */
	Glib::ustring gameListParseSmartRule(const GamelistRuleField field, const GamelistRuleOperation operation, const Glib::ustring& value);

public:
	/***************************************
		Gestión de los juegos
	****************************************/

	/**
	 * Obtiene el juego especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Juego búscado o null si no existe
	 */
	Game* gameGet(const long long int id = 0);

	/**
	 * Añade un nuevo juego a la base de datos
	 * @param game Nuevo juego a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro juego con el
	 * mismo nombre de set
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool gameAdd(Game* game);

	/**
	 * Actualiza la entrada en la base de datos del juego pasado
	 * @param game Juego a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 */
	bool gameUpdate(Game* game);

	/**
	 * Elimina el juego indicado de la base de datos
	 * @param id Identificador del juego a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameDelete(const long long int id);

	/**
	 * Establece el flag de estado de un juego
	 * @param id Identificador del juego a marcar
	 * @param state Valor que recibirá el flag de estado
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameSetState(const long long int id, const GameState state = GAME_STATE_UNKNOWN);

	/**
	 * Establece la puntuación de un juego
	 * @param id Identificador del juego a puntuar
	 * @param rank Puntuación a asignar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameRating(const long long int id, const int rating = 0);

	/**
	 * Resetea los contadores del juego
	 * @param id Identificador del juego del que resetear sus contadores
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameResetCounters(const long long int id);

	/**
	 * Incrementa en 1 el contador de partidas de un juego
	 * @param id Identificador del juego a incrementar su contador de partidas
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameIncTimesPlayed(const long long int id);

	/**
	 * Actualiza la fecha en la que se jugó a un juego por última vez
	 * @param id Identificador del juego actualizar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameUpdateLastTimePlayed(const long long int id);

	/**
	 * Establece el flag Favorito de un juego
	 * @param id Identificador del juego a marcar
	 * @param favorite Valor que recibirá el flag favorito
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameSetFavorite(const long long int id, const bool favorite = false);

	/**
	 * Cuenta el número total de juegos en la base de datos
	 * @return Número total de juegos en la base de datos
	 */
	unsigned int gameCount(void);

	/**
	 * Obtiene un grupo de juegos especificados por sus identificadores
	 * @param list Listado de identificadores de los juegos a obtener
	 * @param games Vector donde se dejarán los juegos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note games será vaciado previamente
	 */
	bool gameGetGroup(std::vector<long long int >& list, std::vector<Game* >& games);

	/**
	 * Actualiza la entrada en la base de datos de los juegos pasados
	 * @param list Listado de juegos a actualizar en la base de datos
	 * @param game Juego base de donde tomar los datos comunes
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Solo se actualizarán los datos comunes
	 */
	bool gameUpdateGroup(std::vector<long long int >& list, Game* game);

	/**
	 * Elimina una lista de juegos indicados por sus identificadores
	 * @param list Listado de identificador de juego a eliminar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameDeleteGroup(std::vector<long long int >& list);

	/**
	 * Establece la puntuación de un listado de juegos
	 * @param list Lista de identificadores de juegos a puntuar
	 * @param rank Puntuación a asignar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameRatingGroup(std::vector<long long int >& list, const int rating = 0);

	/**
	 * Resetea los contadores de un listado de juegos
	 * @param list Lista de identificadores de juegos a resetear
	 * @return true si se pudo realizar la tarea false en otro caso
	 */
	bool gameResetCountersGroup(std::vector<long long int >& list);

	/**
	 * Establece el flag Favorito de un listado de juegos
	 * @param list Lista de identificador de juegos a marcar
	 * @param favorite Valor que recibirá el flag favorito
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameSetFavoriteGroup(std::vector<long long int >& list, const bool favorite = false);

	/**
	 * Obtiene una lista con todas las listas de juegos a las que pertence un
	 * juego dado
	 * @param id Identificador del juego del que obtener sus listas
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool gameGetGameListsIn(const long long int id, std::vector<Item* >& list);

	/**
	 * Obtiene una lista con todas las listas de juegos a las que NO pertence un
	 * juego dado
	 * @param id Identificador del juego del que obtener las listas en las que
	 * no está
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool gameGetGameListsNotIn(const long long int id, std::vector<Item* >& list);

	/**
	 * Añade una etiqueta a un juego
	 * @param game_id Identificador del juego a etiquetar
	 * @param tag_id Identificador de la etiqueta
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameAddTag(const long long int game_id, const long long int tag_id);

	/**
	 * Quita una etiqueta de un juego
	 * @param game_id Identificador del juego
	 * @param tag_id Identificador de la etiqueta a quitar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameRemoveTag(const long long int game_id, const long long int tag_id);

	/**
	 * Etiqueta un listado de juegos con la etiqueta indicada
	 * @param list Lista de identificadores de los juegos a etiquetar
	 * @param tag_id Identificador de la etiqueta
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameAddTagGroup(const std::vector<long long int >& list, const long long int tag_id);

	/**
	 * Quita una etiqueta de un listado de juegos
	 * @param list Lista de identificadores de los juegos
	 * @param tag_id Identificador de la etiqueta a quitar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameRemoveTagGroup(const std::vector<long long int >& list, const long long int tag_id);

	/**
	 * Obtiene una lista con todas las etiquetas de un juego dado
	 * @param id Identificador del juego del que obtener sus etiquetas
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool gameGetTagsIn(const long long int id, std::vector<Item* >& list);

	/**
	 * Obtiene una lista con todas las etiquetas que no pertenecen a un juego
	 * @param id Identificador del juego del que obtener las etiquetas que no
	 * tiene
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool gameGetTagsNotIn(const long long int id, std::vector<Item* >& list);

	/**
	 * Obtiene la ruta completa de un fichero de recurso
	 * @param id Identificador del juego de la que obtener el fichero
	 * @param resource_id Identificador del recurso solicitado
	 * @return Path completo al fichero si existe el recurso o "" si no existe
	 */
	Glib::ustring gameGetResourceFile(const long long int id, const long long int resource_id);

	/**
	 * Obtiene todos los ficheros de recursos vinculados au juego
	 * @param id Identificador del juego del que obtener sus fichero
	 * @param list Almacen para los paths de los ficheros
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool gameGetResourceFiles(const long long int id, std::vector<Glib::ustring >& list);

	/**
	 * Añade un fichero a un recurso de un juego
	 * @param id Identificador del juego al que añadir el fichero
	 * @param resource_id Identificador del recurso de destino
	 * @param file Path del fichero que se añadirá
	 * @return true si se pudo realizar la acción, false en otro caso
	 */
	bool gameAddResourceFile(const long long int id, const long long int resource_id, const Glib::ustring& file);

	/**
	 * Elimina la entrada de un fichero de recurso para un juego
	 * @param id Identificador del juego del que eliminar el fichero
	 * @param resource_id Identificador del recurso donde está el fichero
	 * @return
	 */
	bool gameRemoveResourceFile(const long long int id, const long long int resource_id);


	/***************************************
		Gestión de las etiquetas
	****************************************/

	/**
	 * Obtiene la etiqueta especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Etiqueta búscada o null si no existe
	 */
	Tag* tagGet(const long long int id = 0);

	/**
	 * Añade una nueva etiqueta a la base de datos
	 * @param tag Nueva etiqueta a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool tagAdd(Tag* tag);

	/**
	 * Actualiza la entrada en la base de datos de la etiqueta pasada
	 * @param tag Etiqueta a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 */
	bool tagUpdate(Tag* tag);

	/**
	 * Elimina la etiqueta indicada de la base de datos
	 * @param id Identificador de la etiqueta a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool tagDelete(const long long int id);

	/**
	 * Obtiene una lista con todas las etiquetas en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool tagGetAll(std::vector<Tag* >& list);


	/***************************************
		Gestión de las galerías del usuario
	****************************************/

	/**
	 * Obtiene la galería especificada por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Galería búscada o null si no existe
	 */
	Resource* resourceGet(const long long int id = 0);

	/**
	 * Añade una nueva galería a la base de datos
	 * @param gallery Nueva galería a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool resourceAdd(Resource* resource);

	/**
	 * Actualiza la entrada en la base de datos de la galería pasada
	 * @param gallery Galería a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 */
	bool resourceUpdate(Resource* resource);

	/**
	 * Elimina la galería indicada de la base de datos
	 * @param id Identificador de la galería a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool resourceDelete(const long long int id);

	/**
	 * Obtiene una lista con todas las galerías en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool resourceGetAll(std::vector<Resource* >& list);

	/**
	 * Obtiene todos los ficheros vinculados a un recurso
	 * @param id Identificador del recurso del que obtener sus ficheros
	 * @param list Almacen para los paths de los ficheros
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool resourceGetFiles(const long long int id, std::vector<Glib::ustring >& list);


	/***************************************
		Gestión de los fabricantes
	****************************************/

	/**
	 * Obtiene el fabricante especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Fabricante búscado o null si no existe
	 * @note El identificador 1 se utiliza para marcar al elemento desconocido
	 */
	Manufacturer* manufacturerGet(const long long int id = 0);

	/**
	 * Añade un nuevo fabricante a la base de datos
	 * @param manufacturer Nuevo fabricante a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool manufacturerAdd(Manufacturer* manufacturer);

	/**
	 * Actualiza la entrada en la base de datos del fabricante pasado
	 * @param manufacturer Fabricante a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 */
	bool manufacturerUpdate(Manufacturer* manufacturer);

	/**
	 * Elimina el fabricante indicado de la base de datos
	 * @param id Identificador del fabricante a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool manufacturerDelete(const long long int id);

	/**
	 * Obtiene una lista con todos los fabricantes en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool manufacturerGetAll(std::vector<Manufacturer* >& list);


	/***************************************
		Gestión de los géneros
	****************************************/

	/**
	 * Obtiene el género especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Género búscado o null si no existe
	 * @note El identificador 1 se utiliza para marcar al elemento desconocido
	 */
	Genre* genreGet(const long long int id = 0);

	/**
	 * Añade un nuevo género a la base de datos
	 * @param genre Nuevo género a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool genreAdd(Genre* genre);

	/**
	 * Actualiza la entrada en la base de datos del género pasado
	 * @param genre Género a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Normalmente se devolverá false cuando exista otro elemento con el
	 * mismo nombre
	 */
	bool genreUpdate(Genre* genre);

	/**
	 * Elimina el género indicado de la base de datos
	 * @param id Identificador del género a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool genreDelete(const long long int id);

	/**
	 * Obtiene una lista con todos los género en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool genreGetAll(std::vector<Genre* >& list);


	/***************************************
		Gestión de los años
	****************************************/

	/**
	 * Obtiene el año especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Año búscado o null si no existe
	 * @note El identificador 1 se utiliza para marcar al elemento desconocido
	 */
	Year* yearGet(const long long int id = 0);

	/**
	 * Obtiene una lista con todos los fabricantes en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool yearGetAll(std::vector<Year* >& list);


	/*******************************************
		Gestión del número de jugadores
	********************************************/

	/**
	 * Obtiene el número de jugadores especificada por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Número de jugadores buscado o null si no existe
	 */
	Players* playersGet(const long long int id = 0);

	/**
	 * Obtiene una lista con todas los numeros de jugadores en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool playersGetAll(std::vector<Players* >& list);


	/***************************************
		Gestión de las puntuaciones
	****************************************/

	/**
	 * Obtiene la puntuación especificada por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Puntuación buscada o null si no existe
	 */
	Rating* ratingGet(const long long int id = 0);

	/**
	 * Obtiene una lista con todas las puntuaciones en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool ratingGetAll(std::vector<Rating* >& list);


	/*******************************************
		Gestión de las letras para filtrado
	********************************************/

	/**
	 * Obtiene la letra especificada por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Letra buscada o null si no existe
	 */
	Letter* letterGet(const long long int id = 0);

	/**
	 * Obtiene una lista con todas las letras en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool letterGetAll(std::vector<Letter* >& list);


	/***************************************
		Gestión de los filtros según número de partidas
	****************************************/

	/**
	 * Obtiene el filtro de número de partidas
	 * @param id Identificador del elemento solicitado
	 * @return Filtro de número de partidas buscado o null si no existe
	 */
	TimesPlayed* timesPlayedGet(const long long int id = 0);

	/**
	 * Obtiene una lista con todos los filtros según el número de partidas
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool timesPlayedGetAll(std::vector<TimesPlayed* >& list);


	/***************************************
		Gestión de los tipos
	****************************************/

	/**
	 * Obtiene el tipo especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Tipo buscado o null si no existe
	 */
	Type* typeGet(const long long int id = 0);

	/**
	 * Obtiene una lista con todos los tipos en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool typeGetAll(std::vector<Type* >& list);


	/***************************************
		Gestión de los estados
	****************************************/

	/**
	 * Obtiene el estado especificado por un identificador
	 * @param id Identificador del elemento solicitado
	 * @return Estado buscado o null si no existe
	 */
	State* stateGet(const long long int id = 0);

	/**
	 * Obtiene una lista con todos los estados en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool stateGetAll(std::vector<State* >& list);

private:

	/**
	 * Comprueba la tabla de años y si es necesario añade el año actual
	 * @ return true si se pudo realizar la tarea, false en otro caso
	 */
	bool updateYearsTable(void);

	/**
	 * Vacía completamente una tabla de la base de datos
	 * @param table Tabla a vaciar en la base de datos
	 * @return true si se pudo realizar la tarea, false en otro caso
	 */
	bool emptyTable(const Glib::ustring& table);

	/**
	 * Cuenta el número de registros en una tabla dada
	 * @param table Tabla de la que contar sus registros
	 * @return Número de registros en la tabla
	 */
	unsigned int countTable(const Glib::ustring& table);

	/**
	 * Genera una cadena de consulta sql a partir de un vector de filtros
	 * @param filters Vector de filtros (pares FilterType, valor)
	 * @return Cadena preparada para la clausula where
	 */
	Glib::ustring parseFiltersVector(std::vector<Filter >& filters);


	Sqlite m_db;					/**< Base de datos SQLite interna */
	Glib::ustring m_db_file;		/**< Path a la base de datos */
};

} // namespace gelide

#endif // _DB_MANAGER_HPP_
