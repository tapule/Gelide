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

#ifndef _MODEL_COLUMNS_HPP_
#define _MODEL_COLUMNS_HPP_

#include "../gelide.hpp"
#include <gtkmm/liststore.h>
#include <glibmm/ustring.h>
#include <gdkmm/pixbuf.h>


// Columnas para los listados de juegos
class CGameModelColumns:public Gtk::TreeModel::ColumnRecord
{
public:
	CGameModelColumns(void);
	virtual ~CGameModelColumns(){};

	Gtk::TreeModelColumn<bool> m_favorite;
	Gtk::TreeModelColumn<Glib::ustring> m_name;
	// Flags....PWAU: played working available unknown
	Gtk::TreeModelColumn<int> m_flags;
	Gtk::TreeModelColumn<unsigned int> m_rank;
	Gtk::TreeModelColumn<unsigned int> m_year;
	Gtk::TreeModelColumn<Glib::ustring> m_description;
	Gtk::TreeModelColumn<Glib::ustring> m_manufacturer;
	Gtk::TreeModelColumn<Glib::ustring> m_path;
	Gtk::TreeModelColumn<Glib::ustring> m_rom_crc;
};

// Columnas para los listados de emuladores
class CEmulatorModelColumns:public Gtk::TreeModel::ColumnRecord
{
public:
	CEmulatorModelColumns(void);
	virtual ~CEmulatorModelColumns(){};

	Gtk::TreeModelColumn<unsigned int> m_id;
	Gtk::TreeModelColumn<Glib::ustring> m_icon_file;			// Path al icono o "" para icono por defecto
	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> >  m_icon;
	Gtk::TreeModelColumn<Glib::ustring> m_name;
	Gtk::TreeModelColumn<Glib::ustring> m_description;
	Gtk::TreeModelColumn<Glib::ustring> m_version;
	Gtk::TreeModelColumn<Glib::ustring> m_author;
	Gtk::TreeModelColumn<Glib::ustring> m_homepage;
	Gtk::TreeModelColumn<Glib::ustring> m_path;				// Path al ejecutable
	Gtk::TreeModelColumn<Glib::ustring> m_params;			// Parámetros de ejecución
															// $gf hace referencia al juego
															// $bf hace referencia a la bios
															// $bp hace referencia a la bios
};

// Columnas para los listados de sistemas
class CSystemModelColumns:public Gtk::TreeModel::ColumnRecord
{
public:
	CSystemModelColumns(void);
	virtual ~CSystemModelColumns(){};

	Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> >  m_icon;
	Gtk::TreeModelColumn<unsigned int> m_id;
	Gtk::TreeModelColumn<Glib::ustring> m_name;
	Gtk::TreeModelColumn<Glib::ustring> m_description;
};

#endif // _MODEL_COLUMNS_HPP_
