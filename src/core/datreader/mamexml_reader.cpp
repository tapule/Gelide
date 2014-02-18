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

#include "mamexml_reader.hpp"


namespace gelide{

MameXmlReader::MameXmlReader(void):
	DatReader()
{
}

MameXmlReader::~MameXmlReader(void)
{
	m_reader.close();
}

bool MameXmlReader::load(const char* buffer, const unsigned int size)
{
	XmlNode node;

	assert(buffer);
	assert(size);

	if (m_reader.load(buffer, size))
	{
		// Comprobamos si el primer nodo es "mame"
		node = m_reader.getRootElement();
		if (node.getName() == "mame")
		{
			m_loaded = true;
			return true;
		}
	}
	return false;
}

bool MameXmlReader::read(std::map<Glib::ustring, DatSet>& set_collection)
{
	XmlNode::iterator iter;
	DatSet set;
	Glib::ustring isdevice;

	assert(m_loaded);

	set_collection.clear();

	/*
	 * Estructura principal de un dat xml generado por Mame:
	 *
	 * <mame>
	 *	<game>...</game>
	 * </mame>
	 */
	// Recorremos todos los nodos del xml buscando los bloques "game"
	for (iter = m_reader.getRootElement().begin(); iter != m_reader.getRootElement().end(); ++iter)
	{
		iter->getAttribute("isdevice", isdevice);
		if ((iter->getName() == "game") && (isdevice == "no" ? true : false))
		{
			if (this->parseGameBlock(set, *iter))
			{
				set_collection[set.name] = set;
			}
		}
	}
	return true;
}

bool MameXmlReader::read(std::vector<DatSet>& set_collection)
{
	XmlNode::iterator iter;
	DatSet set;
	Glib::ustring isdevice;

	assert(m_loaded);

	set_collection.clear();

	/*
	 * Estructura principal de un dat xml generado por Mame:
	 *
	 * <mame>
	 *	<game>...</game>
	 *	<game ... isdevice=yes>
	 * </mame>
	 */
	// Recorremos todos los nodos del xml buscando los bloques "game"
	for (iter = m_reader.getRootElement().begin(); iter != m_reader.getRootElement().end(); ++iter)
	{
		iter->getAttribute("isdevice", isdevice);
		if ((iter->getName() == "game") && (isdevice == "no" ? true : false))
		{
			if (this->parseGameBlock(set, *iter))
			{
				set_collection.push_back(set);
			}
		}
	}
	return true;
}

Glib::ustring MameXmlReader::getType()
{
	static const Glib::ustring type = "Mame XML Dat File Format";

	return type;
}

bool MameXmlReader::parseGameBlock(DatSet& set, XmlNode& node)
{
	Glib::ustring value;
	XmlNode::iterator iter;

	/*
	 * Estructura de un bloque game:
	 *
	 *	<game name="" sourcefile="" isbios=yes/no isdevice=yes/no ismechanical=yes/no runnable="yes/no" cloneof="" romof="" sampleof="" >
	 *		<description></description>
	 *		<year></year>
	 *		<manufacturer></manufacturer>
	 *		<biosset />
	 *		<rom />
	 *		<disk />
	 *		<device_ref />
	 *		<sample />
	 *		<chip />
	 *		<display />
	 *		<sound />
	 *		<input service=yes/no tilt=yes/no players="" buttons="" coins=""></input>
	 *		<dipswitch></dipswitch>
	 *		<configuration></configuration>
	 *		<port></port>
	 *		<adjuster />
	 *		<category></category>	// Aparece en xml's viejos, pero nunca se usa
	 *		<driver />
	 *		<device></device>
	 *		<slot></slot>
	 *		<softwarelist />
	 *		<ramoption />
	 *	</game>
	 *
	 */
	set.name.clear();
	set.description.clear();
	set.year.clear();
	set.manufacturer.clear();
	set.clone_of.clear();
	set.crc.clear();
	set.players = 1;

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
		else if (value == "input")
		{
			iter->getAttribute("players", set.players);
		}
	}
	return true;
}

Glib::ustring MameXmlReader::parseRomBlock(XmlNode& node)
{
	Glib::ustring crc;

	/*
	 * Estructura de un bloque rom:
	 *
	 * <rom/disk name="" bios="" size="" crc="" md5="" sha1="" merge="" region="" offset="" status="baddump|nodump|good" date="" optional="yes/no" />
	 *
	 */
	// Obtenemos el nombre del set
		node.getAttribute("crc", crc);
		return crc;
}

} // namespace gelide
