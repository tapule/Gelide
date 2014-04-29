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
#include "../../utils/utils.hpp"


namespace gelide{

MetaDbSet* MetaDb::setGetByName(const Glib::ustring& system, const Glib::ustring& name)
{
	SqliteStatement* stm;
	MetaDbSet* element;

	assert(m_db.isOpen());
	assert(!system.empty());
	assert(!name.empty());

	// Creamos el comando sql para obtener el juego
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE System = :system AND Name = :name"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, system);
	stm->bind(2, name);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->crc = stm->getColumnText(4);
		//element->title = stm->getColumnText(5);
		element->manufacturer = stm->getColumnText(6);
		element->year = stm->getColumnText(7);
		element->genre = stm->getColumnText(8);
		element->players = stm->getColumnInt(9);
	}
	else{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

MetaDbSet* MetaDb::setGetByCrc(const Glib::ustring& system, const Glib::ustring& crc)
{
	SqliteStatement* stm;
	MetaDbSet* element;

	assert(m_db.isOpen());
	assert(!system.empty());
	assert(!crc.empty());

	// Creamos el comando sql para obtener el juego
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE System = :system AND CRC = :crc"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, system);
	stm->bind(2, crc);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->crc = stm->getColumnText(4);
		//element->title = stm->getColumnText(5);
		element->manufacturer = stm->getColumnText(6);
		element->year = stm->getColumnText(7);
		element->genre = stm->getColumnText(8);
		element->players = stm->getColumnInt(9);
	}
	else{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool MetaDb::setAdd(MetaDbSet* set)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(set);
	assert(!set->name.empty());
	assert(!set->system.empty());
	assert(!set->crc.empty());

	stm = m_db.createStatement(
			"INSERT INTO Sets (Name, System, Type, CRC, Title, Manufacturer, Year, Genre, Players)\n"
			"VALUES (:name, :system, :type, :crc, :title, :manufacturer, :year, :genre, :players)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, set->name);
	stm->bind(2, set->system);
	stm->bind(3, set->type);
	stm->bind(4, set->crc);
	stm->bind(5, set->title);
	stm->bind(6, set->manufacturer);
	stm->bind(7, set->year);
	stm->bind(8, set->genre);
	stm->bind(9, set->players);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		set->id = m_db.getLastInsertId();
		return true;
	}

	return false;
}

bool MetaDb::setUpdate(MetaDbSet* set)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(set);
	assert(set->id);
	assert(!set->name.empty());
	assert(!set->system.empty());
	assert(!set->crc.empty());

	// En las actualizaciones, no se modifican:
	// Id, System, CRC
	stm = m_db.createStatement(
			"UPDATE Sets\n"
			"SET Name = :name, Type = :type, Title = :title, Manufacturer = :manufacturer, Year = :year, Genre = :genre, Players = :players\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, set->name);
	stm->bind(2, set->type);
	stm->bind(3, set->title);
	stm->bind(4, set->manufacturer);
	stm->bind(5, set->year);
	stm->bind(6, set->genre);
	stm->bind(7, set->players);
	stm->bind(8, set->id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}

	return false;
}

bool MetaDb::setDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(id);

	m_db.transactionBegin();
		// Preparamos el comando para eliminar el set
		stm = m_db.createStatement(
				"DELETE FROM Sets\n"
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
		delete stm;
	m_db.transactionCommit();

	return true;
}

bool MetaDb::setGetAll(std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	MetaDbSet* element;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener los sets
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"ORDER BY Title"
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

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->crc = stm->getColumnText(4);
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

bool MetaDb::setFindName(const Glib::ustring& name, std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	MetaDbSet* element;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;

	assert(m_db.isOpen());
	assert(!name.empty());

	// Creamos el comando sql para obtener los sets
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE Name = :name\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, name);

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->crc = stm->getColumnText(4);
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

bool MetaDb::setFindCrc(const Glib::ustring& crc, std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	MetaDbSet* element;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;

	assert(m_db.isOpen());
	assert(!crc.empty());

	// Creamos el comando sql para obtener los sets
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE CRC = :crc\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, crc);

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->crc = stm->getColumnText(4);
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

bool MetaDb::setFindTitle(const Glib::ustring& title, std::vector<MetaDbSet* >& list)
{
	SqliteStatement* stm;
	MetaDbSet* element;
	std::vector<MetaDbSet* >::iterator iter;
	int ret;

	assert(m_db.isOpen());
	assert(!title.empty());

	// Creamos el comando sql para obtener los sets
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Sets\n"
			"WHERE Title LIKE :title\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, title + "%");

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new MetaDbSet(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(5));
		element->system = stm->getColumnText(2);
		element->type = static_cast<SetType>(stm->getColumnInt(3));
		element->crc = stm->getColumnText(4);
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
