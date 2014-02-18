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
#include "gelide.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include "../utils/utils.hpp"
#include "../utils/sqlite.hpp"
//#include "gelide_gui.hpp"
//#include "../core/dbmetadata/db_metadata.hpp"
//#include "../utils/tokenizer.hpp"


namespace gelide{

GelideApp::GelideApp(const Glib::ustring& working_dir):
	m_db_manager(NULL),
	m_config(NULL),
	m_first_run(true),
	m_working_dir(working_dir),
	m_screen_mode(SCREEN_MODE_UNKNOWN)
{
	assert(!m_working_dir.empty());
}

GelideApp::~GelideApp(void)
{
};

int GelideApp::run(int argc, char** argv)
{
	int ret;
	Glib::ustring library, tmp, db_file;
#ifdef ENABLE_DEBUG_MODE
	bool log_enabled = true;
#else
	bool log_enabled = false;
#endif

	// Iniciamos el sistema de configuración
	LOG_DEBUG("Gelide: Initializing Config system...");
	m_config = new Config();
	m_config->setRoot(GELIDE_CFG_ROOT, GELIDE_CFG_VERSION);
	// Buscamos el fichero de configuración y lo cargamos
	if (findConfigFile(m_cfg_file))
	{
		m_config->load(m_cfg_file);
	}
	// Almacenamos el directorio de trabajo actual en la configuración
	m_config->setKey(GELIDE_CFG_GLOBAL, "current_working_dir", m_working_dir);

	// Parseamos los parámetros introducidos por linea de comandos
	LOG_DEBUG("Gelide: Parsing parameters...");
	ret = parseParams(argc, argv);
	if (ret < 1)
	{
		clean();
		if (ret == 0)
		{
			return EXIT_SUCCESS;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}

	// Inicializamos el sistema de log
	LOG_DEBUG("Gelide: Initializing log...");
	if (!m_config->getKey(GELIDE_CFG_GLOBAL, "log_enabled", log_enabled))
	{
		m_config->setKey(GELIDE_CFG_GLOBAL, "log_enabled", log_enabled);
	}
	if(log_enabled)
	{
		LOG_DEBUG("Gelide: Log enabled.");
		LOG_OPEN(Glib::build_filename(m_working_dir, GELIDE_LOG_FILE));
		// Mensajes de inicialización
		LOG_INFO("Initializing Gelide...");
		LOG_INFO("Gelide: Version " << PACKAGE_VERSION);
		LOG_INFO("Gelide: Working dir " << m_working_dir);
	}
	else
	{
		LOG_DEBUG("Gelide: Log disabled.");
	}

	// Comprobamos el directorio de datos del usuario (library)
	LOG_INFO("Gelide: Checking user library directory...");
	library = Glib::build_filename(m_working_dir, USER_LIBRARY_DIR);
	if (!utils::checkOrCreateDir(library))
	{
		clean();
		return EXIT_FAILURE;
	}
	tmp = Glib::build_filename(library, USER_COLLECTIONS_DIR);
	if (!utils::checkOrCreateDir(tmp))
	{
		clean();
		return EXIT_FAILURE;
	}

	// Comprobamos si existe el path para la librería
	if (!m_config->hasKey(GELIDE_CFG_GLOBAL, "library_path"))
	{
		m_config->setKey(GELIDE_CFG_GLOBAL, "library_path", library);
	}

	LOG_INFO("Gelide: Initializing data base engine...");
	if (!Sqlite::initialize())
	{
		clean();
		return EXIT_FAILURE;
	}

	LOG_INFO("Gelide: Initializing data base manager...");
	// Iniciamos el gestor de la base de datos
	m_db_manager = new DbManager();
	// Intentamos cargar la base de datos del usuario.
	// La base de datos determina si se trata del primer arranque
	db_file = Glib::build_filename(m_working_dir, GELIDE_DB_FILE);
	if (!m_db_manager->loadDataBase(db_file))
	{
		LOG_INFO("Gelide: User data base not found.");
		LOG_INFO("Gelide: Setting first run...");
		m_first_run = true;

		// Intentamos crear una BD nueva
		if (!m_db_manager->createDataBase(db_file))
		{
			LOG_ERROR("Gelide: Can't create a new data base.");
			clean();
			return EXIT_FAILURE;
		}
	}
	else
	{
		m_first_run = false;
	}

	/*******************************************************
	 * Bucle principal de la aplicación
	*******************************************************/
	LOG_INFO("Gelide: Running main_sec UI...");
	LOG_INFO("Gelide: Gtkmm version: " << GTKMM_MAJOR_VERSION << "." << GTKMM_MINOR_VERSION << "." << GTKMM_MICRO_VERSION );

	/*	GelideGui* gui;
		Gtk::Main kit(argc, argv);
		gui = new GelideGui(m_db_manager, m_config, m_first_run);
		kit.run(*gui);
		delete gui;*/


	/*******************************************************/

	// Limpiamos los sistemas creados
	LOG_INFO("Shutting down Gelide...");
	clean();

	return EXIT_SUCCESS;
}

bool GelideApp::findConfigFile(Glib::ustring& file)
{
	Glib::ustring cfg_file;

	// Construimos el path al fichero de configuración
	cfg_file = Glib::build_filename(m_working_dir, GELIDE_CFG_FILE);
	if (!Glib::file_test(cfg_file, Glib::FILE_TEST_EXISTS))
	{
		DEBUG("Gelide: Configuration file \""<< cfg_file << "\" not found");
		return false;
	}
	else
	{
		DEBUG("Gelide: Configuration file found \"" << cfg_file << "\"");
		file = cfg_file;
		return true;
	}
}

int GelideApp::parseParams(int argc, char** argv)
{
	int i;

	// Comenzamos en 1 ya que argv[0] es el nombre del programa ejecutado
	i = 1;
	while (i < argc)
	{
		// -v muestra su info y termina la ejecución de forma controlada
		if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--version") == 0))
		{
			showVersion();
			return 0;
		}
		// -h muestra su info y terminan la ejecución de forma controlada
		else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
		{
			showHelp(argv[0]);
			return 0;
		}
		// -l desactiva el log
		else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--disable-log") == 0))
		{
			DEBUG("Gelide: Disabling logging...");
			m_config->setKey(GELIDE_CFG_GLOBAL, "log_enabled", FALSE);
			return 1;
		}
		// -L activa el log
		else if ((strcmp(argv[i], "-L") == 0) || (strcmp(argv[i], "--enable-log") == 0))
		{
			DEBUG("Gelide: Enabling logging...");
			m_config->setKey(GELIDE_CFG_GLOBAL, "log_enabled", TRUE);
			return 1;
		}
		else if ((strcmp(argv[i], "-w") == 0) || (strcmp(argv[i], "--windowed") == 0))
		{
			DEBUG("Gelide: Disabling fullscreen...");
			m_config->setKey(GELIDE_CFG_UI, "fullscreen_mode", FALSE);
		}
		else if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--fullscreen") == 0))
		{
			DEBUG("Gelide: Enabling fullscreen...");
			m_config->setKey(GELIDE_CFG_UI, "fullscreen_mode", TRUE);
		}
		else{
			DEBUG("Gelide: Unknown option \"" << argv[i] << "\"");
			std::cout << "Unknown option \"" << argv[i] << "\"" << std::endl;
			return -1;
		}
		++i;
	}
	return 1;
}

void GelideApp::showHelp(const Glib::ustring& p_program){
	showVersion();
	std::cout << "Usage: " << p_program << " [Options]" << std::endl
			  << std::endl
			  << "Available options:" << std::endl
			  << "  -v, --version            Display Gelide version information and exit" << std::endl
			  << "  -h, --help               Display this help message and exit" << std::endl
			  << "  -l, --disable-log        Disable logging" << std::endl
			  << "  -L, --enable-log         Enable logging" << std::endl
			  << "  -w, --windowed           Force windowed mode" << std::endl
			  << "  -f, --fullscreen         Force full-screen mode" << std::endl;
}

void GelideApp::showVersion(void){
	std::cout << "Gelide v" << PACKAGE_VERSION << " " << PACKAGE_CODENAME << std::endl
			  << "Copyright (C) 2008 - 2013 Juan Ángel Moreno Fernández" << std::endl;
}

void GelideApp::clean(void)
{
	// Cerramos el gestor de la base de datos
	if (m_db_manager)
	{
		LOG_INFO("Gelide: Shutting down data base manager...");
		m_db_manager->closeDataBase();
		delete m_db_manager;
		m_db_manager = NULL;
		LOG_INFO("Gelide: Shutting down data base engine...");
		Sqlite::shutDown();
	}

	// Guardamos la configuración
	if (m_config)
	{
		LOG_INFO("Gelide: Shutting down Config system...");
		// Almacenamos antes el último directorio de trabajo
		m_config->setKey(GELIDE_CFG_GLOBAL, "last_working_dir", m_working_dir);
		m_cfg_file = Glib::build_filename(m_working_dir, GELIDE_CFG_FILE);
		m_config->save(m_cfg_file);
		delete m_config;
		m_config = NULL;
	}

	LOG_CLOSE();
}

} // namespace gelide
