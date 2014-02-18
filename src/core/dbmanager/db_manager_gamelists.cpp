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
#include <map>
#include "../../utils/utils.hpp"
#include "../../utils/xml_reader.hpp"


namespace gelide{

GameList* DbManager::gameListGet(const long long int id)
{
	SqliteStatement* stm;
	GameList* element;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM GamesLists\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new GameList(stm->getColumnInt64(0), stm->getColumnText(1));
		element->use_custom_emulator = stm->getColumnBool(2);
		element->emulator_id = stm->getColumnInt64(3);
	}
	else
	{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::gameListAdd(GameList* list)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(list);
	assert(!list->name.empty());

	stm = m_db.createStatement(
			"INSERT INTO GamesLists (Name, UseCustomEmulator, EmulatorId)\n"
			"VALUES (:name, :usecustom, :emuid)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, list->name);
	stm->bind(2, list->use_custom_emulator);
	stm->bind(3, list->emulator_id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		list->id = m_db.getLastInsertId();
		return true;
	}
	return false;
}

bool DbManager::gameListUpdate(GameList* list)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(list);
	assert(list->id);
	assert(!list->name.empty());

	// En las actualizaciones, no se modifica el id
	stm = m_db.createStatement(
			"UPDATE GamesLists\n"
			"SET Name = :name, UseCustomEmulator = :custemu, EmulatorId = :emuid\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, list->name);
	stm->bind(2, list->use_custom_emulator);
	stm->bind(3, list->emulator_id);
	stm->bind(4, list->id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}

	return false;
}

bool DbManager::gameListDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(id);

	m_db.transactionBegin();
		// Preparamos el comando para eliminar la lista de juegos
		stm = m_db.createStatement(
				"DELETE FROM GamesLists\n"
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

		// Preparamos el comando para eliminar los juegos de la lista
		if (!stm->prepare(
				"DELETE FROM GamesListEntries\n"
				"WHERE GamelistId = :id"
			)
		)
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

		// Preparamos el comando para eliminar el emulador de la lista
		if (!stm->prepare(
				"DELETE FROM Emulators\n"
				"WHERE ParentId = :id AND ParentType = :type"
			)
		)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, id);
		stm->bind(2, EMULATOR_PARENT_GAMELIST);
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

bool DbManager::gamelistGetAll(std::vector<GameList* >& list)
{
	SqliteStatement* stm;
	std::vector<GameList* >::iterator iter;
	int ret;
	GameList* element;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener los elementos
	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM GamesLists\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}

	for (iter = list.begin(); iter!= list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		element = new GameList(stm->getColumnInt64(0), stm->getColumnText(1));
		element->use_custom_emulator = stm->getColumnBool(2);
		element->emulator_id = stm->getColumnInt64(3);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::gamelistGetAll(std::vector<Item* >& list)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	int ret;
	Item* element;

	assert(m_db.isOpen());

	// Creamos el comando sql para obtener los elementos
	stm = m_db.createStatement(
			"SELECT Id, Name \n"
			"FROM GamesLists\n"
			"ORDER BY Name"
	);
	if (!stm)
	{
		return false;
	}

	for (iter = list.begin(); iter!= list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		element = new Item(stm->getColumnInt64(0), stm->getColumnText(1));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

unsigned int DbManager::gameListCount(void)
{
	return countTable("GamesLists");
}

bool DbManager::gameListGetGames(const long long int id, std::vector<Game* >& list, const bool filtered)
{
	SqliteStatement* stm;
	std::vector<Game* >::iterator iter;
	int ret;
	Game* element;
	Glib::ustring query, where, order;
	int i;
	Glib::ustring title;

	assert(m_db.isOpen());
	assert(id);

	// Generamos las clausulas iniciales de la consulta
	query = "SELECT Games.*, Manufacturers.Name, Years.Name, Genres.Name\n"
			"FROM Games, Manufacturers, Years, Genres\n";
	where = "WHERE Games.Enabled = 1 AND Games.ManufacturerId = Manufacturers.Id AND Games.YearId = Years.Id AND Games.GenreId = Genres.Id AND Games.Id IN (\n"
			"   SELECT GameId\n"
			"   FROM GamesListEntries\n"
			"   WHERE GamelistId = " + utils::toStr(id) + "\n"
			")";
	order = "ORDER BY Games.Title ASC\n";

	// Agregamos opciones de filtrado si corresponde
	if (filtered)
	{
		if (m_only_favorites)
		{
			where += " AND Games.Favorite = 1";
		}

		// Comprobamos el filtrado por título
		if (!m_filter_title.empty())
		{
			title = m_filter_title;
			// Parseamos los * internos
			i = title.find("*");
			while (i > -1)
			{
				title = title.replace(i, 1, "%");
				i = title.find("*", i + 1);
			}
			// Parseamos los ? internos
			i = title.find("?");
			while (i > -1)
			{
				title = title.replace(i, 1, "_");
				i = title.find("?", i + 1);
			}
			title += "%";
			where += " AND Games.Title LIKE ':title'";
		}

		switch (m_filter_type)
		{
		case DBFILTER_GENRE:
			where += " AND Games.GenreId = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_LETTER:
			where += " AND Games.Title LIKE '" + letterGet(m_filter_value)->name + "%'";
			break;
		case DBFILTER_MANUFACTURER:
			where += " AND Games.ManufacturerId = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_PLAYERS:
			where += " AND Games.Players = " + playersGet(m_filter_value)->name;
			break;
		case DBFILTER_RATING:
			where += " AND Games.Rating = " + ratingGet(m_filter_value)->name;
			break;
		case DBFILTER_STATE:
			where += " AND Games.State = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_TAG:
			where += " AND Games.Id IN (\n"
					 "   SELECT GameId\n"
					 "   FROM TagEntries\n"
					 "   WHERE TagId = " + utils::toStr(m_filter_value) + "\n"
					 ")";
			break;
		case DBFILTER_TIMES_PLAYED:
			switch (m_filter_value)
			{
			// Ninguna
			case 0:
			// Una
			case 1:
				where += " AND Games.TimesPlayed = " + utils::toStr(m_filter_value);
				break;
			// Más de 1
			default:
				where += " AND Games.TimesPlayed > 1";
				break;
			}
			break;
		case DBFILTER_GAME_TYPE:
			where += " AND Games.Type = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_YEAR:
			where += " AND Games.YearId = " + utils::toStr(m_filter_value);
			break;
		}
	}
	where += "\n";

	// Creamos el comando sql para hacer la consulta final
	stm = m_db.createStatement(query + where + order);
	if (!stm)
	{
		return false;
	}
	// Si se está filtrando y hay filtro de título, hacemos un bind
	// para evitar sql injection
	if (!m_filter_title.empty() && filtered)
	{
		stm->bind(1, title);
	}

	for(iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new Game(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(8));
		element->enabled = stm->getColumnBool(2);
		element->collection_id = stm->getColumnInt64(3);
		element->state = static_cast<GameState>(stm->getColumnInt(4));
		element->file = stm->getColumnText(5);
		element->type = static_cast<GameType>(stm->getColumnInt(6));
		element->crc = stm->getColumnText(7);
		//element->title = stm->getColumnText(8);
		element->manufacturer_id = stm->getColumnInt64(9);
		element->year_id = stm->getColumnInt64(10);
		element->genre_id = stm->getColumnInt64(11);
		element->players = stm->getColumnInt(12);
		element->rating = stm->getColumnInt(13);
		element->times_played = stm->getColumnInt(15);
		element->last_time_played = stm->getColumnText(15);
		element->date_added = stm->getColumnText(16);
		element->favorite = stm->getColumnBool(17);
		element->use_custom_emulator = stm->getColumnBool(18);
		element->emulator_id = stm->getColumnInt64(19);
		element->manufacturer = stm->getColumnText(20);
		element->year = stm->getColumnText(21);
		element->genre = stm->getColumnText(22);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::gameListGetGames(const long long int id, std::vector<Item* >& list, const bool filtered)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	int ret;
	Item* element;
	Glib::ustring query, where, order;
	int i;
	Glib::ustring title;

	assert(m_db.isOpen());
	assert(id);

	// Generamos las clausulas iniciales de la consulta
	query = "SELECT Games.Id, Games.Name, Games.Title\n"
			"FROM Games\n";
	where = "WHERE Games.Enabled = 1 AND Games.Id IN (\n"
			"   SELECT GameId\n"
			"   FROM GamesListEntries\n"
			"   WHERE GamelistId = " + utils::toStr(id) + "\n"
			")";
	order = "ORDER BY Games.Title ASC\n";

	// Agregamos opciones de filtrado si corresponde
	if (filtered)
	{
		if (m_only_favorites)
		{
			where += " AND Games.Favorite = 1";
		}

		// Comprobamos el filtrado por título
		if (!m_filter_title.empty())
		{
			title = m_filter_title;
			// Parseamos los * internos
			i = title.find("*");
			while (i > -1)
			{
				title = title.replace(i, 1, "%");
				i = title.find("*", i + 1);
			}
			// Parseamos los ? internos
			i = title.find("?");
			while (i > -1)
			{
				title = title.replace(i, 1, "_");
				i = title.find("?", i + 1);
			}
			title += "%";
			where += " AND Games.Title LIKE ':title'";
		}

		switch (m_filter_type)
		{
		case DBFILTER_GENRE:
			where += " AND Games.GenreId = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_LETTER:
			where += " AND Games.Title LIKE '" + letterGet(m_filter_value)->name + "%'";
			break;
		case DBFILTER_MANUFACTURER:
			where += " AND Games.ManufacturerId = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_PLAYERS:
			where += " AND Games.Players = " + playersGet(m_filter_value)->name;
			break;
		case DBFILTER_RATING:
			where += " AND Games.Rating = " + ratingGet(m_filter_value)->name;
			break;
		case DBFILTER_STATE:
			where += " AND Games.State = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_TAG:
			where += " AND Games.Id IN (\n"
					 "   SELECT GameId\n"
					 "   FROM TagEntries\n"
					 "   WHERE TagId = " + utils::toStr(m_filter_value) + "\n"
					 ")";
			break;
		case DBFILTER_TIMES_PLAYED:
			switch (m_filter_value)
			{
			// Ninguna
			case 0:
			// Una
			case 1:
				where += " AND Games.TimesPlayed = " + utils::toStr(m_filter_value);
				break;
			// Más de 1
			default:
				where += " AND Games.TimesPlayed > 1";
				break;
			}
			break;
		case DBFILTER_GAME_TYPE:
			where += " AND Games.Type = " + utils::toStr(m_filter_value);
			break;
		case DBFILTER_YEAR:
			where += " AND Games.YearId = " + utils::toStr(m_filter_value);
			break;
		}
	}
	where += "\n";

	// Creamos el comando sql para hacer la consulta final
	stm = m_db.createStatement(query + where + order);
	if (!stm)
	{
		return false;
	}
	// Si se está filtrando y hay filtro de título, hacemos un bind
	// para evitar sql injection
	if (!m_filter_title.empty() && filtered)
	{
		stm->bind(1, title);
	}

	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		delete (*iter);
	}
	list.clear();

	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		element = new Item(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(2));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::gameListAddGame(const long long int id, const long long int game_id)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);
	assert(game_id);

	// Preparamos el comando sql para la inserción y asociamos los valores
	// Si la entrada existe, la ignoramos
	stm = m_db.createStatement(
			"INSERT OR IGNORE INTO GamesListEntries (GamelistId, GameId)\n"
			"VALUES (:gamelistid, :gameid)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);
	stm->bind(2, game_id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameListRemoveGame(const long long int id, const long long int game_id)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);
	assert(game_id);

	stm = m_db.createStatement(
			"DELETE FROM GamesListEntries\n"
			"WHERE GamelistId = :gamelistid AND GameId = :gameid"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);
	stm->bind(2, game_id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameListAddGameGroup(const long long int id, const std::vector<long long int >& list)
{
	std::vector<long long int >::const_iterator iter;
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(id);
	assert(!list.empty());

	m_db.transactionBegin();
		stm = m_db.createStatement(
				"INSERT OR IGNORE INTO GamesListEntries (GamelistId, GameId)\n"
				"VALUES (:gamelistid, :gameid)"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		// Iteramos el listado de los juegos agregandolos a la lista
		for (iter = list.begin(); iter != list.end(); ++iter)
		{
			stm->bind(1, id);
			stm->bind(2, (*iter));
			if (stm->step() != SqliteStatement::STATEMENT_DONE)
			{
				stm->finalize();
				delete stm;
				m_db.transactionRollBack();
				return false;
			}
			stm->reset();
		}
		stm->finalize();
		delete stm;
	m_db.transactionCommit();

	return true;
}

bool DbManager::gameListRemoveGameGroup(const long long int id, const std::vector<long long int>& list)
{
	SqliteStatement* stm;
	Glib::ustring ids;

	assert(m_db.isOpen());
	assert(id);
	assert(!list.empty());

	m_db.transactionBegin();
		ids = utils::toStr(list);
		stm = m_db.createStatement(
				"DELETE FROM GamesListEntries\n"
				"WHERE GamelistId = :gamelistid AND GameId IN (" + ids + ")"
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

unsigned int DbManager::gameListCountGames(const long long int id)
{
	SqliteStatement* stm;
	unsigned int ret;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT COUNT(1)\n"
			"FROM Games\n"
			"WHERE Games.Enabled = 1 AND Games.Id IN (\n"
			"   SELECT GameId\n"
			"   FROM GamesListEntries\n"
			"   WHERE GamelistId = :id\n"
			")"
	);
	if (!stm)
	{
		return 0;
	}
	stm->bind(1, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		ret = stm->getColumnInt(0);
	}
	else{
		ret = 0;
	}
	stm->finalize();
	delete stm;

	return ret;
}

} // namespace gelide
