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

#ifndef _SYSTEM_MANAGER_HPP_
#define _SYSTEM_MANAGER_HPP_

#include "../gelide.hpp"
#include <glibmm/ustring.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/liststore.h>
#include <vector>
#include "../config.hpp"
#include "../ui/model_columns.hpp"
#include "system.hpp"

#define SYSTEM_MANAGER_ID	"system_manger"

class CSystem;

// Contenedor para los systemas
class CSystemManager
{
public:

	/**
	 * Obtiene la instancia única de la clase que se define como un singleton
	 * @return Instancia única de la clase
	 */
	static CSystemManager* getInstance(void){
		static CSystemManager m_instance;
		return &m_instance;
	}

	Glib::RefPtr<Gdk::Pixbuf> getSystemDefaultIcon(void);
	Glib::RefPtr<Gdk::Pixbuf> getEmulatorDefaultIcon(void);
	// Cargar sistemas desde un archivo
	bool load(void);
	// Guardar sistemas a un archivo
	bool save(void);
	// Importar sistemas
	bool importSystems(const Glib::ustring& p_file);
	// Exportar sistemas
	bool exportSystems(const Glib::ustring& p_file);
	// Exportar el listado de juegos completo
	bool exportGameList(const Glib::ustring& p_file);

	// Obtener la lista de sistemas
	// CHECKME: Creo que esto no es necesario para nada.
	Glib::RefPtr<Gtk::ListStore>& getSystemList(void);
	std::vector<CSystem*>& getSystems(void);

	// Gestion de sistemas
	Gtk::TreeModel::iterator getSystem(const unsigned int p_id);
	CSystem* getSystem(Gtk::TreeModel::iterator& p_iter);
	Gtk::TreeModel::iterator addSystem(CSystem& p_system);
	void removeSystem(Gtk::TreeModel::iterator& p_iter);
	void updateSystem(Gtk::TreeModel::iterator& p_iter, CSystem& p_system);
	void moveSystemFirst(Gtk::TreeModel::iterator& p_iter);
	void moveSystemUp(Gtk::TreeModel::iterator& p_iter);
	void moveSystemDown(Gtk::TreeModel::iterator& p_iter);
	void moveSystemLast(Gtk::TreeModel::iterator& p_iter);

	// Obtener el número total de juegos
	unsigned int getGames(void);

	// Señales emitidas por el manager
	// CHECKME: Creo que esto ya no es necesario
	/* TODO: Puede ser interesante incluir una señal para indicar la carga y
	 * descarga de un sistema determinado, así se puede tener constancia */
	/*sigc::signal<void, CSystem*> signalAddSystem(void);
	sigc::signal<void, CSystem*> signalRemoveSystem(void);
	sigc::signal<void, CSystem*> signalUpdateSystem(void);
	sigc::signal<void, CSystem*> signalActivateSystem(void);
	sigc::signal<void, CSystem*> signalDeactivateSystem(void);
	sigc::signal<void, CSystem*> signalCreateGameList(void);*/
protected:
	CSystemManager(void);
	~CSystemManager(void);

private:
	// Instancias del configurador
	CConfig* m_config;

	std::vector<CSystem*> m_systems;			// Lista de sistemas reales

	// Modelo del listado de sistemas
	Glib::RefPtr<Gtk::ListStore> m_system_list;
	CSystemModelColumns m_system_columns;


	Glib::RefPtr<Gdk::Pixbuf> m_system_icon;	// Icono por defecto para sistemas
	Glib::RefPtr<Gdk::Pixbuf> m_emulator_icon;	// Icono por defecto para emuladores

	// Señales emitidas por el manager
	/*sigc::signal<void, CSystem* > m_signal_add;
	sigc::signal<void, CSystem* > m_signal_remove;
	sigc::signal<void, CSystem* > m_signal_update;
	sigc::signal<void, CSystem* > m_signal_activate;
	sigc::signal<void, CSystem* > m_signal_deactivate;
	sigc::signal<void, CSystem* > m_signal_create_game_list;*/

	// Genera la lista de sistemas por defecto
	// Esto es util para poder traducir los sistemas por defecto
	void generateDefaultSystems(void);
	bool checkSystemDirs(CSystem& p_system);
};

#endif // _SYSTEM_MANAGER_HPP_
