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

#include "model_columns.hpp"

CGameModelColumns::CGameModelColumns(void){
	add(m_favorite);
	add(m_name);
	add(m_flags);
	add(m_rank);
	add(m_year);
	add(m_description);
	add(m_manufacturer);
	add(m_path);
	add(m_rom_crc);
}

CEmulatorModelColumns::CEmulatorModelColumns(void){
	add(m_id);
	add(m_icon_file);
	add(m_icon);
	add(m_name);
	add(m_description);
	add(m_version);
	add(m_author);
	add(m_homepage);
	add(m_path);
	add(m_params);
}

CSystemModelColumns::CSystemModelColumns(void){
	add(m_icon);
	add(m_id);
	add(m_name);
	add(m_description);
}
