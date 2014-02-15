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

#ifndef _SQLITE_HPP_
#define _SQLITE_HPP_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif /* HAVE_CONFIG_H */

// Si no está definido el modo debug, desactivamos los asserts
#ifndef ENABLE_DEBUG_MODE
	#define NDEBUG
#endif

#include <cassert>
#include <glibmm/ustring.h>
#include "../thirdparty/sqlite3/sqlite3.h"
#include "log.hpp"
#include "sqlite_statement.hpp"


/**
 * Wrapper simple para encapsular el manejor del motor de bases de datos SQLite3
 *
 * SQLite no cubre la totalidad de la funcionalidad de sqlite3, sino que
 * está diseñada para proporcionar un conjunto básico funcional de las funciones
 * más usadas para gestionar una base de datos.
 */
class Sqlite
{
public:
	/**
	 * Constructor de la clase
	 */
	Sqlite(void);

	/**
	 * Constructor con apertura de una bd
	 * @param file Fichero de bd a abrir
	 * @note La base de datos pasada debe existir, el método no creará una nueva
	 */
	Sqlite(const Glib::ustring& file);

	/**
	 * Destructor
	 */
	virtual ~Sqlite();

	/**
	 * Inicializa el motor de bases de datos SQLite
	 * @return True si se inicializó con exito, false en otro caso.
	 */
	static bool initialize(void)
	{
		LOG_INFO("Initializing SQLite engine...");
		if (sqlite3_initialize() != SQLITE_OK)
		{
			LOG_ERROR("SQLite: Can't initialize engine");
			return false;
		}
		LOG_INFO("SQLite: Engine version: " << SQLITE_VERSION);
		return true;
	}

	/**
	 * Apaga el motor de bases de datos SQLite
	 * @return True si se apagó con exito, false en otro caso.
	 */
	static bool shutDown(void)
	{
		LOG_INFO("Shutting down SQLite engine...");
		if (sqlite3_shutdown() != SQLITE_OK)
		{
			LOG_ERROR("SQLite: Can't shutdown engine");
			return false;
		}
		return true;
	}

	/**
	 * Abre una base de datos desde un fichero
	 * @param file Archivo de base de datos sqlite a abrir
	 * @return True si se pudo abrir el fichero, false en otro caso
	 * @note Este método no creará el fichero en el caso de no existir
	 */
	bool open(const Glib::ustring& file);

	/**
	 * Crea o abre si existe, una base de datos sqlite desde un fichero
	 * @param file Archivo de base de datos sqlite a crear o abrir
	 * @return True si se pudo crear o abrir la bd, false en otro caso
	 */
	bool create(const Glib::ustring& file);

	/**
	 * Cierra la base de datos
	 * @return True si se pudo cerrar la bd, false en otro caso
	 */
	bool close(void);

	/**
	 * Indica si la base de datos sqlite ha sido abierta
	 * @return True si la base de datos fue abierta, false en otro caso
	 */
	bool isOpen(void)
	{
		return(m_db != NULL);
	}

	/**
	 * Obtiene la base de datos sqlite real usada internamente
	 * @return Puntero a la base de datos sqlite real
	 */
	sqlite3* getSqlite(void)
	{
		return m_db;
	}

	/**
	 * Obtiene el identificador asignado en la última operación de inserción
	 * @return RowID asignado (long long int)
	 */
	sqlite_int64 getLastInsertId(void);

	/**
	 * Ejecuta una secuencia SQL de forma simple
	 * @param query Secuencia SQL a ejecutar
	 * @return true si se pudo ejecutar la secuencia, false en otro caso.
	 */
	bool exec(const Glib::ustring& query);

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

	/**
	 * Crea un comando para poder usarlo sobre la base de datos actual
	 * @return Comando creado o null si no se pudo crear
	 */
	SqliteStatement* createStatement(void){
		return new SqliteStatement(m_db);
	}

	/**
	 * Crea un comando para poder usarlo sobre la base de datos y lo prepara con
	 * una secuencia sql dada
	 * @param query secuencia sql con la que preparar el comando
	 * @return Comando creado o null si no se pudo crear
	 */
	SqliteStatement* createStatement(const Glib::ustring& query);

private:
	sqlite3* m_db;				/**< Base de datos Sqlite */
};

#endif // _SQLITE_HPP_
