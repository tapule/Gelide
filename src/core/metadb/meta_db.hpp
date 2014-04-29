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

#ifndef _META_DB_HPP_
#define _META_DB_HPP_

#include <vector>
#include <glibmm/ustring.h>
#include "../../defines.hpp"
#include "../../utils/sqlite.hpp"
#include "meta_db_set.hpp"


namespace gelide{

/**
 * Base de datos con metadatos de sets de diferentes plataformas.
 *
 * La metadb almacena información sobre sets de diferentes plataformas
 * que se usan para actualizar la información de los juegos que el usuario
 * agrega a su biblioteca.
 */
class MetaDb
{
public:
	// Version de la base de datos
	static const int DATA_BASE_VERSION = 1;	/**< Versión de la base de datos */

	/**
	 * Constructor de la clase
	 */
	MetaDb(void);

	/**
	 * Destructor de la clase
	 */
	virtual ~MetaDb(void);

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
	 * @return true si se pudo obtener los datos, false en otro caso
	 */
	bool getDataBaseHeader(int& version);


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
		Gestión de  sistemas
	****************************************/

	/**
	 * Elimina el sistema indicado junto con sus juegos de la base de datos
	 * @param system Nombre del sistema a eliminar
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool systemDelete(const Glib::ustring& system);

	/**
	 * Obtiene un listado de los sistemas en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool systemGetAll(std::vector<Glib::ustring>& list);

	/**
	 * Obtiene un listado de las colecciones en la base de datos
	 * @param system Sistema del que obtener sus juegos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool systemGetSets(const Glib::ustring& system,	std::vector<MetaDbSet* >& list);


	/***************************************
		Gestión de los sets
	****************************************/

	/**
	 * Obtiene el set de un sistema dado con un nombre determinado
	 * @param system Sistema del que obtener el set
	 * @param name Nombre de set requerido
	 * @return Set búscado o null si no existe
	 */
	MetaDbSet* setGetByName(const Glib::ustring& system, const Glib::ustring& name);

	/**
	 * Obtiene el set de un sistema dado con un crc determinado
	 * @param system Sistema del que obtener el set
	 * @param crc CRC de set requerido
	 * @return Set búscado o null si no existe
	 */
	MetaDbSet* setGetByCrc(const Glib::ustring& system, const Glib::ustring& crc);

	/**
	 * Añade un nuevo set a la base de datos
	 * @param set Nuevo set a añadir a la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note Si la operación tiene éxito, se actualizará el id del elemento.
	 */
	bool setAdd(MetaDbSet* set);

	/**
	 * Actualiza la entrada en la base de datos del set pasado
	 * @param set Set a actualizar en la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool setUpdate(MetaDbSet* set);

	/**
	 * Elimina el set indicado de la base de datos
	 * @param id Identificador del set a eliminar de la base de datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 */
	bool setDelete(const long long int id);

	/**
	 * Obtiene un listado de los sets en la base de datos
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool setGetAll(std::vector<MetaDbSet* >& list);

	/**
	 * Busca en la base de datos sets cuyo nombre coincida con el nombre dado
	 * @param name Nombre de set a buscar
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool setFindName(const Glib::ustring& name, std::vector<MetaDbSet* >& list);

	/**
	 * Busca en la base de datos sets cuyo CRC coincida con el CRC dado
	 * @param crc CRC a buscar
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool setFindCrc(const Glib::ustring& crc, std::vector<MetaDbSet* >& list);

	/**
	 * Busca en la base de datos sets cuyo título comience con el título dado
	 * @param titulo Título de set a buscar
	 * @param list Vector donde se almacenarán los datos
	 * @return true si se pudo realizar la operación, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool setFindTitle(const Glib::ustring& title, std::vector<MetaDbSet* >& list);

private:

	Sqlite m_db;					/**< Base de datos SQLite interna */
	Glib::ustring m_db_file;		/**< Path a la base de datos */
};

} // namespace gelide

#endif // _META_DB_HPP_
