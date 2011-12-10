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

#include "dialog_preferences.hpp"
#include <gtkmm/stock.h>

CDialogPreferences::CDialogPreferences():
	Gtk::Dialog(),
	// Inicialización de botones
	m_button_close(Gtk::Stock::CLOSE)//, m_button_help(Gtk::Stock::HELP),
{
	// Obtenermos las instancias
	m_config = CConfig::getInstance();

	// Configuración visual del dialogo
	this->set_title( _("Gelide configuration") );
	this->set_resizable(false);
	this->set_size_request( 350, -1 );
	this->set_border_width(5);

	// Inicializamos el cuerpo
	initBody();

	// Configuración de los botones principales
	//m_button_help.signal_clicked().connect( sigc::mem_fun(*this,
	//	&CDialogPreferences::onHelpClicked) );
	m_button_close.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogPreferences::onCloseClicked) );

	// Añadimos el notebook y los botones al dialogo
	this->get_vbox()->set_spacing(12);
	this->get_vbox()->pack_start(m_vbox_default_sys, false, true);
	this->get_vbox()->pack_start(m_vbox_emulation, false, true);
	this->get_vbox()->pack_start(m_vbox_list_generator, false, true);
	this->get_vbox()->pack_start(m_vbox_exporting, false, true);

	//this->get_action_area()->pack_start(m_button_help);
	this->get_action_area()->pack_start(m_button_close);
	this->get_action_area()->set_layout(Gtk::BUTTONBOX_END);

	// Establecemos el diálogo por defecto en el centro del padre
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	// Cargamos la configuración
	loadConfig();

	// Mostramos todos los widgets
	this->show_all_children();
}

CDialogPreferences::~CDialogPreferences(){
	saveConfig();
}

void CDialogPreferences::loadConfig(void){
	int l_x, l_y;
	bool l_value;

	// Obtenemos la posición almacenada
	m_config->getKey(GELIDE_CFG_DIALOGS, "preferences_dialog_x", l_x);
	m_config->getKey(GELIDE_CFG_DIALOGS, "preferences_dialog_y", l_y);
	if((l_x!= -1) && (l_y != -1))
		this->move(l_x, l_y);

	m_config->getKey(GELIDE_CFG_PREFERENCES, "import_default_systems", l_value);
	m_check_load_default_sys.set_active(l_value);

	m_config->getKey(GELIDE_CFG_PREFERENCES, "show_emulator_output", l_value);
	m_check_show_output.set_active(l_value);

	m_config->getKey(GELIDE_CFG_PREFERENCES, "preserving_games_data", l_value);
	m_check_preserve_data.set_active(l_value);
	m_config->getKey(GELIDE_CFG_PREFERENCES, "add_unknown_games", l_value);
	m_check_add_unknown.set_active(l_value);

	m_config->getKey(GELIDE_CFG_PREFERENCES, "export_icons", l_value);
	m_check_export_icons.set_active(l_value);

	m_config->getKey(GELIDE_CFG_PREFERENCES, "export_dats", l_value);
	m_check_export_dats.set_active(l_value);
}

void CDialogPreferences::saveConfig(void){
	int l_x, l_y;

	// Guardamos solo la posición del dialogo ya que el resto de valores estará
	// establecido por los respectivos callback
	this->get_position(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "preferences_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "preferences_dialog_y", l_y);
}

void CDialogPreferences::initBody(void){

	// Configuración de la etiqueta superior
	m_label_default_sys.set_markup((Glib::ustring) "<b>" + _("Defaults systems") + "</b>");
	m_label_default_sys.set_alignment(0,0.5);
	// Configuración del checkbutton
	m_check_load_default_sys.set_label(_("Load default systems on boot"));
	m_check_load_default_sys.set_active(true);
	m_check_load_default_sys.signal_toggled().connect( sigc::mem_fun(*this,	&CDialogPreferences::onLoadDefaultSysToggled) );
	// Configuración de un alineamiento para tabular
	//m_alignment_body.set(0, 0, 1, 1);
	m_alignment_default_sys.set_padding(0, 0, 12, 0);
	m_alignment_default_sys.add(m_check_load_default_sys);
	// Empaquetamos etiqueta y alineamiento
	m_vbox_default_sys.set_spacing(5);
	m_vbox_default_sys.pack_start(m_label_default_sys, false, false);
	m_vbox_default_sys.pack_start(m_alignment_default_sys, true, true);

	// Configuración de la etiqueta superior
	m_label_emulation.set_markup((Glib::ustring) "<b>" + _("Emulation") + "</b>");
	m_label_emulation.set_alignment(0,0.5);
	// Configuración del checkbutton
	m_check_show_output.set_label(_("Show emulator's output"));
	m_check_show_output.set_active(false);
	m_check_show_output.signal_toggled().connect( sigc::mem_fun(*this,	&CDialogPreferences::onShowOutputToggled) );
	// Configuración de un alineamiento para tabular
	//m_alignment_body.set(0, 0, 1, 1);
	m_alignment_emulation.set_padding(0, 0, 12, 0);
	m_alignment_emulation.add(m_check_show_output);
	// Empaquetamos etiqueta y alineamiento
	m_vbox_emulation.set_spacing(5);
	m_vbox_emulation.pack_start(m_label_emulation, false, false);
	m_vbox_emulation.pack_start(m_alignment_emulation, true, true);

  	// Configuración de la etiqueta superior
	m_label_list_generator.set_markup((Glib::ustring) "<b>" + _("Games list generator") + "</b>");
	m_label_list_generator.set_alignment(0,0.5);
	// Configuración del checkbutton
	m_check_preserve_data.set_label(_("Preserving games data"));
	m_check_preserve_data.set_active(true);
	m_check_preserve_data.signal_toggled().connect( sigc::mem_fun(*this, &CDialogPreferences::onPreserveDataToggled) );
	m_check_add_unknown.set_label(_("Add unknown games"));
	m_check_add_unknown.set_active(true);
	m_check_add_unknown.signal_toggled().connect( sigc::mem_fun(*this, &CDialogPreferences::onAddUnknownToggled) );
	// Configuración de un alineamiento para tabular
	//m_alignment_body.set(0, 0, 1, 1);
	m_alignment_list_generator.set_padding(0, 0, 12, 0);
	m_alignment_list_generator.add(m_vbox_list_generator_checks);
	m_vbox_list_generator_checks.pack_start(m_check_preserve_data, false, true);
	m_vbox_list_generator_checks.pack_start(m_check_add_unknown, false, true);
	// Empaquetamos etiqueta y alineamiento
	m_vbox_list_generator.set_spacing(5);
	m_vbox_list_generator.pack_start(m_label_list_generator, false, false);
	m_vbox_list_generator.pack_start(m_alignment_list_generator, true, true);

	// Configuración de la etiqueta superior
	m_label_exporting.set_markup((Glib::ustring) "<b>" + _("Export") + "</b>");
	m_label_exporting.set_alignment(0,0.5);
	// Configuración del checkbutton
	m_check_export_icons.set_label(_("Export icons"));
	m_check_export_icons.set_active(true);
	m_check_export_icons.signal_toggled().connect( sigc::mem_fun(*this,	&CDialogPreferences::onExportIconsToggled) );
	m_check_export_dats.set_label(_("Export dat files"));
	m_check_export_dats.set_active(false);
	m_check_export_dats.signal_toggled().connect( sigc::mem_fun(*this,	&CDialogPreferences::onExportDatsToggled) );
	// Configuración de un alineamiento para tabular
	//m_alignment_body.set(0, 0, 1, 1);
	m_alignment_exporting.set_padding(0, 0, 12, 0);
	m_alignment_exporting.add(m_vbox_exporting_checks);
	m_vbox_exporting_checks.pack_start(m_check_export_icons, false, true);
	m_vbox_exporting_checks.pack_start(m_check_export_dats, false, true);
	// Empaquetamos etiqueta y alineamiento
	m_vbox_exporting.set_spacing(5);
	m_vbox_exporting.pack_start(m_label_exporting, false, false);
	m_vbox_exporting.pack_start(m_alignment_exporting, true, true);
}

void CDialogPreferences::onLoadDefaultSysToggled(void){
	m_config->setKey(GELIDE_CFG_PREFERENCES, "import_default_systems", m_check_load_default_sys.get_active());
}

void CDialogPreferences::onShowOutputToggled(void){
	m_config->setKey(GELIDE_CFG_PREFERENCES, "show_emulator_output", m_check_show_output.get_active());
}

void CDialogPreferences::onPreserveDataToggled(void){
	m_config->setKey(GELIDE_CFG_PREFERENCES, "preserving_games_data", m_check_preserve_data.get_active());
}

void CDialogPreferences::onAddUnknownToggled(void){
	m_config->setKey(GELIDE_CFG_PREFERENCES, "add_unknown_games", m_check_add_unknown.get_active());
}

void CDialogPreferences::onExportIconsToggled(void){
	m_config->setKey(GELIDE_CFG_PREFERENCES, "export_icons", m_check_export_icons.get_active());
}

void CDialogPreferences::onExportDatsToggled(void){
	m_config->setKey(GELIDE_CFG_PREFERENCES, "export_dats", m_check_export_dats.get_active());
}

/*void CDialogPreferences::onHelpClicked(void){
	Gtk::MessageDialog l_dialog(*this, _("No implementado."));

	l_dialog.set_secondary_text(
		_("Esta acción está en proceso de desarrollo."));
	l_dialog.run();
}
*/

void CDialogPreferences::onCloseClicked(void){
	this->response(Gtk::RESPONSE_CLOSE);
}

