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

#ifndef _GELIDE_HPP_
#define _GELIDE_HPP_

#include "../defines.hpp"
#include <glibmm/ustring.h>
#include "../core/dbmanager/db_manager.hpp"
#include "../utils/config.hpp"


namespace gelide{

/**
 */
class GelideApp
{
public:

	/** Posibles modos de ventana pasados por parámetros */
	static const int SCREEN_MODE_UNKNOWN	= -1;	/**< No se especificó el modo de ventana */
	static const int SCREEN_MODE_WINDOWED	= 0;	/**< Forzar windowd */
	static const int SCREEN_MODE_FULL		= 1;	/**< Forzar pantalla completa */

	/**
	 * Constructor parametrizado
	 * @param working_dir Directorio de trabajo
	 */
	GelideApp(const Glib::ustring& working_dir);

	/**
	 * Destructor de la clase
	 */
	virtual ~GelideApp(void);

	/**
	 * Ejecuta el bucle principal de la aplicación
	 * @param argc Contador de argumentos
	 * @param argv Vector de argumentos
	 * @return EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE si se produjo
	 * algún error.
	 */
	int run(int argc, char** argv);

private:

	/**
	 * Busca el fichero de configuración del usuario
	 * @param file Lugar donde dejar el path del fichero de configuración
	 * @return True si localizó el fichero de configuración del usuario, false
	 * en otro caso.
	 */
	bool findConfigFile(Glib::ustring& file);

	/**
	 * Parsea los parámetros pasados modificando el estado de la aplicación
	 * @param argc Contador de parámetros
	 * @param argv Vector de parámetros
	 * @return -1 si los parámetros son incorrectos
	 * 			0 si los parámetros son correctos pero se debe terminar la app
	 * 			1 si los parámetros son correctos y podemos continuar la app
	 */
	int parseParams(int argc, char** argv);

	/**
	 * Muestra por la salida estandar un mensaje con información de uso
	 * @param program Nombre real del binario ejecutado
	 */
	void showHelp(const Glib::ustring& program);

	/**
	 * Muestra por la salida estandar un mensaje con información de la versión
	 */
	void showVersion(void);

	/**
	 * Se encarga de cerrar adecuadamente los sistemas creados por el programa
	 */
	void clean(void);

	DbManager* m_db_manager;		/**< Gestor de la bd de la app */
	Config* m_config;				/**< Sistema de configuración para la app */

	bool m_first_run;				/**< Indica si se trata del primer arranque */
	Glib::ustring m_cfg_file;		/**< Path del fichero de configuración */
	Glib::ustring m_working_dir;	/**< Path del directorio de trabajo */
	int m_screen_mode;				/**< Modo de pantalla windowed/fullscreen */
};


} // namespace gelide

#endif // _GELIDE_HPP_
