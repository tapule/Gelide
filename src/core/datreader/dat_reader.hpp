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

#ifndef _DAT_READER_HPP_
#define _DAT_READER_HPP_

#include <glibmm/ustring.h>
#include <map>
#include "game.hpp"

/**
 * Clase base para construir parsers lectores de ficheros dat.
 */
class CDatReader
{
public:
	/**
	 * Constructor virtual, se encarga de iniciar el lector
	 */
	CDatReader(void);

	/**
	 * Destructor de la clase, debe encargarse de limpiar los datos del lector
	 */
	virtual ~CDatReader(void){};

	/**
	 * Carga un dat desde un fichero
	 * @param p_file Path del fichero dat
	 * @return true si se pudo cargar el fichero dat, false en otro caso
	 */
	virtual bool open(const Glib::ustring& p_file){
		return false;
	}

	/**
	 * Carga un dat desde un buffer de memoria
	 * @param p_buffer Buffer con los datos del dat
	 * @param p_size Tamaño total del buffer
	 * @return true si se pudo cargar el dat, false en otro caso
	 */
	virtual bool load(const char* p_buffer, int p_size){
		return false;
	}

	/**
	 * Obtiene los datos de los sets contenidos en el dat
	 * @param p_sets Mapa donde se almacenarán los sets del dat indexados por
	 * el nombre del set
	 * @return true si se realizó la lectura correctamente, false en otro caso
	 * @pre El mapa destino está listo para añadir los sets
	 */
	virtual bool read(std::map<Glib::ustring, CGame*>& p_sets){
		return false;
	}

	/**
	 * Resetea el lector liberando la memoria utilizada y limpiando sus campos
	 */
	virtual void reset(void);

	/**
	 * Indica si el lector tiene un dat cargado y está listo para su lectura
	 * @return true si el lector está listo, false en otro caso
	 */
	bool isLoaded(void){
		return m_loaded;
	}

	/**
	 * Obtiene una cadena identificativa del formato soportado por el lector
	 * @return Cadena descriptiva del tipo de dat soportado
	 */
	virtual Glib::ustring getType(void);

	/**
	 * Indica si se deben ignorar los sets bios durante la lectura del dat
	 * @return true si se deben ignorar los sets bios, false en otro caso
	 */
	bool getIgnoreBios(void){
		return m_ignore_bios;
	}

	/**
	 * Indica si se deben parsear los flags que aparecen en los títulos de los
	 * sets cada vez que se lee un set.
	 * @return true si se deben parsear, false en otro caso.
	 * @note Los flags en los títulos suelen indicar ciertos parámetros que los
	 * grupos de creación de dats (no-intro, goodtools, tosec, etc) dan al set.
	 */
	bool getParseFlags(void){
		return m_parse_flags;
	}

	/**
	 * Obtiene el nombre del dat contenido en la cabecera del mismo
	 * @return Cadena de texto con el nombre del dat
	 */
	Glib::ustring getName(void){
		return m_name;
	}

	/**
	 * Obtiene la descripción del dat contenida en la cabecera del mismo
	 * @return Descripción del dat
	 */
	Glib::ustring getDescription(void){
		return m_description;
	}

	/**
	 * Obtiene el campo categoría del dat contenido en la cabecera del mismo
	 * @return Categoría del dat
	 */
	Glib::ustring getCategory(void){
		return m_category;
	}

	/**
	 * Obtiene la cadena de versión del dat contenida en la cabecera del mismo
	 * @return Versión del dat
	 */
	Glib::ustring getVersion(void){
		return m_version;
	}

	/**
	 * Obtiene una cadena con la fecha del dat que aparece en la cabecera
	 * @return Fecha del dat
	 */
	Glib::ustring getDate(void){
		return m_date;
	}

	/**
	 * Obtiene el autor del dat según consta en la cabecera del mismo
	 * @return Autor del dat
	 */
	Glib::ustring getAuthor(void){
		return m_author;
	}

	/**
	 * Obtiene el email del autor del dat que aparece en la cabecera
	 * @return Email del autor
	 */
	Glib::ustring getEmail(void){
		return m_email;
	}

	/**
	 * Obtiene la cadena descriptiva de la web del autor en la cabecera del dat
	 * @return Cadena descriptiva
	 */
	Glib::ustring getHomepage(void){
		return m_homepage;
	}

	/**
	 * Obtiene la url de la web del autor que aparece en la cabecera del dat
	 * @return Url
	 */
	Glib::ustring getUrl(void){
		return m_url;
	}

	/**
	 * Obtiene los comentarios del autor que aparecen en la cabecera
	 * @return Comentarios del autor
	 */
	Glib::ustring getComment(void){
		return m_url;
	}

	/**
	 * Establece si se deben ignorar los sets bios durante la lectura del dat
	 * @param p_ignore Nuevo valor para el ignorado de sets bios
	 */
	void setIgnoreBios(bool p_ignore){
		m_ignore_bios = p_ignore;
	}

	/**
	 * Establece si se deben parsear los flags en los títulos de los sets
	 * @param p_parse Nuevo valor para el parseado de flags en los títulos
	 */
	void setParseFlags(bool p_parse){
		m_parse_flags = p_parse;
	}

protected:
	bool m_loaded;					/**< Se cargó el dat correctamente? */
	bool m_ignore_bios;				/**< Indíca si se deben parsear los sets bios */
	bool m_parse_flags;				/**< Indíca si se deben parsear los flags en el título del set */

	// Información en las cabeceras de los dats
	Glib::ustring m_name;			/**< Nombre del dat */
	Glib::ustring m_description;	/**< Descripción del dat */
	Glib::ustring m_category;		/**< Categoría del dat */
	Glib::ustring m_version;		/**< Versión del dat */
	Glib::ustring m_date;			/**< Fecha de creación del dat */
	Glib::ustring m_author;			/**< Autor del dat */
	Glib::ustring m_email;			/**< Email del autor del dat */
	Glib::ustring m_homepage;		/**< Descripción de la web del autor */
	Glib::ustring m_url;			/**< Url de la web del autor */
	Glib::ustring m_comment;		/**< Comentarios del autor */
};

#endif // _DAT_PARSER_HPP_
