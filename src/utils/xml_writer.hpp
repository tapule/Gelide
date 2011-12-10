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

#ifndef _XML_WRITER_HPP_
#define _XML_WRITER_HPP_

#include "../gelide.hpp"
#include "utils.hpp"
#include <glibmm/ustring.h>
#include <libxml2/libxml/xmlwriter.h>


/**
 * Clase orientada a la creación de ficheros xml de forma sencilla.
 *
 * Permite la creación de documentos xml con cualquier numero de elementos,
 * atributos, comentarios, etc.
 */
class CXmlWriter
{
public:

	/**
	 * Constructor básico
	 */
	CXmlWriter();

	/**
	 * Constructor con apertura directa de fichero para escritura del xml
	 * @param p_file Nombre que tendrá el fichero xml
	 */
	CXmlWriter(const Glib::ustring& p_file);

	/**
	 * Destructor básico
	 */
	~CXmlWriter();

	/**
	 * Inicia el fichero xml en disco
	 * @param p_file Nombre que tendrá el fichero xml
	 * @param p_wdeclaration Indica si se añade la declaración xml al crear
	 * @return true si se inicializó correctamente
	 */
	bool open(const Glib::ustring& p_file, const bool p_wdeclaration = true);

	/**
	 * Cierra el fichero xml terminando su escritura
	 */
	void close(void);

	/**
	 * Escribe un comentario en el documento
	 * @param p_comment Comentario a insertar
	 * @return true si se realizó con exito
	 */
	bool writeComment(const Glib::ustring& p_comment);

	/**
	 * Realiza la escritura de la declaración xml
	 * @return true si se realizo la escritura correctamente
	 * @note Siempre se utiliza la declaración:
	 * "<?xml version="1.0" encoding="UTF-8"?>"
	 */
	bool writeDeclaration(void);

	/**
	 * Comienza un nuevo elemento en el documento
	 * @param p_name nombre del elemento o nodo
	 * @return true si se realizó la tarea con exito
	 * @note Cada nuevo elemento se crea como hijo del anterior elemento no
	 * cerrado
	 */
	bool startElement(const Glib::ustring& p_name);

	/**
	 * Finaliza la escritura de un elemento
	 * @return true si se realizó con exito
	 */
	bool endElement(void);

	/**
	 * Escribe un atributo de un tipo dado en el último elemento abierto
	 * @param p_name Nombre del atributo
	 * @param p_value Valor del atributo
	 * @return true si se realizó con exito
	 * @note Aunque virtualmente el método puede escribir cualquier tipo de
	 * valor, depende directamente de gelide::utils::toStr y por lo tanto está
	 * orientada a la escritura de tipos básicos y vectores de tipos básicos
	 * (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool writeAttribute(const Glib::ustring& p_name, const T& p_value){
		int l_ret;

		assert(m_writer);
		assert(p_name.size());

		l_ret = xmlTextWriterWriteAttribute (m_writer, BAD_CAST p_name.c_str(),	BAD_CAST utils::toStr(p_value).c_str());

		if (l_ret < 0)
			return false;
		return true;
	}

	/**
	 * Escribe el contenido de un elemento abierto previamente
	 * @param p_conten Contenido a insertar en el elemento
	 * @return true su se realizó con exito
	 * @note Aunque virtualmente el método puede escribir cualquier tipo de
	 * valor, depende directamente de gelide::utils::toStr y por lo tanto está
	 * orientada a la escritura de tipos básicos y vectores de tipos básicos
	 * (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool writeContent(const T& p_content){
		int l_ret;

		assert(m_writer);

		l_ret = xmlTextWriterWriteString(m_writer, BAD_CAST utils::toStr(p_content).c_str());

		if (l_ret < 0)
			return false;
		return true;
	}

private:
	xmlTextWriterPtr m_writer;		/**< Writer xml interno */
};


#endif // _XML_WRITER_HPP_
