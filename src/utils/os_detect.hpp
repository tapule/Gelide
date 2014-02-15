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

#ifndef _OS_DETECT_HPP_
#define _OS_DETECT_HPP_

/*
	Definiciones de macros para detectar el sistema
*/
#if defined(__linux__)
	#define OS_LINUX
#elif defined(__APPLE__)
	#define OS_MACOSX
#elif defined(__FreeBSD__)
	#define OS_FREEBSD
#elif defined(__OpenBSD__)
	#define OS_OPENBSD
#elif defined(__sun)
	#define OS_SOLARIS
#elif defined(_WIN32)
	#define OS_WIN
#else
	#error Platform not supported
#endif

#if defined(OS_LINUX) || defined(OS_MACOSX) ||  defined(OS_FREEBSD) || defined(OS_OPENBSD) || defined(OS_SOLARIS)
	#define OS_POSIX
#endif

#endif // _OS_DETECT_HPP_
