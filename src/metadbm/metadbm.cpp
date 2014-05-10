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

#include <gdkmm.h>
#include "metadbm.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include "../utils/utils.hpp"
#include "../utils/sqlite.hpp"
#include "../core/datreader/dat_reader_factory.hpp"

namespace gelide{

MetadbmApp::MetadbmApp(const Glib::ustring& working_dir):
	m_db(NULL),
#ifdef ENABLE_DEBUG_MODE
	m_log_enabled(true),
#else
	m_log_enabled(false),
#endif
	m_working_dir(working_dir),
	m_command(COMMAND_SHOW_HELP)
{
}

MetadbmApp::~MetadbmApp(void)
{
};

int MetadbmApp::run(int argc, char** argv)
{
	int ret;
	long long int id;
	Glib::ustring db_file;

	// Inicializamos el sistema de log
	LOG_DEBUG("Metadbm: Initializing log...");
	if(m_log_enabled)
	{
		LOG_DEBUG("Metadbm: Log enabled.");
		LOG_OPEN(Glib::build_filename(m_working_dir, METADBM_LOG_FILE));
		// Mensajes de inicialización
		LOG_INFO("Initializing Metadbm...");
		LOG_INFO("Metadbm: Version " << PACKAGE_VERSION);
		LOG_INFO("Metadbm: Working dir " << m_working_dir);
	}
	else
	{
		LOG_DEBUG("Metadbm: Log disabled.");
	}

	// Parseamos los parámetros introducidos por linea de comandos
	LOG_DEBUG("Metadbm: Parsing parameters...");
	ret = parseParams(argc, argv);
	if (ret == 1)
	{
		clean();
		return EXIT_FAILURE;
	}

	LOG_INFO("Metadbm: Initializing data base engine...");
	if (!Sqlite::initialize())
	{
		clean();
		return EXIT_FAILURE;
	}

	LOG_INFO("Metadbm: Initializing Metadata data base...");
	// Iniciamos el gestor de la base de datos
	m_db = new MetaDb();
	// Intentamos cargar la base de datos del usuario.
	db_file = Glib::build_filename(GELIDE_DATA_DIR, METADBM_DB_FILE);
	if (!m_db->loadDataBase(db_file))
	{
		LOG_INFO("Metadbm: User Metadata data base not found.");

		// Intentamos crear una BD nueva
		if (!m_db->createDataBase(db_file))
		{
			LOG_ERROR("Metadbm: Can't create a new data base.");
			std::cout << "Error: Can't create a new metadata data base." << std::endl;
			clean();
			return EXIT_FAILURE;
		}
	}

	// Ejecución de los posibles comandos
	switch (m_command)
	{
	case COMMAND_SHOW_VERSION:
		showVersion();
		break;
	case COMMAND_SHOW_HELP:
		showHelp(m_param1);
		break;
	case COMMAND_LIST_SYSTEMS:
		listSystems();
		break;
	case COMMAND_LIST_GAMES:
		listGames(m_param1);
		break;
	case COMMAND_DELETE_SYSTEM:
		deleteSystem(m_param1);
		break;
	case COMMAND_DELETE_GAME:
		utils::strTo(m_param1, id);
		deleteGame(id);
		break;
	case COMMAND_SHOW_GAME:
		utils::strTo(m_param1, id);
		showGame(id);
		break;
	case COMMAND_ADD_NAME:
	case COMMAND_ADD_CRC:
	case COMMAND_ADD_MD5:
	case COMMAND_ADD_SHA1:
		addSets(m_param1, m_param2, m_command);
		break;
	case COMMAND_FIND_NAME:
		findName(m_param1, m_param2);
		break;
	case COMMAND_FIND_HASH:
		findHash(m_param1, m_param2);
		break;
	case COMMAND_FIND_TITLE:
		findTitle(m_param1, m_param2);
		break;
	}

	// Limpiamos los sistemas creados
	LOG_INFO("Shutting down Metadbm...");
	clean();

	return EXIT_SUCCESS;
}

int MetadbmApp::parseParams(int argc, char** argv)
{
	int i;

	// Por defecto mostramos la ayuda si no hay parámetros
	m_command = COMMAND_SHOW_HELP;
	m_param1.clear();
	m_param2.clear();

	// Comenzamos en 1 ya que argv[0] es el nombre del programa ejecutado
	i = 1;
	while (i < argc)
	{
		// -v muestra la versión del programa y termina
		if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--version") == 0))
		{
			m_command = COMMAND_SHOW_VERSION;
			return 0;
		}
		// -h muestra la ayuda del programa y termina
		else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
		{
			m_command = COMMAND_SHOW_HELP;
			m_param1 = argv[0];
			return 0;
		}
		// -l desactiva el log
		else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--disable-log") == 0))
		{
			if (m_log_enabled)
			{
				LOG_DEBUG("Metadbm: Disabling logging...");
				m_log_enabled = false;
				LOG_CLOSE();
			}
		}
		// -L activa el log
		else if ((strcmp(argv[i], "-L") == 0) || (strcmp(argv[i], "--enable-log") == 0))
		{
			if (!m_log_enabled)
			{
				LOG_DEBUG("Metadbm: Enabling logging...");
				LOG_OPEN(Glib::build_filename(m_working_dir, METADBM_LOG_FILE));
				m_log_enabled = true;
			}
		}
		// -ls lista los sistemas y termina
		else if ((strcmp(argv[i], "-ls") == 0) || (strcmp(argv[i], "--list-systems") == 0))
		{
			m_command = COMMAND_LIST_SYSTEMS;
			return 0;
		}
		// -lg lista todos los sets o los de un sistema dado
		else if ((strcmp(argv[i], "-lg") == 0) || (strcmp(argv[i], "--list-games") == 0))
		{
			m_command = COMMAND_LIST_GAMES;
			// Si hay un parámetro más, lo interpretamos como el sistema
			if (i < argc - 1)
			{
				m_param1 = argv[i + 1];
			}
			return 0;
		}
		// -ds elimina un sistema y todos sus sets
		else if ((strcmp(argv[i], "-ds") == 0) || (strcmp(argv[i], "--delete-system") == 0))
		{
			m_command = COMMAND_DELETE_SYSTEM;
			// Necesitamos un parámetros más para el sistema
			if (i < argc - 1)
			{
				m_param1 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --delete-system wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --delete-system System" << std::endl;
				return 1;
			}
			return 0;
		}
		// -dg elimina un set por su identificador
		else if ((strcmp(argv[i], "-dg") == 0) || (strcmp(argv[i], "--delete-game") == 0))
		{
			m_command = COMMAND_DELETE_GAME;
			// Necesitamos un parámetros más para el id
			if (i < argc - 1)
			{
				m_param1 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --delete-game wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --delete-game Id" << std::endl;
				return 1;
			}
			return 0;
		}
		// -sg muestra la ficha completa de un set
		else if ((strcmp(argv[i], "-sg") == 0) || (strcmp(argv[i], "--show-game") == 0))
		{
			m_command = COMMAND_SHOW_GAME;
			// Necesitamos un parámetros más para el id del set
			if (i < argc - 1)
			{
				m_param1 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --show-game wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --show-game Id" << std::endl;
				return 1;
			}
			return 0;
		}
		// -an añade o actualiza sets desde un dat usando el set name como hash
		else if ((strcmp(argv[i], "-an") == 0) || (strcmp(argv[i], "--add-name") == 0))
		{
			m_command = COMMAND_ADD_NAME;
			// Necesitamos dos parámetros más, el sistema y el fichero dat
			if (i < argc - 2)
			{
				m_param1 = argv[i + 1];
				m_param2 = argv[i + 2];
			}
			else
			{
				std::cout << "Metadbm: --add-name wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --add-name System File" << std::endl;
				return 1;
			}
			return 0;
		}
		// -ac añade o actualiza sets desde un dat usando el CRC como hash
		else if ((strcmp(argv[i], "-ac") == 0) || (strcmp(argv[i], "--add-crc") == 0))
		{
			m_command = COMMAND_ADD_CRC;
			// Necesitamos dos parámetros más, el sistema y el fichero dat
			if (i < argc - 2)
			{
				m_param1 = argv[i + 1];
				m_param2 = argv[i + 2];
			}
			else
			{
				std::cout << "Metadbm: --add-crc wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --add-crc System File" << std::endl;
				return 1;
			}
			return 0;
		}
		// -am añade o actualiza sets desde un dat usando el MD5 como hash
		else if ((strcmp(argv[i], "-am") == 0) || (strcmp(argv[i], "--add-md5") == 0))
		{
			m_command = COMMAND_ADD_MD5;
			// Necesitamos dos parámetros más, el sistema y el fichero dat
			if (i < argc - 2)
			{
				m_param1 = argv[i + 1];
				m_param2 = argv[i + 2];
			}
			else
			{
				std::cout << "Metadbm: --add-md5 wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --add-md5 System File" << std::endl;
				return 1;
			}
			return 0;
		}
		// -as añade o actualiza sets desde un dat usando el SHA1 como hash
		else if ((strcmp(argv[i], "-as") == 0) || (strcmp(argv[i], "--add-sha1") == 0))
		{
			m_command = COMMAND_ADD_SHA1;
			// Necesitamos dos parámetros más, el sistema y el fichero dat
			if (i < argc - 2)
			{
				m_param1 = argv[i + 1];
				m_param2 = argv[i + 2];
			}
			else
			{
				std::cout << "Metadbm: --add-sha1 wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --add-sha1 System File" << std::endl;
				return 1;
			}
			return 0;
		}
		// -fn busca sets por set name, en un sistema o de forma global
		else if ((strcmp(argv[i], "-fn") == 0) || (strcmp(argv[i], "--find-name") == 0))
		{
			m_command = COMMAND_FIND_NAME;
			// Necesitamos uno o dos parámetros más, el sistema y el name
			if (i < argc - 2)
			{
				m_param1 = argv[i + 1];
				m_param2 = argv[i + 2];
			}
			else if (i < argc - 1)
			{
				// El set name siempre en el parámetro 2
				m_param2 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --find-name wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --find-name [System] Name" << std::endl;
				return 1;
			}
			return 0;
		}
		// -fh busca sets por hash, en un sistema o de forma global
		else if ((strcmp(argv[i], "-fh") == 0) || (strcmp(argv[i], "--find-hash") == 0))
		{
			m_command = COMMAND_FIND_HASH;
			// Necesitamos uno o dos parámetros más, el sistema y el hash
			if (i < argc - 2)
			{
				m_param1 = argv[i + 1];
				m_param2 = argv[i + 2];
			}
			else if (i < argc - 1)
			{
				// El hash siempre en el parámetro 2
				m_param2 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --find-hash wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --find-hash [System] Hash" << std::endl;
				return 1;
			}
			return 0;
		}
		// -fh busca sets que comiencen por titulo, en un sistema o de forma global
		else if ((strcmp(argv[i], "-ft") == 0) || (strcmp(argv[i], "--find-title") == 0))
		{
			m_command = COMMAND_FIND_TITLE;
			// Necesitamos uno o dos parámetros más, el sistema y el título
			if (i < argc - 2)
			{
				m_param1 = argv[i + 1];
				m_param2 = argv[i + 2];
			}
			else if (i < argc - 1)
			{
				// El título siempre en el parámetro 2
				m_param2 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --find-title wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --find-title [System] Title" << std::endl;
				return 1;
			}
			return 0;
		}
		else{
			DEBUG("Metadbm: Unknown option \"" << argv[i] << "\"");
			std::cout << "Unknown option \"" << argv[i] << "\"" << std::endl;
			return 1;
		}
		++i;
	}
	return 0;
}

void MetadbmApp::showHelp(const Glib::ustring& p_program)
{
	showVersion();
	std::cout << "Usage: " << p_program << " [Options] [System / Id / Name / Hash / Title] [File]" << std::endl
			  << std::endl
			  << "Available options:" << std::endl
			  << "  -v,  --version            Display Metadbm version information and exit" << std::endl
			  << "  -h,  --help               Display this help message and exit" << std::endl
			  << "  -l,  --disable-log        Disable logging" << std::endl
			  << "  -L,  --enable-log         Enable logging" << std::endl
			  << "  -ls, --list-systems       List systems in the data base" << std::endl
			  << "  -lg, --list-games         List game sets in the data base" << std::endl
			  << "  -ds, --delete-system      Delete a system and all his games" << std::endl
			  << "  -dg, --delete-game        Delete a game set by id" << std::endl
			  << "  -sg, --show-game          Show game set information by id" << std::endl
			  << "  -an, --add-name           Add sets from dat file using Name as hash" << std::endl
			  << "  -ac, --add-crc            Add sets from dat file using CRC as hash" << std::endl
			  << "  -am, --add-md5            Add sets from dat file using MD5 as hash" << std::endl
			  << "  -as, --add-sha1           Add sets from dat file using SHA1 as hash" << std::endl
			  << "  -fn, --find-name          Find game sets by set name" << std::endl
			  << "  -fh, --find-hash          Find game sets by hash" << std::endl
			  << "  -ft, --find-title         Find game sets by title" << std::endl;
}

void MetadbmApp::showVersion(void)
{
	std::cout << "Gelide Metadbm v" << METADBM_VERSION << std::endl
			  << "Copyright (C) 2008 - 2014 Juan Ángel Moreno Fernández" << std::endl;
}

void MetadbmApp::listSystems(void)
{
	std::vector<Glib::ustring> list;
	std::vector<Glib::ustring>::iterator iter;

	m_db->systemGetAll(list);

	std::cout << "Systems: " << list.size() <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		std::cout << *iter <<  std::endl;
	}
}

void MetadbmApp::listGames(const Glib::ustring& system)
{
	std::vector<MetaDbSet* > list;
	std::vector<MetaDbSet* >::iterator iter;

	if (system.empty())
	{
		m_db->setGetAll(list);
	}
	else
	{
		m_db->systemGetSets(system, list);
	}

	std::cout << "Sets: " << list.size() <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		std::cout << "(" << (*iter)->system << ") " << (*iter)->id << ": " << (*iter)->title << std::endl;
		// Una vez mostrado el set, lo eliminamos para ir limpiando
		delete (*iter);
	}
}

void MetadbmApp::deleteSystem(const Glib::ustring& system)
{
	assert(!system.empty());

	std::cout << "Removing system \"" << system << "\"... ";
	if (!m_db->systemDelete(system))
	{
		std::cout << "Error";
	}
	std::cout <<  std::endl;
}

void MetadbmApp::deleteGame(const long long int id)
{
	std::cout << "Removing game set \"" << id << "\"... ";
	if (!m_db->setDelete(id))
	{
		std::cout << "Error";
	}
	std::cout <<  std::endl;
}

void MetadbmApp::showGame(const long long int id)
{
	MetaDbSet* set = NULL;

	set = m_db->setGet(id);
	if (!set)
	{
		std::cout << "Game set not found.";
		return;
	}
	std::cout << "Set Id: " << set->id <<  std::endl;
	std::cout << "System: " << set->system <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;
	std::cout << "Name: " << set->name <<  std::endl;
	std::cout << "Title: " << set->title <<  std::endl;
	std::cout << "Hash: " << set->hash <<  std::endl;
	std::cout << "Type: " << set->type <<  std::endl;
	std::cout << "Manufacturer: " << set->manufacturer <<  std::endl;
	std::cout << "Year: " << set->year <<  std::endl;
	std::cout << "Genre: " << set->genre <<  std::endl;
	std::cout << "Players: " << set->players <<  std::endl;

	delete set;
}


void MetadbmApp::addSets(const Glib::ustring& system, const Glib::ustring& file, const MetadbmCommands type)
{
	DatReader* dat = NULL;
	std::vector<DatSet> sets;
	std::vector<DatSet>::iterator iter;
	Glib::ustring hash;
	MetaDbSet* set = NULL;
	int added = 0;
	int updated = 0;

	assert(type >= COMMAND_ADD_NAME);
	assert(type <= COMMAND_ADD_SHA1);

	// De momento no tenemos soporte MD5 y SHA1 en el DatReader
	if ((type == COMMAND_ADD_MD5) || (type == COMMAND_ADD_SHA1))
	{
		std::cout << "Not yet implemented!!" <<  std::endl;
		return;
	}

	// Obtenemos un lector de dat para el fichero
	dat = DatReaderFactory::getDatReader(file);
	if (!dat)
	{
		std::cout << "Dat reader not found for file \"" << file << "\"" <<  std::endl;
		return;
	}
	// Mostramos la info del dat y sus sets
	std::cout << "Dat type: " << dat->getType() <<  std::endl;
	if (!dat->read(sets))
	{
		std::cout << "Error reading sets" <<  std::endl;
		delete dat;
		return;
	}
	std::cout << "Total dat sets: " << sets.size() <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;

	// Procesamos todos los sets del dat
	for (iter = sets.begin(); iter != sets.end(); ++iter)
	{
		// Type nos indica que campo debemos usar como hash
		switch (type)
		{
		case COMMAND_ADD_NAME:
			hash = iter->name;
			break;
		case COMMAND_ADD_CRC:
			hash = iter->crc;
			break;
		case COMMAND_ADD_MD5:
		case COMMAND_ADD_SHA1:
			continue;
			break;
		}

		// Comprobamos si ya tenemos el set con ese hash para el sistema dado
		set = m_db->setGetByHash(system, hash);
		if (set)
		{
			// Para la actualización, rellenamos los campos que estén vacíos
			// Excepto Id, Name System y Hash
			if (set->type == 0)
			{
				if (iter->is_bios)
				{
					set->type = SET_TYPE_BIOS;
				}
				else if (!iter->clone_of.empty())
				{
					set->type = SET_TYPE_CLONE;
				}
				else
				{
					set->type = SET_TYPE_ORIGINAL;
				}
			}

			if (set->title.empty())
			{
				set->title = iter->description;
			}

			if (set->manufacturer.empty())
			{
				set->manufacturer = iter->manufacturer;
			}

			if (set->year.empty())
			{
				set->year = iter->year;
			}

			if (set->genre.empty())
			{
				set->genre = iter->genre;
			}

			set->players = iter->players;

			std::cout << "Updating set " << set->id << ", Hash: " << hash <<  std::endl;
			m_db->setUpdate(set);
			delete set;
			set = NULL;
			++updated;
		}
		else
		{
			set = new MetaDbSet();
			set->name = iter->name;
			set->system = system;
			if (iter->is_bios)
			{
				set->type = SET_TYPE_BIOS;
			}
			else if (!iter->clone_of.empty())
			{
				set->type = SET_TYPE_CLONE;
			}
			else
			{
				set->type = SET_TYPE_ORIGINAL;
			}
			set->hash = hash;
			set->title = iter->description;
			set->manufacturer = iter->manufacturer;
			set->year = iter->year;
			set->genre = iter->genre;
			set->players = iter->players;
			std::cout << "Adding set, Hash: " << hash <<  std::endl;
			m_db->setAdd(set);
			delete set;
			set = NULL;
			++added;
		}
	}
	std::cout << "-------------------------------------" <<  std::endl;
	std::cout << "New sets: " << added << std::endl;
	std::cout << "Updated sets: " << updated << std::endl;
	delete dat;
}

void MetadbmApp::findName(const Glib::ustring& system, const Glib::ustring& name)
{
	std::vector<MetaDbSet* > list;
	std::vector<MetaDbSet* >::iterator iter;

	if (system.empty())
	{
		m_db->setFindName(name, list);
	}
	else
	{
		m_db->systemFindSetsName(system, name, list);
	}

	std::cout << "Sets: " << list.size() <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		std::cout << "(" << (*iter)->system << ") " << (*iter)->id << ": " << (*iter)->title << std::endl;
		delete (*iter);
	}
}

void MetadbmApp::findHash(const Glib::ustring& system, const Glib::ustring& hash)
{
	std::vector<MetaDbSet* > list;
	std::vector<MetaDbSet* >::iterator iter;

	if (system.empty())
	{
		m_db->setFindHash(hash, list);
	}
	else
	{
		m_db->systemFindSetsHash(system, hash, list);
	}

	std::cout << "Sets: " << list.size() <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		std::cout << "(" << (*iter)->system << ") " << (*iter)->id << ": " << (*iter)->title << std::endl;
		delete (*iter);
	}
}

void MetadbmApp::findTitle(const Glib::ustring& system, const Glib::ustring& title)
{
	std::vector<MetaDbSet* > list;
	std::vector<MetaDbSet* >::iterator iter;

	if (system.empty())
	{
		m_db->setFindTitle(title, list);
	}
	else
	{
		m_db->systemFindSetsTitle(system, title, list);
	}

	std::cout << "Sets: " << list.size() <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;
	for (iter = list.begin(); iter != list.end(); ++iter)
	{
		std::cout << "(" << (*iter)->system << ") " << (*iter)->id << ": " << (*iter)->title << std::endl;
		delete (*iter);
	}
}

void MetadbmApp::clean(void)
{
	// Cerramos el gestor de la base de datos
	if (m_db)
	{
		LOG_INFO("Metadbm: Shutting down data base...");
		m_db->closeDataBase();
		delete m_db;
		m_db = NULL;
		LOG_INFO("Metadbm: Shutting down data base engine...");
		Sqlite::shutDown();
	}

	LOG_CLOSE();
}

} // namespace gelide
