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

#include <fstream>
#include <algorithm>
#include <glib/gstdio.h>
#include <sstream>
#include "system.hpp"
#include "../utils/tokenizer.hpp"



CSystem::CSystem(void){
	// Inicialización de los campos
	m_id = 0;
	m_name = "";
	m_description = "";
	m_icon_file = "";
	m_dat_file = "";
	m_bios_file = "";
	m_home_dir = "";
	m_roms_dir = "";
	m_snap_dir = "";
	m_tittle_dir = "";
	m_front_dir = "";
	m_back_dir = "";
	m_media_dir = "";
	m_emulator_id = 0;

	m_dat_name = "";
	m_dat_description = "";
	m_dat_category = "";
	m_dat_version = "";
	m_dat_author = "";
	m_dat_games = 0;

	// Inicialización de los listados
	m_game_list = Gtk::ListStore::create(m_game_columns);
	m_emulator_list = Gtk::ListStore::create(m_emulator_columns);

	// Instancia del manager
	m_manager = CSystemManager::getInstance();
}

CSystem::~CSystem(void){
	m_game_list->clear();
	m_emulator_list->clear();
}

CSystem& CSystem::operator=(CSystem& p_system){
	if(this != &p_system){
		// Se copian todos los atributos excepto el id, que debe ser único
		this->setName(p_system.getName());
		this->setDescription(p_system.getDescription());
		this->setIcon(p_system.getIcon());
		this->setIconFile(p_system.getIconFile());
		this->setDatFile(p_system.getDatFile());
		this->setBiosFile(p_system.getBiosFile());
		this->setHomeDir(p_system.getHomeDir());
		this->setRomsDir(p_system.getRomsDir());
		this->setSnapDir(p_system.getSnapDir());
		this->setTitleDir(p_system.getTitleDir());
		this->setFrontDir(p_system.getFrontDir());
		this->setBackDir(p_system.getBackDir());
		this->setMediaDir(p_system.getMediaDir());
		this->setEmulatorId(p_system.getEmulatorId());
		this->setDatName(p_system.getDatName());
		this->setDatDescription(p_system.getDatDescription());
		this->setDatCategory(p_system.getDatCategory());
		this->setDatVersion(p_system.getDatVersion());
		this->setDatAuthor(p_system.getDatAuthor());
		this->setDatGames(p_system.getDatGames());
	}
	return *this;
}

bool CSystem::load(void){
	CTokenizer l_tokenizer;
	CTokenizer::CToken l_token;
	Gtk::TreeModel::iterator l_iter;
	CEmulator l_emulator;
	int l_ind;
	Glib::ustring l_tmp_str;

	// Comprobamos si tiene asignado un home
	if(m_home_dir == "")
		return false;

	// Carga de la lista de juegos
	GELIDE_DEBUG("System " << getName().data() << ": Loading games...");
	// Iniciamos la carga
	if(l_tokenizer.initFromFile(m_home_dir + "/" + SYSTEMS_GAMES_LIST)){
		//l_tokenizer.setDelimiters((char *)CSV_SEPARATOR, 1);
		l_tokenizer.setDelimiters("\n\x3B", 2);
		while(l_tokenizer.hasMoreTokens()){
			l_iter = m_game_list->append();

			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_favorite] = l_token.getValue();
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_name] = Glib::strcompress(l_token.getString());
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_flags] = l_token.getValue();
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_rank] = l_token.getValue();
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_year] = l_token.getValue();
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_description] = Glib::strcompress(l_token.getString());
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_manufacturer] = Glib::strcompress(l_token.getString());
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_path] = Glib::strcompress(l_token.getString());
			l_tokenizer.nextToken(l_token);
			(*l_iter)[m_game_columns.m_rom_crc] = Glib::strcompress(l_token.getString());
		}
	}
	GELIDE_DEBUG("System " << getName().data() << ": " << m_game_list->children().size() << " games loaded.");

	// Carga de la lista de emuladores
	GELIDE_DEBUG("System " << getName().data() << ": Loading emulators...");
	// Iniciamos la carga
	if(l_tokenizer.initFromFile(m_home_dir + "/" + SYSTEMS_EMULATORS_LIST)){
		//l_tokenizer.setDelimiters((char *)CSV_SEPARATOR, 1);
		l_tokenizer.setDelimiters("\n\x3B", 2);
		while(l_tokenizer.hasMoreTokens()){
			l_tokenizer.nextToken(l_token);
			l_emulator.setId(l_token.getValue());
			l_tokenizer.nextToken(l_token);
			l_emulator.setIconFile(l_token.getString());
			l_tokenizer.nextToken(l_token);
			l_emulator.setName(Glib::strcompress(l_token.getString()));
			l_tokenizer.nextToken(l_token);
			l_emulator.setDescription(Glib::strcompress(l_token.getString()));
			l_tokenizer.nextToken(l_token);
			l_emulator.setVersion(Glib::strcompress(l_token.getString()));
			l_tokenizer.nextToken(l_token);
			l_emulator.setAuthor(Glib::strcompress(l_token.getString()));
			l_tokenizer.nextToken(l_token);
			l_emulator.setHomepage(Glib::strcompress(l_token.getString()));
			l_tokenizer.nextToken(l_token);
			l_emulator.setPath(Glib::strcompress(l_token.getString()));
			l_tokenizer.nextToken(l_token);
			/*
			 * HACK: En la versión 0.1.2 se realizó el cambio de etiqueta
			 * $rp por $gp, está porción de código realiza el cambio automático
			 */
			l_tmp_str = Glib::strcompress(l_token.getString());
			l_ind = l_tmp_str.find(PARAMS_TAG_ROMS_PATH);
			while(l_ind > -1){
				l_tmp_str = l_tmp_str.replace(l_ind, PARAMS_TAG_SIZE, PARAMS_TAG_GAME_PATH);
				l_ind = l_tmp_str.find(PARAMS_TAG_ROMS_PATH, l_ind + 1);
			}
			l_emulator.setParams(l_tmp_str);

			addEmulator(l_emulator);
		}
	}
	GELIDE_DEBUG("System " << getName().data() << ": " << m_emulator_list->children().size() << " emulators loaded.");
	return true;
}

bool CSystem::save(void){
	Gtk::TreeModel::iterator l_iter;
	// Fichero de salida para las listados
	std::ofstream l_file;
	Glib::ustring l_path;

	// Comprobamos si tiene asignado un home
	if(m_home_dir == "")
		return false;

	// Guardar lista de juegos
	GELIDE_DEBUG("System " << getName().data() << ": Saving games...");
	l_path = m_home_dir + "/" + SYSTEMS_GAMES_LIST;
	l_file.open(l_path.data());
	if(l_file.good()){
		for(l_iter = m_game_list->children().begin();l_iter != m_game_list->children().end(); l_iter++){
			l_file << (*l_iter)[m_game_columns.m_favorite] << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_game_columns.m_name]) << "\"" << CSV_SEPARATOR
			<< (*l_iter)[m_game_columns.m_flags] << CSV_SEPARATOR
			<< (*l_iter)[m_game_columns.m_rank] << CSV_SEPARATOR
			<< (*l_iter)[m_game_columns.m_year] << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_game_columns.m_description]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_game_columns.m_manufacturer]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_game_columns.m_path]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_game_columns.m_rom_crc]) << "\"" << CSV_SEPARATOR
			<< std::endl;
		}
		l_file.close();
	}
	else
		return false;

	// Guardar lista de emuladores
	GELIDE_DEBUG("System " << getName().data() << ": Saving emulators...");
	l_path = m_home_dir + "/" + SYSTEMS_EMULATORS_LIST;
	l_file.open(l_path.data());
	if(l_file.good()){
		for(l_iter = m_emulator_list->children().begin();l_iter != m_emulator_list->children().end(); l_iter++){
			l_file << (*l_iter)[m_emulator_columns.m_id] << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_icon_file]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_name]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_description]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_version]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_author]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_homepage]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_path]) << "\"" << CSV_SEPARATOR
			<< "\"" << Glib::strescape((Glib::ustring)(*l_iter)[m_emulator_columns.m_params]) << "\"" << CSV_SEPARATOR
			<< std::endl;
		}
		l_file.close();
	}
	else
		return false;

	return true;
}

// Actualizar el sistema (emuladores)
void CSystem::update(CSystem& p_system){
	Gtk::TreeModel::iterator l_iter;
	CEmulator l_emulator;

	GELIDE_DEBUG("System " << getName().data() << ": Updating...");
	// Intentamos añadir cada uno de los emuladores del sistema
	for(l_iter = p_system.getEmulatorList()->children().begin();
		l_iter != p_system.getEmulatorList()->children().end(); l_iter++){
		l_emulator.setId(0);
		l_emulator.setIconFile((*l_iter)[m_emulator_columns.m_icon_file]);
		l_emulator.setName((*l_iter)[m_emulator_columns.m_name]);
		l_emulator.setDescription((*l_iter)[m_emulator_columns.m_description]);
		l_emulator.setVersion((*l_iter)[m_emulator_columns.m_version]);
		l_emulator.setAuthor((*l_iter)[m_emulator_columns.m_author]);
		l_emulator.setHomepage((*l_iter)[m_emulator_columns.m_homepage]);
		l_emulator.setPath((*l_iter)[m_emulator_columns.m_path]);
		l_emulator.setParams((*l_iter)[m_emulator_columns.m_params]);
		addEmulator(l_emulator);
	}
}

Glib::ustring CSystem::getAsString(void){
	std::stringstream l_str;
	/*
	 id;"name";"description";"icon_file";"dat_file";"bios_file";"home_dir";""rom_dir";
	"snap_dir";"tittle_dir";"front_dir";"back_dir";"media_dir";emulator_id;
	"datname";"datDescription";"datcategory";datversion";"datauthor";datgames;
	*/

	l_str << getId() << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getName()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getDescription()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getIconFile()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getDatFile()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getBiosFile()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getHomeDir()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getRomsDir()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getSnapDir()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getTitleDir()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getFrontDir()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getBackDir()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getMediaDir()) << "\"" << CSV_SEPARATOR
		<< getEmulatorId() << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getDatName()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getDatDescription()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getDatCategory()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getDatVersion()) << "\"" << CSV_SEPARATOR
		<< "\"" << Glib::strescape(getDatAuthor()) << "\"" << CSV_SEPARATOR
		<< getDatGames() << CSV_SEPARATOR
		<< std::endl;
	return l_str.str();
}

Glib::RefPtr<Gtk::ListStore>& CSystem::getGameList(void){
	return m_game_list;
}

Glib::RefPtr<Gtk::ListStore>& CSystem::getEmulatorList(void){
	return m_emulator_list;
}

// Gestión de juegos
// El llamador debe encargarse de eliminar el CGame
CGame* CSystem::getGame(Gtk::TreeModel::iterator& p_iter){
	CGame* l_game;

	assert(p_iter);

	l_game = new CGame();

	l_game->setSystemId(m_id);
	l_game->setFavorite((*p_iter)[m_game_columns.m_favorite]);
	l_game->setName((*p_iter)[m_game_columns.m_name]);
	l_game->setFlags((*p_iter)[m_game_columns.m_flags]);
	l_game->setRank((*p_iter)[m_game_columns.m_rank]);
	l_game->setYear((*p_iter)[m_game_columns.m_year]);
	l_game->setDescription((*p_iter)[m_game_columns.m_description]);
	l_game->setManufacturer((*p_iter)[m_game_columns.m_manufacturer]);
	l_game->setPath((*p_iter)[m_game_columns.m_path]);
	l_game->setRomCrc((*p_iter)[m_game_columns.m_rom_crc]);

	return l_game;
}

void CSystem::addGame(CGame& p_game){
	Gtk::TreeModel::iterator l_iter;

	l_iter = m_game_list->append();
	(*l_iter)[m_game_columns.m_favorite] = p_game.getFavorite();
	(*l_iter)[m_game_columns.m_name] = p_game.getName();
	(*l_iter)[m_game_columns.m_flags] = p_game.getFlags();
	(*l_iter)[m_game_columns.m_rank] = p_game.getRank();
	(*l_iter)[m_game_columns.m_year] = p_game.getYear();
	(*l_iter)[m_game_columns.m_description] = p_game.getDescription();
	(*l_iter)[m_game_columns.m_manufacturer] = p_game.getManufacturer();
	(*l_iter)[m_game_columns.m_path] = p_game.getPath();
	(*l_iter)[m_game_columns.m_rom_crc] = p_game.getRomCrc();
}

void CSystem::removeGame(Gtk::TreeModel::iterator& p_iter){
	assert(p_iter);
	m_game_list->erase(p_iter);
}

void CSystem::updateGame(Gtk::TreeModel::iterator& p_iter, CGame& p_game){

	assert(p_iter);

	(*p_iter)[m_game_columns.m_favorite] = p_game.getFavorite();
	(*p_iter)[m_game_columns.m_name] = p_game.getName();
	(*p_iter)[m_game_columns.m_flags] = p_game.getFlags();
	(*p_iter)[m_game_columns.m_rank] = p_game.getRank();
	(*p_iter)[m_game_columns.m_year] = p_game.getYear();
	(*p_iter)[m_game_columns.m_description] = p_game.getDescription();
	(*p_iter)[m_game_columns.m_manufacturer] = p_game.getManufacturer();
	(*p_iter)[m_game_columns.m_path] = p_game.getPath();
	(*p_iter)[m_game_columns.m_rom_crc] = p_game.getRomCrc();
}

Gtk::TreeModel::iterator CSystem::getEmulator(const unsigned int p_id){
	Gtk::TreeModel::iterator l_iter;
	Gtk::TreeModel::Row l_row;

	// Obtenemos el iterador
	for(l_iter = m_emulator_list->children().begin();
		l_iter != m_emulator_list->children().end(); l_iter++){
		if((*l_iter)[m_emulator_columns.m_id] == p_id)
			break;
	}
	return l_iter;
}

// Gestión de emuladores
CEmulator* CSystem::getEmulator(Gtk::TreeModel::iterator& p_iter){
	CEmulator* l_emulator;
	/* CHECKME: Se puede evitar la perdida de memoria del puntero haciendo un static CEmulator
	 * y utilizando referencias en vez de puntero */
	assert(p_iter);
	l_emulator = new CEmulator();

	l_emulator->setSystemId(m_id);
	l_emulator->setId((*p_iter)[m_emulator_columns.m_id]);
	l_emulator->setIconFile((*p_iter)[m_emulator_columns.m_icon_file]);
	l_emulator->setIcon((*p_iter)[m_emulator_columns.m_icon]);
	l_emulator->setName((*p_iter)[m_emulator_columns.m_name]);
	l_emulator->setDescription((*p_iter)[m_emulator_columns.m_description]);
	l_emulator->setVersion((*p_iter)[m_emulator_columns.m_version]);
	l_emulator->setAuthor((*p_iter)[m_emulator_columns.m_author]);
	l_emulator->setHomepage((*p_iter)[m_emulator_columns.m_homepage]);
	l_emulator->setPath((*p_iter)[m_emulator_columns.m_path]);
	l_emulator->setParams((*p_iter)[m_emulator_columns.m_params]);
	return l_emulator;
}

Gtk::TreeModel::iterator CSystem::addEmulator(CEmulator& p_emulator){
	Gtk::TreeNodeChildren::reverse_iterator l_riter;
	Gtk::TreeModel::iterator l_iter;
	int l_id;

	l_id = 1;
	// Comprobamos si existe un emulador con el mismo identificador o nombre
	for(l_iter = m_emulator_list->children().begin(); l_iter != m_emulator_list->children().end(); l_iter++){
		if(((*l_iter)[m_emulator_columns.m_id] == p_emulator.getId()) ||
			((*l_iter)[m_emulator_columns.m_name] == p_emulator.getName()))
			return l_iter;
		// Obtenemos el mayor índice de los emuladores
		l_id = l_id >= (*l_iter)[m_emulator_columns.m_id] ? l_id : (*l_iter)[m_emulator_columns.m_id];
	}
	// Si es un emulador nuevo (Id = 0), establecemos el id adecuado
	if(p_emulator.getId() == 0)
		p_emulator.setId(l_id + 1);

	// Añadimos el emulador
	GELIDE_DEBUG("System " << getName().data() << ": Adding emulator " << p_emulator.getName() << "...");
	l_iter = m_emulator_list->append();
	if(l_iter){
		(*l_iter)[m_emulator_columns.m_id] = p_emulator.getId();
		(*l_iter)[m_emulator_columns.m_icon_file] = p_emulator.getIconFile();
		//if(p_emulator.getIcon())
		//	(*l_iter)[m_emulator_columns.m_icon] = p_emulator.getIcon();
		//else{
			try{
				(*l_iter)[m_emulator_columns.m_icon] = Gdk::Pixbuf::create_from_file(p_emulator.getIconFile())->scale_simple(32, 32, Gdk::INTERP_BILINEAR);
			}
			catch(const Glib::Exception& l_exception){
				GELIDE_WARNING("Loading emulator icon (" << l_exception.what().c_str() << ")");
				(*l_iter)[m_emulator_columns.m_icon] = m_manager->getEmulatorDefaultIcon();
			}
		//}
		(*l_iter)[m_emulator_columns.m_name] = p_emulator.getName();
		(*l_iter)[m_emulator_columns.m_description] = p_emulator.getDescription();
		(*l_iter)[m_emulator_columns.m_version] = p_emulator.getVersion();
		(*l_iter)[m_emulator_columns.m_author] = p_emulator.getAuthor();
		(*l_iter)[m_emulator_columns.m_homepage] = p_emulator.getHomepage();
		(*l_iter)[m_emulator_columns.m_path] = p_emulator.getPath();
		(*l_iter)[m_emulator_columns.m_params] = p_emulator.getParams();
	}
	return l_iter;
}

void CSystem::removeEmulator(Gtk::TreeModel::iterator& p_iter){
	Gtk::TreeModel::iterator l_iter;
	unsigned int l_id;

	assert(p_iter);

	l_id = (*p_iter)[m_emulator_columns.m_id];
	GELIDE_DEBUG("System " << getName().data() << ": Deleting emulator " << (*p_iter)[m_emulator_columns.m_name] << "...");
	m_emulator_list->erase(p_iter);
	// Comprobamos si se trataba del emulador seleccionado
	if(m_emulator_id == l_id){
		l_iter = m_emulator_list->children().begin();
		if(l_iter)
			m_emulator_id = (*l_iter)[m_emulator_columns.m_id];
		else
			m_emulator_id = 0;
	}
}

void CSystem::updateEmulator(Gtk::TreeModel::iterator& p_iter, CEmulator& p_emulator){
	assert(p_iter);

	(*p_iter)[m_emulator_columns.m_icon_file] = p_emulator.getIconFile();
	(*p_iter)[m_emulator_columns.m_icon] = p_emulator.getIcon();
	(*p_iter)[m_emulator_columns.m_name] = p_emulator.getName();
	(*p_iter)[m_emulator_columns.m_description] = p_emulator.getDescription();
	(*p_iter)[m_emulator_columns.m_version] = p_emulator.getVersion();
	(*p_iter)[m_emulator_columns.m_author] = p_emulator.getAuthor();
	(*p_iter)[m_emulator_columns.m_homepage] = p_emulator.getHomepage();
	(*p_iter)[m_emulator_columns.m_path] = p_emulator.getPath();
	(*p_iter)[m_emulator_columns.m_params] = p_emulator.getParams();
}

// Obtener los campos del sistema
unsigned int CSystem::getId(void){
	return m_id;
}

Glib::ustring CSystem::getName(void){
	return m_name;
}

Glib::ustring CSystem::getDescription(void){
	return m_description;
}

Glib::ustring CSystem::getIconFile(void){
	return m_icon_file;
}

Glib::RefPtr<Gdk::Pixbuf> CSystem::getIcon(void){
	return m_icon;
}

Glib::ustring CSystem::getDatFile(void){
	return m_dat_file;
}

Glib::ustring CSystem::getBiosFile(void){
	return m_bios_file;
}

Glib::ustring CSystem::getHomeDir(void){
	return m_home_dir;
}

Glib::ustring CSystem::getRomsDir(void){
	return m_roms_dir;
}

Glib::ustring CSystem::getSnapDir(void){
	return m_snap_dir;
}

Glib::ustring CSystem::getTitleDir(void){
	return m_tittle_dir;
}

Glib::ustring CSystem::getFrontDir(void){
	return m_front_dir;
}

Glib::ustring CSystem::getBackDir(void){
	return m_back_dir;
}

Glib::ustring CSystem::getMediaDir(void){
	return m_media_dir;
}

unsigned int CSystem::getGames(void){
	return m_game_list->children().size();
}

unsigned int CSystem::getEmulatorId(void){
	return m_emulator_id;
}

// Obtener informacion del dat
Glib::ustring CSystem::getDatName(void){
	return m_dat_name;
}

Glib::ustring CSystem::getDatDescription(void){
	return m_description;
}

Glib::ustring CSystem::getDatCategory(void){
	return m_dat_category;
}

Glib::ustring CSystem::getDatVersion(void){
	return m_dat_version;
}

Glib::ustring CSystem::getDatAuthor(void){
	return m_dat_author;
}

unsigned int CSystem::getDatGames(void){
	return m_dat_games;
}

// Establecer el indice del sistema
void CSystem::setId(const unsigned int p_id){
	m_id = p_id;
}

void CSystem::setName(const Glib::ustring& p_name){
	m_name = p_name;
}

void CSystem::setDescription(const Glib::ustring& p_description){
	m_description = p_description;
}

void CSystem::setIconFile(const Glib::ustring& p_icon){
	m_icon_file = p_icon;
}

void CSystem::setIcon(Glib::RefPtr<Gdk::Pixbuf> p_icon){
	m_icon = p_icon;
}

void CSystem::setDatFile(const Glib::ustring& p_dat_file){
	m_dat_file = p_dat_file;
}

void CSystem::setBiosFile(const Glib::ustring& p_bios_file){
	m_bios_file = p_bios_file;
}

void CSystem::setHomeDir(const Glib::ustring& p_home_dir){
	m_home_dir = p_home_dir;
}

void CSystem::setRomsDir(const Glib::ustring& p_roms_dir){
	m_roms_dir = p_roms_dir;
}

void CSystem::setSnapDir(const Glib::ustring& p_snap_dir){
	m_snap_dir = p_snap_dir;
}

void CSystem::setTitleDir(const Glib::ustring& p_tittle_dir){
	m_tittle_dir = p_tittle_dir;
}

void CSystem::setFrontDir(const Glib::ustring& p_front_dir){
	m_front_dir = p_front_dir;
}
void CSystem::setBackDir(const Glib::ustring& p_back_dir){
	m_back_dir = p_back_dir;
}

void CSystem::setMediaDir(const Glib::ustring& p_media_dir){
	m_media_dir = p_media_dir;
}

void CSystem::setEmulatorId(const  unsigned int p_id){
	m_emulator_id = p_id;
}


// Establecer información del dat
void CSystem::setDatName(const Glib::ustring& p_name){
	m_dat_name = p_name;
}

void CSystem::setDatDescription(const Glib::ustring& p_description){
	m_dat_description = p_description;
}

void CSystem::setDatCategory(const Glib::ustring& p_category){
	m_dat_category = p_category;
}

void CSystem::setDatVersion(const Glib::ustring& p_version){
	m_dat_version = p_version;
}

void CSystem::setDatAuthor(const Glib::ustring& p_author){
	m_dat_author = p_author;
}

void CSystem::setDatGames(unsigned int p_games){
	m_dat_games = p_games;
}
