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

#ifndef _TREEVIEW_FILTERS_HPP_
#define _TREEVIEW_FILTERS_HPP_

#include "../gelide.hpp"
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <sigc++/sigc++.h>
#include "../config.hpp"
#include "../interface_configurable.hpp"
#include "../core/system.hpp"

class CTreeViewFilters: public Gtk::TreeView, public IConfigurable
{
public:
	CTreeViewFilters(void);
	virtual ~CTreeViewFilters();

	// Obtener y establecer el filtro en el treeview
	void getFilter(int* p_type, unsigned int* p_value);

	// Señales emitida por el treeview al cambiar de filtro
	sigc::signal<void, int, unsigned int> signalFilterChanged(void);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);
private:
	// Instancias del configurador y de los gestores
	CConfig* m_config;

	Gtk::TreeModel::iterator m_systems;

	// Señal emitida por el treeview
	sigc::signal<void, int, unsigned int > m_signal_filter_changed;

	// Modelo del listado
	Glib::RefPtr<Gtk::TreeStore> m_tree;
	//Columnas del modelo de los sistemas:
	class CTreeColumns:public Gtk::TreeModel::ColumnRecord
	{
	public:
		CTreeColumns(void){
			add(m_icon);
			add(m_type);
			add(m_value);
			add(m_name);
		}
		Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> >  m_icon;
		Gtk::TreeModelColumn<int> m_type;
		Gtk::TreeModelColumn<unsigned int> m_value;
		Gtk::TreeModelColumn<Glib::ustring> m_name;
	};
	CTreeColumns m_columns;

	// Callback para el cambio de filtros
	void onFilterChanged(void);

	// Añade un filtro al árbol
	Gtk::TreeModel::iterator addFilter(const int p_type, const unsigned int p_value,
			Glib::RefPtr<Gdk::Pixbuf> p_pixbuf,	Glib::ustring p_name);
	// Añade un filtro a un contenedor de filtros
	void addChildFilter(Gtk::TreeModel::iterator p_iter, const int p_type,
			const unsigned int p_value, Glib::RefPtr<Gdk::Pixbuf> p_pixbuf,
			Glib::ustring p_name);
};

#endif // _TREEVIEW_FILTERS_HPP_
