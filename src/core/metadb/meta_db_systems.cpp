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


namespace gelide{


bool MetaDb::systemDelete(const Glib::ustring& system)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(!system.empty());

	// Eliminar un sistema consiste en eliminar todos sus juegos

	m_db.transactionBegin();
		// Preparamos el comando para eliminar
		stm = m_db.createStatement(
				"DELETE FROM Sets\n"
				"WHERE System = :system"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, system);
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

bool MetaDb::systemGetAll(std::vector<Glib::ustring>& list)
{
	SqliteStatement* stm;
	int ret;
	Glib::ustring element;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener los elementos
	stm = m_db.createStatement(
			"SELECT DISTINCT System\n"
			"FROM Sets\n"
			"ORDER BY System"
	);
	if (!stm)
	{
		return false;
	}
	list.clear();

	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		element = stm->getColumnText(0);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool MetaDb::systemGetSets(const Glib::ustring& system,	std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;
	MetaDbSet* element;

	Glib::ustring query, where, order;
	Glib::ustring title;

	assert(m_db.isOpen());
	assert(!system.empty());

	// Creamos el comando sql para obtener los elementos
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE System = :system\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, system);

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->hash = stm->getColumnText(4);
		//element->title = stm->getColumnText(5);
		element->manufacturer = stm->getColumnText(6);
		element->year = stm->getColumnText(7);
		element->genre = stm->getColumnText(8);
		element->players = stm->getColumnInt(9);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool MetaDb::systemFindSetsName(const Glib::ustring& system, const Glib::ustring& name, std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	MetaDbSet* element;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;

	assert(m_db.isOpen());
	assert(!system.empty());
	assert(!name.empty());

	// Creamos el comando sql para obtener los sets
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE System = :system AND Name = :name\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, system);
	stm->bind(2, name);

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->hash = stm->getColumnText(4);
		//element->title = stm->getColumnText(5);
		element->manufacturer = stm->getColumnText(6);
		element->year = stm->getColumnText(7);
		element->genre = stm->getColumnText(8);
		element->players = stm->getColumnInt(9);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool MetaDb::systemFindSetsHash(const Glib::ustring& system, const Glib::ustring& hash, std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	MetaDbSet* element;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;

	assert(m_db.isOpen());
	assert(!system.empty());
	assert(!hash.empty());

	// Creamos el comando sql para obtener los sets
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE System = :system AND Hash = :hash\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, system);
	stm->bind(2, hash.lowercase());

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->hash = stm->getColumnText(4);
		//element->title = stm->getColumnText(5);
		element->manufacturer = stm->getColumnText(6);
		element->year = stm->getColumnText(7);
		element->genre = stm->getColumnText(8);
		element->players = stm->getColumnInt(9);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool MetaDb::systemFindSetsTitle(const Glib::ustring& system, const Glib::ustring& title, std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	MetaDbSet* element;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;

	assert(m_db.isOpen());
	assert(!system.empty());
	assert(!title.empty());

	// Creamos el comando sql para obtener los sets
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE System = :system AND Title LIKE :title\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, system);
	stm->bind(2, title + "%");

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->hash = stm->getColumnText(4);
		//element->title = stm->getColumnText(5);
		element->manufacturer = stm->getColumnText(6);
		element->year = stm->getColumnText(7);
		element->genre = stm->getColumnText(8);
		element->players = stm->getColumnInt(9);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

} // namespace gelide
