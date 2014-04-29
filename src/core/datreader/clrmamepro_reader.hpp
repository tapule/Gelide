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

#ifndef _CLRMAMEPRO_READER_HPP_
#define _CLRMAMEPRO_READER_HPP_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif /* HAVE_CONFIG_H */

// Si no está definido el modo debug, desactivamos los asserts
#ifndef ENABLE_DEBUG_MODE
	#define NDEBUG
#endif

#include <cassert>
#include <map>
#include <vector>
#include <glibmm/ustring.h>
#include "dat_reader.hpp"
#include "dat_set.hpp"
#include "../../utils/parser.hpp"


namespace gelide{

/**
 * Lector para ficheros dat en formato ClrMamePro
 *
 * Obtiene los sets en un fichero dat en formato ClrMamePro, incluyendo los sets
 * bios.
 * @note Este lector consume el recurso en su lectura y se resetea
 * @note Más información en http://mamedev.emulab.it/clrmamepro/index.htm
 */
class ClrMameProReader : public DatReader
{
public:
	/**
	 * Constructor de la clase
	 */
	ClrMameProReader(void);

	/**
	 * Destructor de la clase
	 */
	~ClrMameProReader(void);

	/**
	 * Carga un dat desde un buffer de memoria
	 * @param buffer Buffer con los datos del dat
	 * @param size Tamaño total del buffer
	 * @return true si se pudo cargar el dat, false en otro caso
	 */
	bool load(const char* buffer, const unsigned int size);

	/**
	 * Obtiene los datos de los sets contenidos en el dat
	 * @param set_collection Mapa donde se almacenarán los sets del dat
	 * indexados por el nombre del set
	 * @return true si se realizó la lectura correctamente, false en otro caso
	 * @note El mapa será vaciado previamente
	 */
	bool read(std::map<Glib::ustring, DatSet>& set_collection);

	/**
	 * Obtiene los datos de los sets contenidos en el dat
	 * @param set_collection Vector donde se almacenarán los sets del dat
	 * @return true si se realizó la lectura correctamente, false en otro caso
	 * @note El vector será vaciado previamente
	 */
	bool read(std::vector<DatSet>& set_collection);

	/**
	 * Obtiene una cadena identificativa del formato soportado por el lector
	 * @return Cadena descriptiva del tipo de dat soportado
	 */
	Glib::ustring getType(void);

private:

	/**
	 * Realiza el análisis de los bloques "game" y "resource"
	 * @param set Set donde dejará el resultado
	 * @return true si se localizó un set, false en otro caso
	 */
	bool parseGameBlock(DatSet& set);

	/**
	 * Realiza el análisis de un bloque "rom" devolviendo su crc
	 * @return Cadena con el crc del bloque rom
	 */
	Glib::ustring parseRomBlock(void);

	/**
	 * Salta un bloque completo encerrado entre '(' y ')' realizando conteo
	 * de bloques internos adicionales
	 */
	void skipBlock(void);

	/**
	 * Limpia los recursos usados por el lector y lo devuelve a su estado
	 * inicial.
	 */
	void clean(void);

	Parser m_parser;			/**<  utilizado internamente */
};

} // namespace gelide

#endif // _DAT_READER_CLRMAMEPRO_HPP_
