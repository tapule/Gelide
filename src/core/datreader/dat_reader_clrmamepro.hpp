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

#ifndef _DAT_READER_CLRMAMEPRO_HPP_
#define _DAT_READER_CLRMAMEPRO_HPP_

#include <glibmm/ustring.h>
#include <map>
#include "dat_reader.hpp"
#include "game.hpp"
#include "../utils/tokenizer.hpp"

/**
 * Parser lector para ficheros dat en formato ClrMamePro
 * @note Más información en http://mamedev.emulab.it/clrmamepro/index.htm
 */
class CDatReaderClrMamePro : public CDatReader
{
public:
	/**
	 * Constructor de la clase
	 */
	CDatReaderClrMamePro(void);

	/**
	 * Destructor de la clase
	 */
	~CDatReaderClrMamePro(void);

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
	 * Realiza el análisis de un bloque "clrmamepro" que contiene la cabecera
	 * con la información del dat.
	 */
	void parseHeaderBlock(void);

	/**
	 * Realiza el análisis de los bloques "game" y "resource"
	 * @return CGame con los datos leidos
	 */
	CGame* parseGameBlock(void);

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

	CTokenizer m_tokenizer;			/**< Tokenizador utilizado internamente */
};

#endif // _DAT_READER_CLRMAMEPRO_HPP_
