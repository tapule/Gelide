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

#ifndef _DAT_READER_FACTORY_HPP_
#define _DAT_READER_FACTORY_HPP_

#include "../gelide.hpp"
#include "dat_reader.hpp"
#include <glibmm/fileutils.h>

/**
 * Clase factory para lectores de ficheros dat.
 *
 * Dispone de un solo método estático que se encargará de buscar el lector de
 * ficheros dat adecuado para el fichero pasado como parámetro.
 *
 */
class CDatReaderFactory
{
public:

	/**
	 * Obtiene un lector de ficheros dat adecuado para el fichero pasado
	 * @param p_file Path al fichero dat para el que se necesita un lector
	 * @return El lector adecuado para el fichero o NULL si el formato del
	 * fichero pasado no está soportado.
	 * @note El usuario debe encargarse de liberar la memoria ocupada por el
	 * lector devuelto por el método.
	 */
	static CDatReader* getDatReader(const Glib::ustring& p_file);

};

#endif // _DAT_READER_FACTORY_HPP_
