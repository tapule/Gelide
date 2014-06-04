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

Collection* DbManager::collectionGet(const long long int id)
{
	SqliteStatement* stm;
	Collection* element;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Collections\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return NULL;
	}
	stm->bind(1, id);
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		element = new Collection(stm->getColumnInt64(0), stm->getColumnText(1));
		element->sort_order = stm->getColumnInt(2);
		element->system_id = stm->getColumnText(3);
		element->manufacturer = stm->getColumnText(4);
		element->year = stm->getColumnText(5);
		element->Icon16 = stm->getColumnText(6);
		element->Icon32 = stm->getColumnText(7);
		element->emulator_id = stm->getColumnInt64(8);
	}
	else{
		element = NULL;
	}
	stm->finalize();
	delete stm;

	return element;
}

bool DbManager::collectionAdd(Collection* collection)
{
	SqliteStatement* stm;
	int order;
	int ret;
	Emulator* emu;

	assert(m_db.isOpen());
	assert(collection);
	assert(!collection->name.empty());

	// Las colecciones siempre tienen que tener un emulador asociado, asi que
	// creamos un emulador unico para la colección
	emu = new Emulator();
	if (!emu)
	{
		return false;
	}
	emu->parent_type = EMULATOR_PARENT_COLLECTION;
	if (!emulatorAdd(emu))
	{
		delete emu;
		return false;
	}

	// Las colecciones están ordenadas, así que obtenemos el orden adecuado
	stm = m_db.createStatement(
			"SELECT MAX(SortOrder)\n"
			"FROM Collections"
	);
	if (!stm)
	{
		return false;
	}
	if (stm->step() == SqliteStatement::STATEMENT_ROW)
	{
		order = stm->getColumnInt(0) + 1;
	}
	else
	{
		order = 1;
	}
	stm->finalize();

	// Preparamos el comando sql para la inserción y asociamos los valores
	if (!stm->prepare(
			"INSERT INTO Collections (Name, SortOrder, SystemId, Manufacturer, Year, Icon16, Icon32, EmulatorId)\n"
			"VALUES (:name, :order, :systemid, :manufacturer, :year, :icon16, :icon32, :emuid)"
		)
	)
	{
		return false;
	}
	stm->bind(1, collection->name);
	stm->bind(2, order);
	stm->bind(3, collection->system_id);
	stm->bind(4, collection->manufacturer);
	stm->bind(5, collection->year);
	stm->bind(6, collection->Icon16);
	stm->bind(7, collection->Icon32);
	stm->bind(8, emu->id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		collection->id = m_db.getLastInsertId();
		collection->sort_order = order;
		emu->parent_id = collection->id;
		emulatorUpdate(emu);
		delete emu;
		return true;
	}
	else
	{
		emulatorDelete(emu->id);
		delete emu;
		return false;
	}
}

bool DbManager::collectionUpdate(Collection* collection)
{
	SqliteStatement* stm;
	int ret;

	assert(m_db.isOpen());
	assert(collection);
	assert(collection->id);
	assert(!collection->name.empty());

	// En las actualizaciones, no se modifica ni el id ni el orden ni el emu
	stm = m_db.createStatement(
			"UPDATE Collections\n"
			"SET Name = :name, SystemId = :systemid, Manufacturer = :manufacturer, Year = :year, Icon16 = :icon16, Icon32 = :icon32\n"
			"WHERE Id = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, collection->name);
	stm->bind(2, collection->system_id);
	stm->bind(3, collection->manufacturer);
	stm->bind(4, collection->year);
	stm->bind(5, collection->Icon16);
	stm->bind(6, collection->Icon32);
	stm->bind(7, collection->id);
	ret = stm->step();
	stm->finalize();
	delete stm;
	if (ret == SqliteStatement::STATEMENT_DONE)
	{
		return true;
	}
	return false;
}

bool DbManager::collectionDelete(const long long int id)
{
	SqliteStatement* stm;

	assert(m_db.isOpen());
	assert(id);

	// Eliminar una colección es una de las operaciones más complejas ya que
	// tenemos que eliminar absolutamente todos los elementos vinculados.

	m_db.transactionBegin();
		// Preparamos el comando para eliminar la colección
		stm = m_db.createStatement(
				"DELETE FROM Collections\n"
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

		// Eliminamos su emulador
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
		stm->bind(2, EMULATOR_PARENT_COLLECTION);
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Preparamos el comando para eliminar las entradas en los recursos
		if (!stm->prepare(
				"DELETE FROM ResourceEntries\n"
				"WHERE ParentId = :id AND ParentType = 0"
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


		/** JUEGOS DE LA COLECCIÓN Y ELEMENTOS RELACIONADOS **/

		// Preparamos el comando para eliminar las entradas en las listas, de
		// los juegos de la colección
		if (!stm->prepare(
				"DELETE FROM GamesListEntries\n"
				"WHERE GameId IN (\n"
				"   SELECT Id\n"
				"   FROM Games\n"
				"   WHERE CollectionId = :id\n"
				")"
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

		// Preparamos el comando para eliminar las entradas en las etiquetas
		// de juegos que se deben eliminar
		if (!stm->prepare(
				"DELETE FROM TagEntries\n"
				"WHERE GameId IN (\n"
				"   SELECT Id\n"
				"   FROM Games\n"
				"   WHERE CollectionId = :id\n"
				")"
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

		// Preparamos el comando para eliminar las entradas en los emuladores
		// de juegos que se deben eliminar
		if (!stm->prepare(
				"DELETE FROM Emulators\n"
				"WHERE ParentType = :type AND ParentId IN (\n"
				"   SELECT Id\n"
				"   FROM Games\n"
				"   WHERE CollectionId = :id\n"
				")"
			)
		)
		{
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, EMULATOR_PARENT_GAME);
		stm->bind(2, id);
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Preparamos el comando para eliminar las entradas en los recursos
		// de juegos que se deben eliminar
		if (!stm->prepare(
				"DELETE FROM ResourceEntries\n"
				"WHERE ParentType = 1 and ParentId IN (\n"
				"   SELECT Id\n"
				"   FROM Games\n"
				"   WHERE CollectionId = :id\n"
				")"
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

		// Preparamos el comando para eliminar los juegos de la colección
		if (!stm->prepare(
				"DELETE FROM Games\n"
				"WHERE CollectionId = :id"
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

bool DbManager::collectionMove(const long long int orig_id, const long long int dest_id)
{
	SqliteStatement* stm;
	long long int tmp;
	int order_orig, order_dest, i;
	Glib::ustring query;

	assert(m_db.isOpen());
	assert(orig_id);
	assert(dest_id);

	if (orig_id == dest_id)
	{
		return true;
	}

	stm = m_db.createStatement(
			"SELECT Id, SortOrder\n"
			"FROM Collections\n"
			"WHERE Id = :id1 OR Id = :id2\n"
			"ORDER BY SortOrder ASC"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, orig_id);
	stm->bind(2, dest_id);

	// Obtenemos el orden para los dos elementos
	for(i = 0; i < 2; ++i){
		if (stm->step() == SqliteStatement::STATEMENT_ROW)
		{
			tmp = stm->getColumnInt64(0);
			if (tmp == orig_id)
			{
				order_orig = stm->getColumnInt(1);
			}
			else
			{
				order_dest = stm->getColumnInt(1);
			}
		}
		else
		{
			stm->finalize();
			delete stm;
			return false;
		}
	}
	stm->finalize();

	m_db.transactionBegin();
		// Preparamos la actualización dependiendo del orden de los elementos
		if (order_orig < order_dest)
		{
			query = "UPDATE Collections\n"
					"SET SortOrder = SortOrder - 1\n"
					"WHERE SortOrder > :orderorig AND SortOrder <= :orderdest";
		}
		else
		{
			query = "UPDATE Collections\n"
					"SET SortOrder = SortOrder + 1\n"
					"WHERE SortOrder < :orderorig AND SortOrder >= :orderdest";
		}
		if (!stm->prepare(query))
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, order_orig);
		stm->bind(2, order_dest);
		if (stm->step() != SqliteStatement::STATEMENT_DONE)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->finalize();

		// Creamos el comando sql para actualizar el orden del elemento origen
		if (!stm->prepare(
				"UPDATE Collections\n"
				"SET SortOrder = :orderdest\n"
				"WHERE Id = :idorig"
			)
		)
		{
			stm->finalize();
			delete stm;
			m_db.transactionRollBack();
			return false;
		}
		stm->bind(1, order_dest);
		stm->bind(2, orig_id);
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

bool DbManager::collectionGetAll(std::vector<Collection* >& list)
{
	SqliteStatement* stm;
	std::vector<Collection* >::iterator iter;
	int ret;
	Collection* element;

	assert(m_db.isOpen());

	stm = m_db.createStatement(
			"SELECT * \n"
			"FROM Collections\n"
			"ORDER BY SortOrder"
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
		element = new Collection(stm->getColumnInt64(0), stm->getColumnText(1));
		element->sort_order = stm->getColumnInt(2);
		element->system_id = stm->getColumnText(3);
		element->manufacturer = stm->getColumnText(4);
		element->year = stm->getColumnText(5);
		element->Icon16 = stm->getColumnText(6);
		element->Icon32 = stm->getColumnText(7);
		element->emulator_id = stm->getColumnInt64(8);
		list.push_back(element);
	}
	stm->finalize();
	delete stm;

	return true;
}

bool DbManager::collectionGetAll(std::vector<Item* >& list)
{
	SqliteStatement* stm;
	std::vector<Item* >::iterator iter;
	int ret;
	Item* element;

	assert(m_db.isOpen());

	stm = m_db.createStatement(
			"SELECT Id, Name \n"
			"FROM Collections\n"
			"ORDER BY SortOrder"
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

unsigned int DbManager::collectionCount(void)
{
	return countTable("Collections");
}

bool DbManager::collectionGetGames(const long long int id, std::vector<Game* >& list, const bool filtered)
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
	where = "WHERE Games.ManufacturerId = Manufacturers.Id AND Games.YearId = Years.Id AND Games.GenreId = Genres.Id AND Games.CollectionId = " + utils::toStr(id);
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

	while((ret = stm->step()) == SqliteStatement::STATEMENT_ROW){
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

bool DbManager::collectionGetGames(const long long int id, std::vector<Item* >& list, const bool filtered)
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
	where = "WHERE Games.CollectionId = " + utils::toStr(id);
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

unsigned int DbManager::collectionCountGames(const long long int id)
{
	SqliteStatement* stm;
	unsigned int ret;

	assert(m_db.isOpen());
	assert(id);

	stm = m_db.createStatement(
			"SELECT COUNT(1)\n"
			"FROM Games\n"
			"WHERE CollectionId = :id"
	);
	if (!stm)
	{
		return false;
	}
	stm->bind(1, id);
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

Glib::ustring DbManager::collectionGetResourceFile(const long long int id, const long long int resource_id)
{
	SqliteStatement* stm;
	Glib::ustring ret;

	assert(m_db.isOpen());
	assert(id);
	assert(resource_id);

	stm = m_db.createStatement(
			"SELECT File\n"
			"FROM ResourceEntries\n"
			"WHERE ResourceId = :rid AND ParentId = :pid AND ParentType = 0"
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

bool DbManager::collectionGetResourceFiles(const long long int id, std::vector<Glib::ustring >& list)
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

bool DbManager::collectionAddResourceFile(const long long int id, const long long int resource_id, const Glib::ustring& file)
{
	SqliteStatement* stm;
	unsigned int ret;

	assert(m_db.isOpen());
	assert(id);
	assert(resource_id);
	assert(!file.empty());

	stm = m_db.createStatement(
			"INSERT OR REPLACE INTO ResourceEntries (ResourceId, ParentId, ParentType, File)\n"
			"VALUES (:rid, :pid, 0, :file)"
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

bool DbManager::collectionRemoveResourceFile(const long long int id, const long long int resource_id)
{
	SqliteStatement* stm;
	unsigned int ret;

	assert(m_db.isOpen());
	assert(id);
	assert(resource_id);

	stm = m_db.createStatement(
			"DELETE FROM ResourceEntries\n"
			"WHERE ResourceId = :rid AND ParentId = :pid AND ParentType = 0"
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
