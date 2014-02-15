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

#ifndef _XML_WRITER_HPP_
#define _XML_WRITER_HPP_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif /* HAVE_CONFIG_H */

// Si no está definido el modo debug, desactivamos los asserts
#ifndef ENABLE_DEBUG_MODE
	#define NDEBUG
#endif

#include <cassert>
#include <glibmm/ustring.h>
#include <libxml2/libxml/xmlwriter.h>
#include "utils.hpp"


/**
 * Clase orientada a la creación de ficheros xml de forma sencilla.
 *
 * Permite la creación de documentos xml con cualquier numero de elementos,
 * atributos, comentarios, etc.
 */
class XmlWriter
{
public:

	/**
	 * Constructor básico
	 */
	XmlWriter();

	/**
	 * Constructor con apertura directa de fichero para escritura del xml
	 * @param file Nombre que tendrá el fichero xml
	 */
	XmlWriter(const Glib::ustring& file);

	/**
	 * Destructor básico
	 */
	~XmlWriter();

	/**
	 * Inicia el fichero xml en disco
	 * @param file Nombre que tendrá el fichero xml
	 * @param with_declaration Indica si se añade la declaración xml al crear
	 * @return true si se inicializó correctamente
	 */
	bool open(const Glib::ustring& file, const bool with_declaration = true);

	/**
	 * Cierra el fichero xml terminando su escritura
	 */
	void close(void);

	/**
	 * Escribe un comentario en el documento
	 * @param comment Comentario a insertar
	 * @return true si se realizó con exito
	 */
	bool writeComment(const Glib::ustring& comment);

	/**
	 * Realiza la escritura de la declaración xml
	 * @return true si se realizo la escritura correctamente
	 * @note Siempre se utiliza la declaración:
	 * "<?xml version="1.0" encoding="UTF-8"?>"
	 */
	bool writeDeclaration(void);

	/**
	 * Comienza un nuevo elemento en el documento
	 * @param name nombre del elemento o nodo
	 * @return true si se realizó la tarea con exito
	 * @note Cada nuevo elemento se crea como hijo del anterior elemento no
	 * cerrado
	 */
	bool startElement(const Glib::ustring& name);

	/**
	 * Finaliza la escritura de un elemento
	 * @return true si se realizó con exito
	 */
	bool endElement(void);

	/**
	 * Escribe un atributo de un tipo dado en el último elemento abierto
	 * @param name Nombre del atributo
	 * @param value Valor del atributo
	 * @return true si se realizó con exito
	 * @note Aunque virtualmente el método puede escribir cualquier tipo de
	 * valor, depende directamente de utils::toStr y por lo tanto está
	 * orientada a la escritura de tipos básicos y vectores de tipos básicos
	 * (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool writeAttribute(const Glib::ustring& name, const T& value)
	{
		int ret;

		assert(m_writer);
		assert(!name.empty());

		ret = xmlTextWriterWriteAttribute(m_writer, BAD_CAST name.c_str(), BAD_CAST utils::toStr(value).c_str());
		if (ret < 0)
		{
			return false;
		}
		return true;
	}

	/**
	 * Escribe el contenido de un elemento abierto previamente
	 * @param conten Contenido a insertar en el elemento
	 * @return true su se realizó con exito
	 * @note Aunque virtualmente el método puede escribir cualquier tipo de
	 * valor, depende directamente de utils::toStr y por lo tanto está
	 * orientada a la escritura de tipos básicos y vectores de tipos básicos
	 * (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool writeContent(const T& content)
	{
		int ret;

		assert(m_writer);

		ret = xmlTextWriterWriteString(m_writer, BAD_CAST utils::toStr(content).c_str());
		if (ret < 0)
		{
			return false;
		}
		return true;
	}

private:
	xmlTextWriterPtr m_writer;		/**< Writer xml interno */
};

#endif // _XML_WRITER_HPP_
