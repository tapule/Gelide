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

#include "sqlite_statement.hpp"


SqliteStatement::SqliteStatement(void):
	m_db(NULL),
	m_stmt(NULL)
{
}

SqliteStatement::SqliteStatement(sqlite3* db):
	m_db(db),
	m_stmt(NULL)
{
}

SqliteStatement::~SqliteStatement()
{
	sqlite3_finalize(m_stmt);
}

bool SqliteStatement::prepare(const Glib::ustring& query)
{
	bool ret = false;

	assert(m_db);

	finalize();
	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &m_stmt, NULL) == SQLITE_OK)
	{
		ret = true;
	}
	else
	{
		LOG_ERROR("SQLite: \"" << query << "\" " << sqlite3_errmsg(m_db));
	}
	return ret;
}

SqliteStatement::StatementResult SqliteStatement::step(void)
{
	int result;

	assert(m_db);

	result =  sqlite3_step(m_stmt);

	if (result == SQLITE_ROW)
	{
		return STATEMENT_ROW;
	}
	if (result == SQLITE_DONE)
	{
		return STATEMENT_DONE;
	}

	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return STATEMENT_ERROR;
}

bool SqliteStatement::reset(void)
{

	assert(m_db);

	if (sqlite3_reset(m_stmt)  == SQLITE_OK)
	{
		return true;
	}

	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}

bool SqliteStatement::finalize(void)
{

	assert(m_db);

	if (sqlite3_finalize(m_stmt)  == SQLITE_OK)
	{
		m_stmt = NULL;
		return true;
	}
	m_stmt = NULL;
	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}

int SqliteStatement::getColumnCount(void)
{
	return sqlite3_column_count(m_stmt);
}

Glib::ustring SqliteStatement::getColumnName(const int index)
{
	const char* name;

	name = sqlite3_column_name(m_stmt, index);

	return (name == NULL ? "" : name);
}

bool SqliteStatement::getColumnBool(const int index)
{
	return (getColumnInt(index) == 0 ? false : true);
}

int SqliteStatement::getColumnInt(const int index)
{
	return sqlite3_column_int(m_stmt, index);
}

sqlite3_int64 SqliteStatement::getColumnInt64(const int index)
{
	return sqlite3_column_int64(m_stmt, index);
}

double SqliteStatement::getColumnDouble(const int index)
{
	return sqlite3_column_double(m_stmt, index);
}

Glib::ustring SqliteStatement::getColumnText(const int index)
{
	const char* text;

	text = reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, index));
	//text = sqlite3_column_text(m_stmt, index);

	return (text == NULL ? "" : text);
}

const void* SqliteStatement::getColumnBlob(const int index)
{
	return sqlite3_column_blob(m_stmt, index);
}

bool SqliteStatement::bind(const int index, const bool value)
{
	return bind(index, static_cast<int>(value));
}

bool SqliteStatement::bind(const int index, const int value)
{
	if (sqlite3_bind_int(m_stmt, index, value) == SQLITE_OK)
	{
		return true;
	}
	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}

bool SqliteStatement::bind(const int index, const sqlite_int64 value)
{
	if (sqlite3_bind_int64(m_stmt, index, value) == SQLITE_OK)
	{
		return true;
	}
	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}

bool SqliteStatement::bind(const int index, const double value)
{
	if (sqlite3_bind_double(m_stmt, index, value) == SQLITE_OK)
	{
		return true;
	}
	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}

bool SqliteStatement::bind(const int index, const Glib::ustring& value)
{
	if (sqlite3_bind_text(m_stmt, index, value.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
	{
		return true;
	}
	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}

bool SqliteStatement::bind(const int index, const void* value)
{
	if (sqlite3_bind_blob(m_stmt, index, value, -1, SQLITE_TRANSIENT) == SQLITE_OK)
	{
		return true;
	}
	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}

bool SqliteStatement::bind(const int index)
{
	if (sqlite3_bind_null(m_stmt, index) == SQLITE_OK)
	{
		return true;
	}
	LOG_ERROR("SQLite: " << sqlite3_errmsg(m_db));
	return false;
}
