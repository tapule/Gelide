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

#ifndef _DAT_READER_FACTORY_HPP_
#define _DAT_READER_FACTORY_HPP_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif /* HAVE_CONFIG_H */

// Si no está definido el modo debug, desactivamos los asserts
#ifndef ENABLE_DEBUG_MODE
	#define NDEBUG
#endif

#include <cassert>
#include <glibmm/ustring.h>
#include "dat_reader.hpp"


namespace gelide{

/**
 * Clase factory para lectores de ficheros dat.
 *
 * Dispone de un solo método estático que se encargará de buscar el lector de
 * ficheros dat adecuado para el fichero pasado como parámetro.
 *
 */
class DatReaderFactory
{
public:

	/**
	 * Obtiene un lector de ficheros dat adecuado para el fichero pasado
	 * @param file Path al fichero dat para el que se necesita un lector
	 * @return El lector adecuado para el fichero o NULL si el formato del
	 * fichero pasado no está soportado.
	 * @note El usuario debe encargarse de liberar la memoria ocupada por el
	 * lector devuelto por el método.
	 */
	static DatReader* getDatReader(const Glib::ustring& file);

};

} // namespace gelide

#endif // _DAT_READER_FACTORY_HPP_
