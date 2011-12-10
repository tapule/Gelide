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

#ifndef _ROM_HPP_
#define _ROM_HPP_

#include <glibmm/ustring.h>

// Tipos de rom
#define ROM_TYPE_ROM			0	// Rom tipo rom
#define ROM_TYPE_DISK			1	// Rom tipo disco

// Estados de la rom
#define ROM_STATE_UNKNOWN		0	// Desconocida
#define ROM_STATE_OK			1	// Ok
#define ROM_STATE_NOTFOUND		2	// No encontrada
#define ROM_STATE_BADNAMED		3	// Mal nombrada (aunque encontrada)
#define ROM_STATE_BADDUMP		4	// Mal dumpeada (esto viene directamente del dat)
#define ROM_STATE_NODUMP		5	// No dumpeada (esto viene directamente del dat)

/*------------------------------------------------------
	Mantiene la información de una rom.
--------------------------------------------------------*/
class CRom
{
public:
	CRom(void);

	CRom(const int p_type = ROM_TYPE_ROM, const int p_state = ROM_STATE_UNKNOWN,
			const Glib::ustring& p_name = "",	const unsigned int p_size = 0,
			const Glib::ustring& p_crc = "", const bool p_merge = false);

	~CRom();

	void setType(const int p_type = ROM_TYPE_ROM);
	void setState(const int p_state = ROM_STATE_UNKNOWN);
	void setName(const Glib::ustring& p_name = "");
	void setSize(const unsigned int p_size = 0);
	void setCrc(const Glib::ustring& p_crc = "");
	void setMerge(const bool p_merge = false);

	int getType(void);
	int getState(void);
	Glib::ustring getName(void);
	unsigned int getSize(void);
	Glib::ustring getCrc(void);
	bool getMerge(void);

private:
	int m_type;
	int m_state;
	Glib::ustring m_name;
	unsigned int m_size;
	Glib::ustring m_crc;
	bool m_merge;
};

#endif // _ROM_HPP_
