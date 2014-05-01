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

// Posibles comandos ejecutables por la aplicación
enum MetadbmCommands{
	COMMAND_SHOW_VERSION = 0,		/**< Tipo de filtro ninguno */
	COMMAND_SHOW_HELP,
	COMMAND_LIST_SYSTEMS,
	COMMAND_LIST_GAMES,
	COMMAND_ADD_CRC,
	COMMAND_ADD_NAME,
	COMMAND_DELETE_SYSTEM,
	COMMAND_DELETE_GAME,
	COMMAND_FIND_NAME,
	COMMAND_FIND_CRC,
	COMMAND_FIND_TITLE
};

/**
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
	 * Muestra por la salida estandar un mensaje con información de uso
	 * @param program Nombre real del binario ejecutado
	 */
	void showHelp(const Glib::ustring& program);

	/**
	 * Muestra por la salida estandar un mensaje con información de la versión
	 */
	void showVersion(void);

	/**
	 * Muestra por la salida estandar el listado de sistemas en la bd
	 */
	void listSystems(void);

	/**
	 * Se encarga de cerrar adecuadamente los sistemas creados por el programa
	 */
	void clean(void);

	MetaDb* m_db;					/**< BD de la app */
	bool m_log_enabled;
	Glib::ustring m_working_dir;	/**< Path del directorio de trabajo */

	MetadbmCommands m_command;
	Glib::ustring m_param1;
	Glib::ustring m_param2;
};


} // namespace gelide

#endif // _METADBM_HPP_
