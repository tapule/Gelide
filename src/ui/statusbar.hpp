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

#ifndef _STATUSBAR_HPP_
#define _STATUSBAR_HPP_

#include "../gelide.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/separator.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>

class CStatusBar: public Gtk::HBox
{
public:
	CStatusBar(void);
	virtual ~CStatusBar(){};

	void setGameInfo(const Glib::ustring p_system, const Glib::ustring p_game);
	void setListInfo(const unsigned int p_list, const unsigned int p_system, const unsigned int p_total);
	void setWorking(const bool p_working);

private:
	// Barra de estado
	Gtk::VBox m_vbox_info;
	Gtk::HSeparator m_hseparator;
	Gtk::HBox m_hbox_info;
	Gtk::Label m_label_game;
	Gtk::Image m_image_status;
	Gtk::Label m_label_list;

	Gtk::Statusbar m_statusbar;

	Glib::RefPtr<Gdk::Pixbuf> m_flag_working[2];

	void initFlagsIcons(void);

};

#endif // _STATUSBAR_HPP_
