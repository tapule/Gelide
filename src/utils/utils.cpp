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
#include "utils.hpp"
#include "../gelide.hpp"
#include <ctime>
#include <glib/gfileutils.h>
//#include <glibmm.h>
//#include <glib/gstdio.h>
//#include <stdlib.h>

namespace utils{



Glib::RefPtr<Gdk::Pixbuf> createNegativePixbuf(Glib::RefPtr<Gdk::Pixbuf> p_pixbuf){
	Glib::RefPtr<Gdk::Pixbuf> l_pixbuf;

	l_pixbuf = p_pixbuf->copy();
	p_pixbuf->saturate_and_pixelate(l_pixbuf, 0, true);

	return l_pixbuf;
}

bool createDir(const Glib::ustring& p_path){

	if(g_mkdir_with_parents((p_path).c_str(), 0755)){
		GELIDE_ERROR("Could not create directory " << p_path.c_str());
		return false;
	}
	GELIDE_DEBUG("Successfully created Directory " << p_path.c_str());
	return true;
}

unsigned int getYear(void){
	time_t l_rawtime;
	struct tm * l_timeinfo;

	time( &l_rawtime );
	l_timeinfo = localtime ( &l_rawtime );

	return l_timeinfo->tm_year + 1900;
}

/**
 * Obtiene el path al directorio home de Gelide
 * @return Path al directorio home de Gelide
 */
Glib::ustring getGelideDir(void){
	return Glib::build_filename(Glib::get_home_dir(), GELIDE_DIR);
}

/**
 * Obtiene el path a la lista de sistemas del usuario
 * @return Path a la lista de sistemas del usuario
 */
Glib::ustring getSystemsList(void){
	return Glib::build_filename(getGelideDir(), USER_SYSTEMS_LIST);
}

} // namespace gelide


