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

#ifndef _METADBM_HPP_
#define _METADBM_HPP_

#include "../defines.hpp"
#include <glibmm/ustring.h>
#include "../core/metadb/meta_db.hpp"

namespace gelide{

// Versión del metadbm
#define METADBM_VERSION	"0.1"

// Posibles comandos ejecutables por la aplicación
enum MetadbmCommands{
	COMMAND_SHOW_VERSION = 0,	/**< Mostrar versión del programa */
	COMMAND_SHOW_HELP,			/**< Mostrar la ayuda del programa */
	COMMAND_LIST_SYSTEMS,		/**< Listar los sistemas de la bd */
	COMMAND_LIST_GAMES,			/**< Listar los sets de la bd */
	COMMAND_DELETE_SYSTEM,		/**< Borrar un sistema y sus sets */
	COMMAND_DELETE_GAME,		/**< Borrar un set */
	COMMAND_SHOW_GAME,			/**< Mostrar la info detallada de un set */
	COMMAND_ADD_NAME,			/**< Añadir sets usando el set name como hash */
	COMMAND_ADD_CRC,			/**< Añadir sets usando el CRC como hash */
	COMMAND_ADD_MD5,			/**< Añadir sets usando el MD5 como hash */
	COMMAND_ADD_SHA1,			/**< Añadir sets usando el SHA1 como hash */
	COMMAND_FIND_NAME,			/**< Buscar sets por nombre de set */
	COMMAND_FIND_HASH,			/**< Buscar sets por hash */
	COMMAND_FIND_TITLE			/**< Buscar sets que comiencen por título */
};

/**
 * Metadbm (Meta Data Base Manager) es un gestor por línea de comandos de
 * la base de datos que contiene meta datos de sets de diferentes sistemas.
 * Permite ejecutar comandos básicos sobre la metadb tales como:
 * - Listar sistemas
 * - Listar sets
 * - Eliminar sistemas y sets
 * - Agregar nuevos sets desde ficheros dat
 * - Realizar búsquedas de sets de diferentes maneras.
 */
class MetadbmApp
{
public:

	/**
	 * Constructor parametrizado
	 * @param working_dir Directorio de trabajo
	 */
	MetadbmApp(const Glib::ustring& working_dir);

	/**
	 * Destructor de la clase
	 */
	virtual ~MetadbmApp(void);

	/**
	 * Ejecuta el bucle principal de la aplicación
	 * @param argc Contador de argumentos
	 * @param argv Vector de argumentos
	 * @return EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE si se produjo
	 * algún error.
	 */
	int run(int argc, char** argv);

private:

	/**
	 * Parsea los parámetros pasados modificando el estado de la aplicación
	 * @param argc Contador de parámetros
	 * @param argv Vector de parámetros
	 * @return 	0 si los parámetros son correctos
	 * 			1 si los parámetros no son correctos
	 */
	int parseParams(int argc, char** argv);

	/**
	 * Muestra por la salida estándar un mensaje con información de uso
	 * @param program Nombre real del binario ejecutado
	 */
	void showHelp(const Glib::ustring& program);

	/**
	 * Muestra por la salida estándar un mensaje con información de la versión
	 */
	void showVersion(void);

	/**
	 * Muestra por la salida estándar el listado de sistemas en la bd
	 */
	void listSystems(void);

	/**
	 * Muestra por la salida estándar el listado de juegos en la bd
	 * @param system Sistema por el que filtrar la lista o vacío para todos
	 */
	void listGames(const Glib::ustring& system);

	/**
	 * Elimina un sistema junto con sus juegos
	 * @param system Sistema a eliminar
	 */
	void deleteSystem(const Glib::ustring& system);

	/**
	 * Elimina un juego por id
	 * @param id Identificador del juego a eliminar
	 */
	void deleteGame(const long long int id);

	/**
	 * Muestra la ficha del juego indicado
	 * @param id Identificador del juego a mostrar
	 */
	void showGame(const long long int id);

	/**
	 * Busca sets por nombre
	 * @param system Sistema en el que buscar o vacío para búsqueda global
	 * @param name Nombe de set a buscar
	 */
	void findName(const Glib::ustring& system, const Glib::ustring& name);

	/**
	 * Busca sets por hash
	 * @param system Sistema en el que buscar o vacío para búsqueda global
	 * @param hash Hash a buscar
	 */
	void findHash(const Glib::ustring& system, const Glib::ustring& hash);

	/**
	 * Busca sets cuyo título comience por
	 * @param system Sistema en el que buscar o vacío para búsqueda global
	 * @param title Inicio del título a buscar
	 */
	void findTitle(const Glib::ustring& system, const Glib::ustring& title);


	/**
	 * Añade o actualiza sets de un sistema desde un fichero dat
	 * @param system Sistema al que añadir los sets
	 * @param file Fichero dat origen
	 * @param type Tipo de hash a usar
	 */
	void addSets(const Glib::ustring& system, const Glib::ustring& file, const MetadbmCommands type);

	/**
	 * Se encarga de cerrar adecuadamente los sistemas creados por el programa
	 */
	void clean(void);

	MetaDb* m_db;					/**< BD de la app */
	bool m_log_enabled;				/**< Indica si el log está hablitado */
	Glib::ustring m_working_dir;	/**< Path del directorio de trabajo */

	MetadbmCommands m_command;		/**< Comando a ejecutar */
	Glib::ustring m_param1;			/**< Primer parámetro para el comando */
	Glib::ustring m_param2;			/**< Segundo parámetro para el comando */
};


} // namespace gelide

#endif // _METADBM_HPP_
