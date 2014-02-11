/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gelide
 * Copyright (C) 2008 - 2014 Juan Ángel Moreno Fernández
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

#ifndef _CONFIGURABLE_INTERFACE_HPP_
#define _CONFIGURABLE_INTERFACEHPP_

#include <glibmm/ustring.h>
#include "config.hpp"


class ConfigurableInterface
{
public:
	/**
	 * Constructor
	 */
	virtual ~ConfigurableInterface()
	{
	};

	/**
	 * Se encarga de cargar la configuración
	 */
	virtual void loadConfig(void)
	{
	};

	/**
	 * Se encarga de guardar la configuración
	 */
	virtual void saveConfig(void)
	{
	};
};

#endif // _CONFIGURABLE_INTERFACE_HPP_
