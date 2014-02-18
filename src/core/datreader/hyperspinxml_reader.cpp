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

#include "hyperspinxml_reader.hpp"


namespace gelide{

HyperspinXmlReader::HyperspinXmlReader(void):
	DatReader()
{
}

HyperspinXmlReader::~HyperspinXmlReader(void)
{
	m_reader.close();
}

bool HyperspinXmlReader::load(const char* buffer, const unsigned int size)
{
	XmlNode node;

	assert(buffer);
	assert(size);

	if (m_reader.load(buffer, size))
	{
		// Comprobamos si el primer nodo es "menu"
		node = m_reader.getRootElement();
		if (node.getName() == "menu")
		{
			m_loaded = true;
			return true;
		}
	}
	return false;
}

bool HyperspinXmlReader::read(std::map<Glib::ustring, DatSet>& set_collection)
{
	XmlNode::iterator iter;
	DatSet set;

	assert(m_loaded);

	set_collection.clear();

	/*
	 * Estructura principal de un dat xml en formato Hyperspin:
	 *
	 * <memu>
	 *  <header>...</header>
	 *	<game>...</game>
	 * </mame>
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

bool HyperspinXmlReader::read(std::vector<DatSet>& set_collection)
{
	XmlNode::iterator iter;
	DatSet set;

	assert(m_loaded);

	set_collection.clear();

	/*
	 * Estructura principal de un dat xml en formato Hyperspin:
	 *
	 * <memu>
	 *  <header>...</header>
	 *	<game>...</game>
	 * </mame>
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

Glib::ustring HyperspinXmlReader::getType()
{
	static const Glib::ustring type = "HyperSpin XML Dat File";

	return type;
}

bool HyperspinXmlReader::parseGameBlock(DatSet& set, XmlNode& node)
{
	Glib::ustring value;
	XmlNode::iterator iter;

	/*
	 * Estructura de un bloque game:
	 *
	 *	<game name="" index=yes/no image="" >
	 *		<description></description>
	 *		<cloneof></cloneof>
	 *		<crc></crc>
	 *		<manufacturer></manufacturer>
	 *		<year></year>
	 *		<genre></genre>
	 *		<rating></rating>
	 *		<enabled></enabled>
	 *	</game>
	 *
	 */
	set.name.clear();
	set.description.clear();
	set.clone_of.clear();
	set.crc.clear();
	set.manufacturer.clear();
	set.year.clear();
	set.genre.clear();
	set.players = 1;
	set.is_bios = false;

	node.getAttribute("name", set.name);
	// Recorremos todos los nodos del set
	for (iter = node.begin(); iter != node.end(); ++iter)
	{
		value = iter->getName();
		if (value == "description")
		{
			iter->getContent(set.description);
		}
		if (value == "cloneof")
		{
			iter->getContent(set.clone_of);
		}
		else if (value == "crc")
		{
			iter->getContent(set.crc);
		}
		else if (value == "manufacturer")
		{
			iter->getContent(set.manufacturer);
		}
		else if (value == "year")
		{
			iter->getContent(set.year);
		}
		else if (value == "genre")
		{
			iter->getContent(set.genre);
		}
	}
	return true;
}

} // namespace gelide
