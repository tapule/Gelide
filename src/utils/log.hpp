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

#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <cstdlib>
#include <fstream>
#include <glibmm/ustring.h>
#include "debug.hpp"


// Definición de macros para gestionar el sistema de log de forma transparente
#define LOG_OPEN(file) \
	Log::getInstance().open(file);\
	Log::getInstance().putTimestamp(); \
	Log::getInstance() << "Initializing Log system..."  << std::endl; \
	DEBUG("Initializing Log system...")

#define LOG_CLOSE() \
	Log::getInstance().putTimestamp(); \
	Log::getInstance() << "Shutting down Log system..."  << std::endl; \
	Log::getInstance().close(); \
	DEBUG("Shutting down Log system...")

#define LOG_INFO(str) \
	Log::getInstance().putTimestamp(); \
	Log::getInstance() << "Info**  " << str << std::endl; \
	DEBUG(str)

#define LOG_DEBUG(str) \
	Log::getInstance().putTimestamp(); \
	Log::getInstance() << "Debug**  " << str << std::endl; \
	DEBUG(str)

#define LOG_ERROR(str) \
	Log::getInstance().putTimestamp(); \
	Log::getInstance() << "Error**  " << str << std::endl; \
	WARNING(str)


/**
 * Clase encargada de gestionar el sistema de registro de sucesos (logger).
 *
 * La clase se define como un singleton.
 * Mantiene un fichero al que se van volcando los mensajes. Siempre se escribe
 * al final del fichero y solamente lo crea si no existe.
 * Lo ideal es no usar la clase directamente, sino usar las macros definidas
 * para el acceso al sistema de log de forma transparente
 *
 */
class Log
{
public:
	// Alias de un manipulador en iostream (ostream& manipulator(ostream&))
	typedef std::ostream& (*Manipulator)(std::ostream&);

protected:

	/**
	 * Constructor de la clase
	 */
	Log(void)
	{
	};

	/**
	 * Destructor
	 */
	~Log(void)
	{
		close();
	};

public:

	/**
	 * Obtiene la instancia única de la clase que se define como un singleton
	 * @return Instancia única de la clase
	 */
	static Log& getInstance(void)
	{
		static Log m_instance;
		return m_instance;
	}

	/**
	 * Operador << utilizando template
	 * @param value valor a volcar en el log
	 * @return El propio log
	 */
	template<class T>
	Log& operator<<(T value)
	{
		if (m_file.is_open())
		{
			m_file << value;
		}
		return *this;
	}

	/**
	 * Operador << utilizando para volcar los manipuladores estandar
	 * @param manipulator valor a volcar en el log
	 * @return El propio log
	 */
	Log& operator<<(Manipulator manipulator)
	{
		if (m_file.is_open())
		{
			m_file << manipulator;
		}
		return *this;
	}

	/**
	 * Abre un fichero como destino del log, añadiendo al final si existe
	 * @param file Fichero de destino para el log
	 * @note Si el sistema de log estaba inicializado, se cierra el fichero.
	 */
	void open(const Glib::ustring& file)
	{
		close();
		if (!file.empty())
		{
			m_file.open(file.c_str(), std::ofstream::out|std::ofstream::app);
		}
	}

	/**
	 * Cierra el fichero de destino del log
	 */
	void close(void)
	{
		if (m_file.is_open())
		{
			m_file.close();
		}
	}

	/**
	 * Coloca en el log una marca de tiempo con el formato "[HH:MM:SS]"
	 */
	void putTimestamp(void)
	{
		static char buff[10];
		time_t raw_time;
		struct tm * time_info;

		if (m_file.is_open())
		{
			time(&raw_time);
			time_info = localtime(&raw_time);
			// HH:MM:SS
			strftime(buff, 10, "%T", time_info);
			m_file << "[" << buff << "] ";
		}
	}

private:
	std::ofstream m_file;		/**< Fichero de destino del log */
};

#endif // _LOG_HPP_
