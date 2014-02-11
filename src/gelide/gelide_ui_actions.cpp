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
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include "gelide_ui.hpp"
#include "dialog_emulator_launcher.hpp"
#include "dialog_changelog.hpp"
#include "dialog_about.hpp"

/*
 * Acciones menu Archivo
 */

void CGelideUI::onActionFilePlayGame(void){
	Gtk::TreeModel::iterator l_siter, l_giter;

	m_treeview_games.playGame();
	l_siter = m_treeview_systems.getSelected();
	l_giter = m_treeview_games.getSelected();
	m_info_pannel.setGame(l_siter, l_giter);
}

void CGelideUI::onActionFileExportList(void){
	Gtk::FileChooserDialog l_filechooser(_("Export games list..."));
	Gtk::MessageDialog l_dialog(*this,	"", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
	Glib::ustring l_path;

	// Configuramos el selector del fichero de icono y lo mostramos
	l_filechooser.set_action(Gtk::FILE_CHOOSER_ACTION_SAVE);
	l_filechooser.set_transient_for(*this);
	l_filechooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	l_filechooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

	if(l_filechooser.run() == Gtk::RESPONSE_OK){
		l_path = l_filechooser.get_filename();
		if(Glib::file_test(l_path, Glib::FILE_TEST_EXISTS)){
			//"Ya existe un archivo llamado \"" + Glib::path_get_basename(l_path) + "\".\n¿Quiere reemplazarlo?"
			//l_dialog.set_message(_("File \"") + Glib::path_get_basename(l_path) + _(" alredy exists\".\nDo you want to replace it?"));
			l_dialog.set_message( Glib::ustring::compose(_("File \"%1\" alredy exists\".\nDo you want to replace it?"), Glib::path_get_basename(l_path)));
			l_dialog.set_secondary_text( _("Contents will be overwritten if you continue."));
			// Mostramos un mensaje avisando de la operación
			if(l_dialog.run() == Gtk::RESPONSE_CANCEL)
				return;
		}
		m_smanager->exportGameList(l_path);
	}
}

void CGelideUI::onActionFileExportSystems(void){
	Gtk::FileChooserDialog l_filechooser(_("Export systems..."));
	Gtk::MessageDialog l_dialog(*this,	"", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
	Glib::ustring l_path;

	// Configuramos el selector del fichero de icono y lo mostramos
	l_filechooser.set_action(Gtk::FILE_CHOOSER_ACTION_SAVE);
	l_filechooser.set_transient_for(*this);
	l_filechooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	l_filechooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

	if(l_filechooser.run() == Gtk::RESPONSE_OK){
		l_path = l_filechooser.get_filename();
		if(Glib::file_test(l_path, Glib::FILE_TEST_EXISTS)){
			l_dialog.set_message( Glib::ustring::compose(_("File \"%1\" alredy exists\".\nDo you want to replace it?"), Glib::path_get_basename(l_path)));
			l_dialog.set_secondary_text( _("Contents will be overwritten if you continue."));
			// Mostramos un mensaje avisando de la operación
			if(l_dialog.run() == Gtk::RESPONSE_CANCEL)
				return;
		}
		m_smanager->exportSystems(l_path);
	}
}

void CGelideUI::onActionFileImportSystems(void){
	Gtk::FileChooserDialog l_filechooser(_("Import systems..."));
	Glib::ustring l_path;

	// Configuramos el selector del fichero de icono y lo mostramos
	l_filechooser.set_action(Gtk::FILE_CHOOSER_ACTION_SAVE);
	l_filechooser.set_transient_for(*this);
	l_filechooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	l_filechooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

	if(l_filechooser.run() == Gtk::RESPONSE_OK){
		l_path = l_filechooser.get_filename();
		m_smanager->importSystems(l_path);
	}
}

void CGelideUI::onActionFileProperties(void){

}

void CGelideUI::onActionFileQuit(void){
	// Guardamos los sistemas y la configuración de la UI
	m_smanager->save();
	saveConfig();
	this->hide();
}


/*
 * Acciones menu Edición
 */

void CGelideUI::onActionEditFavorite(void){
	Gtk::TreeModel::iterator l_siter, l_giter;

	m_treeview_games.setFavorite(m_action_favorite->get_active());
	l_siter = m_treeview_systems.getSelected();
	l_giter = m_treeview_games.getSelected();
	m_info_pannel.setGame(l_siter, l_giter);
}

void CGelideUI::onActionEditPlayed(void){
	Gtk::TreeModel::iterator l_siter, l_giter;

	m_treeview_games.setPlayed(m_action_played->get_active());
	l_siter = m_treeview_systems.getSelected();
	l_giter = m_treeview_games.getSelected();
	m_info_pannel.setGame(l_siter, l_giter);
}

void CGelideUI::onActionEditWorking(void){
	Gtk::TreeModel::iterator l_siter, l_giter;

	m_treeview_games.setWorking(m_action_working->get_active());
	l_siter = m_treeview_systems.getSelected();
	l_giter = m_treeview_games.getSelected();
	m_info_pannel.setGame(l_siter, l_giter);

}

void CGelideUI::onActionEditRank(const int p_rank){
	Gtk::TreeModel::iterator l_siter, l_giter;

	m_treeview_games.setRank(p_rank);
	l_siter = m_treeview_systems.getSelected();
	l_giter = m_treeview_games.getSelected();
	m_info_pannel.setGame(l_siter, l_giter);
}

void CGelideUI::onActionEditColumns(void){
	CDialogColumns* l_columns_dialog;

	l_columns_dialog = new CDialogColumns();
	l_columns_dialog->set_transient_for(*this);
	l_columns_dialog->run(&m_treeview_games);
	delete l_columns_dialog;
}

void CGelideUI::onActionEditPreferences(void){
	CDialogPreferences* l_preferences_dialog;

	l_preferences_dialog = new CDialogPreferences();
	l_preferences_dialog->set_transient_for(*this);
	l_preferences_dialog->run();
	delete l_preferences_dialog;
}


/*
 * Acciones menu Ver
 */

void CGelideUI::onActionViewFullscreen(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;

	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("FullScreen"));
	if(l_toggleaction->get_active())
		// Pasamos a modo fullscreen
		this->fullscreen();
	else
		this->unfullscreen();
}

void CGelideUI::onActionViewToolsBar(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;

	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("ToolsBar"));
	if(l_toggleaction->get_active())
		m_toolbar->show();
	else
		m_toolbar->hide();
}

void CGelideUI::onActionViewStatusBar(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;

	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("StatusBar"));
	if(l_toggleaction->get_active())
		m_statusbar.show();
	else
		m_statusbar.hide();
}

void CGelideUI::onActionViewFilters(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;
	int l_pos;

	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Filters"));
	if(l_toggleaction->get_active()){
		//m_config->getUIFiltersWidth(l_pos);
		m_config->getKey(GELIDE_CFG_UI, "filters_width", l_pos);
		m_vbox_filters.show();
		m_hpaned_filters.set_position(l_pos);
	}
	else{
		//m_config->setUIFiltersWidth(m_hpaned_filters.get_position());
		m_config->setKey(GELIDE_CFG_UI, "filters_width", m_hpaned_filters.get_position());
		m_vbox_filters.hide();
	}
}

void CGelideUI::onActionViewBrowser(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;
	int l_posv, l_posh;

	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Browser"));
	if(!l_toggleaction->get_active()){
		if(m_browser_mode){
			// Visible el examinador superior guardamos su estado
			//m_config->setUIBrowserTopSize(m_hpaned_browser.get_position(), m_vpaned_games.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_top_width", m_hpaned_browser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_top_height", m_vpaned_games.get_position());
			m_hpaned_browser.hide();
			// Extraemos los treeviews para que no creen warnings
			m_hpaned_browser.remove(m_scroll_systems);
			m_hpaned_browser.remove(m_scroll_emulators);
		}
		else{
			// Visible el examinador izquierdo guardamos su estado
			//m_config->setUIBrowserLeftSize(m_hpaned_lbrowser.get_position(), m_vpaned_browser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_left_width", m_hpaned_lbrowser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_left_height", m_vpaned_browser.get_position());

			m_vpaned_browser.hide();
			// Extraemos los treeviews para que no creen warnings
			m_vpaned_browser.remove(m_scroll_systems);
			m_vpaned_browser.remove(m_scroll_emulators);
		}
	}
	else{
		if(m_browser_mode){
			// Visible el examinador superior cargamos su estado
			m_hpaned_browser.pack1(m_scroll_systems, true, true);
			m_hpaned_browser.pack2(m_scroll_emulators, true, true);
			// Obtenemos la configuración del horizontal
			//m_config->getUIBrowserTopSize(l_posh, l_posv);
			m_config->getKey(GELIDE_CFG_UI, "browser_top_width", l_posh);
			m_config->getKey(GELIDE_CFG_UI, "browser_top_height", l_posv);
			m_hpaned_browser.show();
			m_vpaned_games.set_position(l_posv);
			m_hpaned_browser.set_position(l_posh);
		}
		else{
			// Visible el examinador izquierdo cargamos su estado
			m_vpaned_browser.pack1(m_scroll_systems, true, true);
			m_vpaned_browser.pack2(m_scroll_emulators, true, true);
			// Obtenemos la configuración del horizontal
			//m_config->getUIBrowserLeftSize(l_posh, l_posv);
			m_config->getKey(GELIDE_CFG_UI, "browser_left_width", l_posh);
			m_config->getKey(GELIDE_CFG_UI, "browser_left_height", l_posv);
			m_vpaned_browser.show();
			m_hpaned_lbrowser.set_position(l_posh);
			m_vpaned_browser.set_position(l_posv);
		}
	}
}

void CGelideUI::onActionViewTopBrowser(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;
	int l_posv;
	int l_posh;

	if(!m_browser_mode){
		m_browser_mode = 1;
		l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Browser"));
		if(l_toggleaction->get_active()){
			//m_config->setUIBrowserMode(1);
			m_config->setKey(GELIDE_CFG_UI, "browser_mode", 1);
			// Guardamos el estado del browser superior
			//m_config->setUIBrowserLeftSize(m_hpaned_lbrowser.get_position(), m_vpaned_browser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_left_width", m_hpaned_lbrowser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_left_height", m_vpaned_browser.get_position());
			m_vpaned_browser.hide();
			// Extraemos los treeviews para ponerlos en el horizontal
			m_vpaned_browser.remove(m_scroll_systems);
			m_vpaned_browser.remove(m_scroll_emulators);
			m_hpaned_browser.pack1(m_scroll_systems, true, true);
			m_hpaned_browser.pack2(m_scroll_emulators, true, true);
			// Obtenemos la configuración del horizontal
			//m_config->getUIBrowserTopSize(l_posh, l_posv);
			m_config->getKey(GELIDE_CFG_UI, "browser_top_width", l_posh);
			m_config->getKey(GELIDE_CFG_UI, "browser_top_height", l_posv);
			m_hpaned_browser.show();
			m_vpaned_games.set_position(l_posv);
			m_hpaned_browser.set_position(l_posh);
		}
	}
}

void CGelideUI::onActionViewLeftBrowser(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;
	int l_posv;
	int l_posh;

	if(m_browser_mode){
		m_browser_mode = 0;
		l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Browser"));
		if(l_toggleaction->get_active()){
			//m_config->setUIBrowserMode(0);
			m_config->setKey(GELIDE_CFG_UI, "browser_mode", 0);
			// Guardamos el estado del browser superior
			//m_config->setUIBrowserTopSize(m_hpaned_browser.get_position(), m_vpaned_games.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_top_width", m_hpaned_browser.get_position());
			m_config->setKey(GELIDE_CFG_UI, "browser_top_height", m_vpaned_games.get_position());
			m_hpaned_browser.hide();
			// Extraemos los treeviews para ponerlos en el horizontal
			m_hpaned_browser.remove(m_scroll_systems);
			m_hpaned_browser.remove(m_scroll_emulators);
			m_vpaned_browser.pack1(m_scroll_systems, true, true);
			m_vpaned_browser.pack2(m_scroll_emulators, true, true);
			// Obtenemos la configuración del horizontal
			//m_config->getUIBrowserLeftSize(l_posh, l_posv);
			m_config->getKey(GELIDE_CFG_UI, "browser_left_width", l_posh);
			m_config->getKey(GELIDE_CFG_UI, "browser_left_height", l_posv);
			m_vpaned_browser.show();
			m_hpaned_lbrowser.set_position(l_posh);
			m_vpaned_browser.set_position(l_posv);
		}
	}
}

void CGelideUI::onActionViewInfo(void){
	Glib::RefPtr<Gtk::ToggleAction> l_toggleaction;
	int l_pos;

	l_toggleaction = Glib::RefPtr< Gtk::ToggleAction >::cast_dynamic( m_action_group->get_action("Info"));
	if(l_toggleaction->get_active()){
		//m_config->getUIInfoPannelWidth(l_pos);
		m_config->getKey(GELIDE_CFG_UI, "info_pannel_widht", l_pos);
		m_info_pannel.show();
		m_hpaned_info.set_position(l_pos);
	}
	else{
		//m_config->setUIInfoPannelWidth(m_hpaned_info.get_position());
		m_config->setKey(GELIDE_CFG_UI, "info_pannel_widht", m_hpaned_info.get_position());
		m_info_pannel.hide();
	}
}


/*
 * Acciones menu Ayuda
 */

void CGelideUI::onActionHelpContents(void){

}

void CGelideUI::onActionHelpChangeLog(void){
	CDialogChangelog* l_dialog;

	l_dialog = new CDialogChangelog();
	l_dialog->set_transient_for(*this);
	l_dialog->run();
	delete l_dialog;
}

void CGelideUI::onActionHelpAbout(void){
	CDialogAbout l_dialog;

	l_dialog.run();
}

