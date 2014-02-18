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

#include "logiqxxml_reader.hpp"


namespace gelide{

LogiqxXmlReader::LogiqxXmlReader(void):
	DatReader()
{
}

LogiqxXmlReader::~LogiqxXmlReader(void)
{
	m_reader.close();
}

bool LogiqxXmlReader::load(const char* buffer, const unsigned int size)
{
	XmlNode node;

	assert(buffer);
	assert(size);

	if (m_reader.load(buffer, size))
	{
		// Comprobamos si el primer nodo es "datafile"
		node = m_reader.getRootElement();
		if (node.getName() == "datafile")
		{
			m_loaded = true;
			return true;
		}
	}
	return false;
}

bool LogiqxXmlReader::read(std::map<Glib::ustring, DatSet>& set_collection)
{
	XmlNode::iterator iter;
	DatSet set;

	assert(m_loaded);

	set_collection.clear();

	/*
	 * Estructura principal de un dat xml genérico de Logiqx:
	 *
	 * <datafile>
	 *	<header>...</header>
	 *	<game>...</game>
	 * </datafile>
	 */
	// Recorremos todos los nodos del xml buscando los bloques "game"
	for (iter = m_reader.getRootElement().begin(); iter != m_reader.getRootElement().end(); ++iter)
	{
		if (iter->getName() == "game")
		{
			if (this->parseGameBlock(set, *iter))
			{
				set_collection[set.name] = set;
			}
		}
	}
	return true;
}

bool LogiqxXmlReader::read(std::vector<DatSet>& set_collection)
{
	XmlNode::iterator iter;
	DatSet set;

	assert(m_loaded);

	set_collection.clear();

	/*
	 * Estructura principal de un dat xml genérico de Logiqx:
	 *
	 * <datafile>
	 *	<header>...</header>
	 *	<game>...</game>
	 * </datafile>
	 */
	// Recorremos todos los nodos del xml buscando los bloques "game"
	for (iter = m_reader.getRootElement().begin(); iter != m_reader.getRootElement().end(); ++iter)
	{
		if (iter->getName() == "game")
		{
			if (this->parseGameBlock(set, *iter))
			{
				set_collection.push_back(set);
			}
		}
	}
	return true;
}

Glib::ustring LogiqxXmlReader::getType()
{
	static const Glib::ustring type = "Logiqx XML ROM Management Datafile";

	return type;
}

bool LogiqxXmlReader::parseGameBlock(DatSet& set, XmlNode& node)
{
	Glib::ustring value;
	XmlNode::iterator iter;

	/*
	 * Estructura de un bloque game:
	 *
	 *	<game name="" sourcefile="" isbios=yes/no cloneof="" romof="" sampleof="" board="" rebuildto="">
	 *		<comment></comment>
	 *		<description></description>
	 *		<year></year>
	 *		<manufacturer></manufacturer>
	 *		<release />
	 *		<biosset />
	 *		<rom />
	 *		<disk />
	 *		<sample />
	 *		<archive />
	 *	</game>
	 *
	 */
	set.name.clear();
	set.description.clear();
	set.year.clear();
	set.manufacturer.clear();
	set.clone_of.clear();
	set.crc.clear();

	node.getAttribute("name", set.name);
	// Comprobamos si es un set bios y si debemos parserlos
	node.getAttribute("isbios", value);
	set.is_bios = (value == "yes" ? true : false);
	node.getAttribute("cloneof", set.clone_of);
	// Recorremos todos los nodos del set
	for (iter = node.begin(); iter != node.end(); ++iter)
	{
		value = iter->getName();
		if (value == "description")
		{
			iter->getContent(set.description);
		}
		else if (value == "year")
		{
			iter->getContent(set.year);
		}
		else if (value == "manufacturer")
		{
			iter->getContent(set.manufacturer);
		}
		else if ((value == "rom") && set.crc.empty())
		{
			set.crc = parseRomBlock(*iter);
		}
	}
	return true;
}

Glib::ustring LogiqxXmlReader::parseRomBlock(XmlNode& node)
{
	Glib::ustring crc;

	/*
	 * Estructura de un bloque rom o disk:
	 *
	 * <rom/disk name="" size="" crc="" sha1="" md5="" merge="" status=baddump|nodump|good|verified date=""/>
	 *
	 */
	// Obtenemos el nombre del set
	node.getAttribute("crc", crc);
	return crc;
}

} // namespace gelide
