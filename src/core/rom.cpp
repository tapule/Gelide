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

#include "rom.hpp"

CRom::CRom(void){
	m_type = ROM_TYPE_ROM;
	m_state = ROM_STATE_UNKNOWN;
	m_name = "";
	m_size = 0;
	m_crc = "";
	m_merge = false;
}

CRom::CRom(const int p_type, const int p_state, const Glib::ustring& p_name,
			const unsigned int p_size,	const Glib::ustring& p_crc, const bool p_merge){
	m_type = p_type;
	m_state = p_state;
	m_name = p_name;
	m_size = p_size;
	m_crc = p_crc;
	m_merge = p_merge;
}

CRom::~CRom(void){
}

void CRom::setType(const int p_type){
	m_type = p_type;
}

void CRom::setState(const int p_state){
	m_state = p_state;
}

void CRom::setName(const Glib::ustring& p_name){
	m_name = p_name;
}

void CRom::setSize(const unsigned int p_size){
	m_size = p_size;
}

void CRom::setCrc(const Glib::ustring& p_crc){
	m_crc = p_crc;
}

void CRom::setMerge(const bool p_merge){
	m_merge = p_merge;
}

int CRom::getType(void){
	return m_type;
}

int CRom::getState(void){
	return m_state;
}

Glib::ustring CRom::getName(void){
	return m_name;
}

unsigned int CRom::getSize(void){
	return m_size;
}

Glib::ustring CRom::getCrc(void){
	return m_crc;
}

bool CRom::getMerge(void){
	return m_merge;
}
