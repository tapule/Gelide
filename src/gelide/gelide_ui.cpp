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

#include <gtkmm/toggleaction.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/icontheme.h>
#include "gelide_ui.hpp"
#include "dialog_emulator_launcher.hpp"

CGelideUI::CGelideUI(void):
	Gtk::Window(Gtk::WINDOW_TOPLEVEL),
	m_browser_mode(0)
{
	// Obtenermos las instancias
	m_config = CConfig::getInstance();
	m_smanager = CSystemManager::getInstance();

	// Inicialización de la interfaz
	this->set_title("Gelide");
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	//this->set_default_size( GELIDE_WIDTH, GELIDE_HEIGHT );
	this->set_default_size(GELIDE_WIDTH, GELIDE_HEIGHT );
	try
	{
		this->set_icon_from_file( GELIDE_ICON );
		Gtk::Window::set_default_icon(this->get_icon());
	}
	catch ( Glib::Exception & l_exception )
	{
		GELIDE_WARNING("Loading Gelide icon (" << l_exception.what().c_str() << ")");
	}

	// Inicializamos las acciones del menú y barra de herramientas
	initActions();
	// Barra de menús.
	initMenubar();
	// Barra de herramientas
	initToolbar();
	// Cuerpo principal
	initBody();
	// Inicializa el panel de filtros
	initFilters();

	// Iniciamos el panel de busquedas
	initSearch();
	// Barra de estado
	initStatusbar();

	// Añadimos la caja principal
	this->add(m_vbox_main);
	this->show_all_children();

	// Cargamos los sistemas y emuladores
	m_smanager->load();

	// Inicializa el panel de filtros
	initBrowser();
	// Inicializamos el panel de juegos
	initGames();
	// Inicializa el panel de información
	initInfo();

	this->show_all_children();

	// Establecemos el diálogo por defecto en el centro del padre
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	// Cargamos la configuración
	loadConfig();

	updateStatusBar();
}

CGelideUI::~CGelideUI(){
	//m_smanager->save();
	//saveConfig();
}

void CGelideUI::loadConfig(void){
	int l_x, l_y;
	bool l_val;
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;
	Glib::RefPtr<Gtk::RadioAction> l_radioaction;
	Gtk::TreeModel::iterator l_iter;
	Glib::ustring l_str;

	GELIDE_DEBUG("Loading configuration...");

	// Cargamos el modo fullscreen
	m_config->getKey(GELIDE_CFG_UI, "fullscreen_mode", l_val);
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("FullScreen"));
	l_toggleaction->set_active(l_val);

	if(!l_val){
		// Obtenemos las dimensiones almacenadas
		m_config->getKey(GELIDE_CFG_UI, "ui_width", l_x);
		m_config->getKey(GELIDE_CFG_UI, "ui_height", l_y);
		// Comprobamos que son válidas
		if((l_x!= -1) && (l_y != -1))
			this->set_default_size(l_x, l_y);

		// Obtenemos la posición almacenada
		m_config->getKey(GELIDE_CFG_UI, "ui_x", l_x);
		m_config->getKey(GELIDE_CFG_UI, "ui_y", l_y);
		if((l_x!= -1) && (l_y != -1))
			this->move(l_x, l_y);
	}
	// Obtenemos el estado de la barra de herramientas
	m_config->getKey(GELIDE_CFG_UI, "tool_bar_visible", l_val);
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("ToolsBar"));
	l_toggleaction->set_active(l_val);

	// Obtenemos las propiedades del árbol de filtros
	m_config->getKey(GELIDE_CFG_UI, "filters_visible", l_val);
	m_config->getKey(GELIDE_CFG_UI, "filters_width", l_x);
	if(l_x!= -1)
		m_hpaned_filters.set_position(l_x);
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Filters"));
	l_toggleaction->set_active(l_val);

	// Obtenemos las dimensiones de los examinadores (izquierdo y superior)
	m_config->getKey(GELIDE_CFG_UI, "browser_left_width", l_x);
	m_config->getKey(GELIDE_CFG_UI, "browser_left_height", l_y);
	if((l_x!= -1) && (l_y != -1)){
		m_hpaned_lbrowser.set_position(l_x);
		m_vpaned_browser.set_position(l_y);
	}
	m_config->getKey(GELIDE_CFG_UI, "browser_top_width", l_x);
	m_config->getKey(GELIDE_CFG_UI, "browser_top_height", l_y);
	if((l_x!= -1) && (l_y != -1)){
		m_hpaned_browser.set_position(l_x);
		m_vpaned_games.set_position(l_y);
	}
	// Obtenemos el modo de examinador
	m_config->getKey(GELIDE_CFG_UI, "browser_mode", m_browser_mode);
	if(m_browser_mode < 0)
		m_browser_mode = 0;
	// Activamos el examinador superior
	if(m_browser_mode){
		// Establecemos el modo contrario para que se produzca el cambio de modo
		m_browser_mode = 0;
		l_radioaction = Glib::RefPtr< Gtk::RadioAction >::cast_dynamic( m_action_group->get_action("TopBrowser"));
		l_radioaction->activate();
	}
	else
		// El examinador izquierdo es el examinador por defecto, así que no activamos nada
		m_hpaned_browser.hide();
	// Obtenemos la visibilidad del browser
	m_config->getKey(GELIDE_CFG_UI, "browser_visible", l_val);
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Browser"));
	l_toggleaction->set_active(l_val);

	// Cargamos el estado del panel de información
	m_config->getKey(GELIDE_CFG_UI, "info_pannel_widht", l_x);
	if(l_x!= -1)
		m_hpaned_info.set_position(l_x);
	m_config->getKey(GELIDE_CFG_UI, "info_pannel_visible", l_val);
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Info"));
	l_toggleaction->set_active(l_val);

	// Obtenemos el estado de la barra de estado
	m_config->getKey(GELIDE_CFG_UI, "status_bar_visible", l_val);
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("StatusBar"));
	l_toggleaction->set_active(l_val);

	// Cargamos el sistema seleccionado
	m_config->getKey(GELIDE_CFG_UI, "selected_system", l_x);
	if(l_x!= -1){
		l_iter = m_smanager->getSystem(l_x);
		if(l_iter)
			m_treeview_systems.setSelected(l_iter);
	}
}

void CGelideUI::saveConfig(void){
	int l_x, l_y;
	unsigned int l_z;
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;
	Gtk::TreeModel::iterator l_iter;

	GELIDE_DEBUG("Saving configuration...");
	// Guardamos el modo fullscreen
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("FullScreen"));
	m_config->setKey(GELIDE_CFG_UI, "fullscreen_mode", l_toggleaction->get_active());

	// Guardamos las dimensiones y posición de la ventana principal si no
	// estamos en modo fullscreen
	if(!l_toggleaction->get_active()){
		// Dimensiones de la ventana
		this->get_size(l_x, l_y);
		m_config->setKey(GELIDE_CFG_UI, "ui_width", l_x);
		m_config->setKey(GELIDE_CFG_UI, "ui_height", l_y);

		// Posición la posición de la ventana
		this->get_position(l_x, l_y);
		m_config->setKey(GELIDE_CFG_UI, "ui_x", l_x);
		m_config->setKey(GELIDE_CFG_UI, "ui_y", l_y);
	}

	// Guardamos el estado de la barra de herramientas
	m_config->setKey(GELIDE_CFG_UI, "tool_bar_visible", m_toolbar->is_visible());

	// Guardamos el estado de los filtros
	m_config->setKey(GELIDE_CFG_UI, "filters_visible", m_vbox_filters.is_visible());
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Filters"));
	if(l_toggleaction->get_active())
		m_config->setKey(GELIDE_CFG_UI, "filters_width", m_hpaned_filters.get_position());

	// Guardamos el estado del browser
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Browser"));
	if(l_toggleaction->get_active()){
		if(m_browser_mode){
			// Visible el examinador superior guardamos su estado
			m_config->setKey(GELIDE_CFG_UI, "browser_top_width", m_hpaned_browser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_top_height", m_vpaned_games.get_position());
		}
		else{
			// Visible el examinador izquierdo guardamos su estado
			m_config->setKey(GELIDE_CFG_UI, "browser_left_width", m_hpaned_lbrowser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_left_height", m_vpaned_browser.get_position());
		}
	}
	m_config->setKey(GELIDE_CFG_UI, "browser_visible", l_toggleaction->get_active());
	m_config->setKey(GELIDE_CFG_UI, "browser_mode", m_browser_mode);

	// Guardamos el estado del panel de información
	m_config->setKey(GELIDE_CFG_UI, "info_pannel_visible", m_info_pannel.is_visible());
	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Info"));
	if(l_toggleaction->get_active())
		m_config->setKey(GELIDE_CFG_UI, "info_pannel_widht", m_hpaned_info.get_position());

	// Guardamos el estado de la barra de estado
	m_config->setKey(GELIDE_CFG_UI, "status_bar_visible", m_statusbar.is_visible());

	// Guardamos el sistema seleccionado
	l_iter = m_treeview_systems.getSelected();
	l_z = 0;
	if(l_iter)
		l_z = m_smanager->getSystem(l_iter)->getId();
	m_config->setKey(GELIDE_CFG_UI, "selected_system", l_z);
}

bool CGelideUI::on_delete_event(GdkEventAny *p_event){
	// Llamamos a File->Quit para que realice las tareas adecuadas
	onActionFileQuit();
	return false;
}


void CGelideUI::initActions(void){

	// Creamos las acciones
	m_action_group = Gtk::ActionGroup::create();

	// Menu file
	//TODO: Debería añadir aceleradores de teclado "<control>q" - Use <control>, <shift>, <alt> and <release>. Use F1, F2, etc, for function keys.
	//accel_path 	For instance, "<MainWindow>/File/Open"
	m_action_group->add(Gtk::Action::create("MenuFile", _("_File")));
	m_action_group->add(Gtk::Action::create("PlayGame",
		Gtk::Stock::MEDIA_PLAY,	_("_Launch game"), _("Launch selected game")),
		Gtk::AccelKey("Return"),
		sigc::mem_fun(*this, &CGelideUI::onActionFilePlayGame) );
	m_action_group->get_action("PlayGame")->set_sensitive(false);

	m_action_group->add(Gtk::Action::create("ExportList",
		Gtk::StockID("gelide-export-list"),	_("Export l_ist...")),
		sigc::mem_fun(*this, &CGelideUI::onActionFileExportList));

	m_action_group->add(Gtk::Action::create("ExportSystems",
		Gtk::StockID("gelide-export-systems"), _("Export s_ystems...")),
		sigc::mem_fun(*this, &CGelideUI::onActionFileExportSystems));

	m_action_group->add(Gtk::Action::create("ImportSystems",
		Gtk::StockID("gelide-import-systems"), _("Import _systems...")),
		sigc::mem_fun(*this, &CGelideUI::onActionFileImportSystems));

	m_action_group->add(Gtk::Action::create("Properties",
		Gtk::Stock::PROPERTIES,	_("_Properties..."),
		_("Show selected item properties.")),
		sigc::mem_fun(*this, &CGelideUI::onActionFileProperties) );
	m_action_group->get_action("Properties")->set_sensitive(false);
	//TODO: Aquí debería ir los tres juegos recientes,
	m_action_group->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
			sigc::mem_fun(*this, &CGelideUI::onActionFileQuit));

	// Menu edit
	m_action_group->add(Gtk::Action::create("MenuEdit", _("_Edit")));
	// Creamos las acciones modificables
	m_action_favorite = Gtk::ToggleAction::create("Favorite", _("_Favorite"));
	m_action_played = Gtk::ToggleAction::create("Played", _("_Played"));
	m_action_working = Gtk::ToggleAction::create("Working", _("_Working"));
	m_action_rank = Gtk::Action::create("MenuRank", _("_Rank"));
	// Creamos las acciones para el submenú rank
	Gtk::RadioAction::Group l_rank_group;
	m_action_ranks[0] = Gtk::RadioAction::create(l_rank_group, "Rank0", _("_0"));
	m_action_ranks[1] = Gtk::RadioAction::create(l_rank_group, "Rank1", _("_1"));
	m_action_ranks[2] = Gtk::RadioAction::create(l_rank_group, "Rank2", _("_2"));
	m_action_ranks[3] = Gtk::RadioAction::create(l_rank_group, "Rank3", _("_3"));
	m_action_ranks[4] = Gtk::RadioAction::create(l_rank_group, "Rank4", _("_4"));
	m_action_ranks[5] = Gtk::RadioAction::create(l_rank_group, "Rank5", _("_5"));
	// Conectamos las acciones y guardamos la conexión
	m_conn_favorite = m_action_favorite->signal_activate().connect(sigc::mem_fun(*this, &CGelideUI::onActionEditFavorite));
	m_conn_played = m_action_played->signal_activate().connect(sigc::mem_fun(*this, &CGelideUI::onActionEditPlayed));
	m_conn_working = m_action_working->signal_activate().connect(sigc::mem_fun(*this, &CGelideUI::onActionEditWorking));
	m_conn_ranks[0] = m_action_ranks[0]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CGelideUI::onActionEditRank), 0));
	m_conn_ranks[1] = m_action_ranks[1]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CGelideUI::onActionEditRank), 1));
	m_conn_ranks[2] = m_action_ranks[2]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CGelideUI::onActionEditRank), 2));
	m_conn_ranks[3] = m_action_ranks[3]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CGelideUI::onActionEditRank), 3));
	m_conn_ranks[4] = m_action_ranks[4]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CGelideUI::onActionEditRank), 4));
	m_conn_ranks[5] = m_action_ranks[5]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CGelideUI::onActionEditRank), 5));

	// Desactivamos inicialmente
	m_action_favorite->set_sensitive(false);
	m_action_played->set_sensitive(false);
	m_action_working->set_sensitive(false);
	m_action_rank->set_sensitive(false);
	// Insertamos las acciones en el grupo de acciones
	m_action_group->add(m_action_favorite);
	m_action_group->add(m_action_played);
	m_action_group->add(m_action_working);
	m_action_group->add(m_action_rank);
	m_action_group->add(m_action_ranks[0]);
	m_action_group->add(m_action_ranks[1]);
	m_action_group->add(m_action_ranks[2]);
	m_action_group->add(m_action_ranks[3]);
	m_action_group->add(m_action_ranks[4]);
	m_action_group->add(m_action_ranks[5]);

	m_action_group->add(Gtk::Action::create("Columns",
		Gtk::StockID("gelide-columns"),_("_Columns..."),
		_("Game list columns configuration")),
		Gtk::AccelKey("<control>c"),
		sigc::mem_fun(*this, &CGelideUI::onActionEditColumns));
	m_action_group->add(Gtk::Action::create("Preferences",
		Gtk::Stock::PREFERENCES, _("Pre_ferences..."),
		_("Gelide configuration")),
		sigc::mem_fun(*this, &CGelideUI::onActionEditPreferences) );

   // Menu view
	m_action_group->add(Gtk::Action::create("MenuView", _("_View")));
	m_action_group->add(Gtk::ToggleAction::create("FullScreen", Gtk::Stock::FULLSCREEN),
			sigc::mem_fun(*this, &CGelideUI::onActionViewFullscreen));
	m_action_group->add(Gtk::ToggleAction::create("ToolsBar",
		_("_Tool bar"), "", true),
		Gtk::AccelKey("F5"),
		sigc::mem_fun(*this, &CGelideUI::onActionViewToolsBar) );
	m_action_group->add(Gtk::ToggleAction::create("StatusBar",
		_("_Status bar"), "", true),
		Gtk::AccelKey("F6"),
		sigc::mem_fun(*this, &CGelideUI::onActionViewStatusBar) );
	m_action_group->add(Gtk::ToggleAction::create("Filters",
		Gtk::StockID("gelide-show-filters"),	_("_Filters"),
		_("Show / hide filters tree"), true),
		Gtk::AccelKey("F7"),
		sigc::mem_fun(*this, &CGelideUI::onActionViewFilters) );
	m_action_group->add(Gtk::ToggleAction::create("Browser",
		Gtk::StockID("gelide-show-browser"),_("_Browser"),
		_("Show / Hide systems browser"), true),
		Gtk::AccelKey("F8"),
		sigc::mem_fun(*this, &CGelideUI::onActionViewBrowser) );

	Gtk::RadioAction::Group l_browser_group;
	m_action_group->add(Gtk::RadioAction::create(l_browser_group, "LeftBrowser",
		Gtk::StockID("gelide-vertical-browser"), _("_Left browser")),
		Gtk::AccelKey("<control>F8"),
		sigc::mem_fun(*this, &CGelideUI::onActionViewLeftBrowser) );

	m_action_group->add(Gtk::RadioAction::create(l_browser_group, "TopBrowser",
		Gtk::StockID("gelide-horizontal-browser"), _("_Top browser")),
		Gtk::AccelKey("<shift>F8"),
		sigc::mem_fun(*this, &CGelideUI::onActionViewTopBrowser) );

	m_action_group->add(Gtk::ToggleAction::create("Info",
		Gtk::StockID("gelide-show-images"),_("_Information"),
		_("Show / hide info pannel"), true),
		Gtk::AccelKey("F9"),
		sigc::mem_fun(*this, &CGelideUI::onActionViewInfo) );


	// Menu tools de momento no activado
	m_action_group->add(Gtk::Action::create("MenuTools", _("_Tools")));

	// Menu help
	m_action_group->add(Gtk::Action::create("MenuHelp", _("_Help")));
	m_action_group->add(Gtk::Action::create("Contents", Gtk::Stock::HELP,
		_("_Contents")),
		sigc::mem_fun(*this, &CGelideUI::onActionHelpContents) );
	m_action_group->add(Gtk::Action::create("ChangeLog", Gtk::Stock::DIALOG_INFO,
		_("Change_Log")),
		sigc::mem_fun(*this, &CGelideUI::onActionHelpChangeLog) );
	m_action_group->add(Gtk::Action::create("About", Gtk::Stock::ABOUT,
		_("_About..."), _("About Gelide")),
		sigc::mem_fun(*this, &CGelideUI::onActionHelpAbout) );

	m_ui_manager = Gtk::UIManager::create();
	m_ui_manager->insert_action_group(m_action_group);
	add_accel_group(m_ui_manager->get_accel_group());

	Glib::ustring l_ui_info =
		"<ui>"
		"	<menubar name='MenuBar'>"
		"		<menu action='MenuFile'>"
		"			<menuitem action='PlayGame' />"
		"			<separator />"
		"			<menuitem action='ExportList' />"
		"			<separator />"
		"			<menuitem action='ExportSystems' />"
		"			<menuitem action='ImportSystems' />"
		"			<separator />"
		"			<menuitem action='Properties' />"
		"			<separator />"
		"			<menuitem action='Quit' />"
		"		</menu>"
		"		<menu action='MenuEdit'>"
		"			<menuitem action='Favorite' />"
		"			<menuitem action='Played' />"
		"			<menuitem action='Working' />"
		"			<separator />"
		"			<menu action='MenuRank'>"
		"				<menuitem action='Rank0' />"
		"				<menuitem action='Rank1' />"
		"				<menuitem action='Rank2' />"
		"				<menuitem action='Rank3' />"
		"				<menuitem action='Rank4' />"
		"				<menuitem action='Rank5' />"
		"			</menu>"
		"			<separator />"
		"			<menuitem action='Columns' />"
		"			<menuitem action='Preferences' />"
		"		</menu>"
		"		<menu action='MenuView'>"
		"			<menuitem action='FullScreen' />"
		"			<separator />"
		"			<menuitem action='ToolsBar' />"
		"			<menuitem action='StatusBar' />"
		"			<menuitem action='Filters' />"
		"			<menuitem action='Browser' />"
		"			<separator />"
		"			<menuitem action='TopBrowser' />"
		"			<menuitem action='LeftBrowser' />"
		"			<separator />"
		"			<menuitem action='Info' />"
		"		</menu>"
		"		<menu action='MenuHelp'>"
		"			<menuitem action='Contents' />"
		"			<menuitem action='ChangeLog' />"
		"			<separator />"
		"			<menuitem action='About' />"
		"		</menu>"
		"	</menubar>"

		"	<toolbar name='ToolBar'>"
		"		<toolitem action='PlayGame' />"
		"		<toolitem action='Properties' />"
		"		<separator />"
		"		<toolitem action='Filters' />"
		"		<toolitem action='Browser' />"
		"		<toolitem action='TopBrowser' />"
		"		<toolitem action='LeftBrowser' />"
		"		<toolitem action='Info' />"
		"		<toolitem action='FullScreen' />"
		"		<separator />"
		"		<toolitem action='Preferences' />"
		"		<toolitem action='Columns' />"
		"		<separator />"
		"		<toolitem action='About' />"
		"	</toolbar>"
		"</ui>";

	try{
		m_ui_manager->add_ui_from_string(l_ui_info);
	}
	catch (Glib::Error& l_exception)
	{
		GELIDE_ERROR("Generating menu (" << l_exception.what().c_str() << ")");
	}
}

void CGelideUI::initMenubar(void){
	// Obtenemos la barra de menú y la ponemos en su contenedor:
	m_menubar = (Gtk::MenuBar *) m_ui_manager->get_widget("/MenuBar");
	if(m_menubar)
		m_vbox_main.pack_start(*m_menubar, Gtk::PACK_SHRINK);
}

void CGelideUI::initToolbar(void){
	// Obtenemos la barra de herramientas y la ponemos en su contenedor:
	m_toolbar = (Gtk::Toolbar *) m_ui_manager->get_widget("/ToolBar");
	if(m_toolbar)
		m_vbox_main.pack_start(*m_toolbar, Gtk::PACK_SHRINK);
}

void CGelideUI::initBody(void){
	m_vbox_main.pack_start(m_hpaned_filters);
	m_hpaned_filters.set_position(175);
	m_hpaned_filters.pack1(m_vbox_filters, false, false);
	m_hpaned_filters.pack2(m_hpaned_info, false, false);
	m_hpaned_info.pack1(m_vbox_center, true, false);
	m_vbox_center.set_border_width(2);
	m_vbox_center.set_spacing(2);
	m_hpaned_info.pack2(m_info_pannel, true, false);
	m_info_pannel.set_border_width(2);
	m_vbox_center.pack_start(m_hbox_search, Gtk::PACK_SHRINK);
	m_vbox_center.pack_start(m_hpaned_lbrowser);
	m_hpaned_lbrowser.set_position(225);
	m_hpaned_lbrowser.pack1(m_vpaned_browser, false, false);
	m_hpaned_lbrowser.pack2(m_vpaned_games, true, false);
	m_vpaned_games.pack1(m_hpaned_browser, false,false);
	m_vpaned_games.pack2(m_scroll_games, true,false);
}

void CGelideUI::initFilters(void){
	// Configuración del scroll que contiene la lista
	m_scroll_filters.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scroll_filters.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_filters.add(m_treeview_filters);
	// Empaquetado de todos los elementos
	m_vbox_filters.set_border_width(2);
	m_vbox_filters.pack_start(m_scroll_filters, true, true);
	// Conectamos la señal de cambio de filtro
	m_treeview_filters.signalFilterChanged().connect(sigc::mem_fun(*this, &CGelideUI::onFilterChanged));
}

void CGelideUI::initSearch(void){
	// Configuración de la etiqueta de los sistemas
	m_label_gamelibrary.set_markup((Glib::ustring) "<span weight=\"bold\">" + _("Games library") + "</span>");
	m_label_gamelibrary.set_alignment(0,0.5);

	// Configuración de la etiqueta de búsqueda
	m_label_search.set_text(_("Search:"));
	m_label_search.set_alignment(0,0.5);

	// Configuración del entry de busqueda y su botón
	m_entry_search.set_width_chars(15);
	m_entry_search.signal_changed().connect(sigc::mem_fun(*this, &CGelideUI::onSearchChanged));
	m_image_clear.set(Gtk::Stock::CLEAR, Gtk::ICON_SIZE_SMALL_TOOLBAR);
	m_button_clear.add(m_image_clear);
	m_button_clear.signal_clicked().connect(sigc::mem_fun(*this, &CGelideUI::onSearchClear));

	m_hbox_search.set_spacing(5);
	m_hbox_search.pack_start(m_label_gamelibrary,false,false);
	m_hbox_search.pack_end(m_button_clear, false, false);
	m_hbox_search.pack_end(m_entry_search, false, false);
	m_hbox_search.pack_end(m_label_search, false,false);
}

void CGelideUI::initBrowser(void){
	// Configuración del scroll que contiene la lista de sistemas
	m_scroll_systems.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scroll_systems.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_systems.add(m_treeview_systems);
	m_scroll_systems.set_border_width(2);
	m_treeview_systems.signalChanged().connect(sigc::mem_fun(*this, &CGelideUI::onSystemChanged));
	// Configuración del scroll que contiene la lista de emus
	m_scroll_emulators.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scroll_emulators.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_emulators.add(m_treeview_emulators);
	m_scroll_emulators.set_border_width(2);
	m_treeview_emulators.signalChanged().connect(sigc::mem_fun(*this, &CGelideUI::onEmulatorChanged));
	// Empaquetado de todos los elementos
	m_vpaned_browser.pack1(m_scroll_systems,true,true);
	m_vpaned_browser.pack2(m_scroll_emulators, true, true);

}

void CGelideUI::initGames(void){
	// Configuración del scroll que contiene la lista
	m_scroll_games.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scroll_games.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_games.add(m_treeview_games);
	m_scroll_games.set_border_width(2);
	m_treeview_games.signalChanged().connect(sigc::mem_fun(*this, &CGelideUI::onGameChanged));
}

void CGelideUI::initInfo(void){
	// Conectamos la señal de juego con la del menú
	m_info_pannel.signalPlayGame().connect(sigc::mem_fun(*this, &CGelideUI::onActionFilePlayGame));
}

void CGelideUI::initStatusbar(void){
	m_vbox_main.pack_start(m_statusbar, Gtk::PACK_SHRINK);
}


void CGelideUI::onSearchChanged(void){
	m_treeview_games.setNameFilter(m_entry_search.get_text());
	updateStatusBar();
}

void CGelideUI::onSearchClear(void){
	m_entry_search.set_text("");
	m_treeview_games.setNameFilter("");
	updateStatusBar();
}

void CGelideUI::onFilterChanged(int p_type, unsigned int p_value){
	m_treeview_games.setFilter(p_type, p_value);
	updateStatusBar();
}

void CGelideUI::onSystemChanged(void){
	Gtk::TreeModel::iterator l_iter;

	l_iter = m_treeview_systems.getSelected();
	m_treeview_emulators.setSystem(l_iter);
	m_treeview_games.setSystem(l_iter);
}

void CGelideUI::onEmulatorChanged(void){
	Gtk::TreeModel::iterator l_siter, l_giter;

	l_siter = m_treeview_systems.getSelected();
	l_giter = m_treeview_games.getSelected();
	m_info_pannel.setGame(l_siter, l_giter);
}

void CGelideUI::onGameChanged(void){
	Gtk::TreeModel::iterator l_siter, l_giter;
	CGame* l_game;

	l_siter = m_treeview_systems.getSelected();
	l_giter = m_treeview_games.getSelected();
	m_info_pannel.setGame(l_siter, l_giter);
	// Actualización de las acciones
	if(l_giter){
		l_game = m_smanager->getSystem(l_siter)->getGame(l_giter);
		m_action_group->get_action("PlayGame")->set_sensitive(true);
		m_action_rank->set_sensitive(true);
		m_action_favorite->set_sensitive(true);
  		m_action_played->set_sensitive(true);
		m_action_working->set_sensitive(true);
		blockSignals();
 		m_action_ranks[l_game->getRank()]->set_active(true);
		m_action_favorite->set_active(l_game->getFavorite());
  		m_action_played->set_active(l_game->getPlayed());
  		m_action_working->set_active(l_game->getWorking());
		unblockSignals();
		delete l_game;
	}
	else{
		m_action_group->get_action("PlayGame")->set_sensitive(false);

		blockSignals();
		m_action_favorite->set_sensitive(false);
		m_action_favorite->set_active(false);
  		m_action_played->set_sensitive(false);
		m_action_played->set_active(false);
		m_action_working->set_sensitive(false);
		m_action_working->set_active(false);
  		m_action_rank->set_sensitive(false);
		unblockSignals();
	}

	updateStatusBar();
}

void CGelideUI::updateStatusBar(void){
	Gtk::TreeModel::iterator l_siter, l_giter;
	int l_vgames, l_sgames, l_tgames;
	Glib::ustring l_sname, l_gname;
	bool l_working;
	CSystem* l_system = NULL;
	CGame* l_game = NULL;

	// Juegos visibles y totales
	l_vgames = m_treeview_games.getGames();
	l_tgames = m_smanager->getGames();

	// Juegos y nombre del sistema seleccionado
	l_siter = m_treeview_systems.getSelected();
	if(l_siter){
		l_system = m_smanager->getSystem(l_siter);
		l_sgames = l_system->getGames();
		l_sname = l_system->getName();
	}
	else{
		l_sgames = 0;
		l_sname = "";
	}

	// Nombre del juego y estado
	l_giter = m_treeview_games.getSelected();
	if(l_giter){
		l_game = l_system->getGame(l_giter);
		l_gname = l_game->getDescription();
		l_working = l_game->getWorking();
		delete l_game;
	}
	else{
		l_gname = "";
		l_working = false;
	}
	// Actualización de la barra de estado
	m_statusbar.setGameInfo(l_sname, l_gname);
	m_statusbar.setWorking(l_working);
	m_statusbar.setListInfo(l_vgames, l_sgames , l_tgames);
}

void CGelideUI::blockSignals(void){
	int l_i;

	m_conn_favorite.block();
	m_conn_played.block();
	m_conn_working.block();
	for(l_i = 0; l_i <= 5; l_i++)
		m_conn_ranks[l_i].block();

}

void CGelideUI::unblockSignals(void){
	int l_i;

	m_conn_favorite.unblock();
	m_conn_played.unblock();
	m_conn_working.unblock();
	for(l_i = 0; l_i <= 5; l_i++)
		m_conn_ranks[l_i].unblock();
}
