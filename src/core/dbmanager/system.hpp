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

#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "../gelide.hpp"
#include <gtkmm/liststore.h>
#include <glibmm/ustring.h>
#include <gdkmm/pixbuf.h>
#include "game.hpp"
#include "emulator.hpp"
#include "../ui/model_columns.hpp"
#include "system_manager.hpp"

class CSystemManager;

class CSystem
{
public:
	// Constructor
	CSystem(void);
	virtual ~CSystem(void);

	CSystem& operator=(CSystem& p_system);

	// Cargar el sistema (juegos y emuladores)
	bool load(void);
	// Guardar el sistema (juegos y emuladores)
	bool save(void);
	// Actualizar el sistema (emuladores)
	void update(CSystem& p_system);

	// Método para obtener el sistema como una cadena csv
	Glib::ustring getAsString(void);

	// Obtener los listados
	Glib::RefPtr<Gtk::ListStore>& getGameList(void);
	Glib::RefPtr<Gtk::ListStore>& getEmulatorList(void);

	// Gestión de juegos
	CGame* getGame(Gtk::TreeModel::iterator& p_iter);
	void addGame(CGame& p_game);
	void removeGame(Gtk::TreeModel::iterator& p_iter);
	void updateGame(Gtk::TreeModel::iterator& p_iter, CGame& p_game);

	// Gestión de emuladores
	Gtk::TreeModel::iterator getEmulator(const unsigned int p_id);
	CEmulator* getEmulator(Gtk::TreeModel::iterator& p_iter);
	Gtk::TreeModel::iterator addEmulator(CEmulator& p_emulator);
	void removeEmulator(Gtk::TreeModel::iterator& p_iter);
	void updateEmulator(Gtk::TreeModel::iterator& p_iter, CEmulator& p_emulator);

	// Obtener los campos del sistema
	unsigned int getId(void);
	Glib::ustring getName(void);
	Glib::ustring getDescription(void);
	Glib::ustring getIconFile(void);
	Glib::RefPtr<Gdk::Pixbuf> getIcon(void);
	Glib::ustring getDatFile(void);
	Glib::ustring getBiosFile(void);
	Glib::ustring getHomeDir(void);
	Glib::ustring getRomsDir(void);
	Glib::ustring getSnapDir(void);
	Glib::ustring getTitleDir(void);
	Glib::ustring getFrontDir(void);
	Glib::ustring getBackDir(void);
	Glib::ustring getMediaDir(void);
	unsigned int getGames(void);
	unsigned int getEmulatorId(void);

	// Obtener informacion del dat
	Glib::ustring getDatName(void);
	Glib::ustring getDatDescription(void);
	Glib::ustring getDatCategory(void);
	Glib::ustring getDatVersion(void);
	Glib::ustring getDatAuthor(void);
	unsigned int getDatGames(void);

	// Establecer los campos del sistema
	void setId(const unsigned int p_id);
	void setName(const Glib::ustring& p_name);
	void setDescription(const Glib::ustring& p_description);
	void setIconFile(const Glib::ustring& p_icon);
	void setIcon(Glib::RefPtr<Gdk::Pixbuf> p_icon);
	void setDatFile(const Glib::ustring& p_dat_file);
	void setBiosFile(const Glib::ustring& p_bios_file);
	void setHomeDir(const Glib::ustring& p_home_dir);
	void setRomsDir(const Glib::ustring& p_roms_dir);
	void setSnapDir(const Glib::ustring& p_snap_dir);
	void setTitleDir(const Glib::ustring& p_tittle_dir);
	void setFrontDir(const Glib::ustring& p_front_dir);
	void setBackDir(const Glib::ustring& p_back_dir);
	void setMediaDir(const Glib::ustring& p_media_dir);
	void setEmulatorId(const  unsigned int p_id);

	// Establecer información del dat
	void setDatName(const Glib::ustring& p_name);
	void setDatDescription(const Glib::ustring& p_description);
	void setDatCategory(const Glib::ustring& p_category);
	void setDatVersion(const Glib::ustring& p_version);
	void setDatAuthor(const Glib::ustring& p_author);
	void setDatGames(unsigned int p_games);

private:
	// Instancia del manager
	CSystemManager* m_manager;

	// Campos especificos del sistema
	unsigned int m_id;					// Identificador del sistema
	Glib::ustring m_name;				// Nombre del sistema (Se utiliza como home)
	Glib::ustring m_description;		// Descripción
	Glib::ustring m_icon_file;			// Path al icono o "" para icono por defecto
	Glib::RefPtr<Gdk::Pixbuf> m_icon;	// Icono del sistema
	Glib::ustring m_dat_file;			// Path al fichero dat
	Glib::ustring m_bios_file;			// Path al fichero bios del sistema
	Glib::ustring m_home_dir;			// Directorio home del emulador, no se almacena
	Glib::ustring m_roms_dir;			// Directorio de las roms
	Glib::ustring m_snap_dir;			// Directorio de imagenes de capturas
	Glib::ustring m_tittle_dir;			// Directorio de imagenes de titulos
	Glib::ustring m_front_dir;			// Directorio de imagenes frontales
	Glib::ustring m_back_dir;			// Directorio de imagenes traseras
	Glib::ustring m_media_dir;			// Directorio de imagenes del soporte del sistema
	unsigned int m_emulator_id;			// Identificador del emulador asignado

	// Campo de la cabecera del dat
	Glib::ustring m_dat_name;			// Nombre del dat
	Glib::ustring m_dat_description;	// Descripción del dat
	Glib::ustring m_dat_category;		// Categoría del dat
	Glib::ustring m_dat_version;		// Versión del dat
	Glib::ustring m_dat_author;			// Autor del dat
	unsigned int m_dat_games;			// Juegos totales en el dat

	// Modelo del listado de juegos
	Glib::RefPtr<Gtk::ListStore> m_game_list;
	CGameModelColumns m_game_columns;

	// Modelo del listado de los emuladores
	Glib::RefPtr<Gtk::ListStore> m_emulator_list;
	CEmulatorModelColumns m_emulator_columns;
};

#endif // _SYSTEM_HPP_
