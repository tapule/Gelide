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

#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <glibmm/ustring.h>
#include <glibmm/iochannel.h>
#include <glibmm/spawn.h>
#include <sigc++/sigc++.h>


/**
 * Gestiona la ejecución y el control de un proceso lanzado mediante un comando.
 *
 * @note La ejecución de un comando no bloquea al llamante a menos que se llame
 * a wait().
 */
class Process
{
public:
	/**
	 * Constructor de la clase
	 */
	Process(void);

	/**
	 * Destructor
	 */
	virtual ~Process();

	/**
	 * Lanza el proceso que ejecutará un comando dado en un path determinado
	 * @param command Comando que debe ejecutar el proceso
	 * @param path Path donde se lanzará el proceso (working directory)
	 */
	void run(const Glib::ustring& command, const Glib::ustring& path);

	/**
	 * Espera a que el proceso termine su tarea bloqueando al llamante
	 * @return Código de finalización del proceso:
	 * 	-1: Se produjo en error en la llamada
	 * 	 0: El proceso no estaba en ejecución.
	 * 	 1: El proceso finalizó normalmente
	 * 	 2: El proceso finalizó por una señal
	 * 	 3: El proceso finalizó por una causa desconocida
	 */
	int wait(void);

	/**
	 * Fuerza la terminación del proceso
	 * @return Código de finalización del proceso como en wait
	 */
	int stop(void);

	/**
	 * Indica si el proceso está en ejecución o no.
	 * @return
	 */
	bool isRunning(void);

	/**
	 * Devuelve el Identificador de proceso actual
	 * @return
	 */
	Glib::Pid getId(void);

	/**
	 * Devuelve el canal de escritura (entrada) del proceso actual
	 * @return Canal de escritura
	 */
	Glib::RefPtr<Glib::IOChannel> getInChannel(void);

	/**
	 * Devuelve el canal de lectura (salida) del proceso actual
	 * @return Canal de lectura
	 */
	Glib::RefPtr<Glib::IOChannel> getOutChannel(void);

	/**
	 * Devuelve el canal de error del proceso actual
	 * @return Canal de salida de error
	 */
	Glib::RefPtr<Glib::IOChannel> getErrChannel(void);

	/**
	 * Señal emitida por el proceso cuando termina su tarea
	 * @return Señal emitida
	 */
	sigc::signal<void, int> signalExit(void);

private:
	Glib::Pid m_proc_id;							/**< Id del proceso */
	Glib::RefPtr<Glib::IOChannel> m_channel_in;		/**< Canal de entrada al proceso */
	Glib::RefPtr<Glib::IOChannel> m_channel_out;	/**< Canal donde escribe el proceso como salida */
	Glib::RefPtr<Glib::IOChannel> m_channel_err;	/**< Canal donde escribe el proceso como error */

	sigc::connection m_connection;
	sigc::signal<void, int> m_signal_exit;			/**< Señal emitida por el proceso al terminar su tarea */

	/**
	 * Callback que utilizará el proceso para informar de su terminación
	 * @param p_pid Identificador de proceso
	 * @param p_priority Prioridad del proceso
	 */
	void onProcessExit(Glib::Pid id, int priority);
};

#endif // _PROCESS_HPP_
