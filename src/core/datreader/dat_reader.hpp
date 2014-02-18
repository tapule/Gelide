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

#ifndef _DAT_READER_HPP_
#define _DAT_READER_HPP_

#include <map>
#include <vector>
#include <glibmm/ustring.h>
#include "dat_set.hpp"


namespace gelide{

/**
 * Clase base para construir lectores de ficheros dat.
 *
 * Proporciona una interfaz común para todos los parsers de ficheros dat.
 */
class DatReader
{
public:
	/**
	 * Constructor, se encarga de iniciar el lector
	 */
	DatReader(void):
		m_loaded(false)
	{
	}

	/**
	 * Destructor de la clase, debe encargarse de limpiar los datos del lector
	 */
	virtual ~DatReader(void)
	{
	}

	/**
	 * Carga un dat desde un buffer de memoria
	 * @param buffer Buffer con los datos del dat
	 * @param size Tamaño total del buffer
	 * @return true si se pudo cargar el dat, false en otro caso
	 */
	virtual bool load(const char* buffer, const unsigned int size)
	{
		return false;
	}

	/**
	 * Obtiene los datos de los sets contenidos en el dat
	 * @param set_collection Mapa donde se almacenarán los sets del dat
	 * indexados por el nombre del set
	 * @return true si se realizó la lectura correctamente, false en otro caso
	 * @note El mapa será vaciado previamente
	 */
	virtual bool read(std::map<Glib::ustring, DatSet>& set_collection)
	{
		return false;
	}

	/**
	 * Obtiene los datos de los sets contenidos en el dat
	 * @param set_collection vector donde se almacenarán los sets del dat
	 * @return true si se realizó la lectura correctamente, false en otro caso
	 * @note el vector será vaciado previamente
	 */
	virtual bool read(std::vector<DatSet>& set_collection)
	{
		return false;
	}

	/**
	 * Indica si el lector tiene un dat cargado y está listo para su lectura
	 * @return true si el lector está listo, false en otro caso
	 */
	bool isLoaded(void)
	{
		return m_loaded;
	}

	/**
	 * Obtiene una cadena identificativa del formato soportado por el lector
	 * @return Cadena descriptiva del tipo de dat soportado
	 */
	virtual Glib::ustring getType(void)
	{
		return "";
	}

protected:
	bool m_loaded;	/**< Indica si se cargó el dat correctamente */
};

} // namespace gelide

#endif // _DAT_PARSER_HPP_
