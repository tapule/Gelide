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


namespace gelide{

Genre* DbManager::genreGet(const long long int id)
{
	SqliteStatement* stm;
	Genre* element;

	assert(m_db.isOpen());

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Genres\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new Genre(stm->getColumnInt64(0), stm->getColumnText(1));
	}
	else
	{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::genreAdd(Genre* genre)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(genre);
	assert(!genre->name.empty());

	stm = m_db.createStatement(
			"INSERT INTO Genres (Name)\n"
			"VALUES (:name)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, genre->name);
	ret = stm->step();
	stm->finalize();
	delete stm;

	// Si se insertó el elemento, obtenemos su identificador
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		genre->id = m_db.getLastInsertId();
		return true;
	}
	return false;
}

bool DbManager::genreUpdate(Genre* genre)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(genre);
	// Evitamos modificación de elementos no válidos y del 1 = Desconocido
	assert(genre->id > 1);
	assert(!genre->name.empty());

	// En las actualizaciones, no se modifica el id
	stm = m_db.createStatement(
			"UPDATE Genres\n"
			"SET Name = :name\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, genre->name);
	stm->bind(2, genre->id);
	ret = stm->step();
	stm->finalize();
	delete stm;

	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::genreDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	// Evitamos eliminación de elementos no válidos y del 1 = Desconocido
	assert(id > 1);

	m_db.transactionBegin();
		// Preparamos el comando para eliminar el género
		stm = m_db.createStatement(
				"DELETE FROM Genres\n"
				"WHERE Id = :id"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, id);
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Creamos el comando sql para actualizar la tabla de juegos
		if (!stm->prepare(
				"UPDATE Games\n"
				"SET GenreId = 1\n"
				"WHERE GenreId = :id"
			)
		)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, id);
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();
		delete stm;
	m_db.transactionCommit();

	// Desactivamos el filtro activo si coincide con el que eliminamos
	if( (m_filter_type == DBFILTER_GENRE) && (m_filter_value == id))
	{
		m_filter_type = DBFILTER_NONE;
		m_filter_value = 0;
	}

	return true;
}

bool DbManager::genreGetAll(std::vector<Genre* >& list)
{
	SqliteStatement* stm;
	std::vector<Genre* >::iterator iter;
	int ret;
	Genre* element;

	assert(m_db.isOpen());

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Genres\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		element = new Genre(stm->getColumnInt64(0), stm->getColumnText(1));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

} // namespace gelide
