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

#ifndef _MAMEXML_READER_HPP_
#define _MAMEXML_READER_HPP_

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
#include "../../utils/xml_reader.hpp"


namespace gelide{

/**
 * Parser lector para ficheros dat en el formato xml generado por Mame
 *
 * @note Más información en http://mamedev.org/
 * @note Para generar un dat con mame: mame -listxml
 */
class MameXmlReader : public DatReader
{
public:
	/**
	 * Constructor de la clase
	 */
	MameXmlReader(void);

	/**
	 * Destructor de la clase
	 */
	~MameXmlReader(void);

	/**
	 * Carga un dat desde un buffer de memoria
	 * @param buffer Buffer con los datos del dat
	 * @param size Tamaño total del buffer
	 * @return true si se pudo cargar el dat, false en otro caso
	 */
	bool load(const char* buffer, const unsigned int size);

	/**
	 * Obtiene los datos de los sets contenidos en el dat
	 * @param p_sets Mapa donde se almacenarán los sets del dat indexados por
	 * el nombre del set
	 * @return true si se realizó la lectura correctamente, false en otro caso
	 * @pre El mapa destino está listo para añadir los sets
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
	 * Realiza el análisis de un bloque "game"
	 * @param set Set donde dejará el resultado
	 * @param Nodo xml apuntando al bloque game
	 * @return true si se localizó un set, false en otro caso
	 */
	bool parseGameBlock(DatSet& set, XmlNode& node);

	/**
	 * Realiza el análisis de un bloque "rom" devolviendo su crc
	 * @param Nodo xml apuntando al bloque rom
	 * @return Cadena con el crc del bloque rom
	 */
	Glib::ustring parseRomBlock(XmlNode& node);

	XmlReader m_reader;	/**<Reader xml usado internamente */
};

} // namespace gelide

#endif // _MAMEXML_READER_HPP_
