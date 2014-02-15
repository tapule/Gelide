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

#include "xml_parser.hpp"
#include <cstring>


XmlParser::XmlParser()
{
	// Inicializamos todos los callbacks a 0 para evitar problemas
	memset( &m_sax_handler, 0, sizeof(m_sax_handler) );

	// Establecemos en el sax handler los callbacks que nos interesan
	m_sax_handler.startDocument = onStartDocument;
	m_sax_handler.endDocument = onEndDocument;
	m_sax_handler.startElement = onStartElement;
	m_sax_handler.endElement = onEndElement;
	m_sax_handler.characters = onContent;
	m_sax_handler.getEntity = onGetEntity;
	m_sax_handler.warning = onWarning;
	m_sax_handler.error = onError;
	m_sax_handler.fatalError = onFatalError;
}

bool XmlParser::parse(const Glib::ustring& file)
{
	int ret;

	// Iniciamos el parser sax pasando los callbacks y la referencia al parser
	ret = xmlSAXUserParseFile(&m_sax_handler, this, file.c_str());
	return ret;
}

void XmlParser::onStartDocument(void* data)
{
	XmlParser* parser;

	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);
	// Emitimos una señal indicando el inicio del documento
	parser->m_signal_start_document.emit();
}

void XmlParser::onEndDocument(void* data)
{
	XmlParser* parser;

	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);
	// Emitimos una señal indicando el fin del documento
	parser->m_signal_end_document.emit();
}

void XmlParser::onStartElement(void* data, const xmlChar* name, const xmlChar** attributes)
{
	XmlParser* parser;
	Glib::ustring element_name, element_key, element_value;
	std::map<Glib::ustring, Glib::ustring> attributes_map;
	const xmlChar** tmp;

	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);

	// Obtenemos el nombre del elemento
	element_name = reinterpret_cast<const char*>(name);
	// Obtenemos los atributos
	if (attributes)
	{
		tmp = attributes;
		// Mientras que queden valores para leer, estos vienen en pares
		while (*tmp)
		{
			// Obtenemos el nombre del atributo y su valor
			element_key = reinterpret_cast<const char* >(*(tmp++));
			element_value = reinterpret_cast<const char* >(*(tmp++));
			// Lo añadimos al mapa
			attributes_map[element_key] = element_value;
		}
	}
	// Emitimos una señal indicando el inicio del elemento
    parser->m_signal_start_element.emit(element_name, attributes_map);
}

void XmlParser::onEndElement(void *data, const xmlChar *name)
{
	XmlParser* parser;
	Glib::ustring element_name;


	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);
	element_name = reinterpret_cast<const char*>(name);

	// Emitimos una señal indicando el inicio del elemento
	parser->m_signal_end_element.emit(element_name);
}

void XmlParser::onContent(void *data, const xmlChar *content, int length)
{
	XmlParser* parser;
	Glib::ustring element_content;

	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);

	//BUG: Parece tener problemas con acentos
	// Añadimos el contenido a la cadena que se pasará en la señal
	element_content.append(reinterpret_cast<const char*>(content), length);
	// Emitimos una señal indicando el contenido del elemento
	parser->m_signal_content.emit(element_content);
	}

void XmlParser::onWarning(void *data, const char *msg, ...)
{
	XmlParser* parser;
	char buffer[2048];
	va_list args;
	Glib::ustring ret_msg;

	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);

	// Parseamos el mensaje
	va_start(args, msg);
	vsnprintf(buffer, 2048, msg, args);
	ret_msg.append(buffer);
	va_end(args);

	// Emitimos una señal indicando el mensaje del warning
	parser->m_signal_warning.emit(ret_msg);
}

void XmlParser::onError(void *data, const char *msg, ...)
{
	XmlParser* parser;
	char buffer[2048];
	va_list args;
	Glib::ustring ret_msg;

	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);

	// Parseamos el mensaje
	va_start(args, msg);
	vsnprintf(buffer, 2048, msg, args);
	ret_msg.append(buffer);
	va_end(args);

	// Emitimos una señal indicando el mensaje de error
	parser->m_signal_error.emit(ret_msg);
}

void XmlParser::onFatalError(void *data, const char *msg, ...)
{
	XmlParser* parser;
	char buffer[2048];
	va_list args;
	Glib::ustring ret_msg;

	// Obtenemos la referencia al parser usado
	parser = reinterpret_cast<XmlParser*>(data);

	// Parseamos el mensaje
	va_start(args, msg);
	vsnprintf(buffer, 2048, msg, args);
	ret_msg.append(buffer);
	va_end(args);

	// Emitimos una señal indicando el mensaje de error
	parser->m_signal_fatal_error.emit(ret_msg);
}
