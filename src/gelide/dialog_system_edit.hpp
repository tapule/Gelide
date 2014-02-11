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

#ifndef _DIALOG_SYSTEM_EDIT_HPP_
#define _DIALOG_SYSTEM_EDIT_HPP_

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

class CDialogSystemEdit: public Gtk::Dialog, public IConfigurable
{
public:
	// Constructor y destructor de la clase
	CDialogSystemEdit();
	~CDialogSystemEdit();
	// Establece el sistema a editar, debe ser no nulo
	void setSystem(CSystem* p_system);
	// Ejecuta el dialogo
	int run(void);
	// Ejecuta el dialogo estableciendo primero el sistema
	int run(CSystem* p_system);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);
private:
	// Instancias del configurador y de los gestores
	CConfig* m_config;
	CSystemManager* m_smanager;

	CSystem* m_system;				// Sistema a editar

	// Cuerpo principal
	Gtk::Notebook m_notebook_body;
	Gtk::Button m_button_help;
	Gtk::Button m_button_cancel;
	Gtk::Button m_button_accept;
	// Callbacks para el cuerpo principal
	void onHelpClicked(void);
	void onCancelClicked(void);
	void onAcceptClicked(void);

	// Pestaña Información
	Gtk::VBox m_vbox_info;
	Gtk::Alignment m_alignment_info;
	Gtk::Label m_label_info;
	Gtk::Table m_table_info;
	Gtk::Label m_label_name;
	Gtk::Entry m_entry_name;
	Gtk::Label m_label_description;
	Gtk::ScrolledWindow m_scroll_description;
	Gtk::TextView m_text_description;
	Glib::RefPtr<Gtk::TextBuffer> m_description_buf;
	Gtk::Label m_label_dat;
	Gtk::FileChooserButton m_button_dat;
	Gtk::Label m_label_bios;
	Gtk::FileChooserButton m_button_bios;
	Gtk::Label m_label_icon;
	Gtk::Button m_button_icon;
	Gtk::Image m_image_icon;
	Glib::ustring m_icon_file;

	void onIconClicked(void);

	// Pestaña Directorios
	Gtk::VBox m_vbox_paths;
	Gtk::Label m_label_home;
	Gtk::Alignment m_alignment_home;
	Gtk::HBox m_hbox_home;
	Gtk::Entry m_entry_home;
	Gtk::Button m_button_home;

	Gtk::Label m_label_roms;
	Gtk::Alignment m_alignment_roms;
	Gtk::HBox m_hbox_roms;
	Gtk::Entry m_entry_roms;
	Gtk::Button m_button_roms;
	Gtk::Label m_label_images;
	Gtk::Alignment m_alignment_images;
	Gtk::Table m_table_images;
	Gtk::HBox m_hbox_snap;
	Gtk::Label m_label_snap;
	Gtk::Entry m_entry_snap;
	Gtk::Button m_button_snap;
	Gtk::Label m_label_tittles;
	Gtk::HBox m_hbox_tittles;
	Gtk::Entry m_entry_tittles;
	Gtk::Button m_button_tittles;
	Gtk::Label m_label_front;
	Gtk::HBox m_hbox_front;
	Gtk::Entry m_entry_front;
	Gtk::Button m_button_front;
	Gtk::Label m_label_back;
	Gtk::HBox m_hbox_back;
	Gtk::Entry m_entry_back;
	Gtk::Button m_button_back;
	Gtk::Label m_label_media;
	Gtk::HBox m_hbox_media;
	Gtk::Entry m_entry_media;
	Gtk::Button m_button_media;

	// Configura el panel de información
	void initInfoTab(void);
	// Configura el panel de directorios
	void initPathsTab(void);
	// Función genérica para cambiar el directorio en un entry
	void onSelectFolderClicked(Gtk::Entry* p_entry);

};

#endif // _DIALOG_SYSTEM_EDIT_HPP_
