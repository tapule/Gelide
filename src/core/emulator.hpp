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

#ifndef _EMULATOR_HPP_
#define _EMULATOR_HPP_

#include <glibmm/ustring.h>
#include <gdkmm/pixbuf.h>

class CEmulator
{
public:
	CEmulator(void);
	virtual ~CEmulator(void){};

	// Obtener los campos del emu
	unsigned int getSystemId(void);
	unsigned int getId(void);
	Glib::ustring getIconFile(void);
	Glib::RefPtr<Gdk::Pixbuf> getIcon(void);
	Glib::ustring getName(void);
	Glib::ustring getDescription(void);
	Glib::ustring getVersion(void);
	Glib::ustring getAuthor(void);
	Glib::ustring getHomepage(void);
	Glib::ustring getPath(void);
	Glib::ustring getParams(void);

	// Establecer los campos del emu
	void setSystemId(const unsigned int p_id);
	void setId(const unsigned int p_id);
	void setIconFile(const Glib::ustring& p_icon);
	void setIcon(Glib::RefPtr<Gdk::Pixbuf> p_icon);
	void setName(const Glib::ustring& p_name);
	void setDescription(const Glib::ustring& p_description);
	void setVersion(const Glib::ustring& p_version);
	void setAuthor(const Glib::ustring& p_author);
	void setHomepage(const Glib::ustring& p_homepage);
	void setPath(const Glib::ustring& p_path);
	void setParams(const Glib::ustring& p_params);

private:
	// Información de descripción del emulador
	unsigned int m_system_id;
	unsigned int m_id;
	Glib::ustring m_icon_file;
	Glib::RefPtr<Gdk::Pixbuf> m_icon;	// Icono del e
	Glib::ustring m_name;
	Glib::ustring m_description;
	Glib::ustring m_version;
	Glib::ustring m_author;
	Glib::ustring m_homepage;
	Glib::ustring m_path;
	Glib::ustring m_params;
};

#endif // _EMULATOR_HPP_
