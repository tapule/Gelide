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

#include "dat_reader.hpp"
#include <iostream>
#include "../gelide.hpp"

CDatReader::CDatReader(void):
	m_loaded(false),
	m_ignore_bios(true),
	m_parse_flags(false)
{
}

Glib::ustring CDatReader::getType(void){
	return "";
}

void CDatReader::reset(void){
	m_loaded = false;

	m_name.clear();
	m_description.clear();
	m_category.clear();
	m_version.clear();
	m_date.clear();
	m_author.clear();
	m_email.clear();
	m_homepage.clear();
	m_url.clear();
	m_comment.clear();
}
