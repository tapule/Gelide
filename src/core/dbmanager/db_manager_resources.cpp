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

// Número de recursos fijos (no editables, no eliminables)
#define FIXED_RESOURCES	2

namespace gelide{

Resource* DbManager::resourceGet(const long long int id)
{
	SqliteStatement* stm;
	Resource* element;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener el género
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Resources\n"
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
		element = new Resource(stm->getColumnInt64(0), stm->getColumnText(1));
	}
	else
	{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::resourceAdd(Resource* resource)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(resource);
	assert(!resource->name.empty());

	// Preparamos el comando sql para la inserción y asociamos los valores
	stm = m_db.createStatement(
			"INSERT INTO Resources (Name)\n"
			"VALUES (:name)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, resource->name);
	ret = stm->step();
	stm->finalize();
	delete stm;

	// Si se ha añadido correctamente, obtenemos el identificador
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		resource->id = m_db.getLastInsertId();
		return true;
	}
	return false;
}

bool DbManager::resourceUpdate(Resource* resource)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(resource);
	// Evitamos modificación de elementos no válidos y de los recursos fijos
	assert(resource->id > FIXED_RESOURCES);
	assert(!resource->name.empty());

	// En las actualizaciones, no se modifica el id
	stm = m_db.createStatement(
			"UPDATE Resources\n"
			"SET Name = :name\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, resource->name);
	stm->bind(2, resource->id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::resourceDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	// Evitamos eliminación de elementos no válidos y de los recursos fijos
	assert(id > FIXED_RESOURCES);

	// Realizamos la operación en una transacción
	m_db.transactionBegin();
		// Preparamos el comando para eliminar el recurso
		stm = m_db.createStatement(
				"DELETE FROM Resources\n"
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

		// Preparamos el comando para eliminar los ficheros asociados al recurso
		if (!stm->prepare(
				"DELETE FROM ResourceEntries\n"
				"WHERE ResourceId = :id"
			)
		)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, id);
		// Ejecutamos
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

bool DbManager::resourceGetAll(std::vector<Resource* >& list){
	SqliteStatement* stm;
	std::vector<Resource* >::iterator iter;
	int ret;
	Resource* element;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener los elementos, ordenamos por Id para
	// que siempre se muestren los fijos por encima de los personalizados
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Resources\n"
			"ORDER BY Id"
	);
	if (!stm)
	{
		return false;
	}

	for(iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	// Obtenemos los elementos de la base de datos
	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		element = new Resource(stm->getColumnInt64(0), stm->getColumnText(1));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::resourceGetFiles(const long long int id, std::vector<Glib::ustring >& list)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);

	// Creamos el comando sql
	stm = m_db.createStatement(
			"SELECT File\n"
			"FROM ResourceEntries\n"
			"WHERE ResourceId = :pid"
			"ORDER BY File"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);

	list.clear();

	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		list.push_back(stm->getColumnText(0));
	}
	stm->finalize();
	delete stm;

	return true;
}

} // namespace gelide
