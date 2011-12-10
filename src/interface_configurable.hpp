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

#ifndef _INTERFACE_CONFIGURABLE_HPP_
#define _INTERFACE_CONFIGURABLE_HPP_

#include <glibmm/ustring.h>
#include "config.hpp"

class IConfigurable
{
public:
	// Constructor y destructor de la interfaz
	virtual ~IConfigurable(){};

	virtual void loadConfig(void){};
	virtual void saveConfig(void){};
};

#endif // _INTERFACE_CONFIGURABLE_HPP_
