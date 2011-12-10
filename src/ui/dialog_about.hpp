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

#ifndef _DIALOG_ABOUT_HPP_
#define _DIALOG_ABOUT_HPP_

#include <gtkmm/aboutdialog.h>
#include "../gelide.hpp"
#include "../config.hpp"
#include "../interface_configurable.hpp"

class CDialogAbout: public Gtk::AboutDialog, public IConfigurable
{
public:
	CDialogAbout(void);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);

private:
	// Instancias del configurador y gestor de sistemas
	CConfig* m_config;

    std::vector<Glib::ustring> m_authors;
    std::vector<Glib::ustring> m_artists;

    virtual void on_response(int p_id);

};

#endif // _DIALOG_ABOUT_HPP_
