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

#include "config.hpp"
#include <glibmm.h>
#include "utils/xml_reader.hpp"
#include "utils/xml_writer.hpp"

const Glib::ustring CConfig::m_version = "0.1";

CConfig::CConfig(){
	Glib::ustring l_cfg;

	l_cfg = Glib::build_filename(utils::getGelideDir(), GELIDE_CFG_FILE);
	// Comprobamos si existe el fichero de configuración personal
	if(Glib::file_test(l_cfg, Glib::FILE_TEST_EXISTS))
		load(l_cfg);
	else
		load(GELIDE_CFG_DEFAULT);
}

CConfig::~CConfig(void){
	save(Glib::build_filename(utils::getGelideDir(), GELIDE_CFG_FILE));
	clear();
};


/**
 * Limpia el almacen interno de claves
 */
void CConfig::clear(void){
/*	group_map_t::iterator l_giter;

	for(l_giter = m_groups.begin(); l_giter != m_groups.end(); ++l_giter)
		delete l_giter->second;*/
	m_groups.clear();
}


/*
 * Gelide config file format v0.1
 * ------------------------------
 * <gelidecfg version="0.1">
 * 		<group name="mygrup_1">
 *			<key name="mykey_1" value="myvalue">
 *			...
 *			<key name="mykey_n" value="myvalue">
 *		</group>
 *		...
 * 		<group name="mygrup_n">
 *			<key name="mykey_1" value="myvalue">
 *			...
 *			<key name="mykey_n" value="myvalue">
 *		</group>
 * </gelidecfg>
 */
bool CConfig::load(const Glib::ustring& p_file){
	CXmlReader l_xml;
	CXmlNode l_root;
	CXmlNode::iterator l_giter, l_kiter;
	key_map_t l_key;
	Glib::ustring l_gname, l_kname, l_kvalue;

	assert(p_file.size());

	GELIDE_DEBUG("Loading config file: " << p_file << "...");
	if(l_xml.open(p_file)){
		l_root = l_xml.getRootElement();		// <gelidecfg>
		if(l_root.getName() == "gelidecfg"){
			// Limpiamos el almacen interno de claves antes de continuar
			clear();
			// Guardamos el nuevo nombre del fichero
			m_file = p_file;
			// Recorremos los grupos
			for(l_giter = l_root.begin(); l_giter != l_root.end(); ++l_giter){
				// Obtenemos los valores de las claves
				for(l_kiter = l_giter->begin(); l_kiter != l_giter->end(); ++l_kiter){
					l_kiter->getAttribute("name", l_kname);
					l_kiter->getAttribute("value", l_kvalue);
					l_key[l_kname] = l_kvalue;
				}
				// Obtenemos el nombre del grupo
				l_giter->getAttribute("name", l_gname);
				// Creamos el grupo con una copia del mapa de claves creado
				m_groups[l_gname] = l_key;
				l_key.clear();
			}
			return true;
		}
	}
	GELIDE_WARNING(p_file << " is not a valid v" << m_version << " config file.");
	return false;
}

bool CConfig::save(const Glib::ustring& p_file){
	CXmlWriter l_xml;
	group_map_t::iterator l_giter;
	key_map_t::iterator l_kiter;

	assert(p_file.size());

	// Abrimos el xml
	if(!l_xml.open(p_file))
		return false;

	GELIDE_DEBUG("Saving config file: " << p_file << "...");
	l_xml.startElement("gelidecfg");
	l_xml.writeAttribute("version", m_version);
	for(l_giter = m_groups.begin(); l_giter != m_groups.end(); ++l_giter){
		l_xml.startElement("group");
		l_xml.writeAttribute("name", l_giter->first);
		for(l_kiter = l_giter->second.begin(); l_kiter != l_giter->second.end(); ++l_kiter){
			l_xml.startElement("key");
			l_xml.writeAttribute("name", l_kiter->first);
			l_xml.writeAttribute("value", l_kiter->second);
			l_xml.endElement();
		}
		l_xml.endElement();
	}
	l_xml.endElement();
	l_xml.close();
	return true;
}

bool CConfig::hasGroup(const Glib::ustring& p_group){
	group_map_t::iterator l_giter;

	assert(p_group.size());

	// Buscamos el grupo
	l_giter = m_groups.find(p_group);
	if(l_giter != m_groups.end())
		return true;
	return false;
}

bool CConfig::hasKey(const Glib::ustring& p_group, const Glib::ustring& p_key){
	group_map_t::iterator l_giter;
	key_map_t::iterator l_kiter;

	assert(p_group.size());
	assert(p_key.size());

	// Buscamos el grupo
	l_giter = m_groups.find(p_group);
	if(l_giter != m_groups.end()){
		// Buscamos la clave dentro del grupo
		l_kiter = l_giter->second.find(p_key);
		if(l_kiter != l_giter->second.end())
			return true;
	}
	return false;
}

