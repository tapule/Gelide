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

#include <sstream>
#include "statusbar.hpp"
#include "../utils/utils.hpp"

CStatusBar::CStatusBar(){
	initFlagsIcons();

	// Configuramos la etiqueta con la info del juego
	m_label_game.set_alignment(0, 0.5);
	m_label_game.set_padding(10, 0);
	m_label_game.set_ellipsize(Pango::ELLIPSIZE_END);

	// Configuración de la imagen con el estado
	m_image_status.set_padding(10, 0);

	// Configuración con la info del listado
	m_label_list.set_alignment(0, 0.5);
	m_label_list.set_padding(10, 0);

	// Empaquetamos los componentes de información
	m_hbox_info.pack_start(m_label_game);
	m_hbox_info.pack_start(m_image_status, false, true);
	m_hbox_info.pack_start(m_label_list, false, true);

	// Empaquetamos la información y el separador superior
	m_vbox_info.pack_start(m_hseparator);
	m_vbox_info.pack_start(m_hbox_info);

	// Configuramos la barra de estado
	m_statusbar.set_size_request(20, -1);

	// Empaquetamos todo
	this->pack_start(m_vbox_info);
	this->pack_start(m_statusbar, false, true);

	setListInfo(0, 0, 0);
	setWorking(false);
}

void CStatusBar::initFlagsIcons(void){
	m_flag_working[0] = this->render_icon(Gtk::StockID("gelide-working"), Gtk::ICON_SIZE_MENU);
	m_flag_working[1] = this->render_icon(Gtk::StockID("gelide-working-neg"), Gtk::ICON_SIZE_MENU);
}

void CStatusBar::setGameInfo(const Glib::ustring p_system, const Glib::ustring p_game){
	m_label_game.set_text(p_system + ": " + p_game);
}

void CStatusBar::setListInfo(const unsigned int p_list, const unsigned int p_system, const unsigned int p_total){
	std::stringstream l_str;

	l_str << p_list << "/" << p_system << "/" << p_total;
	m_label_list.set_text(l_str.str());
}

void CStatusBar::setWorking(const bool p_working){
	if(p_working)
		m_image_status.set(m_flag_working[0]);
	else
		m_image_status.set(m_flag_working[1]);
}


