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

#include "process.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <glibmm.h>


CProcess::CProcess(void): m_proc_id(-1){
}

CProcess::~CProcess(){
	this->stop();
}

bool CProcess::run(const Glib::ustring p_command,  const Glib::ustring p_path){
	std::vector<Glib::ustring> l_argv;
	if(m_proc_id != -1){
		GELIDE_ERROR("Process " << m_proc_id << " is already running.");
		return false;
	}

	GELIDE_DEBUG("Launching process...");

	try{
		l_argv = Glib::shell_parse_argv(p_command);
		Glib::spawn_async_with_pipes(
			p_path,							// working_directory
			l_argv,							// argv
			Glib::SPAWN_DO_NOT_REAP_CHILD,	//Glib::SPAWN_LEAVE_DESCRIPTORS_OPEN,	// SpawnFlags
			sigc::slot< void >(),			// child_setup
			&m_proc_id, 					// child_pid
			&m_proc_in,						// standard_input
			&m_proc_out,					// standard_output
			&m_proc_err						// standard_error
		);
	}
	catch(const Glib::Exception& l_exception){
		GELIDE_WARNING("Running command (" << l_exception.what().c_str() << ")");
		return false;
	}

	// Creamos los canales de lectura
	m_channel_in = Glib::IOChannel::create_from_fd(m_proc_in);
	m_channel_out = Glib::IOChannel::create_from_fd(m_proc_out);
	m_channel_err = Glib::IOChannel::create_from_fd(m_proc_err);

	GELIDE_DEBUG("Process launched correctly with id " << m_proc_id);

	return true;
}

void CProcess::stop(void){
	if (m_proc_id != -1){
		m_channel_in->close();
		m_channel_out->close();
		m_channel_err->close();
		// CHECKME: No se si el iochanel los cierra
		close(m_proc_out);
		close(m_proc_in);
		close(m_proc_err);
		kill(m_proc_id, SIGINT);
		waitpid( m_proc_id, NULL, 0 );
		Glib::spawn_close_pid(m_proc_id);

		GELIDE_DEBUG("Process " << m_proc_id << " finished");

		m_proc_id = -1;

	}
}

bool CProcess::isRunning(void){
	int l_status;
	int l_child_id;

	if (m_proc_id != -1){
		// Comprobamos el estado del proceso
		//waitpid( m_proc_id, &l_status, WNOHANG );
		l_child_id = waitpid( m_proc_id, &l_status, 0);

		// Comprobamos si se produjo un error
		if(l_child_id == -1)
			return false;

		// Comprobamos la terminación del proceso
		if(!WIFEXITED(l_status))
			return true;
		else
			return false;
	}
	return false;
}

int CProcess::outReadLine(Glib::ustring& p_text){
	Glib::IOStatus l_status;

	if (m_proc_id != -1){
		try{
			l_status = m_channel_out->read_line(p_text);
			if((l_status == Glib::IO_STATUS_EOF) || (l_status == Glib::IO_STATUS_ERROR))
				return -1;
			return p_text.size();
		}
		catch(const Glib::Exception& l_exception){
			GELIDE_ERROR("Reading from process (" << l_exception.what().c_str() << ")");
			return -1;
		}
	}
	return -1;
}

int CProcess::errReadLine(Glib::ustring& p_text){
	Glib::IOStatus l_status;

	if (m_proc_id != -1){
		try{
			l_status = m_channel_err->read_line(p_text);
			if((l_status == Glib::IO_STATUS_EOF) || (l_status == Glib::IO_STATUS_ERROR))
				return -1;
			return p_text.size();
		}
		catch(const Glib::Exception& l_exception){
			GELIDE_ERROR("Reading from process (" << l_exception.what().c_str() << ")");
			return -1;
		}
	}
	return -1;
}

bool CProcess::write(const Glib::ustring p_text){
	if (m_proc_id != -1){
		try{
			m_channel_in->write(p_text);
			return true;
		}
		catch(const Glib::Exception& l_exception){
			GELIDE_ERROR("Writing in process (" << l_exception.what().c_str() << ")");
			return false;
		}
	}
	return false;
}


bool CProcess::write( void *p_data, unsigned int p_size ){
	unsigned int l_total_writed;
	unsigned int l_writed;

	if (m_proc_id != -1){
		try{
			l_total_writed = 0;
			while(l_total_writed < p_size){
				m_channel_in->write((char *)p_data, (gssize) p_size,(gsize &) l_writed);
				l_total_writed += l_writed;
			}
			return true;
		}
		catch(const Glib::Exception& l_exception){
			GELIDE_ERROR("Writing in process (" << l_exception.what().c_str() << ")");
			return false;
		}
	}
	return false;
}
