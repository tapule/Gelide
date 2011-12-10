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

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <glibmm/ustring.h>

class CGame
{
public:
	CGame(void);
	virtual ~CGame(){};

	// Obtener los campos del juego
	unsigned int getSystemId(void);
	bool getFavorite(void);
	Glib::ustring getName(void);
	unsigned int getRank(void);
	unsigned int getYear(void);
	Glib::ustring getDescription(void);
	Glib::ustring getManufacturer(void);
	Glib::ustring getPath(void);
	Glib::ustring getRomCrc(void);

	// Obtener los flags del juego
	int getFlags(void);
	bool getPlayed(void);
	bool getWorking(void);
	bool getAvailable(void);
	bool getUnknown(void);

	// Establecer los campos del juego
	void setSystemId(const unsigned int p_id);
	void setFavorite(const bool p_favorite);
	void setName(const Glib::ustring& p_name);
	void setRank(const unsigned int p_rank);
	void setYear(const unsigned int p_year);
	void setDescription(const Glib::ustring& p_description);
	void setManufacturer(const Glib::ustring& p_manufacturer);
	void setPath(const Glib::ustring& p_path);
	void setRomCrc(const Glib::ustring& p_crc);

	// Establecer los flags del juego
	void setFlags(const int p_flags);
	void setPlayed(const bool p_played);
	void setWorking(const bool p_working);
	void setAvailable(const bool p_available);
	void setUnknown(const bool p_unknown);

private:
	unsigned int m_system_id;		// Id del sistema al que pertenece el juego
	bool m_favorite;				// ¿Es favorito?
	Glib::ustring m_name;			// Nombre del juego
	unsigned int m_rank;			// Puntuación
	unsigned int m_year;			// Año
	Glib::ustring m_description;	// Descripción
	Glib::ustring m_manufacturer;	// Fabricante
	Glib::ustring m_path;			// Path del juego
	Glib::ustring m_rom_crc;		// crc de la rom

	//CHECKME: Los flags, se podrían guardar directamente codificados, y
	//decodificar las funciones una a una.
	//Flags decodificados
	bool m_played;					// Jugado?
	bool m_working;					// Funcional?
	bool m_available;				// Disponible ?
	bool m_unknown;					// Desconocido ?
};

#endif // _GAME_HPP_
