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

#ifndef _TREEVIEW_GAMES_HPP_
#define _TREEVIEW_GAMES_HPP_

#include "../gelide.hpp"
#include <gtkmm/liststore.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treemodelsort.h>
#include <gtkmm/treeview.h>
#include <gtkmm/toggleaction.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/uimanager.h>
#include <glibmm/regex.h>
#include <sigc++/connection.h>
#include <sigc++/sigc++.h>
#include "../config.hpp"
#include "../interface_configurable.hpp"
#include "../core/system_manager.hpp"
#include "model_columns.hpp"

#define TREEVIEW_COLUMNS					7
#define TREEVIEW_FAVORITE_COLUMN_ID			0
#define TREEVIEW_NAME_COLUMN_ID				1
#define TREEVIEW_FLAGS_COLUMN_ID			2
#define TREEVIEW_RANK_COLUMN_ID				3
#define TREEVIEW_YEAR_COLUMN_ID				4
#define TREEVIEW_DESCRIPTION_COLUMN_ID		5
#define TREEVIEW_MANUFACTURER_COLUMN_ID		6

class CTreeViewGames: public Gtk::TreeView, public IConfigurable
{
public:
	// Constructor y destructor
	CTreeViewGames(void);
	virtual ~CTreeViewGames();

	void setSystem(Gtk::TreeModel::iterator& p_system);
	Gtk::TreeModel::iterator getSelected(void);

	// Obtiene el número de juegos visibles
	unsigned int getGames(void);

	// Establecer el filtro del listado
	void setFilter(const int p_type, const unsigned int p_value);
	void setNameFilter(Glib::ustring p_str);

	// Ejecutar el juego actual
	void playGame(void);

	// Alternar flags y favorito
	void setFavorite(bool p_favorite);
	void setPlayed(bool p_played);
	void setWorking(bool p_working);
	// Establecer la puntuación
	void setRank(const int p_rank);


	// Señal emitida para iniciar el cambio
	sigc::signal<void> signalChanged(void);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);
private:
	// Instancias del configurador y del gestor de sistemas
	CConfig* m_config;
	CSystemManager* m_smanager;
	// Sistema gestionado
	CSystem* m_system;

	// Array para asociar el id de columna a su puntero
	Gtk::TreeViewColumn* m_columns_id[TREEVIEW_COLUMNS];

	// Señal emitida para iniciar el juego
	sigc::signal<void> m_signal_changed;

	// Columna de filtrado en el listado real y valor del filtro
	int m_filter_type;
	unsigned int m_filter_value;
	//Glib::ustring m_filter_str;
	Glib::RefPtr<Glib::Regex> m_filter_regex;
	bool m_apply_filter_regex;

	// Columna de ordenación y el tipo de orden
	int m_sort_column;
	Gtk::SortType m_sort_type;


	// Iconos para los Flags
	Glib::RefPtr<Gdk::Pixbuf> m_flag_favorite[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_played[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_working[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_available[2];
	Glib::RefPtr<Gdk::Pixbuf> m_flag_unknown[2];

	// Modelos del listado y columnas
	Glib::RefPtr<Gtk::TreeModelFilter> m_filter_list;
	Glib::RefPtr<Gtk::TreeModelSort> m_sort_list;
	CGameModelColumns m_columns;

	// Manager y Actiongroup para el popup del listado
	// TODO: Crear un popup para gestionar las columnas
	Glib::RefPtr<Gtk::UIManager> m_ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> m_action_group;
	// Acciones para el popup
	Glib::RefPtr<Gtk::Action> m_action_playgame;
	Glib::RefPtr<Gtk::ToggleAction> m_action_favorite;
	Glib::RefPtr<Gtk::ToggleAction> m_action_played;
	Glib::RefPtr<Gtk::ToggleAction> m_action_working;
	Glib::RefPtr<Gtk::Action> m_action_rank;
	Glib::RefPtr<Gtk::RadioAction> m_action_ranks[6];
	Glib::RefPtr<Gtk::Action> m_action_remove;
	// Conexiones para las acciones
	sigc::connection m_conn_favorite;
	sigc::connection m_conn_played;
	sigc::connection m_conn_working;
	sigc::connection m_conn_ranks[6];
	// Popup del listado
	Gtk::Menu* m_gamelist_popup;

	// Callback para presentar el icono en la columna favoritos
	void renderFavorite(Gtk::CellRenderer *p_renderer, const Gtk::TreeModel::iterator& p_iter);
	// Callback para presentar los iconos de la columna flags
	void renderFlag(Gtk::CellRenderer *p_renderer, const Gtk::TreeModel::iterator& p_iter,
			int p_mask, Glib::RefPtr<Gdk::Pixbuf> p_active,
			Glib::RefPtr<Gdk::Pixbuf> p_inactive);
	// Callback para renderizar la puntuación
	void renderYearCell(Gtk::CellRenderer* p_renderer,	const Gtk::TreeModel::iterator& p_iter);

	// Inicializa los iconos de los flags
	void initFlagsIcons(void);
	// Callback para filtrado de la lista
	bool visibleFunction(const Gtk::TreeModel::const_iterator& p_iter);


	// Reimplementación del button press para presentar el popup
	bool on_button_press_event(GdkEventButton* p_event);

	// Método para inicializar las acciones del popup
	void initActions(void);
	// Métodos para bloquear y desbloquear las señales
	// CHECKME: ¿No sería mejor acciones dobles y presentar las correspondientes?
	void blockSignals(void);
	void unblockSignals(void);

	void onGameChanged(void);

	void onActionPlaygame(void);
	void onActionFavorite(void);
	void onActionPlayed(void);
	void onActionWorking(void);
	void onActionRank(const int p_rank);
	void onActionRemove(void);

};

#endif // _TREEVIEW_GAMES_HPP_
