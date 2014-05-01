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
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include "../utils/utils.hpp"
#include "../utils/sqlite.hpp"
//#include "gelide_gui.hpp"
//#include "../core/dbmetadata/db_metadata.hpp"
//#include "../utils/tokenizer.hpp"


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

	LOG_INFO("Metadbm: Initializing data base manager...");
	// Iniciamos el gestor de la base de datos
	m_db = new MetaDb();
	// Intentamos cargar la base de datos del usuario.
	db_file = Glib::build_filename(GELIDE_DATA_DIR, METADBM_DB_FILE);
	if (!m_db->loadDataBase(db_file))
	{
		LOG_INFO("Metadbm: User metadata data base not found.");

		// Intentamos crear una BD nueva
		if (!m_db->createDataBase(db_file))
		{
			LOG_ERROR("Metadbm: Can't create a new data base.");
			clean();
			return EXIT_FAILURE;
		}
	}

	/*******************************************************
	 * Cuerpo principal de la aplicación
	*******************************************************/

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

	}


	/*******************************************************/

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
				DEBUG("Metadbm: Disabling logging...");
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
		else if ((strcmp(argv[i], "-ac") == 0) || (strcmp(argv[i], "--add-crc") == 0))
		{
			m_command = COMMAND_ADD_CRC;
			// Necesitamos dos parámetros más
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
		else if ((strcmp(argv[i], "-an") == 0) || (strcmp(argv[i], "--add-name") == 0))
		{
			m_command = COMMAND_ADD_NAME;
			// Necesitamos dos parámetros más
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
		else if ((strcmp(argv[i], "-ds") == 0) || (strcmp(argv[i], "--delete-system") == 0))
		{
			m_command = COMMAND_DELETE_SYSTEM;
			// Necesitamos un parámetros más
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
		else if ((strcmp(argv[i], "-dg") == 0) || (strcmp(argv[i], "--delete-game") == 0))
		{
			m_command = COMMAND_DELETE_GAME;
			// Necesitamos un parámetros más
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
		else if ((strcmp(argv[i], "-fn") == 0) || (strcmp(argv[i], "--find-name") == 0))
		{
			m_command = COMMAND_FIND_NAME;
			// Necesitamos un parámetros más
			if (i < argc - 1)
			{
				m_param1 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --find-name wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --find-name Name" << std::endl;
				return 1;
			}
			return 0;
		}
		else if ((strcmp(argv[i], "-fc") == 0) || (strcmp(argv[i], "--find-crc") == 0))
		{
			m_command = COMMAND_FIND_CRC;
			// Necesitamos un parámetros más
			if (i < argc - 1)
			{
				m_param1 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --find-crc wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --find-crc CRC" << std::endl;
				return 1;
			}
			return 0;
		}
		else if ((strcmp(argv[i], "-ft") == 0) || (strcmp(argv[i], "--find-title") == 0))
		{
			m_command = COMMAND_FIND_TITLE;
			// Necesitamos un parámetros más
			if (i < argc - 1)
			{
				m_param1 = argv[i + 1];
			}
			else
			{
				std::cout << "Metadbm: --find-title wrong params:" << std::endl
						<< "Usage: " << argv[0] << " --find-title Title" << std::endl;
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
	std::cout << "Usage: " << p_program << " [Options] [System / Id / Name / CRC / Title] [File]" << std::endl
			  << std::endl
			  << "Available options:" << std::endl
			  << "  -v,  --version            Display Metadbm version information and exit" << std::endl
			  << "  -h,  --help               Display this help message and exit" << std::endl
			  << "  -l,  --disable-log        Disable logging" << std::endl
			  << "  -L,  --enable-log         Enable logging" << std::endl
			  << "  -ls, --list-systems       List systems in the data base" << std::endl
			  << "  -lg, --list-games         List game sets in the data base" << std::endl
			  << "  -ac, --add-crc            Add sets from dat file indexed with CRC" << std::endl
			  << "  -an, --add-name           Add sets from dat file indexed with set name (mame)" << std::endl
			  << "  -ds, --delete-system      Delete a system and all his games" << std::endl
			  << "  -dg, --delete-game        Delete a game set by id" << std::endl
			  << "  -fn, --find-name          Find game sets by set name" << std::endl
			  << "  -fc, --find-crc           Find game sets by CRC" << std::endl
			  << "  -ft, --find-title         Find game sets by title" << std::endl;
}

void MetadbmApp::showVersion(void)
{
	std::cout << "Gelide Metadbm v" << PACKAGE_VERSION << " " << PACKAGE_CODENAME << std::endl
			  << "Copyright (C) 2008 - 2014 Juan Ángel Moreno Fernández" << std::endl;
}

void MetadbmApp::listSystems(void)
{
	std::vector<Glib::ustring> systems;
	std::vector<Glib::ustring>::iterator iter;

	m_db->systemGetAll(systems);

	std::cout << "Systems: " << systems.size() <<  std::endl;
	std::cout << "-------------------------------------" <<  std::endl;
	for (iter = systems.begin(); iter != systems.end(); ++iter)
	{
		std::cout << *iter <<  std::endl;
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
