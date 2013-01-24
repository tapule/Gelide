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

#include <algorithm>
#include <sstream>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>

#include <map>
#include <gtkmm/liststore.h>
#include "model_columns.hpp"
#include "dialog_gamelist_generator.hpp"
#include "../core/dat_reader_factory.hpp"

CDialogGamelistGenerator::CDialogGamelistGenerator(void):
	Gtk::Dialog(),
	m_button_close(Gtk::Stock::CLOSE)
{
	// Obtenemos las instancias
	m_config = CConfig::getInstance();
	m_smanager = CSystemManager::getInstance();

	m_system = 0;

	// Configuración visual del dialogo
	this->set_title( _("Generating games list") );
	this->set_resizable(false);
	this->set_size_request(400, -1);
	this->set_modal(true);
	this->set_border_width(5);

	// Configuración de las etiquetas
	m_label_description.set_markup((Glib::ustring) "<b><big>" + _("Generating games list") + "</big></b>");
	m_label_description.set_alignment(0,0.5);
	m_label_description.set_line_wrap(true);
	//"Espere un momento mientras se analiza el fichero dat y el directorio de roms del sistema..."
	m_label_info.set_label(_("Please wait while system's dat file and roms directory are processed..."));
	m_label_info.set_alignment(0,0.5);
	m_label_info.set_line_wrap(true);
	m_label_status.set_label(_("Initializing."));
	m_label_status.set_alignment(0,0.5);
	m_label_status.set_line_wrap(false);
	m_label_status.set_ellipsize(Pango::ELLIPSIZE_END);
	m_label_status.set_padding(0, 5);

	// Configuración de la barra de progreso
	m_progressbar.set_pulse_step(0.005);

	// Configuración del cuerpo principal
	m_vbox_body.pack_start(m_label_description, true, true);
	m_vbox_body.pack_start(m_label_info, true, true);
	m_vbox_body.pack_start(m_progressbar, true, true);
	m_vbox_body.pack_start(m_label_status, true, true);
	m_vbox_body.set_spacing(5);

	// Configuración del boton close
	m_button_close.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogGamelistGenerator::onCloseClicked) );

	// Añadimos el notebook y los botones al dialogo
	this->get_vbox()->set_spacing(5);
	this->get_vbox()->pack_start(m_vbox_body, false, false);
	this->get_action_area()->pack_start(m_button_close);
	this->get_action_area()->set_layout(Gtk::BUTTONBOX_END);

	// Establecemos el diálogo por defecto en el centro del padre
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	// Cargamos la configuración
	loadConfig();

	// Mostramos todos los widgets
	this->show_all_children();
}

CDialogGamelistGenerator::~CDialogGamelistGenerator(){
	saveConfig();
}

void CDialogGamelistGenerator::setSystem(CSystem* p_system){
	m_system = p_system;
}

int CDialogGamelistGenerator::run(void){
	Gtk::MessageDialog l_dialog(*this, _("Are you sure you want to create a new list?"), false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL);

	assert(m_system);

	l_dialog.set_secondary_text( _("If you continue, a new games list will be created for the current system."));
	if(l_dialog.run() == Gtk::RESPONSE_CANCEL){
		return Gtk::RESPONSE_CANCEL;
	}
	l_dialog.hide();
	m_button_close.set_sensitive(false);
	this->show();
	// Esperamos a que se oculte el dialog
	while (Gtk::Main::events_pending())
		Gtk::Main::iteration();
	generateGameList();
	m_button_close.set_sensitive(true);
	return Gtk::Dialog::run();
}

int CDialogGamelistGenerator::run(CSystem* p_system){
	setSystem(p_system);
	return this->run();
}

void CDialogGamelistGenerator::loadConfig(void){
	int l_x, l_y;

	// Obtenemos la posición almacenada
	//m_config->getGameListGeneratorDialogPos(l_x, l_y);
	m_config->getKey(GELIDE_CFG_DIALOGS, "gamelistgenerator_dialog_x", l_x);
	m_config->getKey(GELIDE_CFG_DIALOGS, "gamelistgenerator_dialog_y", l_y);
	if((l_x!= -1) && (l_y != -1))
		this->move(l_x, l_y);
}

void CDialogGamelistGenerator::saveConfig(void){
	int l_x, l_y;

	// Guardamos la posición del dialogo
	this->get_position(l_x, l_y);
	//m_config->setGameListGeneratorDialogPos(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "gamelistgenerator_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "gamelistgenerator_dialog_y", l_y);

}

bool CDialogGamelistGenerator::generateGameList(void){
	// Juego temporal para usos múltiples
	CGame* l_game;
	bool l_preserve, l_add_unknown;
	// Datfile y un iterador sobre la lista de juegos del dat
	CDatReader* l_dat_reader;

	// Listado de juegos final y un iterador sobre ella
	std::map<Glib::ustring, CGame*> l_user_games; 	// Lista de juegos del usuario indexados por nombre
	std::map<Glib::ustring, CGame*> l_games; 		// Listado final de juegos
	std::map<Glib::ustring, CGame*>::iterator l_games_iter;
	Gtk::TreeModel::iterator l_user_games_iter;
	CGameModelColumns l_game_columns;

	// Iterador de directorio para acceder a los ficheros
	Glib::Dir::iterator l_dir_iter;
	// Path completo del fichero y nombre del juego
	Glib::ustring l_file;
	Glib::ustring l_name;

	std::stringstream l_str;

	GELIDE_DEBUG("Generating list for system " << m_system->getName().data() << "...");

	// Obtenemos la configuración del generador
	m_config->getKey(GELIDE_CFG_PREFERENCES, "preserving_games_data", l_preserve);
	m_config->getKey(GELIDE_CFG_PREFERENCES, "add_unknown_games", l_add_unknown);

	// Cargamos el dat
	m_progressbar.set_fraction(0.05);
	m_label_status.set_label(_("Loading dat file..."));
	while (Gtk::Main::events_pending())
		Gtk::Main::iteration();

	GELIDE_DEBUG("Dat file: " << m_system->getDatFile());

	l_dat_reader = CDatReaderFactory::getDatReader(m_system->getDatFile());
	if(l_dat_reader){
		GELIDE_DEBUG("Dat reader: " << l_dat_reader->getType());
		GELIDE_DEBUG("Dat info:");
		GELIDE_DEBUG("\tName: " << l_dat_reader->getName());
		GELIDE_DEBUG("\tDescription: " << l_dat_reader->getDescription());
		GELIDE_DEBUG("\tCategory: " << l_dat_reader->getCategory());
		GELIDE_DEBUG("\tVersion: " << l_dat_reader->getVersion());
		GELIDE_DEBUG("\tDate: " << l_dat_reader->getDate());
		GELIDE_DEBUG("\tAuthor: " << l_dat_reader->getAuthor());
		GELIDE_DEBUG("\tEmail: " << l_dat_reader->getEmail());
		GELIDE_DEBUG("\tHomepage: " << l_dat_reader->getHomepage());
		GELIDE_DEBUG("\tUrl: " << l_dat_reader->getUrl());
		GELIDE_DEBUG("\tComment: " << l_dat_reader->getComment());

		l_dat_reader->read(l_games);

		delete l_dat_reader;
	}
	else
		GELIDE_DEBUG("Unsupported dat file.");


	if(l_preserve){
		m_progressbar.set_fraction(0.10);
		m_label_status.set_label(_("Generating the user's game list..."));
		while (Gtk::Main::events_pending())
			Gtk::Main::iteration();
		for(l_user_games_iter = m_system->getGameList()->children().begin();
				l_user_games_iter != m_system->getGameList()->children().end(); ++l_user_games_iter){
			// Buscamos los juegos del usuario en el dat y actualizamos
			l_games_iter = l_games.find((*l_user_games_iter)[l_game_columns.m_name]);
			if(l_games_iter != l_games.end()){
				l_games_iter->second->setFavorite((*l_user_games_iter)[l_game_columns.m_favorite]);
				l_games_iter->second->setRank((*l_user_games_iter)[l_game_columns.m_rank]);
				l_games_iter->second->setFlags((*l_user_games_iter)[l_game_columns.m_flags]);
			}
			// Si no está en el dat, lo añadimos a la lista de juegos del usuario
			// que usaremos después para buscar juegos desconocidos
			else{
				l_game = new CGame();
				// No vamos a necesitar todos los campos
				l_game->setName((*l_user_games_iter)[l_game_columns.m_name]);
				l_game->setFlags((*l_user_games_iter)[l_game_columns.m_flags]);
				l_game->setRank((*l_user_games_iter)[l_game_columns.m_rank]);
				l_game->setFavorite((*l_user_games_iter)[l_game_columns.m_favorite]);
				l_user_games[l_game->getName()] = l_game;
			}
		}
	}

	// Cargamos los ficheros
	m_progressbar.set_fraction(0.20);
	m_label_status.set_label(_("Checking files..."));
	while (Gtk::Main::events_pending())
		Gtk::Main::iteration();
	try{
		Glib::Dir l_dir(m_system->getRomsDir());
		for (l_dir_iter = l_dir.begin(); l_dir_iter != l_dir.end(); l_dir_iter++){
			l_file = Glib::build_filename(m_system->getRomsDir(), (*l_dir_iter));
			if(!Glib::file_test(l_file.c_str(), Glib::FILE_TEST_IS_DIR)){
				l_name = (*l_dir_iter);
				// TODO: Añadir filtrado de extensiones conocidas para el sistema
				l_name = l_name.rfind('.') == std::string::npos ? l_name : l_name.substr(0, l_name.rfind('.'));
				m_progressbar.pulse();
				m_label_status.set_label(_("Checking: ") + l_name);
				while (Gtk::Main::events_pending())
					Gtk::Main::iteration();

				// Buscamos en la lista de juegos del dat
				l_games_iter = l_games.find(l_name);
				if(l_games_iter!= l_games.end()){
					l_game = l_games_iter->second;
					l_game->setWorking(true);
					l_game->setAvailable(true);
					l_game->setUnknown(false);
					l_game->setPath(l_file);
				}
				// Se trata de un juego desconocido, crearemos uno nuevo
				else if(l_add_unknown){
					l_game = new CGame();
					l_game->setName(l_name);
					l_game->setDescription(l_name);
					l_game->setPath(l_file);
					l_game->setWorking(true);
					l_game->setAvailable(true);
					l_game->setUnknown(true);
					if(l_preserve){
						// Buscamos el juego en la lista de juegos del usuario y actualizamos
						l_games_iter = l_user_games.find(l_game->getName());
						if(l_games_iter != l_user_games.end()){
							l_game->setFavorite(l_games_iter->second->getFavorite());
							l_game->setRank(l_games_iter->second->getRank());
							l_game->setPlayed(l_games_iter->second->getPlayed());
							l_game->setWorking(l_games_iter->second->getWorking());
						}
					}
					// Añadimos a la lista de juegos final
					l_games[l_game->getName()] = l_game;
				}
			}
		}
	}
	catch (Glib::Error& l_exception)
	{
		GELIDE_WARNING("Reading directory (" << l_exception.what().c_str() << "...");
	}

	// Guardamos el listado y liberamos la memoria
	m_progressbar.set_fraction(0.80);
	m_label_status.set_label(_("Generating list of games..."));
	while (Gtk::Main::events_pending())
		Gtk::Main::iteration();

	// Limpiamos la lista de juegos
	GELIDE_DEBUG("System " << m_system->getName().data() << ": Loading games...");
	// Vaciamos la lista de juegos
	m_system->getGameList()->clear();
	// Añadimos los juegos
	for (l_games_iter = l_games.begin(); l_games_iter != l_games.end(); ++l_games_iter){
		m_system->addGame(*l_games_iter->second);
		// Eliminamos el juego
		delete l_games_iter->second;
	}
	l_games.clear();

	// Eliminamos el listado de juegos del usuario
	for (l_games_iter = l_user_games.begin(); l_games_iter != l_user_games.end(); ++l_games_iter){
		// Eliminamos el juego
		delete l_games_iter->second;
	}
	l_user_games.clear();

	m_progressbar.set_fraction(1);

	l_str << Glib::ustring::compose(_("End. %1 added games."), m_system->getGames());
	m_label_status.set_label(l_str.str());
	while (Gtk::Main::events_pending())
		Gtk::Main::iteration();
	return true;
}

void CDialogGamelistGenerator::onCloseClicked(void){
	this->response(Gtk::RESPONSE_CLOSE);
}

bool CDialogGamelistGenerator::on_delete_event(GdkEventAny* p_event){
	return ! m_button_close.is_sensitive();
}


