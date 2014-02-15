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

#ifndef _SQLITE_STATEMENT_HPP_
#define _SQLITE_STATEMENT_HPP_

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


/**
 * Mediante SQLiteStatement, gestionamos un comando o secuencia sql que se
 * ejecuta sobre una base de datos.
 */
class SqliteStatement
{
public:


	//static const unsigned char STATEMENT_ERROR	= 0;	/**< Error */
	//static const unsigned char STATEMENT_ROW	= 1;	/**< Hay un resultado */
	//static const unsigned char STATEMENT_DONE	= 2;	/**< Ha finalizado correctamente */

	/** Resultados que puede dar un comando después de su ejecución */
	enum StatementResult
	{
		STATEMENT_ERROR = 0,	/**< Error */
		STATEMENT_ROW,			/**< Hay un resultado */
		STATEMENT_DONE			/**< Ha finalizado correctamente */
	};

	/**
	 * Constructor de la clase
	 */
	SqliteStatement(void);

	/**
	 * Constructor parametrizado
	 * @param db Base de datos sobre la que actuará el comando
	 */
	SqliteStatement(sqlite3* db);

	/**
	 * Destructor
	 */
	virtual ~SqliteStatement();

	/**
	 * Establece la base de datos sqlite real usada internamente
	 * @param db Base de datos interna a usar
	 */
	void setSQLite(sqlite3* db){
		m_db = db;
	}

	/**
	 * Prepara un comando a partir de una secuencia sql dada
	 * @param query Cadena sql a preparar
	 * @return true si se pudo realizar la acción, false en otro caso
	 */
	bool prepare(const Glib::ustring& query);

	/**
	 * Intenta ejecuta el comando preparado
	 * @return
	 * 	STATEMENT_ERROR si hubo algún error en la ejecución
	 *	STATEMENT_ROW si se ejecutó correctamente y hay una fila que procesar
	 *	STATEMENT_DONE si se ejecutó correctamente y finalizó
	 */
	StatementResult step(void);

	/**
	 * Resetea el comando preparado para poder ejecutarlo de nuevo
	 * @return true si se pudo resetear, false en otro caso.
	 */
	bool reset(void);

	/**
	 * Finaliza la ejecución del comando dando por terminado su trabajo
	 * @return true si se pudo finalizar, false en otro caso.
	 */
	bool finalize(void);

	/**
	 * Obtiene el número de columnas en el resultado de la última ejecución del
	 * comando.
	 * @return Número de columnas o 0 si no hay resultado
	 */
	int getColumnCount(void);

	/**
	 * Obtiene el nombre de una columna en el resultado de la última ejecución
	 * del comando.
	 * @param index Indice de la columna cuyo nombre se requiere (0, n-1)
	 * @return Nombre de la columna solicitada.
	 */
	Glib::ustring getColumnName(const int index);

	/**
	 * Extrae un valor booleano en el último comando ejecutado, de la columna
	 * pasada como parámetro
	 * @param index Indice de la columna de la que extraer el valor
	 * @return valor de la columna indicada
	 */
	bool getColumnBool(const int index);

	/**
	 * Extrae un valor entero de 32bits con signo en el último comando
	 * ejecutado, de la columna pasada como parámetro
	 * @param index Indice de la columna de la que extraer el valor
	 * @return valor de la columna indicada
	 */
	int getColumnInt(const int index);

	/**
	 * Extrae un valor entero de 64bits con signo en el último comando
	 * ejecutado, de la columna pasada como parámetro
	 * @param index Indice de la columna de la que extraer el valor
	 * @return valor de la columna indicada
	 */
	sqlite3_int64 getColumnInt64(const int index);

	/**
	 * Extrae un valor en coma flotante de 64bits con signo en el último comando
	 * ejecutado, de la columna pasada como parámetro
	 * @param index Indice de la columna de la que extraer el valor
	 * @return valor de la columna indicada
	 */
	double getColumnDouble(const int index);

	/**
	 * Extrae una cadena de texto en el último comando ejecutado, de la columna
	 * pasada como parámetro
	 * @param index Indice de la columna de la que extraer el valor
	 * @return valor de la columna indicada
	 */
	Glib::ustring getColumnText(const int index);

	/**
	 * Devuelve un puntero a un campo Blob en el último comando ejecutado, de la
	 * columna pasada como parámetro
	 * @param index Indice de la columna de la que extraer el valor
	 * @return Puntero al campo blob
	 */
	const void* getColumnBlob(const int index);

	/**
	 * Vincula un valor booleano al parámetro del comando en la posición
	 * indicada por el índice pasado.
	 * @param index Indice del parámetro en el comando
	 * @param value Valor a vincular con el parámetro
	 * @return true si se pudo realizar la vinculación, false en otro caso
	 */
	bool bind(const int index, const bool value);

	/**
	 * Vincula un valor entero de 32bits con signo al parámetro del comando en
	 * la posición indicada por el índice pasado.
	 * @param index Indice del parámetro en el comando
	 * @param value Valor a vincular con el parámetro
	 * @return true si se pudo realizar la vinculación, false en otro caso
	 */
	bool bind(const int index, const int value);

	/**
	 * Vincula un valor entero de 32bits con signo al parámetro del comando en
	 * la posición indicada por el índice pasado.
	 * @param index Indice del parámetro en el comando
	 * @param value Valor a vincular con el parámetro
	 * @return true si se pudo realizar la vinculación, false en otro caso
	 */
	bool bind(const int index, const sqlite_int64 value);

	/**
	 * Vincula un valor entero de 64bits con signo al parámetro del comando en
	 * la posición indicada por el índice pasado.
	 * @param index Indice del parámetro en el comando
	 * @param value Valor a vincular con el parámetro
	 * @return true si se pudo realizar la vinculación, false en otro caso
	 */
	bool bind(const int index, const double value);

	/**
	 * Vincula una cadena de texto al parámetro del comando en la posición
	 * indicada por el índice pasado.
	 * @param index Indice del parámetro en el comando
	 * @param value Valor a vincular con el parámetro
	 * @return true si se pudo realizar la vinculación, false en otro caso
	 */
	bool bind(const int index, const Glib::ustring& value);

	/**
	 * Vincula un puntero de datos a un campo Blob del comando en la posición
	 * indicada por el índice pasado.
	 * @param index Indice del parámetro en el comando
	 * @param value Valor a vincular con el parámetro
	 * @return true si se pudo realizar la vinculación, false en otro caso
	 * @note Internamente se realiza una copia del buffer pasado
	 */
	bool bind(const int index, const void* value);

	/**
	 * Vincula un valor NULL al parámetro del comando en la posición indicada
	 * por el índice pasado.
	 * @param index Indice del parámetro en el comando
	 * @return true si se pudo realizar la vinculación, false en otro caso
	 */
	bool bind(const int index);

private:
	sqlite3* m_db;				/**< Base de datos Sqlite */
	sqlite3_stmt* m_stmt;		/**< Statement para ejecución interna */
};

#endif // _SQLITE_STATEMENT_HPP_
