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

#include "../defines.hpp"
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <glibmm/ustring.h>
#include <glibmm/miscutils.h>
#include "gelide.hpp"
#include "../utils/utils.hpp"
#include <vector>

using namespace gelide;

/**
 * Callback para la señar de terminación
 * @param sig Valor de la señal
 */
void signalTerm(int sig)
{
	LOG_DEBUG("SIGTERM");
	exit(0);
}

/**
 * Callback para la señar de interrupción
 * @param sig Valor de la señal
 */
void signalInt(int sig)
{
	LOG_DEBUG("SIGINT");
}

/**
 * Callback para la señar de asesinato
 * @param sig Valor de la señal
 */
void signalKill(int sig)
{
	LOG_DEBUG("SIGKILL");
}

int main (int argc, char *argv[])
{
	int app_ret;
	GelideApp* gelide;
	Glib::ustring working_dir;

#ifdef ENABLE_NLS
	bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
#endif


	// Registro de señales de terminación
	signal(SIGTERM, signalTerm);
	//signal(SIGINT, signalInt);
	//signal(SIGKILL, signalKill);

	srand(time(0));

	// Montamos el directorio sobre el que se debe ejecutar la aplicación
#ifdef ENABLE_LOCAL_MODE
	// En local mode siempre es el directorio actual
	working_dir = Glib::get_current_dir();
#else
	// En modo no local, usamos el directorio por defecto del usuario
	working_dir = Glib::build_filename(Glib::get_home_dir(), GELIDE_DEFAULT_DIR);
#endif

	// Comprobamos el directorio de trabajo antes de continuar
	if (utils::checkOrCreateDir(working_dir))
	{
		// Creamos la instancia de la aplicación principal y ejecutamos su bucle
		gelide = new GelideApp(working_dir);
		app_ret = gelide->run(argc, argv);
		delete gelide;
	}
	else
	{
		std::cout << _("Error**\t Can't initialize Gelide's working dir") << std::endl;
		app_ret = EXIT_FAILURE;
	}

	exit(app_ret);
}
