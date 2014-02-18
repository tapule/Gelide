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

Emulator* DbManager::emulatorGet(const long long int id)
{
	SqliteStatement* stm;
	Emulator* element;

	assert(m_db.isOpen());

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Emulators\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new Emulator();
		element->id = stm->getColumnInt64(0);
		element->parent_id = stm->getColumnInt64(1);
		element->parent_type = static_cast<EmulatorParentType>(stm->getColumnInt(2));
		element->binary = stm->getColumnText(3);
		element->params = stm->getColumnText(4);
		element->precommand = stm->getColumnText(5);
		element->postcommad = stm->getColumnText(6);
		element->rundir = stm->getColumnText(7);
	}
	else
	{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::emulatorAdd(Emulator* emulator)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(emulator);

	stm = m_db.createStatement(
			"INSERT INTO Emulators (ParentId, ParentType, Binary, Params, PreCommand, PostCommand, RunDir)\n"
			"VALUES (:parentid, :parenttype, :binary, :params, :pre, :post, :rundir)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, emulator->parent_id);
	stm->bind(2, emulator->parent_type);
	stm->bind(3, emulator->binary);
	stm->bind(4, emulator->params);
	stm->bind(5, emulator->precommand);
	stm->bind(6, emulator->postcommad);
	stm->bind(7, emulator->rundir);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		emulator->id = m_db.getLastInsertId();
		return true;
	}
	return false;
}

bool DbManager::emulatorUpdate(Emulator* emulator)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(emulator);

	// En las actualizaciones, no se modifica el id
	stm = m_db.createStatement(
			"UPDATE Emulators\n"
			"SET ParentId = :pid, ParentType = :type, Binary = :binary, Params = :params, PreCommand = :pre, PostCommand = :post, RunDir = :rundir\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, emulator->parent_id);
	stm->bind(2, emulator->parent_type);
	stm->bind(3, emulator->binary);
	stm->bind(4, emulator->params);
	stm->bind(5, emulator->precommand);
	stm->bind(6, emulator->postcommad);
	stm->bind(7, emulator->rundir);
	stm->bind(8, emulator->id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::emulatorDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());

	// Normalmente los emuladores no se van a eliminar directamente, por ello,
	// no se actualizan objetos vinculados con ellos (Colecciones, Juegos,
	// Listas de juegos)

	m_db.transactionBegin();
		stm = m_db.createStatement(
				"DELETE FROM Emulators\n"
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

unsigned int DbManager::emulatorCount(void)
{
	return countTable("Emulators");
}

} // namespace gelide
