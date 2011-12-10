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

#ifndef _INFO_PANNEL_HPP_
#define _INFO_PANNEL_HPP_

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/notebook.h>
#include <gtkmm/alignment.h>
#include <gtkmm/table.h>
#include <gtkmm/button.h>
#include <glibmm/ustring.h>
#include <sigc++/sigc++.h>
#include "../gelide.hpp"
#include "../config.hpp"
#include "../interface_configurable.hpp"
#include "../core/system_manager.hpp"
#include "../core/system.hpp"


class CInfoPannel: public Gtk::VBox, public IConfigurable
{
public:
	CInfoPannel(void);
	virtual ~CInfoPannel();

	void setGame(Gtk::TreeModel::iterator& p_system, Gtk::TreeModel::iterator& p_game);
	void clear(void);

	// Señal emitida para iniciar el juego
	sigc::signal<void> signalPlayGame(void);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);
protected:
	virtual void on_size_allocate(Gtk::Allocation & p_allocation);
private:
	// Instancias del configurador y de los gestores
	CConfig* m_config;
	CSystemManager* m_smanager;
	CSystem* m_system;
	Gtk::TreeModel::iterator m_game;

	// Indica cuando hay que redimensionar las imágenes
	bool m_sized;

	// Pixbuf por defecto
	Glib::RefPtr<Gdk::Pixbuf> m_default;
	// Pixbufs para las imagenes
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_snapshot;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_tittle;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_front;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_back;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_media;



	// Señal emitida para iniciar el juego
	sigc::signal<void> m_signal_play_game;

	// Iconos para los Flags
	Glib::RefPtr<Gdk::Pixbuf> m_flag_favorite[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_played[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_working[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_available[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_unknown[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_rank[2];
	// Iconos para sistema y emulador negativos
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_system;
	Glib::RefPtr<Gdk::Pixbuf> m_pixbuf_emulator;

	// Etiqueta con la descripción
	Gtk::Label m_label_description;

	// Caja descripción
	Gtk::Table m_table_description;
	Gtk::Label m_label_flags;
	Gtk::Image m_image_favorite;
	Gtk::Image m_image_played;
	Gtk::Image m_image_working;
	Gtk::Image m_image_available;
	Gtk::Image m_image_unknown;
	Gtk::Label m_dummie1;
	Gtk::Label m_label_rank;
	Gtk::Image m_image_rank[5];
	Gtk::Label m_dummie2;
	Gtk::Label m_label_manufacturer;
	Gtk::Label m_label_manufacturer_value;
	Gtk::Label m_label_year;
	Gtk::Label m_label_year_value;
	Gtk::Label m_label_crc;
	Gtk::Label m_label_crc_value;

	// Notebook screenshots
	Gtk::Notebook m_notebook_images;
	Gtk::Image m_image_snapshot;
	Gtk::Image m_image_tittle;
	Gtk::Image m_image_front;
	Gtk::Image m_image_back;
	Gtk::Image m_image_media;

	// Box sistema
	Gtk::VBox m_vbox_system;
	Gtk::Label m_label_system_title;
	Gtk::Alignment m_alignment_system;
	Gtk::Table m_table_system;
	Gtk::Image m_image_system;
	Gtk::Label m_label_system_name;
	Gtk::Label m_label_system_description;

	// Box emulator
	Gtk::VBox m_vbox_emulator;
	Gtk::Label m_label_emulator_title;
	Gtk::Alignment m_alignment_emulator;
	Gtk::Table m_table_emulator;
	Gtk::Image m_image_emulator;
	Gtk::Label m_label_emulator_name;
	Gtk::Label m_label_emulator_description;

	// Boton
	Gtk::Button m_button_run;
	Gtk::HBox m_hbox_run;
	Gtk::Image m_image_run;
	Gtk::Label m_label_run;

	void initFlagsIcons(void);
	void initDescription(void);
	void initImages(void);
	void initSystem(void);
	void initEmulator(void);
	void initRun(void);

	void onRunActivated(void);

	void updateImages(CSystem* p_system, CGame* p_game);
};

#endif // _INFO_PANNEL_HPP_
