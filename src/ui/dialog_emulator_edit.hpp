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

#ifndef _DIALOG_EMULATOR_EDIT_HPP_
#define _DIALOG_EMULATOR_EDIT_HPP_

#include "../gelide.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/notebook.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/alignment.h>
#include <gtkmm/table.h>
#include <gtkmm/textview.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/image.h>
#include <gtkmm/stock.h>
#include "../interface_configurable.hpp"
#include "../config.hpp"
#include "../core/system_manager.hpp"
#include "../core/system.hpp"
#include "../core/emulator.hpp"


class CDialogEmulatorEdit: public Gtk::Dialog, public IConfigurable
{
public:
	// Constructor y destructor de la clase
	CDialogEmulatorEdit();
	~CDialogEmulatorEdit();
	// Establece el sistema a editar, debe ser no nulo
	void setEmulator(CEmulator* p_emulator);
	// Ejecuta el dialogo
	int run(void);
	// Ejecuta el dialogo estableciendo primero el sistema
	int run(CEmulator* p_emulator);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);
private:
	// Instancias del configurador y de los gestores
	CConfig* m_config;
	CSystemManager* m_smanager;

	CEmulator* m_emulator;				// Sistema a editar

	// Cuerpo principal
	Gtk::Label m_label_body;
	Gtk::Alignment m_alignment_body;
	Gtk::Button m_button_help;
	Gtk::Button m_button_cancel;
	Gtk::Button m_button_accept;
	// Callbacks para el cuerpo principal
	void onHelpClicked(void);
	void onCancelClicked(void);
	void onAcceptClicked(void);

	// Pestaña Información
	Gtk::Table m_table_info;
	Gtk::Label m_label_name;
	Gtk::Entry m_entry_name;
	Gtk::Label m_label_description;
	Gtk::ScrolledWindow m_scroll_description;
	Gtk::TextView m_text_description;
	Glib::RefPtr<Gtk::TextBuffer> m_description_buf;
	Gtk::Label m_label_version;
	Gtk::Entry m_entry_version;
	Gtk::Label m_label_author;
	Gtk::Entry m_entry_author;
	Gtk::Label m_label_homepage;
	Gtk::Entry m_entry_homepage;
	Gtk::Label m_label_path;
	Gtk::FileChooserButton m_button_path;
	Gtk::Label m_label_params;
	Gtk::Entry m_entry_params;
	Gtk::Label m_label_icon;
	Gtk::Button m_button_icon;
	Gtk::Image m_image_icon;
	Glib::ustring m_icon_file;
	void onIconClicked(void);

	// Configura el panel de información
	void initInfo(void);
};

#endif // _DIALOG_EMULATOR_EDIT_HPP_
