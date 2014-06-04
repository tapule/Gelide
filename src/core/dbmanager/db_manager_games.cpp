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
#include "../../utils/utils.hpp"


namespace gelide{

Game* DbManager::gameGet(const long long int id)
{
	SqliteStatement* stm;
	Game* element;

	assert(m_db.isOpen());
	assert(id);

	// Creamos el comando sql para obtener el juego
	stm = m_db.createStatement(
			"SELECT Games.*, Manufacturers.Name, Years.Name, Genres.Name\n"
			"FROM Games, Manufacturers, Years, Genres\n"
			"WHERE Games.Id = :id AND Games.ManufacturerId = Manufacturers.Id AND Games.YearId = Years.Id AND Games.GenreId = Genres.Id"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new Game(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(7));
		element->collection_id = stm->getColumnInt64(2);
		element->state = static_cast<GameState>(stm->getColumnInt(3));
		element->file = stm->getColumnText(4);
		element->type = static_cast<GameType>(stm->getColumnInt(5));
		element->crc = stm->getColumnText(6);
		//element->title = stm->getColumnText(7);
		element->manufacturer_id = stm->getColumnInt64(8);
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
	}
	else{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::gameAdd(Game* game)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(game);
	assert(!game->name.empty());

	stm = m_db.createStatement(
			"INSERT INTO Games (Name, CollectionId, State, File, Type, CRC, Title, ManufacturerId, YearId, GenreId, Players, Rating, TimesPlayed, LastTimePlayed, DateAdded, Favorite, UseCustomEmulator, EmulatorId)\n"
			"VALUES (:name, :collectionId, :state, :file, :type, :crc, :title, :manufacturerid, :yearid, :genreid, :players, :rating, 0, :lasttimeplayed, DateTime('now'), :favorite, :usecustemu, :custemuid)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, game->name);
	stm->bind(2, game->collection_id);
	stm->bind(3, game->state);
	stm->bind(4, game->file);
	stm->bind(5, game->type);
	stm->bind(6, game->crc);
	stm->bind(7, game->title);
	stm->bind(8, game->manufacturer);
	stm->bind(9, game->year_id);
	stm->bind(10, game->genre_id);
	stm->bind(11, game->players);
	stm->bind(12, game->rating);
	stm->bind(13, game->last_time_played);
	stm->bind(14, game->favorite);
	stm->bind(15, game->use_custom_emulator);
	stm->bind(16, game->emulator_id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		game->id = m_db.getLastInsertId();
		return true;
	}

	return false;
}

bool DbManager::gameUpdate(Game* game)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(game);
	assert(game->id);
	assert(!game->name.empty());

	// En las actualizaciones, no se modifican:
	// Id, CollectionId, State, TimesPlayed, LastTimePlayed, DateAdded
	stm = m_db.createStatement(
			"UPDATE Games\n"
			"SET Name = :name, File = :file, Type = :type, CRC = :crc, Title = :title, ManufacturerId = :manufacturerid, YearId = :yearid, GenreId = :genreid, Players = :players, Rating = :rating, Favorite = :favorite, UseCustomEmulator = :usecust, EmulatorId = :emuid\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, game->name);
	stm->bind(2, game->file);
	stm->bind(3, game->type);
	stm->bind(4, game->crc);
	stm->bind(5, game->title);
	stm->bind(6, game->manufacturer);
	stm->bind(7, game->year_id);
	stm->bind(8, game->genre_id);
	stm->bind(9, game->players);
	stm->bind(10, game->rating);
	stm->bind(11, game->favorite);
	stm->bind(12, game->use_custom_emulator);
	stm->bind(13, game->emulator_id);
	stm->bind(14, game->id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}

	return false;
}

bool DbManager::gameDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(id);

	m_db.transactionBegin();
		// Preparamos el comando para eliminar el juego
		stm = m_db.createStatement(
				"DELETE FROM Games\n"
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

		// Preparamos el comando para eliminar las entradas del juego en las
		// listas
		if (!stm->prepare(
				"DELETE FROM GamesListEntries\n"
				"WHERE GameId = :id"
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

		// Preparamos el comando para eliminar las entradas del juego en las
		// etiquetas
		if (!stm->prepare(
				"DELETE FROM TagEntries\n"
				"WHERE GameId = :id"
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

		// Preparamos el comando para eliminar el emulador del juego
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
		stm->bind(2, EMULATOR_PARENT_GAME);
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Preparamos el comando para eliminar los recursos del juego
		if (!stm->prepare(
				"DELETE FROM ResourceEntries\n"
				"WHERE ParentId = :id AND ParentType = 1"
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
		delete stm;
	m_db.transactionCommit();

	return true;
}

bool DbManager::gameSetState(const long long int id, const GameState state)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);

	// Comando sql para actualizar el estado
	stm = m_db.createStatement(
			"UPDATE Games\n"
			"SET State = :state\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, state);
	stm->bind(2, id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameRating(const long long int id, const int rating){
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);
	assert(rating <= 5);

	// Comando sql para actualizar la puntuación
	stm = m_db.createStatement(
			"UPDATE Games\n"
			"SET Rating = :rating\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, rating);
	stm->bind(2, id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameResetCounters(const long long int id)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);

	// Comando sql para resetear los contadores
	stm = m_db.createStatement(
			"UPDATE Games\n"
			"SET TimesPlayed = 0, LastTimePlayed = NULL\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameIncTimesPlayed(const long long int id)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);

	// Comando sql para aumentar el número de veces jugado
	stm = m_db.createStatement(
			"UPDATE Games\n"
			"SET TimesPlayed = TimesPlayed + 1\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameUpdateLastTimePlayed(const long long int id)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);

	// Comando sql para actualizar la fecha en la que se jugo al juego
	stm = m_db.createStatement(
			"UPDATE Games\n"
			"SET LastTimePlayed = DateTime('now')\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameSetFavorite(const long long int id, const bool favorite)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);

	// Comando sql para actualizar el flag favorito
	stm = m_db.createStatement(
			"UPDATE Games\n"
			"SET Favorite = :favorite\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, favorite);
	stm->bind(2, id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

unsigned int DbManager::gameCount(void)
{
	return countTable("Games");
}

bool DbManager::gameGetGroup(std::vector<long long int >& list, std::vector<Game* >& games)
{
	SqliteStatement* stm;
	Game* element;
	Glib::ustring ids;
	std::vector<Game* >::iterator iter;
	int ret;

	assert(m_db.isOpen());
	assert(!list.empty());

	ids = utils::toStr(list);

	// Creamos el comando sql para obtener los juegos
	stm = m_db.createStatement(
			"SELECT Games.*, Manufacturers.Name, Years.Name, Genres.Name\n"
			"FROM Games, Manufacturers, Years, Genres\n"
			"WHERE Games.Id IN (" + ids + ") AND Games.ManufacturerId = Manufacturers.Id AND Games.YearId = Years.Id AND Games.GenreId = Genres.Id"
	);
	if (!stm)
	{
		return false;
	}

	for (iter = games.begin(); iter != games.end(); ++iter)
	{
		delete (*iter);
	}
	games.clear();

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
		element = new Game(stm->getColumnInt64(0), stm->getColumnText(1), stm->getColumnText(7));
		element->collection_id = stm->getColumnInt64(2);
		element->state = static_cast<GameState>(stm->getColumnInt(3));
		element->file = stm->getColumnText(4);
		element->type = static_cast<GameType>(stm->getColumnInt(5));
		element->crc = stm->getColumnText(6);
		//element->title = stm->getColumnText(7);
		element->manufacturer_id = stm->getColumnInt64(8);
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
		games.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::gameUpdateGroup(std::vector<long long int >& list, Game* game)
{
	SqliteStatement* stm;
	Glib::ustring ids;

	assert(m_db.isOpen());
	assert(!list.empty());
	assert(game);

	m_db.transactionBegin();
		ids = utils::toStr(list);

		// Preparamos el comando para actualizar los elementos
		stm = m_db.createStatement(
				"UPDATE OR IGNORE Games\n"
				"SET Type = :type, ManufacturerId = :manufacturerid, YearId = :yearid, GenreId = :genreid, Players = :players, Rating = :rating, Favorite = :favorite\n"
				"WHERE Id IN (" + ids + ")"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, game->type);
		stm->bind(2, game->manufacturer_id);
		stm->bind(3, game->year_id);
		stm->bind(4, game->genre_id);
		stm->bind(5, game->players);
		stm->bind(6, game->rating);
		stm->bind(7, game->favorite);
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

bool DbManager::gameDeleteGroup(std::vector<long long int >& list)
{
	SqliteStatement* stm;
	Glib::ustring ids;

	assert(m_db.isOpen());
	assert(!list.empty());

	m_db.transactionBegin();
		ids = utils::toStr(list);

		// Preparamos el comando para eliminar los juego
		stm = m_db.createStatement(
				"DELETE FROM Games\n"
				"WHERE Id IN (" + ids + ")"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Preparamos el comando para eliminar las entradas de los juegos en las
		// listas
		if (!stm->prepare(
				"DELETE FROM GamesListEntries\n"
				"WHERE GameId IN (" + ids + ")"
			)
		)
		{
			m_db.transactionRollBack();
			return false;
		}
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Preparamos el comando para eliminar las entradas de los juegos en las
		// etiquetas
		if (!stm->prepare(
				"DELETE FROM TagEntries\n"
				"WHERE GameId IN (" + ids + ")"
			)
		)
		{
			m_db.transactionRollBack();
			return false;
		}
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Preparamos el comando para eliminar los emuladores de los juegos
		if (!stm->prepare(
				"DELETE FROM Emulators\n"
				"WHERE ParentType = :type AND ParentId IN (" + ids + ")"
			)
		)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, EMULATOR_PARENT_GAME);
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Preparamos el comando para eliminar los recursos de los juegos
		if (!stm->prepare(
				"DELETE FROM ResourceEntries\n"
				"WHERE ParentType = 1 AND ParentId IN (" + ids + ")"
			)
		)
		{
			m_db.transactionRollBack();
			return false;
		}
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

bool DbManager::gameRatingGroup(std::vector<long long int >& list, const int rating)
{
	SqliteStatement* stm;
	Glib::ustring ids;

	assert(m_db.isOpen());
	assert(!list.empty());

	m_db.transactionBegin();
		ids = utils::toStr(list);
		stm = m_db.createStatement(
				"UPDATE Games\n"
				"SET Rating = :rating\n"
				"WHERE Id IN (" + ids + ")"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, rating);
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

bool DbManager::gameResetCountersGroup(std::vector<long long int >& list)
{
	SqliteStatement* stm;
	Glib::ustring ids;

	assert(m_db.isOpen());
	assert(!list.empty());

	m_db.transactionBegin();
		ids = utils::toStr(list);
		stm = m_db.createStatement(
				"UPDATE Games\n"
				"SET TimesPlayed = 0, LastTimePlayed = NULL\n"
				"WHERE Id IN (" + ids + ")"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
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

bool DbManager::gameSetFavoriteGroup(std::vector<long long int >& list, const bool favorite)
{
	SqliteStatement* stm;
	Glib::ustring ids;

	assert(m_db.isOpen());
	assert(!list.empty());

	m_db.transactionBegin();
		ids = utils::toStr(list);
		stm = m_db.createStatement(
				"UPDATE Games\n"
				"SET Favorite = :favorite\n"
				"WHERE Id IN (" + ids + ")"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, favorite);
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

bool DbManager::gameGetGameListsIn(const long long int id, std::vector<Item* >& list)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	int ret;
	Item* element;

	assert(m_db.isOpen());
	assert(id);

	// Creamos el comando sql
	stm = m_db.createStatement(
			"SELECT Id, Name\n"
			"FROM GamesLists\n"
			"WHERE Id IN (\n"
			"   SELECT GameListId\n"
			"   FROM GamesListEntries\n"
			"   WHERE GameId = :id\n"
			")"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);

	for(iter = list.begin(); iter != list.end(); ++iter)
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

bool DbManager::gameGetGameListsNotIn(const long long int id, std::vector<Item* >& list)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	int ret;
	Item* element;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT Id, Name\n"
			"FROM GamesLists\n"
			"WHERE Id NOT IN (\n"
			"   SELECT GameListId\n"
			"   FROM GamesListEntries\n"
			"   WHERE GameId = :id\n"
			")"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);

	for (iter = list.begin(); iter != list.end(); ++iter)
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

bool DbManager::gameAddTag(const long long int game_id, const long long int tag_id)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(game_id);
	assert(tag_id);

	// Preparamos el comando sql para la inserción y asociamos los valores
	// Si la entrada existe, la ignoramos
	stm = m_db.createStatement(
			"INSERT OR IGNORE INTO TagEntries (TagId, GameId)\n"
			"VALUES (:tagid, :gameid)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, tag_id);
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

bool DbManager::gameRemoveTag(const long long int game_id, const long long int tag_id)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(game_id);
	assert(tag_id);

	stm = m_db.createStatement(
			"DELETE FROM TagEntries\n"
			"WHERE TagId = :tagid AND GameId = :gameid"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, tag_id);
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

bool DbManager::gameAddTagGroup(const std::vector<long long int >& list, const long long int tag_id)
{
	std::vector<long long int >::const_iterator iter;
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(!list.empty());
	assert(tag_id);

	m_db.transactionBegin();
		stm = m_db.createStatement(
				"INSERT OR IGNORE INTO TagEntries (TagId, GameId)\n"
				"VALUES (:tagid, :gameid)"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		// Iteramos el listado de los juegos para agregar la etiqueta
		for (iter = list.begin(); iter!= list.end(); ++iter)
		{
			stm->bind(1, tag_id);
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

bool DbManager::gameRemoveTagGroup(const std::vector<long long int >& list, const long long int tag_id)
{
	SqliteStatement* stm;
	Glib::ustring ids;

	assert(m_db.isOpen());
	assert(!list.empty());
	assert(tag_id);

	m_db.transactionBegin();
		ids = utils::toStr(list);
		stm = m_db.createStatement(
				"DELETE FROM TagEntries\n"
				"WHERE TagId = :tagid AND GameId IN (" + ids + ")"
		);
		if (!stm)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, tag_id);
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

bool DbManager::gameGetTagsIn(const long long int id, std::vector<Item* >& list)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	int ret;
	Item* element;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT *\n"
			"FROM Tags\n"
			"WHERE Id IN (\n"
			"   SELECT TagId\n"
			"   FROM TagEntries\n"
			"   WHERE GameId = :id\n"
			")\n"
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
		element = new Item(stm->getColumnInt64(0), stm->getColumnText(1));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::gameGetTagsNotIn(const long long int id, std::vector<Item* >& list)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	int ret;
	Item* element;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT *\n"
			"FROM Tags\n"
			"WHERE Id NOT IN (\n"
			"   SELECT TagId\n"
			"   FROM TagEntries\n"
			"   WHERE GameId = :id\n"
			")\n"
			"ORDER BY Name"
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

	while ((ret = stm->step()) == SqliteStatement::STATEMENT_ROW)
	{
		element = new Item(stm->getColumnInt64(0), stm->getColumnText(1));
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

Glib::ustring DbManager::gameGetResourceFile(const long long int id, const long long int resource_id)
{
	SqliteStatement* stm;
	Glib::ustring ret;

	assert(m_db.isOpen());
	assert(id);
	assert(resource_id);

	stm = m_db.createStatement(
			"SELECT File\n"
			"FROM ResourceEntries\n"
			"WHERE ResourceId = :rid AND ParentId = :pid AND ParentType = 1"
	);
	if (!stm)
	{
		return "";
	}
	stm->bind(1, resource_id);
	stm->bind(2, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		ret = stm->getColumnText(0);
	}
	else
	{
		ret = "";
	}
	stm->finalize();
	delete stm;

	return ret;
}

bool DbManager::gameGetResourceFiles(const long long int id, std::vector<Glib::ustring >& list)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT File\n"
			"FROM ResourceEntries\n"
			"WHERE ParentId = :pid AND ParentType = 1"
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

bool DbManager::gameAddResourceFile(const long long int id, const long long int resource_id, const Glib::ustring& file)
{
	SqliteStatement* stm;
	unsigned int ret;

	assert(m_db.isOpen());
	assert(id);
	assert(resource_id);
	assert(!file.empty());

	// Preparamos el comando sql para la inserción y asociamos los valores
	// Si la entrada existe, la sobreescribimos
	stm = m_db.createStatement(
			"INSERT OR REPLACE INTO ResourceEntries (ResourceId, ParentId, ParentType, File)\n"
			"VALUES (:rid, :pid, 1, :file)"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, resource_id);
	stm->bind(2, id);
	stm->bind(3, file);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::gameRemoveResourceFile(const long long int id, const long long int resource_id)
{
	SqliteStatement* stm;
	unsigned int ret;

	assert(m_db.isOpen());
	assert(id);
	assert(resource_id);

	stm = m_db.createStatement(
			"DELETE FROM ResourceEntries\n"
			"WHERE ResourceId = :rid AND ParentId = :pid AND ParentType = 1"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, resource_id);
	stm->bind(2, id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

} // namespace gelide
