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

#include "dat_reader_mamexml.hpp"
#include "../gelide.hpp"

CDatReaderMameXml::CDatReaderMameXml(void):
	CDatReader(){
}

CDatReaderMameXml::~CDatReaderMameXml(void){}

bool CDatReaderMameXml::open(const Glib::ustring& p_file){
	CXmlNode l_node;

	// El fichero debe ser válido
	assert(p_file.size());

	// Reseteamos si es necesario
	if(m_loaded)
		reset();

	if(m_reader.open(p_file)){
		// Comprobamos si el primer nodo es "mame"
		l_node = m_reader.getRootElement();
		if(l_node.getName() == "mame"){
			m_loaded = true;
			parseHeaderBlock();
			return true;
		}
	}
	return false;
}

bool CDatReaderMameXml::load(const char* p_buffer, int p_size){
	CXmlNode l_node;

	// Los datos tienen que ser válidos
	assert(p_buffer);
	assert(p_size);

	// Reseteamos si es necesario
	if(m_loaded)
		reset();

	if(m_reader.load(p_buffer, p_size)){
		// Comprobamos si el primer nodo es "mame"
		l_node = m_reader.getRootElement();
		if(l_node.getName() == "mame"){
			m_loaded = true;
			parseHeaderBlock();
			return true;
		}
	}
	return false;
}

bool CDatReaderMameXml::read(std::map<Glib::ustring, CGame*>& p_sets){
	CXmlNode::iterator l_iter;
	CGame* l_game = NULL;

	if(!m_loaded)
		return false;

	/*
	 * Estructura principal de un dat xml generado por Mame:
	 *
	 * <mame>
	 *	<game>...</game>
	 * </mame>
	 */
	// Recorremos todos los nodos del xml buscando los bloques "game"
	for(l_iter = m_reader.getRootElement().begin(); l_iter != m_reader.getRootElement().end(); ++l_iter){
		l_game = this->parseGameBlock(*l_iter);
		if(l_game)
			p_sets[l_game->getName()] = l_game;
	}
	return true;
}

void CDatReaderMameXml::reset(void){
	CDatReader::reset();
	m_reader.close();
}

Glib::ustring CDatReaderMameXml::getType(){
	static Glib::ustring l_type = "Mame XML Dat File Format";

	return l_type;
}

void CDatReaderMameXml::parseHeaderBlock(void){
	CXmlNode l_root;
	Glib::ustring l_str;

	/*
	 * Estructura de un bloque header:
	 *
	 *	<mame build="" debug="yes/no" mameconfig="">
	 *
	 */
	l_root = m_reader.getRootElement();
	l_root.getAttribute("build", l_str);
	m_name = "Mame";
	m_description = _("Mame exe based dat file");
	m_category = _("Exe based dat");
	m_version = l_str;
	m_author = "mame";
	m_homepage = "http://mamedev.org/";
}

CGame* CDatReaderMameXml::parseGameBlock(CXmlNode& p_node){
	Glib::ustring l_name, l_str;
	unsigned int l_uint;
	CXmlNode::iterator l_iter;
	CGame* l_game = NULL;

	/*
	 * Estructura de un bloque game:
	 *
	 *	<game name="" sourcefile="" isbios=yes/no runnable="yes/no" cloneof="" romof="" sampleof="" >
	 *		<description></description>
	 *		<year></year>
	 *		<manufacturer></manufacturer>
	 *		<biosset />
	 *		<rom />
	 *		<disk />
	 *		<sample />
	 *		<chip />
	 *		<display />
	 *		<sound />
	 *		<input></input>
	 *		<dipswitch></dipswitch>
	 *		<configuration></configuration>
	 *		<category></category>
	 *		<adjuster />
	 *		<driver />
	 *		<device></device>
	 *		<softwarelist />
	 *	</game>
	 *
	 */
	// Comprobamos si es un set bios y si debemos parserlos
	p_node.getAttribute("isbios", l_str);
	if(m_ignore_bios && (l_str == "yes"))
		return NULL;

	l_game = new CGame();
	l_game->setUnknown(false);
	// Obtenemos el nombre del set
	p_node.getAttribute("name", l_str);
	l_game->setName(l_str);

	// Recorremos todos los nodos del xml buscando el header
	for(l_iter = p_node.begin(); l_iter != p_node.end(); ++l_iter){
		l_name = l_iter->getName();
		if(l_name == "description"){
			l_iter->getContent(l_str);
			l_game->setDescription(l_str);
		}
		else if(l_name == "year"){
			l_iter->getContent(l_uint);
			l_game->setYear(l_uint);
		}
		else if(l_name == "manufacturer"){
			l_iter->getContent(l_str);
			l_game->setManufacturer(l_str);
		}
		else if((l_name == "rom") && !l_game->getRomCrc().size())
			l_game->setRomCrc(parseRomBlock(*l_iter));
	}
	return l_game;
}

Glib::ustring CDatReaderMameXml::parseRomBlock(CXmlNode& p_node){
	Glib::ustring l_str;

	/*
	 * Estructura de un bloque rom:
	 *
	 * <rom/disk name="" bios="" size="" crc="" md5="" sha1="" merge="" region="" offset="" status="baddump|nodump|good" date="" optional="yes/no" />
	 *
	 */
	// Obtenemos el nombre del set
	p_node.getAttribute("crc", l_str);
	return l_str;
}

