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

#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include "../gelide.hpp"
#include <glibmm/ustring.h>
#include <glibmm/iochannel.h>

class CProcess: public sigc::trackable
{
public:
	CProcess(void);
	virtual ~CProcess();

	bool run(const Glib::ustring p_command,  const Glib::ustring p_path);
	void stop(void);
	bool isRunning(void);

	int outReadLine(Glib::ustring& p_text);
	int errReadLine(Glib::ustring& p_text);
	bool write(const Glib::ustring p_text);
	bool write(void *p_data, unsigned int p_size);

private:
	int m_proc_id;		// Id del proceso
	int m_proc_out;		// Fichero donde escribe el proceso como salida
	int m_proc_in;		// Fichero de entrada al proceso
	int m_proc_err;		// Fichero donde escribe el proceso como error
	Glib::RefPtr<Glib::IOChannel> m_channel_out;		// Fichero donde escribe el proceso como salida
	Glib::RefPtr<Glib::IOChannel> m_channel_in;		// Fichero de entrada al proceso
	Glib::RefPtr<Glib::IOChannel> m_channel_err;		// Fichero donde escribe el proceso como error
};

#endif // _PROCESS_HPP_
