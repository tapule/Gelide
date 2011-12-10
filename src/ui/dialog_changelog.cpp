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

#include "dialog_changelog.hpp"
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>


CDialogChangelog::CDialogChangelog(void):
	Gtk::Dialog(),
	m_button_close(Gtk::Stock::CLOSE)
{
	// Obtenemos las instancias
	m_config = CConfig::getInstance();

	// Configuración visual del dialogo
	this->set_title( _("Gelide ChangeLog") );
	this->set_resizable(true);
	this->set_size_request(570, -1);
	this->set_modal(true);
	this->set_border_width(5);

	// Configuración del titulo
	m_label_title.set_markup((Glib::ustring) "<b><big>" + _("Gelide changes history") + "</big></b>");
	m_label_title.set_alignment(0,0.5);
	m_image_title.set(Gtk::Stock::DIALOG_INFO, Gtk::ICON_SIZE_DIALOG);
	m_image_title.set_padding(15,15);
	m_hbox_title.pack_start(m_image_title, false, false);
	m_hbox_title.pack_start(m_label_title, false, false);
	m_hbox_title.set_spacing(3);

	// Configuracion del textview para mensajes
	m_text_changelog.set_size_request(-1, 300);
	m_text_changelog.set_editable(false);
	m_text_changelog.set_wrap_mode(Gtk::WRAP_WORD);
	m_textbuf_changelog = Gtk::TextBuffer::create();
	m_text_changelog.set_buffer(m_textbuf_changelog);
	// Configuración del scroll que contiene la descripcion
	m_scroll_changelog.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	m_scroll_changelog.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_changelog.add(m_text_changelog);

	// Configuración del boton close
	m_button_close.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogChangelog::onCloseClicked) );

	// Añadimos el notebook y los botones al dialogo
	this->get_vbox()->set_spacing(5);
	this->get_vbox()->pack_start(m_hbox_title, false, false);
	this->get_vbox()->pack_start(m_scroll_changelog, true, true);
	this->get_action_area()->pack_start(m_button_close);
	this->get_action_area()->set_layout(Gtk::BUTTONBOX_END);

	// Establecemos el diálogo por defecto en el centro del padre
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	// Cargamos la configuración
	loadConfig();

	loadText();

	// Mostramos todos los widgets
	this->show_all_children();
}

CDialogChangelog::~CDialogChangelog(){
	saveConfig();
}


void CDialogChangelog::loadConfig(void){
	int l_x, l_y;

	// Obtenemos la posición almacenada
	//m_config->getChangeLogDialogPos(l_x, l_y);
	m_config->getKey(GELIDE_CFG_DIALOGS, "changelog_dialog_x", l_x);
	m_config->getKey(GELIDE_CFG_DIALOGS, "changelog_dialog_y", l_y);
	if((l_x!= -1) && (l_y != -1))
		this->move(l_x, l_y);
}

void CDialogChangelog::saveConfig(void){
	int l_x, l_y;

	// Guardamos la posición del dialogo
	this->get_position(l_x, l_y);
	//m_config->setChangeLogDialogPos(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "changelog_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "changelog_dialog_y", l_y);
}

void CDialogChangelog::onCloseClicked(void){
	this->response(Gtk::RESPONSE_CLOSE);
}

void CDialogChangelog::loadText(void){
	std::ifstream l_file;
	char* l_buff;
	int l_fsize;
	int l_readed;

	GELIDE_DEBUG(GELIDE_CHANGELOG);
	l_file.open(GELIDE_CHANGELOG);
	// Comprobamos si la apertura fue correcta
	if(!l_file.good()){
		l_file.close();
		return;
	}

	// Obtenemos el tamaño del fichero
	l_file.seekg (0, std::ios::end);
	l_fsize = l_file.tellg();
	l_file.seekg (0, std::ios::beg);
	// Reservamos la memoria necesaria
	l_buff = new char[l_fsize + 1];
	if(!l_buff){
		l_file.close();
		return;
	}
	// Cargamos el contenido completo del fichero en memoria
	l_file.read (l_buff, l_fsize);
	l_readed = l_file.gcount();
	l_file.close();
	// Marcamos el final del texto
	l_buff[l_readed] = '\0';
	// Establecemos el texto
	m_textbuf_changelog->set_text(l_buff);
	// Liberamos la memoria
	delete[] l_buff;
}
