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

#ifndef _DIALOG_GAMELIST_GENERATOR_HPP_
#define _DIALOG_GAMELIST_GENERATOR_HPP_

#include "../gelide.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/progressbar.h>
#include "../interface_configurable.hpp"
#include "../config.hpp"
#include "../core/system_manager.hpp"
#include "../core/system.hpp"


class CDialogGamelistGenerator:public Gtk::Dialog, public IConfigurable
{
public:
	// Constructor y destructor de la clase
	CDialogGamelistGenerator(void);
	~CDialogGamelistGenerator();
	// Estalbece el sistema a generar
	void setSystem(CSystem* p_system);

	// Ejecuta el dialogo
	int run(void);
	// Ejecuta el dialogo estableciendo el sistema a generar
	int run(CSystem* p_system);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);

private:
	// Instancias del configurador y gestor de sistemas
	CConfig* m_config;
	CSystemManager* m_smanager;

	CSystem* m_system;				// Sistema para el que generar listado

	// UI Cuerpo principal
	Gtk::VBox m_vbox_body;
	Gtk::Label m_label_description;
	Gtk::Label m_label_info;
	Gtk::ProgressBar m_progressbar;
	Gtk::Label m_label_status;
	Gtk::Button m_button_close;
	// Callback llamado al pulsar el botón cerrar
	void onCloseClicked(void);
	// Callback para controlar el evento delete del dialogo
	bool on_delete_event(GdkEventAny* p_event);

	bool generateGameList(void);

};

#endif // _DIALOG_GAMELIST_GENERATOR_HPP_
