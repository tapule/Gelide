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

#include "db_manager.hpp"
#include <cstdio>
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include "../../utils/utils.hpp"


namespace gelide{

DbManager::DbManager(void)
{
	LOG_INFO("DBManager: DB Version: " << DATA_BASE_VERSION);
}

DbManager::~DbManager(void)
{
}

bool DbManager::loadDataBase(const Glib::ustring& file)
{
	assert(!file.empty());

	LOG_INFO("DBManager: Loading data base \"" << file << "\"...");
	if (m_db.open(file))
	{
		updateYearsTable();
		m_db_file = file;
		return true;
	}
	else
	{
		return false;
	}
}

bool DbManager::createDataBase(const Glib::ustring& file)
{
	assert(!file.empty());

	LOG_INFO("DBManager: Creating a new data base \"" << file << "\"...");
	if (!m_db.create(file))
	{
		return false;
	}
	m_db_file = file;

	return resetDataBase();
}

bool DbManager::resetDataBase(void)
{
	int i;
	unsigned int current_year;

	assert(m_db.isOpen());

	// Eliminamos las tablas si existen
	m_db.transactionBegin();
		m_db.exec("DROP TABLE IF EXISTS Header");					//X
		m_db.exec("DROP TABLE IF EXISTS Collections");				//X
		m_db.exec("DROP TABLE IF EXISTS Emulators");				//X
		m_db.exec("DROP TABLE IF EXISTS GamesLists");				//X
		m_db.exec("DROP TABLE IF EXISTS GamesListEntries");			//X
		m_db.exec("DROP TABLE IF EXISTS Games");
		m_db.exec("DROP TABLE IF EXISTS Tags");						//X
		m_db.exec("DROP TABLE IF EXISTS TagEntries");
		m_db.exec("DROP TABLE IF EXISTS Resources");				//X
		m_db.exec("DROP TABLE IF EXISTS ResourceEntries");
		m_db.exec("DROP TABLE IF EXISTS Manufacturers");			//X
		m_db.exec("DROP TABLE IF EXISTS Genres");					//X
		m_db.exec("DROP TABLE IF EXISTS Years");					//X
		m_db.exec("DROP TABLE IF EXISTS Players");					//X
		m_db.exec("DROP TABLE IF EXISTS Ratings");					//X
		m_db.exec("DROP TABLE IF EXISTS Letters");					//X
		m_db.exec("DROP TABLE IF EXISTS TimesPlayed");				//X
		m_db.exec("DROP TABLE IF EXISTS Types");					//X
		m_db.exec("DROP TABLE IF EXISTS States");					//X
	m_db.transactionCommit();

	// Mantiene la información de versión de la base de datos
	m_db.exec(
			"CREATE TABLE Header (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Version            INTEGER NOT NULL,\n"
			"GelideVersion     TEXT NOT NULL\n"
			")"
	);

	// Guarda la información de las colecciones del usuario
	m_db.exec(
			"CREATE TABLE Collections (\n"
			//Información interna
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE,\n"
			"SortOrder          INTEGER NOT NULL DEFAULT 0,\n"

			// Nombre del sistema en la metadb
			"SystemId           TEXT,\n"

			// Información general
			"Manufacturer       TEXT,\n"
			"Year               TEXT,\n"

			// Path de los iconos
			"Icon16             TEXT,\n"
			"Icon32             TEXT,\n"

			// Identificador del emulador
			"EmulatorId         INTEGER NOT NULL DEFAULT 0\n"
			")"
	);

	// Mantiene la información de los emuladores
	// Un emulador puede tener como padre: 0 = colección , 1 = lista de juegos,
	// 2 = juego
	m_db.exec(
			"CREATE TABLE Emulators (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"ParentId           INTEGER NOT NULL DEFAULT 0,\n"
			"ParentType         INTEGER NOT NULL DEFAULT 0,\n"
			"Binary             TEXT,\n"
			"Params             TEXT,\n"
			"PreCommand         TEXT,\n"
			"PostCommand        TEXT,\n"
			"RunDir             TEXT,\n"
			"UNIQUE (ParentId, ParentType)\n"
			")"
	);

	// Guarda la información de las listas de juegos
	m_db.exec(
			"CREATE TABLE GamesLists (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT NOT NULL COLLATE NOCASE,\n"

			// Información de emulador personalizado
			"UseCustomEmulator  INTEGER NOT NULL DEFAULT 0,\n"
			"EmulatorId         INTEGER NOT NULL DEFAULT 0\n"
			")"
	);

	// Almacena los juegos que pertenecen a cada lista
	m_db.exec(
			"CREATE TABLE GamesListEntries (\n"
			"GameListId         INTEGER NOT NULL DEFAULT 0,\n"
			"GameId             INTEGER NOT NULL DEFAULT 0,\n"
			"PRIMARY KEY (GameListId, GameId)\n"
			")"
	);

	// Almacena todos los juegos de cada colección
	m_db.exec(
			"CREATE TABLE Games (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT NOT NULL COLLATE NOCASE,\n"
			"CollectionId       INTEGER NOT NULL DEFAULT 0,\n"
			"State              INTEGER NOT NULL DEFAULT 0,\n"
			"File               TEXT,\n"

			"Type               INTEGER NOT NULL DEFAULT 0,\n"
			"CRC                TEXT,\n"

			"Title              TEXT,\n"
			"ManufacturerId     INTEGER NOT NULL DEFAULT 1,\n"
			"YearId             INTEGER NOT NULL DEFAULT 1,\n"
			"GenreId            INTEGER NOT NULL DEFAULT 1,\n"
			"Players            INTEGER NOT NULL DEFAULT 1,\n"

			"Rating             INTEGER NOT NULL DEFAULT 0,\n"
			"TimesPlayed        INTEGER NOT NULL DEFAULT 0,\n"
			"LastTimePlayed     TEXT,\n"
			"DateAdded          TEXT,\n"
			"Favorite           INTEGER NOT NULL DEFAULT 0,\n"

			// Información de emulador personalizado
			"UseCustomEmulator  INTEGER NOT NULL DEFAULT 0,\n"
			"EmulatorId         INTEGER NOT NULL DEFAULT 0,\n"
			"UNIQUE (Name, CollectionId)\n"
			")"
	);

	// Guarda la información de las etiquetas
	m_db.exec(
			"CREATE TABLE Tags (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena los juegos marcados con etiquetas
	m_db.exec(
			"CREATE TABLE TagEntries (\n"
			"TagId              INTEGER NOT NULL DEFAULT 0,\n"
			"GameId             INTEGER NOT NULL DEFAULT 0,\n"
			"PRIMARY KEY (TagId, GameId)\n"
			")"
	);

	// Mantiene la información de los recursos del usuario
	m_db.exec(
			"CREATE TABLE Resources (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena los ficheros vinculados a colecciones y juegos para cada recurso
	// Un fichero de recurso puede tener como padre: 0 = colección , 1 = juego
	m_db.exec(
			"CREATE TABLE ResourceEntries (\n"
			"ResourceId         INTEGER NOT NULL DEFAULT 0,\n"
			"ParentId           INTEGER NOT NULL DEFAULT 0,\n"
			"ParentType         INTEGER NOT NULL DEFAULT 0,\n"
			"File               Text,\n"
			"PRIMARY KEY (ResourceId, ParentId, ParentType)\n"
			")"
	);

	// Almacena la lista de fabricantes
	m_db.exec(
			"CREATE TABLE Manufacturers (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena la lista de géneros
	m_db.exec(
			"CREATE TABLE Genres (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena la lista de años
	m_db.exec(
			"CREATE TABLE Years (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena la lista de números de jugadores
	m_db.exec(
			"CREATE TABLE Players (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena la lista de las puntuaciones
	m_db.exec(
			"CREATE TABLE Ratings (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena la lista de letras disponibles para filtrar
	m_db.exec(
			"CREATE TABLE Letters (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena los filtros según el número de partidas jugadas
	m_db.exec(
			"CREATE TABLE TimesPlayed (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena los posibles tipos de juegos
	m_db.exec(
			"CREATE TABLE Types (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);

	// Almacena los posibles estados de un juego
	m_db.exec(
			"CREATE TABLE States (\n"
			"Id                 INTEGER PRIMARY KEY,\n"
			"Name               TEXT UNIQUE NOT NULL COLLATE NOCASE\n"
			")"
	);


	// Insertamos los datos estáticos en las diferentes tablas
	m_db.transactionBegin();

		// Tabla cabecera
		m_db.exec(
				"INSERT INTO Header (Id, Version, GelideVersion)\n"
				"VALUES (1, " + utils::toStr(static_cast<int>(DATA_BASE_VERSION)) + ", '" + PACKAGE_VERSION + "')"
		);

		// Tabla de etiquetas
		m_db.exec(
				"INSERT INTO Tags (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>( _("80''s")) +  "')"
		);
		m_db.exec(
				"INSERT INTO Tags (Id, Name)\n"
				"VALUES (2, '" + static_cast<Glib::ustring>( _("90''s")) +  "')"
		);
		m_db.exec(
				"INSERT INTO Tags (Id, Name)\n"
				"VALUES (3, '" + static_cast<Glib::ustring>( _("I Like!")) +  "')"
		);

		// Tabla de recursos, metemos los fijos
		m_db.exec(
				"INSERT INTO Resources (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>("boxes") +  "')"
		);
		m_db.exec(
				"INSERT INTO Resources (Id, Name)\n"
				"VALUES (2, '" + static_cast<Glib::ustring>("snaps") +  "')"
		);

		// Tabla fabricantes
		m_db.exec(
				"INSERT INTO Manufacturers (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>( _("Unknown")) +  "')"
		);

		// Tabla géneros
		m_db.exec(
				"INSERT INTO Genres (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>( _("Unknown")) +  "')"
		);

		// Tabla de años (Unknown, 1975..[current])
		m_db.exec(
				"INSERT INTO Years (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>( _("Unknown")) +  "')"
		);
		current_year = utils::getYear();
		for (i = 1; i <= static_cast<int>(current_year - 1974); ++i)
		{
			m_db.exec(
					"INSERT INTO Years (Id, Name)\n"
					"VALUES (" + utils::toStr(i + 1) + ", '" + utils::toStr(i + 1974) + "')"
			);
		}

		// Tabla juagadores
		for (i = 1; i <= 8; ++i)
		{
			m_db.exec(
					"INSERT INTO Players (Id, Name)\n"
					"VALUES (" + utils::toStr(i) + ", '" + utils::toStr(i) + "')"
			);
		}

		// Tabla puntuaciones
		for (i = 1; i <= 6; ++i)
		{
			m_db.exec(
					"INSERT INTO Ratings (Id, Name)\n"
					"VALUES (" + utils::toStr(i) + ", '" + utils::toStr(i - 1) + "')"
			);
		}

		// Tabla para el filtro por letras
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (1, '0')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (2, '1')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (3, '2')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (4, '3')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (5, '4')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (6, '5')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (7, '6')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (8, '7')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (9, '8')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (10, '9')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (11, 'A')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (12, 'B')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (13, 'C')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (14, 'D')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (15, 'E')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (16, 'F')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (17, 'G')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (18, 'H')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (19, 'I')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (20, 'J')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (21, 'K')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (22, 'L')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (23, 'M')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (24, 'N')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (25, 'O')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (26, 'P')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (27, 'Q')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (28, 'R')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (29, 'S')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (30, 'T')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (31, 'U')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (32, 'V')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (33, 'W')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (34, 'X')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (35, 'Y')"
		);
		m_db.exec(
				"INSERT INTO Letters (Id, Name)\n"
				"VALUES (36, 'Z')"
		);

		// Tabla para el filtro por estado de jugado
		m_db.exec(
				"INSERT INTO TimesPlayed (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>( _("None")) +  "')"
		);
		m_db.exec(
				"INSERT INTO TimesPlayed (Id, Name)\n"
				"VALUES (2, '" + static_cast<Glib::ustring>( _("One")) +  "')"
		);
		m_db.exec(
				"INSERT INTO TimesPlayed (Id, Name)\n"
				"VALUES (3, '" + static_cast<Glib::ustring>( _("More than one")) +  "')"
		);

		// Tabla para el filtro por tipo de juego
		m_db.exec(
				"INSERT INTO Types (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>( _("Original")) +  "')"
		);
		m_db.exec(
				"INSERT INTO Types (Id, Name)\n"
				"VALUES (2, '" + static_cast<Glib::ustring>( _("Clone")) +  "')"
		);
		m_db.exec(
				"INSERT INTO Types (Id, Name)\n"
				"VALUES (3, '" + static_cast<Glib::ustring>( _("Bios")) +  "')"
		);

		// Tabla para el filtro por stado del juego
		m_db.exec(
				"INSERT INTO States (Id, Name)\n"
				"VALUES (1, '" + static_cast<Glib::ustring>( _("Correct")) +  "')"
		);
		m_db.exec(
				"INSERT INTO States (Id, Name)\n"
				"VALUES (2, '" + static_cast<Glib::ustring>( _("Incorrect")) +  "')"
		);
	m_db.transactionCommit();

	return true;
}

bool DbManager::closeDataBase(void)
{
	assert(m_db.isOpen());

	LOG_INFO("DBManager: Closing data base \"" << m_db_file << "\"...");
	// Cerramos la base de datos
	return m_db.close();
}

bool DbManager::getDataBaseHeader(int& version, Glib::ustring& gelide_version)
{
	SqliteStatement* stm;
	bool ret;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener el último año
	stm = m_db.createStatement(
			"SELECT Version, GelideVersion \n"
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
		gelide_version = stm->getColumnText(1);
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

bool DbManager::transactionBegin(void)
{
	assert(m_db.isOpen());
	return m_db.transactionBegin();
}

bool DbManager::transactionCommit(void)
{
	assert(m_db.isOpen());
	return m_db.transactionCommit();
}

bool DbManager::transactionRollBack(void)
{
	assert(m_db.isOpen());
	return m_db.transactionRollBack();
}

bool DbManager::updateYearsTable(void)
{
	SqliteStatement* stm;
	bool ret;
	unsigned int current_year;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener el último año
	stm = m_db.createStatement(
			"SELECT Name \n"
			"FROM Years\n"
			"ORDER BY Id DESC\n"
			"LIMIT 1"
	);
	if (!stm)
	{
		return false;
	}

	// Obtenemos el elemento si existe en la bd
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		current_year = utils::getYear();
		// Si es necesario insertamos el año actual
		if (stm->getColumnInt(0) < current_year)
		{
			m_db.exec(
					"INSERT INTO Years (Name)\n"
					"VALUES ('" + utils::toStr(current_year) + "')"
			);
		}
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

bool DbManager::emptyTable(const Glib::ustring& table)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(!table.empty());

	// Preparamos el comando para eliminar
	stm = m_db.createStatement("DELETE FROM " + table);
	if (!stm)
	{
		return false;
	}
	if (stm->step() != SqliteStatement::STATEMENT_DONE)
	{
		stm->finalize();
		delete stm;
		return false;
	}
	stm->finalize();
	delete stm;

	return true;
}

unsigned int DbManager::countTable(const Glib::ustring& table)
{
	SqliteStatement* stm;
	unsigned int ret;

	assert(m_db.isOpen());
	assert(!table.empty());

	// Preparamos el comando para realizar el recuento
	stm = m_db.createStatement(
			"SELECT COUNT(1)\n"
			"FROM " + table
	);
	if (!stm)
	{
		return false;
	}

	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		ret = stm->getColumnInt(0);
	}
	else
	{
		ret = 0;
	}
	stm->finalize();
	delete stm;

	return ret;
}

Glib::ustring DbManager::parseFiltersVector(std::vector<Filter >& filters)
{
	std::vector<Filter >::iterator iter;
	Glib::ustring query;

	query = "";

	for (iter = filters.begin(); iter != filters.end(); ++iter)
	{
		switch (iter->type)
		{
		case FILTER_FAVORITE:
			query += " AND Games.Favorite = " + utils::toStr(iter->value);
			break;
		case FILTER_GENRE:
			query += " AND Games.GenreId = " + utils::toStr(iter->value);
			break;
		case FILTER_LETTER:
			query += " AND Games.Title LIKE '" + letterGet(iter->value)->name + "%'";
			break;
		case FILTER_MANUFACTURER:
			query += " AND Games.ManufacturerId = " + utils::toStr(iter->value);
			break;
		case FILTER_PLAYERS:
			query += " AND Games.Players = " + playersGet(iter->value)->name;
			break;
		case FILTER_RATING:
			query += " AND Games.Rating = " + ratingGet(iter->value)->name;
			break;
		case FILTER_STATE:
			query += " AND Games.State = " + utils::toStr(iter->value);
			break;
		case FILTER_TAG:
			query += " AND Games.Id IN (\n"
					 "   SELECT GameId\n"
					 "   FROM TagEntries\n"
					 "   WHERE TagId = " + utils::toStr(iter->value) + "\n"
					 ")";
			break;
		case FILTER_TIMES_PLAYED:
			switch (iter->value)
			{
			// Ninguna
			case 0:
			// Una
			case 1:
				query += " AND Games.TimesPlayed = 1";
				break;
			// Más de 1
			default:
				query += " AND Games.TimesPlayed > 1";
				break;
			}
			break;
		case FILTER_GAME_TYPE:
			query += " AND Games.Type = " + utils::toStr(iter->value);
			break;
		case FILTER_YEAR:
			query += " AND Games.YearId = " + utils::toStr(iter->value);
			break;
		}
	}

	return query;
}

} // namespace gelide
