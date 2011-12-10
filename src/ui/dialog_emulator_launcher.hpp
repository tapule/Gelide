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

#ifndef _DIALOG_EMULATOR_LAUNCHER_HPP_
#define _DIALOG_EMULATOR_LAUNCHER_HPP_

#include "../gelide.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include "../interface_configurable.hpp"
#include "../config.hpp"
#include "../core/system_manager.hpp"


class CDialogEmulatorLauncher:public Gtk::Dialog, public IConfigurable
{
public:
	// Constructor y destructor de la clase
	CDialogEmulatorLauncher(void);
	~CDialogEmulatorLauncher();

	// Ejecuta el dialogo estableciendo el sistema a generar
	int run(CSystem* p_system, CGame* p_game);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);

private:
	// Instancias del configurador y gestor de sistemas
	CConfig* m_config;
	CSystemManager* m_smanager;

	// Indica si hay que cerrar el diálogo automáticamente
	bool m_show_output;

	// UI
	Gtk::HBox m_hbox_title;
	Gtk::Image m_image_title;
	Gtk::Label m_label_title;
	Gtk::ScrolledWindow m_scroll_progress;
	Gtk::TextView m_text_progress;
	Glib::RefPtr<Gtk::TextBuffer> m_textbuf_progress;
	Gtk::Button m_button_close;
	// Callback llamado al pulsar el botón cerrar
	void onCloseClicked(void);
	// Callback para controlar el evento delete del dialogo
	bool on_delete_event(GdkEventAny* p_event);
};

#endif // _DIALOG_EMULATOR_LAUNCHER_HPP_
