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

#include "emulator.hpp"

CEmulator::CEmulator(){
	// Información de descripción del emulador
	m_system_id = 0;;
	m_id = 0;
	m_icon_file = "";
	m_name = "";
	m_description = "";
	m_version = "";
	m_author = "";
	m_homepage = "";
	m_path = "";
	m_params = "";
}
// Obtener los campos del emu
unsigned int CEmulator::getSystemId(void){
	return m_system_id;
}
unsigned int CEmulator::getId(void){
	return m_id;
}

Glib::ustring CEmulator::getIconFile(void){
	return m_icon_file;
}

Glib::RefPtr<Gdk::Pixbuf> CEmulator::getIcon(void){
	return m_icon;
}

Glib::ustring CEmulator::getName(void){
	return m_name;
}

Glib::ustring CEmulator::getDescription(void){
	return m_description;
}

Glib::ustring CEmulator::getVersion(void){
	return m_version;
}

Glib::ustring CEmulator::getAuthor(void){
	return m_author;
}

Glib::ustring CEmulator::getHomepage(void){
	return m_homepage;
}

Glib::ustring CEmulator::getPath(void){
	return m_path;
}

Glib::ustring CEmulator::getParams(void){
	return m_params;
}

// Establecer los campos del emu
void CEmulator::setSystemId(const unsigned int p_id){
	m_system_id = p_id;
}

void CEmulator::setId(const unsigned int p_id){
	m_id = p_id;
}

void CEmulator::setIconFile(const Glib::ustring& p_icon){
	m_icon_file = p_icon;
}

void CEmulator::setIcon(Glib::RefPtr<Gdk::Pixbuf> p_icon){
	m_icon = p_icon;
}

void CEmulator::setName(const Glib::ustring& p_name){
	m_name = p_name;
}

void CEmulator::setDescription(const Glib::ustring& p_description){
	m_description = p_description;
}

void CEmulator::setVersion(const Glib::ustring& p_version){
	m_version = p_version;
}

void CEmulator::setAuthor(const Glib::ustring& p_author){
	m_author = p_author;
}

void CEmulator::setHomepage(const Glib::ustring& p_homepage){
	m_homepage = p_homepage;
}

void CEmulator::setPath(const Glib::ustring& p_path){
	m_path = p_path;
}

void CEmulator::setParams(const Glib::ustring& p_params){
	m_params = p_params;
}
