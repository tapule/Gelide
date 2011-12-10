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

#include "dat_reader_logiqxxml.hpp"
#include "../gelide.hpp"

CDatReaderLogiqxXml::CDatReaderLogiqxXml(void):
	CDatReader(){
}

CDatReaderLogiqxXml::~CDatReaderLogiqxXml(void){}

bool CDatReaderLogiqxXml::open(const Glib::ustring& p_file){
	CXmlNode l_node;

	// El fichero debe ser válido
	assert(p_file.size());

	// Reseteamos si es necesario
	if(m_loaded)
		reset();

	if(m_reader.open(p_file)){
		// Comprobamos si el primer nodo es "datafile"
		l_node = m_reader.getRootElement();
		if(l_node.getName() == "datafile"){
			m_loaded = true;
			parseHeaderBlock();
			return true;
		}
	}
	return false;
}

bool CDatReaderLogiqxXml::load(const char* p_buffer, int p_size){
	CXmlNode l_node;

	// Los datos tienen que ser válidos
	assert(p_buffer);
	assert(p_size);

	// Reseteamos si es necesario
	if(m_loaded)
		reset();

	if(m_reader.load(p_buffer, p_size)){
		// Comprobamos si el primer nodo es "datafile"
		l_node = m_reader.getRootElement();
		if(l_node.getName() == "datafile"){
			m_loaded = true;
			parseHeaderBlock();
			return true;
		}
	}
	return false;
}

bool CDatReaderLogiqxXml::read(std::map<Glib::ustring, CGame*>& p_sets){
	CXmlNode::iterator l_iter;
	CGame* l_game = NULL;

	if(!m_loaded)
		return false;

	/*
	 * Estructura principal de un dat xml genérico de Logiqx:
	 *
	 * <datafile>
	 *	<header>...</header>
	 *	<game>...</game>
	 * </datafile>
	 */
	// Recorremos todos los nodos del xml buscando los bloques "game"
	for(l_iter = m_reader.getRootElement().begin(); l_iter != m_reader.getRootElement().end(); ++l_iter){
		if(l_iter->getName() == "game"){
			l_game = this->parseGameBlock(*l_iter);
			if(l_game)
				p_sets[l_game->getName()] = l_game;
		}
	}
	return true;
}

void CDatReaderLogiqxXml::reset(void){
	CDatReader::reset();
	m_reader.close();
}

Glib::ustring CDatReaderLogiqxXml::getType(){
	static Glib::ustring l_type = "Logiqx XML ROM Management Datafile";

	return l_type;
}

void CDatReaderLogiqxXml::parseHeaderBlock(void){
	CXmlNode::iterator l_iter, l_iter_attr;
	Glib::ustring l_str;

	/*
	 * Estructura de un bloque header:
	 *
	 *	 <header>
	 *		<name></name>
	 *		<description></description>
	 *		<category></category>
	 *		<version></version>
	 *		<date></date>
	 *		<author></author>
	 *		<email></email>
	 *		<homepage></homepage>
	 *		<url>/</url>
	 *		<comment>/</comment>
	 *		<clrmamepro />
	 *		<romcenter />
	 *	</header>
	 *
	 */
	// Recorremos todos los nodos del xml buscando el header
	for(l_iter = m_reader.getRootElement().begin(); l_iter != m_reader.getRootElement().end(); ++l_iter){
		if(l_iter->getName() == "header"){
			// Encontramos el header, obtenemos sus campos
			for(l_iter_attr = l_iter->begin(); l_iter_attr != l_iter->end(); ++l_iter_attr){
				l_str = l_iter_attr->getName();
				if(l_str == "name")
					l_iter_attr->getContent(m_name);
				else if(l_str == "description")
					l_iter_attr->getContent(m_description);
				else if(l_str == "category")
					l_iter_attr->getContent(m_category);
				else if(l_str == "version")
					l_iter_attr->getContent(m_version);
				else if(l_str == "date")
					l_iter_attr->getContent(m_date);
				else if(l_str == "author")
					l_iter_attr->getContent(m_author);
				else if(l_str == "email")
					l_iter_attr->getContent(m_email);
				else if(l_str == "homepage")
					l_iter_attr->getContent(m_homepage);
				else if(l_str == "url")
					l_iter_attr->getContent(m_url);
				else if(l_str == "comment")
					l_iter_attr->getContent(m_comment);
			}
			return;
		}
	}
}

CGame* CDatReaderLogiqxXml::parseGameBlock(CXmlNode& p_node){
	Glib::ustring l_name, l_str;
	unsigned int l_uint;
	CXmlNode::iterator l_iter;
	CGame* l_game = NULL;

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

Glib::ustring CDatReaderLogiqxXml::parseRomBlock(CXmlNode& p_node){
	Glib::ustring l_str;

	/*
	 * Estructura de un bloque rom o disk:
	 *
	 * <rom/disk name="" size="" crc="" sha1="" md5="" merge="" status=baddump|nodump|good|verified date=""/>
	 *
	 */
	// Obtenemos el nombre del set
	p_node.getAttribute("crc", l_str);
	return l_str;
}

