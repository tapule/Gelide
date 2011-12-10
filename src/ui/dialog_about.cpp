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

#include "dialog_about.hpp"

CDialogAbout::CDialogAbout(void){
	Glib::ustring l_comments;
	Glib::ustring l_license;
	Glib::ustring l_translators;
	Glib::ustring l_version;
	Glib::RefPtr<Gdk::Pixbuf> l_logo;

	// Obtenemos las instancias
	m_config = CConfig::getInstance();

	set_name("Gelide");
#ifdef PACKAGE_CODENAME
	set_version(GELIDE_VERSION" \""PACKAGE_CODENAME"\"");
#else
	set_version(GELIDE_VERSION);
#endif

	set_website(GELIDE_WEBSITE);
	set_website_label(_("Gelide Website"));

	set_copyright("\xC2\xA9 2008 - 2011 Juan Ángel Moreno Fernández");

	l_comments.append(_("\"One to rule them all\"\n\nA multi system and multi emulator\n frontend for GNU/Linux."));
	set_comments(l_comments);

	l_license =
		"Gelide * Copyright (C) 2008 - 2011 Juan Ángel Moreno Fernández\n"
		"			  Gelide is free software.\n"
		"\n"
		"You may redistribute it and/or modify it under the terms of the\n"
		"GNU General Public License, as published by the Free Software\n"
		"Foundation; either version 2 of the License, or (at your option)\n"
		"any later version.\n"
		"\n"
		"gelide is distributed in the hope that it will be useful,\n"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
		"See the GNU General Public License for more details.\n"
		"\n"
		"You should have received a copy of the GNU General Public License\n"
		"along with gelide.  If not, write to:\n"
		"\tThe Free Software Foundation, Inc.,\n"
		"\t51 Franklin Street, Fifth Floor\n"
		"\tBoston, MA  02110-1301, USA.\n";
	set_license(l_license);

	m_authors.push_back("Juan Ángel Moreno Fernández\n<jamf@users.sourceforge.net>");
	set_authors(m_authors);

	// Translators: change this to your name, separate multiple names with \n
	l_translators = _("Juan Ángel Moreno Fernández\n<jamf@users.sourceforge.net>");
	set_translator_credits(l_translators);

	try{
		l_logo = Gdk::Pixbuf::create_from_file(GELIDE_ICON)->scale_simple(128, 128, Gdk::INTERP_BILINEAR);
		set_logo(l_logo);
	}
	catch (const Glib::Error& l_exception){
		GELIDE_WARNING("Loading icon (" << l_exception.what().c_str() << ")");
	}

	// Establecemos el diálogo por defecto en el centro del padre
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);

	// Cargamos la configuración
	loadConfig();
}

void CDialogAbout::loadConfig(void){
	int l_x, l_y;

	// Obtenemos la posición almacenada
	if(m_config->getKey(GELIDE_CFG_DIALOGS, "about_dialog_x", l_x){
		m_config->getKey(GELIDE_CFG_DIALOGS, "about_dialog_y", l_y);
		this->move(l_x, l_y);
	}
}

void CDialogAbout::saveConfig(void){
	int l_x, l_y;

	// Guardamos la posición del dialogo
	this->get_position(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "about_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "about_dialog_y", l_y);
}

void CDialogAbout::on_response(int p_id){
	if(p_id == Gtk::RESPONSE_DELETE_EVENT || p_id == Gtk::RESPONSE_CANCEL || p_id == Gtk::RESPONSE_CLOSE){
		saveConfig();
		hide();
	}
}

