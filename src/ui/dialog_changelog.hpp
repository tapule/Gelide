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

#ifndef _DIALOG_CHANGELOG_HPP_
#define _DIALOG_CHANGELOG_HPP_

#include "../gelide.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include "../interface_configurable.hpp"
#include "../config.hpp"
#include "../core/system_manager.hpp"

class CDialogChangelog:public Gtk::Dialog, public IConfigurable
{
public:
	// Constructor y destructor de la clase
	CDialogChangelog(void);
	~CDialogChangelog();

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);

private:
	// Instancias del configurador y gestor de sistemas
	CConfig* m_config;

	// UI
	Gtk::HBox m_hbox_title;
	Gtk::Image m_image_title;
	Gtk::Label m_label_title;
	Gtk::ScrolledWindow m_scroll_changelog;
	Gtk::TextView m_text_changelog;
	Glib::RefPtr<Gtk::TextBuffer> m_textbuf_changelog;
	Gtk::Button m_button_close;
	// Callback llamado al pulsar el botón cerrar
	void onCloseClicked(void);
	void loadText(void);
};

#endif // _DIALOG_CHANGELOG_HPP_
