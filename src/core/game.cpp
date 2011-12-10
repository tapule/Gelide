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

#include "game.hpp"

//#include <istream>

CGame::CGame(void){
	m_system_id = 0;
	m_favorite = false;
	m_name = "";
	m_rank = 0;
	m_year = 0;
	m_description = "";
	m_manufacturer = "";
	m_path = "";
	m_rom_crc = "";

	m_played = false;
	m_working = false;
	m_available = false;
	m_unknown = true;
}

// Obtener el indice del sistema
unsigned int CGame::getSystemId(void){
	return m_system_id;
}

bool CGame::getFavorite(void){
	return m_favorite;
}

Glib::ustring CGame::getName(void){
	return m_name;
}

unsigned int CGame::getRank(void){
	return m_rank;
}

unsigned int CGame::getYear(void){
	return m_year;
}

Glib::ustring CGame::getDescription(void){
	return m_description;
}

Glib::ustring CGame::getManufacturer(void){
	return m_manufacturer;
}

Glib::ustring CGame::getPath(void){
	return m_path;
}

Glib::ustring CGame::getRomCrc(void){
	return m_rom_crc;
}


// Obtener los flags del juego
int CGame::getFlags(void){
	int l_flags;

	// Codificación de los flags: x..xPWAU

	l_flags = 0x00;
	if(getPlayed()){
		l_flags |= 0x08;
	}
	if(getWorking()){
		l_flags |= 0x04;
	}
	if(getAvailable()){
		l_flags |= 0x02;
	}
	if(getUnknown()){
		l_flags |= 0x01;
	}
	return l_flags;
}

bool CGame::getPlayed(void){
	return m_played;
}

bool CGame::getWorking(void){
	return m_working;
}

bool CGame::getAvailable(void){
	return m_available;
}

bool CGame::getUnknown(void){
	return m_unknown;
}

// Establecer los campos del juego
void CGame::setSystemId(const unsigned int p_id){
	m_system_id = p_id;
}

void CGame::setFavorite(const bool p_favorite){
	m_favorite = p_favorite;
}

void CGame::setName(const Glib::ustring& p_name){
	m_name = p_name;
}

void CGame::setRank(const unsigned int p_rank){
	m_rank = p_rank;
}

void CGame::setYear(const unsigned int p_year){
	m_year = p_year;
}

void CGame::setDescription(const Glib::ustring& p_description){
	m_description = p_description;
}

void CGame::setManufacturer(const Glib::ustring& p_manufacturer){
	m_manufacturer = p_manufacturer;
}

void CGame::setPath(const Glib::ustring& p_path){
	m_path = p_path;
}

void CGame::setRomCrc(const Glib::ustring& p_crc){
	m_rom_crc = p_crc;
}

// Establecer los flags
void CGame::setFlags(const int p_flags){
	// Codificación de los flags: x..xPWAU
	m_played = p_flags & 0x08;
	m_working = p_flags & 0x04;
	m_available = p_flags & 0x02;
	m_unknown = p_flags & 0x01;
}

void CGame::setPlayed(const bool p_played){
	m_played = p_played;
}

void CGame::setWorking(const bool p_working){
	m_working = p_working;
}

void CGame::setAvailable(const bool p_available){
	m_available = p_available;
}

void CGame::setUnknown(const bool p_unknown){
	m_unknown = p_unknown;
}
