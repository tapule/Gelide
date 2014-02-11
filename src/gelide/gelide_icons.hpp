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

#ifndef _GELIDE_ICONS_HPP_
#define _GELIDE_ICONS_HPP_

#include "../gelide.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/iconfactory.h>
#include <gtkmm/stock.h>

class CGelideIcons
{
public:
	CGelideIcons(void);

	// Añade un icono nuevo en el factory de gelide
	void addStockIcon(const Glib::ustring& p_file, const Glib::ustring& p_id,
					  const Glib::ustring& p_label);
	void addStockIcon(Glib::RefPtr<Gdk::Pixbuf> p_pixbuf,
			const Glib::ustring& p_id, const Glib::ustring& p_label);
	// Inicializa los iconos por defecto de gelide
	void initDefaultIcons(void);
private:
	Glib::RefPtr<Gtk::IconFactory> m_icon_factory;
	//void updateDefaultIcons(void);

	void addThemeIcon(const Glib::ustring& p_icon, const Glib::ustring& p_id,
			const Glib::ustring& p_label, const Glib::ustring& p_file, const bool p_neg);
};

#endif // _GELIDE_ICONS_HPP_
