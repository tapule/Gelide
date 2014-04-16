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

#include "dialog_emulator_launcher.hpp"
#include <glibmm.h>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include "../core/system.hpp"
#include "../core/game.hpp"
#include "../core/emulator.hpp"
#include "../utils/process.hpp"


CDialogEmulatorLauncher::CDialogEmulatorLauncher(void):
	Gtk::Dialog(),
	m_button_close(Gtk::Stock::CLOSE)
{
	// Obtenemos las instancias
	m_config = CConfig::getInstance();
	m_smanager = CSystemManager::getInstance();

	// Configuración visual del dialogo
	this->set_title( _("Games launcher") );
	this->set_resizable(false);
	this->set_size_request(525, -1);
	this->set_modal(true);
	this->set_border_width(5);

	// Configuración del titulo
	m_label_title.set_markup((Glib::ustring) "<b><big>" + _("Starting emulation...") + "</big></b>");
	m_label_title.set_alignment(0,0.5);
	m_image_title.set(Gtk::Stock::EXECUTE, Gtk::ICON_SIZE_DIALOG);
	m_image_title.set_padding(15,15);
	m_hbox_title.pack_start(m_image_title, false, false);
	m_hbox_title.pack_start(m_label_title, false, false);
	m_hbox_title.set_spacing(3);

	// Configuracion del textview para mensajes
	m_text_progress.set_size_request(-1, 200);
	m_text_progress.set_editable(false);
	//m_text_progress.set_wrap_mode(Gtk::WRAP_WORD);
	m_textbuf_progress = Gtk::TextBuffer::create();
	m_text_progress.set_buffer(m_textbuf_progress);
	// Configuración del scroll que contiene la descripcion
	m_scroll_progress.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	m_scroll_progress.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_progress.add(m_text_progress);

	// Añadimos el notebook al dialogo
	this->get_vbox()->set_spacing(5);
	this->get_vbox()->pack_start(m_hbox_title, false, false);
	this->get_vbox()->pack_start(m_scroll_progress, false, false);

	// Configuración del boton close
	m_button_close.signal_clicked().connect( sigc::mem_fun(*this,
			&CDialogEmulatorLauncher::onCloseClicked) );
	this->get_action_area()->pack_start(m_button_close);
	this->get_action_area()->set_layout(Gtk::BUTTONBOX_END);

	// Establecemos el diálogo por defecto en el centro del padre
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	// Cargamos la configuración
	loadConfig();

	// Mostramos todos los widgets
	this->show_all_children();
	// Obtenemos la clave que indica si el diálogo debe cerrarse automáticamente
	m_config->getKey(GELIDE_CFG_PREFERENCES, "show_emulator_output", m_show_output);
	if(!m_show_output)
		this->get_action_area()->hide();
}

CDialogEmulatorLauncher::~CDialogEmulatorLauncher(){
	saveConfig();
}


int CDialogEmulatorLauncher::run(CSystem* p_system, CGame* p_game){
	Gtk::TreeModel::iterator l_iter;
	CEmulator* l_emulator;
	CProcess l_process;
	Glib::ustring l_params, l_path, l_output;
	int l_ind;
	bool l_val;

	assert(p_system);
	assert(p_game);

	m_button_close.set_sensitive(false);
	this->show();
	m_textbuf_progress->insert_at_cursor(_("* Checking emulator:\n"));
	while (Gtk::Main::events_pending())
		Gtk::Main::iteration();

	l_iter = p_system->getEmulator(p_system->getEmulatorId());
	if(l_iter){
		l_emulator = p_system->getEmulator(l_iter);

		if(Glib::file_test(l_emulator->getPath(), Glib::FILE_TEST_EXISTS)){
			m_textbuf_progress->insert_at_cursor("\t" + l_emulator->getPath() +"\n");
			m_textbuf_progress->insert_at_cursor(_("* Checking game:\n"));
			while (Gtk::Main::events_pending())
				Gtk::Main::iteration();
			if(Glib::file_test(p_game->getPath(), Glib::FILE_TEST_EXISTS)){
				m_textbuf_progress->insert_at_cursor("\t" + p_game->getPath() +"\n");
				m_textbuf_progress->insert_at_cursor(_("* Processing parameters:\n"));
				while (Gtk::Main::events_pending())
					Gtk::Main::iteration();

				l_params = l_emulator->getParams();

				// Reemplazamos la etiqueta de juego PARAMS_TAG_GAME_NAME
				l_ind = l_params.find(PARAMS_TAG_GAME_NAME);
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, PARAMS_TAG_SIZE, p_game->getName());
					l_ind = l_params.find(PARAMS_TAG_GAME_NAME, l_ind + 1);
				}

				// Reemplazamos la etiqueta de juego PARAMS_TAG_GAME_FILE
				l_ind = l_params.find(PARAMS_TAG_GAME_FILE);
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, PARAMS_TAG_SIZE, p_game->getPath());
					l_ind = l_params.find(PARAMS_TAG_GAME_FILE, l_ind + 1);
				}

				// Reemplazamos la etiqueta de path a las roms PARAMS_TAG_GAME_PATH
				l_ind = l_params.find(PARAMS_TAG_GAME_PATH);
				l_path = p_system->getRomsDir();
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, PARAMS_TAG_SIZE, l_path);
					l_ind = l_params.find(PARAMS_TAG_GAME_PATH, l_ind + 1);
				}

				// Reemplazamos la etiqueta de bios PARAMS_TAG_BIOS_FILE
				l_ind = l_params.find(PARAMS_TAG_BIOS_FILE);
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, PARAMS_TAG_SIZE, p_system->getBiosFile());
					l_ind = l_params.find(PARAMS_TAG_BIOS_FILE, l_ind + 1);
				}

				// Reemplazamos la etiqueta de path al bios PARAMS_TAG_BIOS_PATH
				l_ind = l_params.find(PARAMS_TAG_BIOS_PATH);
				l_path = Glib::path_get_dirname(p_system->getBiosFile());
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, PARAMS_TAG_SIZE, l_path);
					l_ind = l_params.find(PARAMS_TAG_BIOS_PATH, l_ind + 1);
				}

				// Reemplazamos la etiqueta de bios PARAMS_TAG_EMULATOR_FILE
				l_ind = l_params.find(PARAMS_TAG_EMULATOR_FILE);
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, PARAMS_TAG_SIZE, l_emulator->getPath());
					l_ind = l_params.find(PARAMS_TAG_EMULATOR_FILE, l_ind + 1);
				}

				// Reemplazamos la etiqueta de path al bios PARAMS_TAG_EMULATOR_PATH
				l_ind = l_params.find(PARAMS_TAG_EMULATOR_PATH);
				l_path = Glib::path_get_dirname(l_emulator->getPath());
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, PARAMS_TAG_SIZE, l_path);
					l_ind = l_params.find(PARAMS_TAG_EMULATOR_PATH, l_ind + 1);
				}

				// Si no hay parámetros, añadimos el juego directamente
				if(l_params == "")
					l_params = (Glib::ustring) "\"" + p_game->getPath() + "\"";

				// Añadimos el emulador
				l_params = (Glib::ustring) "\"" + l_emulator->getPath() + "\" " + l_params;

				// Parseamos las " internas al comando
				l_ind = l_params.find("\"");
				while(l_ind > -1){
					l_params = l_params.replace(l_ind, 1, "\\\"");
					l_ind = l_params.find("\"", l_ind + 2);
				}
				// Añadimos la llamada al shell, de esta forma hacemos más rica
				// la cadena de parámetros
				l_params = "/bin/sh -c \"" + l_params + "\"";

				GELIDE_DEBUG(_("Command: ") << l_params);
				m_textbuf_progress->insert_at_cursor("\t" + l_params  + "\n");
				m_textbuf_progress->insert_at_cursor(_("* Launching emulation:\n"));
				while (Gtk::Main::events_pending())
					Gtk::Main::iteration();

				l_process.run(l_params, p_system->getHomeDir());

				// Esperamos la terminación del proceso
				while(l_process.isRunning()){
					while (Gtk::Main::events_pending())
						Gtk::Main::iteration();
				}
				m_textbuf_progress->insert_at_cursor(_("\t Finished.\n"));

				// Comprobamos si hay que mostrar la salida del emulador
				//m_config->getPrefsShowEmulatorOutput(l_val);
				m_config->getKey(GELIDE_CFG_PREFERENCES, "show_emulator_output", l_val);
				if(l_val){
					m_textbuf_progress->insert_at_cursor(_("* Process output:\n"));
					while(l_process.outReadLine(l_output) > -1){
							if(l_output!= "")
									m_textbuf_progress->insert_at_cursor("\t" + l_output);
							while (Gtk::Main::events_pending())
									Gtk::Main::iteration();
					}
					m_textbuf_progress->insert_at_cursor(_("\n* Process error output:\n"));
					while(l_process.errReadLine(l_output) > -1){
							if(l_output!= "")
									m_textbuf_progress->insert_at_cursor("\t" + l_output);
							while (Gtk::Main::events_pending())
									Gtk::Main::iteration();
					}
				}
			}
			else
				m_textbuf_progress->insert_at_cursor((Glib::ustring) _("\tCan not be located") + p_game->getPath() + ".\n");
		}
		else
			m_textbuf_progress->insert_at_cursor((Glib::ustring) _("\tCan not be located") + l_emulator->getPath() + ".\n");
	}
	else
		m_textbuf_progress->insert_at_cursor(_("The system has no configured emulator.\n"));

	if(m_show_output){
		m_button_close.set_sensitive(true);
		return Gtk::Dialog::run();
	}
	else
	{
		this->response(Gtk::RESPONSE_CLOSE);
		return Gtk::RESPONSE_CLOSE;
	}
}

void CDialogEmulatorLauncher::loadConfig(void){
	int l_x, l_y;

	// Obtenemos la posición almacenada
	//m_config->getEmulatorLauncherDialogPos(l_x, l_y);
	m_config->getKey(GELIDE_CFG_DIALOGS, "emulatorlauncher_dialog_x", l_x);
	m_config->getKey(GELIDE_CFG_DIALOGS, "emulatorlauncher_dialog_y", l_y);
	if((l_x!= -1) && (l_y != -1))
		this->move(l_x, l_y);
}

void CDialogEmulatorLauncher::saveConfig(void){
	int l_x, l_y;

	// Guardamos la posición del dialogo
	this->get_position(l_x, l_y);
	//m_config->setEmulatorLauncherDialogPos(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "emulatorlauncher_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "emulatorlauncher_dialog_y", l_y);
}

void CDialogEmulatorLauncher::onCloseClicked(void){
	this->response(Gtk::RESPONSE_CLOSE);
}

bool CDialogEmulatorLauncher::on_delete_event(GdkEventAny* p_event){
	if(m_show_output)
		return ! m_button_close.is_sensitive();
	else
		return true;
}


