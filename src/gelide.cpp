/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gelide
 * Copyright (C) 2008 - 2011 Juan Ángel Moreno Fernández
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

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include <csignal>
#include <glibmm.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include "gelide.hpp"
#include "config.hpp"
#include "utils/utils.hpp"
#include "ui/gelide_icons.hpp"
#include "ui/gelide_ui.hpp"


//using namespace std;

//#define PIXMAPS_DIR PACKAGE_DATA_DIR"/gelide/pixmaps"

/* For testing propose use the local (not installed) glade file */
/* #define GLADE_FILE PACKAGE_DATA_DIR"/gelide/glade/gelide.glade" */
//#define GLADE_FILE "gelide.glade"
/*
	Esto es lo que tendríamos que hacer para definir los path
	#define PLUGINS_DIR PACKAGE_DATA_DIR"/gelide/plugins"
	#define PIXMAPS_DIR PACKAGE_DATA_DIR"/gelide/pixmaps"

	y luego en el configure cambiar la variable libdir por
	libdir='${datadir}/gelide/plugins'
*/

void signalTerm(int sig){
	GELIDE_DEBUG("SIGTERM");

	CGelideUI::getInstance()->saveConfig();
	CSystemManager::getInstance()->save();
	exit(0);
}

void signalInt(int sig){
	GELIDE_DEBUG("SIGINT");
}

void signalKill(int sig){
	GELIDE_DEBUG("SIGKILL");
}

bool checkUserDir(void){
	Glib::ustring l_path;

	// Montamos el path para ~/.gelide
	l_path = Glib::build_filename(Glib::get_home_dir(), GELIDE_DIR);

	// Comprobamos si existe y lo creamos en caso contrario
	if(!Glib::file_test(l_path, Glib::FILE_TEST_EXISTS)){
		if(!utils::createDir(l_path))
			return false;
	}
	return true;
}


int main (int argc, char *argv[])
{
	 // register a SIGTERM handler
	signal(SIGTERM, signalTerm);
	//signal(SIGINT, signalInt);
	//signal(SIGKILL, signalKill);

#ifdef ENABLE_NLS
	bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
#endif

	Gtk::Main kit(argc, argv);
	CGelideIcons gelide_icons;



	if(checkUserDir()){
		gelide_icons.initDefaultIcons();
		kit.run(*CGelideUI::getInstance());
		exit(0);
	}
	else{
		GELIDE_ERROR("Unable to create user directory. Gelide can't continue.");
		exit(-1);
	}

}
