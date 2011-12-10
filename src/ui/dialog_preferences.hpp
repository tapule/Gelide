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

#ifndef _DIALOG_PREFERENCES_HPP_
#define _DIALOG_PREFERENCES_HPP_

#include "../gelide.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/alignment.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/checkbutton.h>
#include "../interface_configurable.hpp"
#include "../config.hpp"

class CDialogPreferences: public Gtk::Dialog, public IConfigurable
{
public:
	// Constructor y destructor de la clase
	CDialogPreferences();
	virtual ~CDialogPreferences();

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);

private:
	// Instancia del configurador
	CConfig* m_config;

	// Cuerpo principal
	Gtk::VBox m_vbox_default_sys;
	Gtk::Label m_label_default_sys;
	Gtk::Alignment m_alignment_default_sys;
	Gtk::CheckButton m_check_load_default_sys;

	Gtk::VBox m_vbox_emulation;
	Gtk::Label m_label_emulation;
	Gtk::Alignment m_alignment_emulation;
	Gtk::CheckButton m_check_show_output;

	Gtk::VBox m_vbox_list_generator;
	Gtk::Label m_label_list_generator;
	Gtk::Alignment m_alignment_list_generator;
	Gtk::VBox m_vbox_list_generator_checks;
	Gtk::CheckButton m_check_preserve_data;
	Gtk::CheckButton m_check_add_unknown;

	Gtk::VBox m_vbox_exporting;
	Gtk::Label m_label_exporting;
	Gtk::Alignment m_alignment_exporting;
	Gtk::VBox m_vbox_exporting_checks;
	Gtk::CheckButton m_check_export_icons;
	Gtk::CheckButton m_check_export_dats;

	// Botones del dialogo
	//Gtk::Button m_button_help;
	Gtk::Button m_button_close;

	// Funciones callbacks para las columnas
	void onLoadDefaultSysToggled(void);
	void onPreserveDataToggled(void);
	void onAddUnknownToggled(void);
	void onShowOutputToggled(void);
	void onExportIconsToggled(void);
	void onExportDatsToggled(void);

	// Callbacks para el cuerpo principal
	//void onHelpClicked(void);
	void onCloseClicked(void);

	void initBody(void);
};

#endif // _DIALOG_PREFERENCES_HPP_
