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

#ifndef _GELIDE_UI_HPP_
#define _GELIDE_UI_HPP_

#include "../gelide.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/window.h>
#include <gtkmm/menubar.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/paned.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/stock.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/liststore.h>
#include <gtkmm/combobox.h>
#include <sigc++/sigc++.h>

#include "../interface_configurable.hpp"
#include "../config.hpp"
#include "../core/system_manager.hpp"
#include "treeview_filters.hpp"
#include "treeview_games.hpp"
#include "treeview_systems.hpp"
#include "treeview_emulators.hpp"
#include "info_pannel.hpp"
#include "statusbar.hpp"
#include "dialog_preferences.hpp"
#include "dialog_columns.hpp"
#include "../utils/process.hpp"


class CGelideUI: public Gtk::Window, public IConfigurable
{
public:

	/**
	 * Obtiene la instancia única de la clase que se define como un singleton
	 * @return Instancia única de la clase
	 */
	static CGelideUI* getInstance(void){
		static CGelideUI m_instance;
		return &m_instance;
	}

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);

protected:
	CGelideUI(void);
	virtual ~CGelideUI(void);

	bool on_delete_event(GdkEventAny *p_event);

private:

	// Instancias del configurador y de los gestores
	CConfig* m_config;
	CSystemManager* m_smanager;

	int m_browser_mode;

	// Cuerpo principal
	Glib::RefPtr<Gtk::UIManager> m_ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> m_action_group;
	// Acciones modificables al cambiar un juego
	Glib::RefPtr<Gtk::ToggleAction> m_action_favorite;
	Glib::RefPtr<Gtk::ToggleAction> m_action_played;
	Glib::RefPtr<Gtk::ToggleAction> m_action_working;
	Glib::RefPtr<Gtk::Action> m_action_rank;
	Glib::RefPtr<Gtk::RadioAction> m_action_ranks[6];

	// Conexiones para las acciones
	sigc::connection m_conn_favorite;
	sigc::connection m_conn_played;
	sigc::connection m_conn_working;
	sigc::connection m_conn_ranks[6];

	Gtk::MenuBar* m_menubar;
	Gtk::Toolbar* m_toolbar;
	Gtk::VBox m_vbox_main;
	Gtk::HPaned m_hpaned_filters;
	Gtk::HPaned m_hpaned_info;
	Gtk::VBox m_vbox_center;
	Gtk::HBox m_hbox_search;
	Gtk::HPaned m_hpaned_lbrowser;
	Gtk::VPaned m_vpaned_browser;
	Gtk::VPaned m_vpaned_games;
	Gtk::HPaned m_hpaned_browser;
	CStatusBar m_statusbar;

	// Árbol de filtros
	Gtk::VBox m_vbox_filters;
	Gtk::ScrolledWindow m_scroll_filters;
	CTreeViewFilters m_treeview_filters;

	// Buscador
	Gtk::Label m_label_gamelibrary;
	Gtk::Label m_label_search;
	Gtk::Entry m_entry_search;
	Gtk::Button m_button_clear;
	Gtk::Image m_image_clear;

	// Examinador
	Gtk::ScrolledWindow m_scroll_systems;
	CTreeViewSystems m_treeview_systems;
	Gtk::ScrolledWindow m_scroll_emulators;
	CTreeViewEmulators m_treeview_emulators;

	// Árbol de juegos
	Gtk::ScrolledWindow m_scroll_games;
	CTreeViewGames	m_treeview_games;

	// Panel de imagenes
	CInfoPannel m_info_pannel;

	// Métodos de inicialización
	void initActions(void);
	void initMenubar(void);
	void initToolbar(void);
	void initBody(void);
	void initFilters(void);
	void initSearch(void);
	void initBrowser(void);
	void initGames(void);
	void initInfo(void);
	void initStatusbar(void);

	// Callbacks para responder al buscador
	void onSearchChanged(void);
	void onSearchClear(void);

	// Callbacks para responder a los cambios en todos los listados
	void onFilterChanged(int p_type, unsigned int p_value);
	void onSystemChanged(void);
	void onEmulatorChanged(void);
	void onGameChanged(void);

	// Acciones del menú Archivo
	void onActionFilePlayGame(void);
	void onActionFileExportList(void);
	void onActionFileExportSystems(void);
	void onActionFileImportSystems(void);
	void onActionFileProperties(void);
	void onActionFileQuit(void);

	// Acciones del menú Edición
	void onActionEditFavorite(void);
	void onActionEditPlayed(void);
	void onActionEditWorking(void);
	void onActionEditRank(const int p_rank);
	void onActionEditColumns(void);
	void onActionEditPreferences(void);

	// Acciones del menú Ver
	void onActionViewFullscreen(void);
	void onActionViewToolsBar(void);
	void onActionViewStatusBar(void);
	void onActionViewFilters(void);
	void onActionViewBrowser(void);
	void onActionViewTopBrowser(void);
	void onActionViewLeftBrowser(void);
	void onActionViewInfo(void);

	// Acciones del menu Ayuda
	void onActionHelpContents(void);
	void onActionHelpChangeLog(void);
	void onActionHelpAbout(void);

	// Método para actualizar la barra de estado
	void updateStatusBar(void);

	// Métodos para bloquear y desbloquear las señales modificables
	void blockSignals(void);
	void unblockSignals(void);
};

#endif // _GELIDE_UI_HPP_
