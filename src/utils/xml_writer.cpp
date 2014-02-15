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

#include "xml_writer.hpp"


XmlWriter::XmlWriter():
	m_writer(0)
{
}

XmlWriter::XmlWriter(const Glib::ustring& file):
	m_writer(0)
{
	open(file);
}

XmlWriter::~XmlWriter()
{
	close();
}

bool XmlWriter::open(const Glib::ustring& file, const bool with_declaration)
{
	assert(!file.empty());

	// Cerramos el writer si estuviera abierto
	close();
    // Creamos el writer sin compresión
    m_writer = xmlNewTextWriterFilename(file.c_str(), 0);

    if (m_writer == NULL)
    {
        return false;
    }
    // Establecemos la indentación
    xmlTextWriterSetIndent(m_writer, 1);
    if (with_declaration)
    {
    	return writeDeclaration();
    }
    return true;
}

void XmlWriter::close(void)
{

	if (m_writer)
	{
		// Finalizamos el documento y liberamos la memoria del writer
		xmlTextWriterEndDocument(m_writer);
		xmlFreeTextWriter(m_writer);
		m_writer = NULL;
	}
}

bool XmlWriter::writeComment(const Glib::ustring& comment)
{
	int ret;

	assert(m_writer);

	ret = xmlTextWriterWriteComment(m_writer, BAD_CAST comment.c_str());
	if (ret < 0)
	{
		return false;
	}
	return true;
}

bool XmlWriter::writeDeclaration(void)
{
	int ret;

    assert(m_writer);

    ret = xmlTextWriterStartDocument(m_writer, "1.0", "utf-8", NULL);
    if (ret < 0)
    {
    	return false;
    }
    return true;
}

bool XmlWriter::startElement(const Glib::ustring& name)
{
	int ret;

	assert(m_writer);
	assert(!name.empty());

	ret = xmlTextWriterStartElement(m_writer, BAD_CAST name.c_str());
	if (ret < 0)
	{
		return false;
	}
	return true;
}

bool XmlWriter::endElement(void)
{
	int ret;

	assert(m_writer);

	ret = xmlTextWriterEndElement(m_writer);
	if (ret < 0)
	{
		return false;
	}
	return true;
}
