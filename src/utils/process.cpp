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

#include "process.hpp"
#include "os_detect.hpp"
#if defined(OS_POSIX)
	#include <sys/types.h>
	#include <sys/wait.h>
#elif defined(OS_WIN)
	#include <windows.h>
#endif
#include <glibmm/shell.h>
#include <glibmm/spawn.h>
#include "log.hpp"
#include "utils.hpp"


Process::Process(void):
	// Iniciamos el id con -1 para indicar que no está en ejecución
#if defined(OS_POSIX)
	m_proc_id(-1),
#elif defined(OS_WIN)
	m_proc_id(NULL),
#endif
	m_channel_in(NULL),
	m_channel_out(NULL),
	m_channel_err(NULL)
{
}

Process::~Process()
{
	stop();
}

void Process::run(const Glib::ustring& command, const Glib::ustring& path)
{
	std::vector<Glib::ustring> argv;
	int proc_in, proc_out, proc_err;
	Glib::ustring run_path;

	// Comprobamos si el proceso ya está en ejecución
#if defined(OS_POSIX)
	if (m_proc_id != -1)
#elif defined(OS_WIN)
	if (m_proc_id != NULL)
#endif
	{
		LOG_ERROR("Process " << m_proc_id << " is already running.");
		return;
	}

	// Si no se especifica un path, actuamos en el directoria actual
	run_path = path.size() ? path: ".";

	LOG_DEBUG("Launching process...");

	try
	{
		argv = Glib::shell_parse_argv(command);
		LOG_DEBUG("Shell argv: " << utils::toStr(argv));

		Glib::spawn_async_with_pipes(
			run_path,						// working_directory
			argv,							// argv
			Glib::SPAWN_DO_NOT_REAP_CHILD,	//Glib::SPAWN_LEAVE_DESCRIPTORS_OPEN,	// SpawnFlags
			sigc::slot< void >(),			// child_setup
			&m_proc_id, 					// child_pid
			&proc_in,						// standard_input
			&proc_out,						// standard_output
			&proc_err						// standard_error
		);
	}
	catch (const Glib::Exception& e)
	{
		LOG_ERROR("Running command (" << e.what().c_str() << ")");
		return;
	}

	// Creamos los canales de lectura
	m_channel_in = Glib::IOChannel::create_from_fd(proc_in);
	m_channel_out = Glib::IOChannel::create_from_fd(proc_out);
	m_channel_err = Glib::IOChannel::create_from_fd(proc_err);

	// Añadimos un watch para enterarnos de cuando termina el proceso
	m_connection = Glib::signal_child_watch().connect(sigc::mem_fun(*this, &Process::onProcessExit), m_proc_id);

	LOG_DEBUG("Process launched correctly with id " << m_proc_id);
}

int Process::wait(void)
{
	int child_id;
	int ret = 0;

	// Comprobamos si el proceso está en ejecución
#if defined(OS_POSIX)
	if (m_proc_id == -1)
#elif defined(OS_WIN)
	if (m_proc_id == NULL)
#endif
	{
		return 0;
	}

#if defined(OS_POSIX)
	int status;

	// Bloqueamos al llamante y comprobamos el estado del proceso
	child_id = waitpid(m_proc_id, &status, 0);
	// Comprobamos si se produjo un error en wait
	//if(l_child_id == -1)
	//	return -1;

	// Comprobamos la terminación del proceso
	if (WIFEXITED(status))
	{
		// El proceso terminó normalmente (llamada a exit o similares)
		LOG_DEBUG("Process " << m_proc_id << " terminated correctly");
		ret = 1;
	}
	else if (WIFSIGNALED(status))
	{
		// El proceso terminó por una señal
		LOG_DEBUG("Process " << m_proc_id << " terminated by a signal");
		ret = 2;
	}
	else
	{
		// El proceso terminó de alguna otra manera
		LOG_DEBUG("Process " << m_proc_id << " terminated by a unknown cause");
		ret = 3;
	}
#elif defined(OS_WIN)
	DWORD status;

	if (WaitForSingleObject((HANDLE)m_proc_id, INFINITE) != WAIT_OBJECT_0 || !GetExitCodeProcess((HANDLE)m_proc_id, &status))
	{
		// El proceso terminó de alguna otra manera
		LOG_DEBUG("Process " << m_proc_id << " terminated by a unknown cause");
		ret = 3;
	}
	else
	{
		// El proceso terminó normalmente (llamada a exit o similares)
		LOG_DEBUG("Process " << m_proc_id << " terminated correctly");
		ret = 1;
	}
#endif
	// Hay que llamar a está función para liberar los recursos (En linux no hace nada)
	Glib::spawn_close_pid(m_proc_id);

	// Reiniciamos el id como no en ejecución
#if defined(OS_POSIX)
	m_proc_id = -1;
#elif defined(OS_WIN)
	m_proc_id = NULL;
#endif

	return ret;
}

int Process::stop(void)
{
	int ret;

	// Comprobamos si el proceso está en ejecución
#if defined(OS_POSIX)
	if (m_proc_id == -1)
#elif defined(OS_WIN)
	if (m_proc_id == NULL)
#endif
	{
		return 0;
	}

#if defined(OS_POSIX)
	// Mandamos al proceso la señar de terminación
	kill(m_proc_id, SIGTERM);
#elif defined(OS_WIN)
	TerminateProcess(m_proc_id, 0);
//	CloseHandle(m_proc_id);
#endif
	// Una vez enviada la señal, esperamos a que termine
	ret = this->wait();

	// Cerramos los canales de lectura
	m_channel_in->close();
	m_channel_out->close();
	m_channel_err->close();

	// Desconectamos la señal del watch
	m_connection.disconnect();

	return ret;
}

bool Process::isRunning(void)
{
	int ret;

#if defined(OS_POSIX)
	if (m_proc_id == -1)
#elif defined(OS_WIN)
	if (m_proc_id == NULL)
#endif
	{
		return false;
	}

#if defined(OS_POSIX)
	// Hacemos un wait sin bloqueo para comprobar el estado del proceso
	ret = waitpid(m_proc_id, NULL, WNOHANG);
#elif defined(OS_WIN)
	DWORD status;

	if (WaitForSingleObject((HANDLE)m_proc_id, 0) != WAIT_TIMEOUT || !GetExitCodeProcess((HANDLE)m_proc_id, &status))
	{
		ret = true;
	}
	else
	{
		if (status == STILL_ACTIVE){
			ret = false;
		}
		else{
			ret = true;
		}
	}
#endif

	// Comprobamos si continua en ejecución
	return !ret;
}

Glib::Pid Process::getId(void)
{
	return m_proc_id;
}

Glib::RefPtr<Glib::IOChannel> Process::getInChannel(void)
{
	return m_channel_in;
}

Glib::RefPtr<Glib::IOChannel> Process::getOutChannel(void)
{
	return m_channel_out;
}

Glib::RefPtr<Glib::IOChannel> Process::getErrChannel(void)
{
	return m_channel_err;
}

sigc::signal<void, int> Process::signalExit(void)
{
	return m_signal_exit;
}

void Process::onProcessExit(Glib::Pid id, int priority)
{
	int ret;

	// Cuando el proceso finalice, emitimos la señal correspondiente
	ret = this->wait();
	m_signal_exit.emit(ret);
}
