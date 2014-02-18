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

Type* DbManager::typeGet(const long long int id)
{
	SqliteStatement* stm;
	Type* element;

	assert(m_db.isOpen());

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Types\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new Type(stm->getColumnInt64(0), stm->getColumnText(1));
	}
	else
	{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::typeGetAll(std::vector<Type* >& list)
{
	SqliteStatement* stm;
	std::vector<Type* >::iterator iter;
	int ret;
	Type* element;

	assert(m_db.isOpen());

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Types\n"
			"ORDER BY Id"
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
		element = new Type(stm->getColumnInt64(0), stm->getColumnText(1));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

} // namespace gelide
