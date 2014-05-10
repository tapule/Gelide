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

#include "meta_db.hpp"
#include <cstdio>
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include "../../utils/utils.hpp"


namespace gelide{

MetaDb::MetaDb(void)
{
	LOG_INFO("MetaDB: DB Version: " << DATA_BASE_VERSION);
}

MetaDb::~MetaDb(void)
{
}

bool MetaDb::loadDataBase(const Glib::ustring& file)
{
	assert(!file.empty());

	LOG_INFO("MetaDB: Loading data base \"" << file << "\"...");
	if (m_db.open(file))
	{
		m_db_file = file;
		return true;
	}
	else
	{
		return false;
	}
}

bool MetaDb::createDataBase(const Glib::ustring& file)
{
	assert(!file.empty());

	LOG_INFO("MetaDB: Creating a new data base \"" << file << "\"...");
	if (!m_db.create(file))
	{
		return false;
	}
	m_db_file = file;

	return resetDataBase();
}

bool MetaDb::resetDataBase(void)
{

	assert(m_db.isOpen());

	// Eliminamos las tablas si existen
	m_db.transactionBegin();
		m_db.exec("DROP TABLE IF EXISTS Header");
		m_db.exec("DROP TABLE IF EXISTS Sets");
	m_db.transactionCommit();

	// Mantiene la información de versión de la base de datos
	m_db.exec(
			"CREATE TABLE Header (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Version            INTEGER NOT NULL\n"
			")"
	);

	// Almacena la información de los sets
	m_db.exec(
			"CREATE TABLE Sets (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT NOT NULL COLLATE NOCASE,\n"
			"System             TEXT NOT NULL COLLATE NOCASE,\n"

			"Type               INTEGER NOT NULL DEFAULT 0,\n"
			"Hash               TEXT,\n"

			"Title              TEXT,\n"
			"Manufacturer       TEXT,\n"
			"Year               TEXT,\n"
			"Genre              TEXT,\n"
			"Players            INTEGER NOT NULL DEFAULT 1,\n"

			"UNIQUE (Name, System),\n"
			"UNIQUE (Hash, System)\n"
			")"
	);

	// Insertamos los datos estáticos de la cabecera
	m_db.transactionBegin();

		// Tabla cabecera
		m_db.exec(
				"INSERT INTO Header (Id, Version)\n"
				"VALUES (1, " + utils::toStr(static_cast<int>(DATA_BASE_VERSION)) + ")"
		);

	m_db.transactionCommit();

	return true;
}

bool MetaDb::closeDataBase(void)
{
	//assert(m_db.isOpen());

	LOG_INFO("MetaDB: Closing data base \"" << m_db_file << "\"...");
	// Cerramos la base de datos
	return m_db.close();
}

bool MetaDb::getDataBaseHeader(int& version)
{
	SqliteStatement* stm;
	bool ret;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener la versión
	stm = m_db.createStatement(
			"SELECT Version \n"
			"FROM Header\n"
			"WHERE Id = 1"
	);
	if (!stm)
	{
		return false;
	}

	// Obtenemos el elemento si existe en la bd
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		version = stm->getColumnInt(0);
		ret = true;
	}
	else
	{
		ret = false;
	}

	stm->finalize();
	delete stm;
	return ret;
}

bool MetaDb::transactionBegin(void)
{
	assert(m_db.isOpen());
	return m_db.transactionBegin();
}

bool MetaDb::transactionCommit(void)
{
	assert(m_db.isOpen());
	return m_db.transactionCommit();
}

bool MetaDb::transactionRollBack(void)
{
	assert(m_db.isOpen());
	return m_db.transactionRollBack();
}

} // namespace gelide
