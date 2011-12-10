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

#include "xml_parser.hpp"
#include <cstring>



CXmlParser::CXmlParser(){
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

bool CXmlParser::parse(const Glib::ustring& p_file){
    int l_ret;

    // Iniciamos el parser sax pasando los callbacks y la referencia al parser
    l_ret = xmlSAXUserParseFile( &m_sax_handler, this, p_file.c_str());
    return l_ret;
}

void CXmlParser::onStartDocument(void *p_data){
	CXmlParser* l_parser;

	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);
	// Emitimos una señal indicando el inicio del documento
    l_parser->m_signal_start_document.emit();
}

void CXmlParser::onEndDocument(void *p_data){
	CXmlParser* l_parser;

	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);
	// Emitimos una señal indicando el fin del documento
    l_parser->m_signal_end_document.emit();
}

void CXmlParser::onStartElement(void *p_data, const xmlChar *p_name, const xmlChar **p_attributes){
	CXmlParser* l_parser;
	Glib::ustring l_name, l_key, l_value;
	std::map<Glib::ustring, Glib::ustring> l_attributes;
	const xmlChar** l_tmp;

	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);

	// Obtenemos el nombre del elemento
	l_name = reinterpret_cast<const char*>(p_name);
	// Obtenemos los atributos
	if(p_attributes){
		l_tmp = p_attributes;
		// Mientras que queden valores para leer, estos vienen en pares
		while (*l_tmp){
			// Obtenemos el nombre del atributo y su valor
			l_key = reinterpret_cast<const char* >(*(l_tmp++));
			l_value = reinterpret_cast<const char* >(*(l_tmp++));
			// Lo añadimos al mapa
			l_attributes[l_key] = l_value;
		}
	}
	// Emitimos una señal indicando el inicio del elemento
    l_parser->m_signal_start_element.emit(l_name, l_attributes);
}

void CXmlParser::onEndElement(void *p_data, const xmlChar *p_name){
	CXmlParser* l_parser;
	Glib::ustring l_name;


	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);

	// Obtenemos el nombre del elemento
	l_name = reinterpret_cast<const char*>(p_name);

	// Emitimos una señal indicando el inicio del elemento
    l_parser->m_signal_end_element.emit(l_name);
}


void CXmlParser::onContent(void *p_data, const xmlChar *p_content, int p_length){
	CXmlParser* l_parser;
	Glib::ustring l_content;

	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);

	//BUG: Parece tener problemas con acentos
	// Añadimos el contenido a la cadena que se pasará en la señal
	l_content.append(reinterpret_cast<const char*>(p_content), p_length);
	// Emitimos una señal indicando el contenido del elemento
    l_parser->m_signal_content.emit(l_content);
}


void CXmlParser::onWarning(void *p_data, const char *p_msg, ...){
	CXmlParser* l_parser;
	char l_buffer[2048];
    va_list l_args;
    Glib::ustring l_msg;


	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);

	// Parseamos el mensaje
	va_start(l_args, p_msg);
    vsnprintf(l_buffer, 2048, p_msg, l_args);
    l_msg.append(l_buffer);
    va_end(l_args);

	// Emitimos una señal indicando el mensaje del warning
    l_parser->m_signal_warning.emit(l_msg);
}

void CXmlParser::onError(void *p_data, const char *p_msg, ...){
	CXmlParser* l_parser;
	char l_buffer[2048];
    va_list l_args;
    Glib::ustring l_msg;


	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);

	// Parseamos el mensaje
	va_start(l_args, p_msg);
    vsnprintf(l_buffer, 2048, p_msg, l_args);
    l_msg.append(l_buffer);
    va_end(l_args);

	// Emitimos una señal indicando el mensaje de error
    l_parser->m_signal_error.emit(l_msg);
}

void CXmlParser::onFatalError(void *p_data, const char *p_msg, ...){
	CXmlParser* l_parser;
	char l_buffer[2048];
    va_list l_args;
    Glib::ustring l_msg;


	// Obtenemos la referencia al parser usado
	l_parser = reinterpret_cast<CXmlParser*>(p_data);

	// Parseamos el mensaje
	va_start(l_args, p_msg);
    vsnprintf(l_buffer, 2048, p_msg, l_args);
    l_msg.append(l_buffer);
    va_end(l_args);

	// Emitimos una señal indicando el mensaje de error
    l_parser->m_signal_fatal_error.emit(l_msg);
}


