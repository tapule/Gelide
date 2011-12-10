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

#include "xml_writer.hpp"


CXmlWriter::CXmlWriter():m_writer(0){
}

CXmlWriter::CXmlWriter(const Glib::ustring& p_file):m_writer(0){
	open(p_file);
}

CXmlWriter::~CXmlWriter(){
	close();
}

bool CXmlWriter::open(const Glib::ustring& p_file, const bool p_wdeclaration){

	assert(p_file.size());

	// Cerramos el writer si estuviera abierto
	close();
    // Creamos el writer sin compresión
    m_writer = xmlNewTextWriterFilename(p_file.c_str(), 0);

    if (m_writer == NULL)
        return false;
    // Establecemos la indentación
    xmlTextWriterSetIndent(m_writer, 1);

    // Añadimos la declaración xml si fuera necesario
    if(p_wdeclaration)
    	return writeDeclaration();
    return true;
}

void CXmlWriter::close(void){

	if(m_writer){
		// Finalizamos el documento y liberamos la memoria del writer
		xmlTextWriterEndDocument(m_writer);
		xmlFreeTextWriter(m_writer);
		m_writer = NULL;
	}
}

bool CXmlWriter::writeComment(const Glib::ustring& p_comment){
	int l_ret;

	assert(m_writer);

	l_ret = xmlTextWriterWriteComment(m_writer, BAD_CAST p_comment.c_str());

	if (l_ret < 0)
		return false;
	return true;
}

bool CXmlWriter::writeDeclaration(void){
	int l_ret;

    assert(m_writer);

    l_ret = xmlTextWriterStartDocument(m_writer, "1.0", "utf-8", NULL);
    if (l_ret < 0)
    	return false;
    return true;
}

bool CXmlWriter::startElement(const Glib::ustring& p_name){
	int l_ret;

	assert(m_writer);
	assert(p_name.size());

	l_ret = xmlTextWriterStartElement(m_writer, BAD_CAST p_name.c_str());

	if (l_ret < 0)
		return false;
	return true;
}

bool CXmlWriter::endElement(void){
	int l_ret;

	assert(m_writer);

	l_ret = xmlTextWriterEndElement(m_writer);

	if (l_ret < 0)
		return false;
	return true;
}


