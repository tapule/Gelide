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

#ifndef _DAT_READER_LOGIQXXML_HPP_
#define _DAT_READER_LOGIQXXML_HPP_

#include <glibmm/ustring.h>
#include <map>
#include "dat_reader.hpp"
#include "game.hpp"
#include "../utils/xml_reader.hpp"

/**
 * Parser lector para ficheros dat en el formato xml genérico de Logiqx
 * @note Más información en http://www.logiqx.com
 */
class CDatReaderLogiqxXml : public CDatReader
{
public:
	/**
	 * Constructor de la clase
	 */
	CDatReaderLogiqxXml(void);

	/**
	 * Destructor de la clase
	 */
	~CDatReaderLogiqxXml(void);

	/**
	 * Carga un dat desde un fichero
	 * @param p_file Path del fichero dat
	 * @return true si se pudo cargar el fichero dat, false en otro caso
	 */
	bool open(const Glib::ustring& p_file = "");

	/**
	 * Carga un dat desde un buffer de memoria
	 * @param p_buffer Buffer con los datos del dat
	 * @param p_size Tamaño total del buffer
	 * @return true si se pudo cargar el dat, false en otro caso
	 */
	bool load(const char* p_buffer, int p_size);

	/**
	 * Obtiene los datos de los sets contenidos en el dat
	 * @param p_sets Mapa donde se almacenarán los sets del dat indexados por
	 * el nombre del set
	 * @return true si se realizó la lectura correctamente, false en otro caso
	 * @pre El mapa destino está listo para añadir los sets
	 */
	bool read(std::map<Glib::ustring, CGame*>& p_sets);

	/**
	 * Resetea el lector liberando la memoria utilizada y limpiando sus campos
	 */
	void reset(void);

	/**
	 * Obtiene una cadena identificativa del formato soportado por el lector
	 * @return Cadena descriptiva del tipo de dat soportado
	 */
	Glib::ustring getType(void);

private:
	/**
	 * Realiza el análisis de los bloques "header" que contiene la cabecera
	 * con la información del dat.
	 */
	void parseHeaderBlock(void);

	/**
	 * Realiza el análisis de un bloque "game"
	 * @param Nodo xml apuntando al bloque game
	 * @return CGame con los datos leidos
	 * @pre El nodo xml pasado apunta a un bloque "game"
	 */
	CGame* parseGameBlock(CXmlNode& p_node);

	/**
	 * Realiza el análisis de un bloque "rom" devolviendo su crc
	 * @param Nodo xml apuntando al bloque rom
	 * @return Cadena con el crc del bloque rom
	 * @pre El nodo xml pasado apunta a un bloque "rom"
	 */
	Glib::ustring parseRomBlock(CXmlNode& p_node);

	CXmlReader m_reader;	/**<Reader xml usado internamente */
};

#endif // _DAT_READER_LOGIQXXML_HPP_
