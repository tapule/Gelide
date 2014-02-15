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

#ifndef _DEBUG_HPP_
#define _DEBUG_HPP_

/**
 * Sistema de debug.
 * Define unas macros para volcado por la salida estandar de mensajes.
 * Para habilitar el modo debug, hay que definir previamente ENABLE_DEBUG_MODE
 */
#ifdef ENABLE_DEBUG_MODE
	#include <iostream>

	// Definición de macros para el modo debug
	#define DEBUG(str) std::cout << "Debug**  " << str << std::endl
	#define WARNING(str) std::cout << "Warning**  " << __FILE__ << "[" << __LINE__ << "]: " << str << std::endl
	#define ERROR(str) std::cout << "Error**  " << __FILE__ << "[" << __LINE__ << "]: " << str << std::endl; exit(1)
#else
	// Definimos las macros de debug como nulas
	#define DEBUG(str)
	#define WARNING(str)
	#define ERROR(str)
#endif  // ENABLE_DEBUG_MODE

#endif // _DEBUG_HPP_
