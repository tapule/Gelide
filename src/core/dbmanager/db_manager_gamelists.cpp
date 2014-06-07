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
		element->type = static_cast<GamelistType>(stm->getColumnInt(2));
		element->operation = static_cast<GamelistOperation>(stm->getColumnInt(3));
		element->rules = stm->getColumnText(4);
		element->limited = stm->getColumnBool(5);
		element->limit_amount = stm->getColumnInt(6);
		element->query_cache = stm->getColumnText(7);
		element->use_custom_emulator = stm->getColumnBool(8);
		element->emulator_id = stm->getColumnInt64(9);
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

	// Si se trata de una lista inteligente actualizamos su cache de consulta
	if (list->type == GAMELIST_TYPE_SMART)
	{
		gameListUpdateQueryCache(list);
	}

	stm = m_db.createStatement(
			"INSERT INTO GamesLists (Name, Type, Operation, Rules, Limited, LimitAmount, QueryCache, UseCustomEmulator, EmulatorId)\n"
			"VALUES (:name, :type, :operation, :rules, :limited, :limitamount, :querycache, :usecustom, :emuid)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, list->name);
	stm->bind(2, list->type);
	stm->bind(3, list->operation);
	stm->bind(4, list->rules);
	stm->bind(5, list->limited);
	stm->bind(6, list->limit_amount);
	stm->bind(7, list->query_cache);
	stm->bind(8, list->use_custom_emulator);
	stm->bind(9, list->emulator_id);
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

	// Si se trata de una lista inteligente actualizamos su cache de consulta
	if (list->type == GAMELIST_TYPE_SMART)
	{
		gameListUpdateQueryCache(list);
	}

	// En las actualizaciones, no se modifica el id mi el tipo
	stm = m_db.createStatement(
			"UPDATE GamesLists\n"
			"SET Name = :name, Operation = :operation, Rules = :rules, Limited = :limited, LimitAmount = :limitamount, QueryCache = :querycache, UseCustomEmulator = :custemu, EmulatorId = :emuid\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, list->name);
	stm->bind(2, list->operation);
	stm->bind(3, list->rules);
	stm->bind(4, list->limited);
	stm->bind(5, list->limit_amount);
	stm->bind(6, list->query_cache);
	stm->bind(7, list->use_custom_emulator);
	stm->bind(8, list->emulator_id);
	stm->bind(9, list->id);
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
		element->type = static_cast<GamelistType>(stm->getColumnInt(2));
		element->operation = static_cast<GamelistOperation>(stm->getColumnInt(3));
		element->rules = stm->getColumnText(4);
		element->limited = stm->getColumnBool(5);
		element->limit_amount = stm->getColumnInt(6);
		element->query_cache = stm->getColumnText(7);
		element->use_custom_emulator = stm->getColumnBool(8);
		element->emulator_id = stm->getColumnInt64(9);
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

bool DbManager::gameListGetGames(const long long int id, std::vector<Game* >& list, std::vector<Filter >& filters)
{
	SqliteStatement* stm;
	std::vector<Game* >::iterator iter;
	GameList* gamelist;
	int ret;
	Game* element;
	Glib::ustring query, where, order, limit;

	assert(m_db.isOpen());
	assert(id);

	gamelist = gameListGet(id);
	if (!gamelist)
	{
		return false;
	}

	// Generamos las clausulas iniciales de la consulta
	query = "SELECT Games.*, Manufacturers.Name, Years.Name, Genres.Name\n"
			"FROM Games, Manufacturers, Years, Genres\n";
	where = "WHERE (Games.ManufacturerId = Manufacturers.Id AND Games.YearId = Years.Id AND Games.GenreId = Genres.Id)\n";
	order = "ORDER BY Games.Title ASC\n";
	if (gamelist->type == GAMELIST_TYPE_STANDARD)
	{
		where += "AND Games.Id IN (\n"
				  "   SELECT GameId\n"
				  "   FROM GamesListEntries\n"
				  "   WHERE GamelistId = " + utils::toStr(id) + "\n"
				  ")\n";
	}
	else
	{
		where += "AND " + gamelist->query_cache + "\n";
		// Comprobamos si hay que aplicar la limitación
		if (gamelist->limited)
		{
			limit = "LIMIT " + utils::toStr(gamelist->limit_amount);
		}

	}
	// Agregamos opciones de filtrado si corresponde
	if (filters.size())
	{
		where += "AND " + parseFiltersVector(filters) + "\n";
	}
	// Ya tenemos la consulta, así que eliminamos la gamelist
	delete gamelist;

	// Creamos el comando sql para hacer la consulta final
	stm = m_db.createStatement(query + where + order + limit);
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
		element = new Game(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(7));
		element->collection_id = stm->getColumnInt64(2);
		element->state = static_cast<GameState>(stm->getColumnInt(3));
		element->file = stm->getColumnText(4);
		element->type = static_cast<GameType>(stm->getColumnInt(5));
		element->crc = stm->getColumnText(6);
		//element->title = stm->getColumnText(7);
		element->manufacturer_id = stm->getColumnInt64(7);
		element->year_id = stm->getColumnInt64(9);
		element->genre_id = stm->getColumnInt64(10);
		element->players = stm->getColumnInt(11);
		element->rating = stm->getColumnInt(12);
		element->times_played = stm->getColumnInt(13);
		element->last_time_played = stm->getColumnText(14);
		element->date_added = stm->getColumnText(15);
		element->favorite = stm->getColumnBool(16);
		element->use_custom_emulator = stm->getColumnBool(17);
		element->emulator_id = stm->getColumnInt64(18);
		element->manufacturer = stm->getColumnText(19);
		element->year = stm->getColumnText(20);
		element->genre = stm->getColumnText(21);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::gameListGetGames(const long long int id, std::vector<Item* >& list, std::vector<Filter >& filters)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	GameList* gamelist;
	int ret;
	Item* element;
	Glib::ustring query, where, order, limit;

	assert(m_db.isOpen());
	assert(id);

	gamelist = gameListGet(id);
	if (!gamelist)
	{
		return false;
	}

	// Generamos las clausulas iniciales de la consulta
	query = "SELECT Games.Id, Games.Name, Games.Title\n"
			"FROM Games\n";
	where = "WHERE ";
	order = "ORDER BY Games.Title ASC\n";

	if (gamelist->type == GAMELIST_TYPE_STANDARD)
	{
		where += "Games.Id IN (\n"
				  "   SELECT GameId\n"
				  "   FROM GamesListEntries\n"
				  "   WHERE GamelistId = " + utils::toStr(id) + "\n"
				  ")\n";
	}
	else
	{
		where += gamelist->query_cache + "\n";
		// Comprobamos si hay que aplicar la limitación
		if (gamelist->limited)
		{
			limit = "LIMIT " + utils::toStr(gamelist->limit_amount);
		}
	}

	// Agregamos opciones de filtrado si corresponde
	if (filters.size())
	{
		where += "AND " + parseFiltersVector(filters) + "\n";
	}

	// Ya tenemos la consulta, así que eliminamos la gamelist
	delete gamelist;

	// Creamos el comando sql para hacer la consulta final
	stm = m_db.createStatement(query + where + order + limit);
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
	GameList* gamelist;
	unsigned int ret;
	Glib::ustring query, where, limit;

	assert(m_db.isOpen());
	assert(id);

	gamelist = gameListGet(id);
	if (!gamelist)
	{
		return 0;
	}

	// Generamos las clausulas iniciales de la consulta
	query = "SELECT COUNT(1)\n"
			"FROM Games\n";
	where = "WHERE ";

	if (gamelist->type == GAMELIST_TYPE_STANDARD)
	{
		where += "Games.Id IN (\n"
				  "   SELECT GameId\n"
				  "   FROM GamesListEntries\n"
				  "   WHERE GamelistId = " + utils::toStr(id) + "\n"
				  ")\n";
	}
	else
	{
		where += gamelist->query_cache + "\n";
		// Comprobamos si hay que aplicar la limitación
		if (gamelist->limited)
		{
			limit = "LIMIT " + utils::toStr(gamelist->limit_amount);
		}
	}

	// Ya tenemos la consulta, así que eliminamos la gamelist
	delete gamelist;

	// Creamos el comando sql para hacer la consulta final
	stm = m_db.createStatement(query + where + limit);
	if (!stm)
	{
		return 0;
	}
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		ret = stm->getColumnInt(0);
	}
	else
	{
		ret = 0;
	}
	stm->finalize();
	delete stm;

	return ret;
}

bool DbManager::gameListUpdateQueryCache(GameList* list)
{
	Glib::ustring query, rules_operation;
	XmlReader xml;
	XmlNode root;
	XmlNode node;
	XmlNode::iterator iter, iter_tmp;
	int ver, tmp;
	GamelistRuleField field;
	GamelistRuleOperation operation;
	Glib::ustring value;

	assert(list);
	assert(list->name.size());

	// Comprobamos si se trata de una lista inteligente
	if (list->type == GAMELIST_TYPE_SMART)
	{
		// Cargamos el xml que contiene las reglas
		if (xml.load(list->rules))
		{
			node = *(xml.getRootElement().begin());
			if (node.getName() == "rules")
			{
				// Comprobamos la versión de las reglas
				node.getAttribute("version", ver);
				if (ver != GameList::RULES_VERSION)
				{
					LOG_ERROR("DBManager: Smart gamelist rules version mismatch");
				}
				// Comprobamos si hay reglas
				iter = node.begin();
				if (iter != node.end())
				{
					// Tipo de operación entre reglas de filtrado
					if (list->operation == GAMELIST_OPERATION_AND)
					{
						rules_operation = "AND";
					}
					else
					{
						rules_operation = "OR";
					}

					// Parseamos las reglas
					query = "(\n";
					for (iter = node.begin(); iter != node.end(); ++iter)
					{
						// Leemos parámetros
						iter->getAttribute("field", tmp);
						field = static_cast<GamelistRuleField>(tmp);
						iter->getAttribute("operation", tmp);
						operation = static_cast<GamelistRuleOperation>(tmp);
						iter->getAttribute("value", value);
						query += gameListParseSmartRule(field, operation, value) + "\n";

						iter_tmp = iter;
						if (++iter_tmp != node.end())
						{
							query += rules_operation + "\n";
						}
					}
					query += ")";
				}
			}
			else
			{
				LOG_ERROR("DBManager: Loading smart gamelist rules");
				xml.close();
				return false;
			}
			xml.close();
		}
		else
		{
			LOG_ERROR("DBManager: Can't load smart gamelist rules");
			return false;
		}
	}

	list->query_cache = query;
	return true;
}

Glib::ustring DbManager::gameListParseSmartRule(const GamelistRuleField field, const GamelistRuleOperation operation, const Glib::ustring& value)
{
	Glib::ustring rule;

	// Nombres de los posibles campos en las reglas
	static const Glib::ustring field_name[FIELD_COUNT] =
	{
		"Collections.Name",
		"Games.Name",
		"Games.Title",
		"Manufacturers.Name",
		"Genres.Name",
		"Tags.Name",
		"Games.State",
		"Games.Type",
		"Years.Name",
		"Games.Players",
		"Games.Rating",
		"Games.TimesPlayed",
		"Games.Favorite",
		"Games.LastTimePlayed",
		"Games.DateAdded"
	};

	// Obtenemos el nombre del campo
	rule = field_name[field];
	// Operación sobre el campo
	switch (operation)
	{
	case RULE_OPERATION_CONTAINS:
		rule += " LIKE '%" + value + "%'";
		break;
	case RULE_OPERATION_NOTCONTAINS:
		rule += " NOT LIKE '%" + value + "%'";
		break;
	case RULE_OPERATION_EQUALS:
		rule += " = '" + value + "'";
		break;
	case RULE_OPERATION_NOTEQUALS:
		rule += " <> '" + value + "'";
		break;
	case RULE_OPERATION_STARTWITH:
		rule += " LIKE '" + value + "%'";
		break;
	case RULE_OPERATION_ENDWITH:
		rule += " LIKE '%" + value + "'";
		break;
	case RULE_OPERATION_LESSTHAN:
		rule += " < '" + value + "'";
		break;
	case RULE_OPERATION_GREATERTHAN:
		rule += " > '" + value + "'";
		break;
	case RULE_OPERATION_LESSTHANEQUALS:
		rule += " <= '" + value + "'";
		break;
	case RULE_OPERATION_GREATERTHANEQUALS:
		rule += " >= '" + value + "'";
		break;
	case RULE_OPERATION_BEFORE:
		rule += " < '" + value + "'";
		break;
	case RULE_OPERATION_AFTER:
		rule += " > '" + value + "'";
		break;
	}

	// Campos especiales
	switch (field)
	{
	case FIELD_COLLECTIONS_NAME:
		rule = 	"Games.CollectionId IN (\n"
				"    SELECT Collections.Id from Collections\n"
				"    WHERE " + rule + "\n"
				")";
		break;
	case FIELD_MANUFACTURERS_NAME:
		rule = 	"Games.ManufacturerId IN (\n"
				"    SELECT Manufacturers.Id from Manufacturers\n"
				"    WHERE " + rule + "\n"
				")";
		break;
	case FIELD_GENRES_NAME:
		rule = 	"Games.GenreId IN (\n"
				"    SELECT Genres.Id from Genres\n"
				"    WHERE " + rule + "\n"
				")";
		break;
	case FIELD_TAGS_NAME:
		rule = 	"Games.Id IN (\n"
				"    SELECT TagEntries.GameId from TagEntries\n"
				"    WHERE TagEntries.TagId IN (\n"
				"        SELECT Tags.Id\n"
				"        FROM Tags\n"
				"        WHERE " + rule + "\n"
				"    )\n"
				")";
		break;
	case FIELD_YEARS_NAME:
		rule = 	"Games.YearId IN (\n"
				"    SELECT Years.Id from Years\n"
				"    WHERE " + rule + "\n"
				")";
		break;
	}

	return rule;
}


} // namespace gelide
