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

Manufacturer* DbManager::manufacturerGet(const long long int id)
{
	SqliteStatement* stm;
	Manufacturer* element;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener el género
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Manufacturers\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, id);

	// Obtenemos el el elemento si existe en la bd
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new Manufacturer(stm->getColumnInt64(0), stm->getColumnText(1));
	}
	else
	{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::manufacturerAdd(Manufacturer* manufacturer)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(manufacturer);
	assert(!manufacturer->name.empty());

	// Preparamos el comando sql para la inserción y asociamos los valores
	stm = m_db.createStatement(
			"INSERT INTO Manufacturers (Name)\n"
			"VALUES (:name)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, manufacturer->name);
	ret = stm->step();
	stm->finalize();
	delete stm;

	// Si se ha añadido correctamente, obtenemos el identificador
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		manufacturer->id = m_db.getLastInsertId();
		return true;
	}
	return false;
}

bool DbManager::manufacturerUpdate(Manufacturer* manufacturer)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(manufacturer);
	// Evitamos modificación de elementos no válidos y del 1 = Desconocido
	assert(manufacturer->id > 1);
	assert(!manufacturer->name.empty());

	// En las actualizaciones, no se modifica el id
	stm = m_db.createStatement(
			"UPDATE Manufacturers\n"
			"SET Name = :name\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, manufacturer->name);
	stm->bind(2, manufacturer->id);
	ret = stm->step();
	stm->finalize();
	delete stm;

	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::manufacturerDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	// Evitamos eliminación de elementos no válidos y del 1 = Desconocido
	assert(id > 1);

	m_db.transactionBegin();
		// Preparamos el comando para eliminar el género
		stm = m_db.createStatement(
				"DELETE FROM Manufacturers\n"
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
				"SET ManufacturerId = 1\n"
				"WHERE ManufacturerId = :id"
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

	return true;
}

bool DbManager::manufacturerGetAll(std::vector<Manufacturer* >& list)
{
	SqliteStatement* stm;
	std::vector<Manufacturer* >::iterator iter;
	int ret;
	Manufacturer* element;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener los elementos
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Manufacturers\n"
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
		element = new Manufacturer(stm->getColumnInt64(0), stm->getColumnText(1));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

} // namespace gelide
