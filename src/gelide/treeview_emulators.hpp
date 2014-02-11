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

#ifndef _TREEVIEW_EMULATORS_HPP_
#define _TREEVIEW_EMULATORS_HPP_

#include "../gelide.hpp"
#include <gtkmm/treeview.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/uimanager.h>
#include <sigc++/sigc++.h>
#include "../config.hpp"
#include "../core/system_manager.hpp"
#include "../core/system.hpp"
#include "model_columns.hpp"

class CTreeViewEmulators: public Gtk::TreeView
{
public:
	CTreeViewEmulators(void);
	virtual ~CTreeViewEmulators(){};

	void setSystem(Gtk::TreeModel::iterator& p_system);

	// Señal emitida para iniciar el cambio
	sigc::signal<void> signalChanged(void);

private:
	CSystemManager* m_smanager;
	CSystem* m_system;
	CEmulatorModelColumns m_columns;

	// Señal emitida para iniciar el juego
	sigc::signal<void> m_signal_changed;

	Glib::RefPtr<Gtk::UIManager> m_ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> m_action_group;

	Glib::RefPtr<Gtk::Action> m_action_add;
	Glib::RefPtr<Gtk::Action> m_action_remove;
	Glib::RefPtr<Gtk::Action> m_action_edit;
	Glib::RefPtr<Gtk::Action> m_action_about;

	Glib::RefPtr<Gtk::Action> m_action_move;
	Glib::RefPtr<Gtk::Action> m_action_first;
	Glib::RefPtr<Gtk::Action> m_action_up;
	Glib::RefPtr<Gtk::Action> m_action_down;
	Glib::RefPtr<Gtk::Action> m_action_last;

	Gtk::Menu* m_popup_menu;

	void initActions();

	void onActionAdd(void);
	void onActionRemove(void);
	void onActionEdit(void);
	void onActionAbout(void);

	void onActionMoveFirst(void);
	void onActionMoveUp(void);
	void onActionMoveDown(void);
	void onActionMoveLast(void);

	void onSystemCellData(Gtk::CellRenderer* p_renderer, const Gtk::TreeModel::iterator& p_iter);
	void onEmulatorChanged(void);
	bool on_button_press_event(GdkEventButton* event);
};


#endif // _TREEVIEW_FILTERS_HPP_
