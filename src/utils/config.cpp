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

#include <glibmm.h>
#include "config.hpp"
#include "xml_reader.hpp"
#include "xml_writer.hpp"


Config::Config():
	m_root(CFG_DEFAULT_ROOT)
{
}

Config::~Config(void)
{
}

void Config::clear(void)
{
	m_groups.clear();
}

bool Config::load(const Glib::ustring& file)
{
	XmlReader xml;
	XmlNode root;
	XmlNode::iterator group_iter, key_iter;
	KeyMap key_map;
	Glib::ustring group_name, key_name, key_value;

	assert(!file.empty());

	LOG_INFO("Config: Loading config file \"" << file << "\"...");
	if (xml.open(file))
	{
		root = xml.getRootElement();		// <rootnode>
		if (root.getName() == m_root)
		{
			// Limpiamos el almacen interno de claves antes de continuar
			clear();
			m_file = file;
			// Recorremos los grupos
			for (group_iter = root.begin(); group_iter != root.end(); ++group_iter)
			{
				// Obtenemos los valores de las claves
				for (key_iter = group_iter->begin(); key_iter != group_iter->end(); ++key_iter)
				{
					key_iter->getAttribute("name", key_name);
					key_iter->getAttribute("value", key_value);
					key_map[key_name] = key_value;
				}
				group_iter->getAttribute("name", group_name);
				// Creamos el grupo con una copia del mapa de claves creado
				m_groups[group_name] = key_map;
				key_map.clear();
			}
			return true;
		}
		LOG_INFO("Config: Root node \"" << m_root<< "\" not found in \""<< file << "\"");
		return false;
	}
	LOG_ERROR("Config: Can't open config file \""<< file << "\" for reading");
	return false;
}

bool Config::save(const Glib::ustring& file)
{
	XmlWriter xml;
	GroupMap::iterator group_iter;
	KeyMap::iterator key_iter;

	assert(!file.empty());

	LOG_INFO("Config: Saving config file \"" << file << "\"...");

	if (!xml.open(file))
	{
		LOG_ERROR("Config: Can't open config file \""<< file << "\" for writing");
		return false;
	}

	xml.startElement(m_root);
	if (!m_version.empty())
	{
		xml.writeAttribute("version", m_version);
	}
	for (group_iter = m_groups.begin(); group_iter != m_groups.end(); ++group_iter)
	{
		xml.startElement("group");
		xml.writeAttribute("name", group_iter->first);
		for (key_iter = group_iter->second.begin(); key_iter != group_iter->second.end(); ++key_iter)
		{
			xml.startElement("key");
			xml.writeAttribute("name", key_iter->first);
			xml.writeAttribute("value", key_iter->second);
			xml.endElement();
		}
		xml.endElement();
	}
	xml.endElement();
	xml.close();
	return true;
}

bool Config::hasGroup(const Glib::ustring& group)
{
	GroupMap::iterator group_iter;

	assert(!group.empty());

	// Buscamos el grupo
	group_iter = m_groups.find(group);
	if (group_iter != m_groups.end())
	{
		return true;
	}
	return false;
}

bool Config::hasKey(const Glib::ustring& group, const Glib::ustring& key)
{
	GroupMap::iterator group_iter;
	KeyMap::iterator key_iter;

	assert(!group.empty());
	assert(!key.empty());

	// Buscamos el grupo
	group_iter = m_groups.find(group);
	if (group_iter != m_groups.end())
	{
		// Buscamos la clave dentro del grupo
		key_iter = group_iter->second.find(key);
		if (key_iter != group_iter->second.end())
		{
			return true;
		}
	}
	return false;
}
