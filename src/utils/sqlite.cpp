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

#include "sqlite.hpp"


Sqlite::Sqlite(void):
	m_db(NULL)
{
}

Sqlite::Sqlite(const Glib::ustring& file):
		m_db(NULL)
{
	open(file);
}

Sqlite::~Sqlite()
{
	close();
}

bool Sqlite::open(const Glib::ustring& file)
{
	if (m_db)
	{
		close();
	}
	if (sqlite3_open_v2(file.c_str(), &m_db, SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK)
	{
		return true;
	}
	else
	{
		LOG_ERROR("SQLite: Can't open db \""  << file << "\"");
		close();
		return false;
	}
}

bool Sqlite::create(const Glib::ustring& file)
{
	if(m_db)
	{
		close();
	}
	if (sqlite3_open_v2(file.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) == SQLITE_OK)
	{
		return true;
	}
	else
	{
		LOG_ERROR("SQLite: Can't create SQLite db \""  << file << "\"");
		close();
		return false;
	}
}

bool Sqlite::close(void)
{
	if (sqlite3_close(m_db) == SQLITE_OK)
	{
		m_db = NULL;
		return true;
	}
	return false;
}

sqlite_int64 Sqlite::getLastInsertId(void)
{
	return sqlite3_last_insert_rowid(m_db);
}

bool Sqlite::exec(const Glib::ustring& query)
{
	char* err = NULL;
	bool ret = false;

	assert(m_db);

	if (sqlite3_exec(m_db, query.c_str(), NULL, NULL, &err) == SQLITE_OK)
	{
		ret = true;
	}
	if (err != NULL)
	{
		LOG_ERROR("SQLite: " << err);
		sqlite3_free(err);
	}
	return ret;
}

bool Sqlite::transactionBegin(void)
{
	return exec("BEGIN");
}

bool Sqlite::transactionCommit(void)
{
	return exec("COMMIT");
}

bool Sqlite::transactionRollBack(void)
{
	return exec("ROLLBACK");
}

SqliteStatement* Sqlite::createStatement(const Glib::ustring& query)
{
	SqliteStatement* stmt = NULL;

	stmt = new SqliteStatement(m_db);
	if (!stmt->prepare(query))
	{
		delete stmt;
		return NULL;
	}
	return stmt;
}
