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

#include <gtkmm/image.h>
#include <gtkmm/messagedialog.h>
#include "dialog_emulator_edit.hpp"

CDialogEmulatorEdit::CDialogEmulatorEdit():
	Gtk::Dialog(),
	// Inicialización de widgets
	m_table_info(8, 2),
	m_button_cancel(Gtk::Stock::CANCEL),m_button_help(Gtk::Stock::HELP),
	m_button_accept(Gtk::Stock::OK)
{
	// Obtenermos las instancias
	m_config = CConfig::getInstance();
	m_smanager = CSystemManager::getInstance();

	m_icon_file = "";

	// Configuración visual del dialogo
	this->set_title( _("Emulators editor") );
	this->set_resizable(false);
	this->set_size_request( 450, -1 );
	this->set_border_width(5);

	// Inicializamos los paneles
	initInfo();

	// Configuramos la etiqueta principal
	m_label_body.set_markup((Glib::ustring) "<b>" +	_("Emulator's general information") + "</b>");
	m_label_body.set_alignment(0,0.5);
	// Configuración de un alineamiento para tabular el cuerpo
	m_alignment_body.set(0, 0, 1, 1);
	m_alignment_body.set_padding(0, 0, 12, 0);
	m_alignment_body.add(m_table_info);

	// Configuración de los botones principales
	m_button_help.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogEmulatorEdit::onHelpClicked) );
	m_button_cancel.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogEmulatorEdit::onCancelClicked) );
	m_button_accept.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogEmulatorEdit::onAcceptClicked) );

	// Añadimos el notebook y los botones al dialogo
	this->get_vbox()->set_spacing(5);
	this->get_vbox()->pack_start(m_label_body,false,false);
	this->get_vbox()->pack_start(m_alignment_body, false, false);

	this->get_action_area()->pack_start(m_button_help);
	this->get_action_area()->pack_start(m_button_cancel);
	this->get_action_area()->pack_start(m_button_accept);
	this->get_action_area()->set_layout(Gtk::BUTTONBOX_END);

	// Establecemos el diálogo por defecto en el centro del padre
	this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
	// Cargamos la configuración
	loadConfig();

	// Mostramos todos los widgets
	this->show_all_children();
}

CDialogEmulatorEdit::~CDialogEmulatorEdit(){
	saveConfig();
}

void CDialogEmulatorEdit::initInfo(void){
	std::vector<CEmulator*>::iterator l_emulators_iter;
	Gtk::TreeModel::iterator l_treemodel_iter;
	Gtk::TreeModel::Row l_row;
	Glib::RefPtr<Gtk::TreeSelection> l_tree_selection;

	// Configuración del textview de la descripción
	m_text_description.set_size_request(-1, 25);
	m_text_description.set_wrap_mode(Gtk::WRAP_WORD);
	// Configuración del scroll que contiene la descripcion
	m_scroll_description.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	m_scroll_description.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_description.add(m_text_description);
	// Configuración de las etiquetas
	m_label_name.set_label(_("Name:"));
	m_label_name.set_alignment(0, 0.50);
	m_label_description.set_label(_("Description:"));
	m_label_description.set_alignment(0, 0);
	m_label_version.set_label(_("Version:"));
	m_label_version.set_alignment(0, 0.50);
	m_label_author.set_label(_("Author:"));
	m_label_author.set_alignment(0, 0.50);
	m_label_homepage.set_label(_("Web:"));
	m_label_homepage.set_alignment(0, 0.50);
	m_label_path.set_label(_("Binary:"));
	m_label_path.set_alignment(0, 0.50);
	m_label_params.set_label(_("Params:"));
	m_label_params.set_alignment(0, 0.50);
	m_label_icon.set_label(_("Icon:"));
	m_label_icon.set_alignment(0, 0);
	// Configuración del boton para el icono
	m_button_icon.signal_clicked().connect( sigc::mem_fun(*this, &CDialogEmulatorEdit::onIconClicked) );
	m_button_icon.add(m_image_icon);
	m_image_icon.set_size_request(48, 48);
	m_image_icon.set(m_smanager->getEmulatorDefaultIcon());
	// Configuración del textview
	m_description_buf = Gtk::TextBuffer::create();
	m_text_description.set_buffer(m_description_buf);
	// Configuración de la tabla
	m_table_info.set_col_spacings(10);
	m_table_info.set_row_spacings(5);
	m_table_info.attach(m_label_name, 0, 1, 0, 1, Gtk::FILL);
	m_table_info.attach(m_entry_name, 1, 2, 0, 1);
	m_table_info.attach(m_label_description, 0, 1, 1, 2, Gtk::FILL);
	m_table_info.attach(m_scroll_description, 1, 2, 1, 2);
	m_table_info.attach(m_label_version, 0, 1, 2, 3, Gtk::FILL);
	m_table_info.attach(m_entry_version, 1, 2, 2, 3);
	m_table_info.attach(m_label_author, 0, 1, 3, 4, Gtk::FILL);
	m_table_info.attach(m_entry_author, 1, 2, 3, 4);
	m_table_info.attach(m_label_homepage, 0, 1, 4, 5, Gtk::FILL);
	m_table_info.attach(m_entry_homepage, 1, 2, 4, 5);
	m_table_info.attach(m_label_path, 0, 1, 5, 6, Gtk::FILL);
	m_table_info.attach(m_button_path, 1, 2, 5, 6);
	m_table_info.attach(m_label_params, 0, 1, 6, 7, Gtk::FILL);
	m_table_info.attach(m_entry_params, 1, 2, 6, 7);
	m_table_info.attach(m_label_icon, 0, 1, 7, 8, Gtk::FILL);
	m_table_info.attach(m_button_icon, 1, 2, 7, 8, Gtk::SHRINK);
}


int CDialogEmulatorEdit::run(void){
	Gtk::TreeModel::Children::iterator l_iter;
	Gtk::TreeModel::Row l_row;

	// No se puede ejecutar sin un sistema que editar
	assert(m_emulator);

	// Establecemos los campos
	m_entry_name.set_text(m_emulator->getName());
	m_description_buf->set_text(m_emulator->getDescription());
	m_entry_version.set_text(m_emulator->getVersion());
	m_entry_author.set_text(m_emulator->getAuthor());
	m_entry_homepage.set_text(m_emulator->getHomepage());
	m_button_path.set_filename(m_emulator->getPath());
	m_entry_params.set_text(m_emulator->getParams());
	m_image_icon.set(m_emulator->getIcon());
	m_icon_file = m_emulator->getIconFile();

	return Gtk::Dialog::run();
}

void CDialogEmulatorEdit::setEmulator(CEmulator* p_emulator){
	assert(p_emulator);
	m_emulator = p_emulator;
}

int CDialogEmulatorEdit::run(CEmulator* p_emulator){
	setEmulator(p_emulator);
	return run();
}


void CDialogEmulatorEdit::onHelpClicked(void){
	Gtk::MessageDialog l_dialog(*this, _("Non implemented."));

	l_dialog.set_secondary_text(
		_("This action is work in progress."));
	l_dialog.run();
}

void CDialogEmulatorEdit::onCancelClicked(void){
	this->response(Gtk::RESPONSE_CANCEL);
}

void CDialogEmulatorEdit::onAcceptClicked(void){
	Gtk::TreeModel::iterator l_iter;
/*	Gtk::MessageDialog l_dialog(*this, _("El campo Nombre no es correcto"));

	// Comprobamos si se especificó el nombre ya que es necesario
	if(m_entry_name.get_text() == ""){
		l_dialog.set_secondary_text(
			_("Este campo es esencial para un emulador,\ndebe contener algún valor."));
		l_dialog.run();
		return;
	}*/

	// Pasamos la configuración al sistema
	m_emulator->setName(m_entry_name.get_text());
	m_emulator->setDescription(m_description_buf->get_text());
	m_emulator->setVersion(m_entry_version.get_text());
	m_emulator->setAuthor(m_entry_author.get_text());
	m_emulator->setHomepage(m_entry_homepage.get_text());
	m_emulator->setPath(m_button_path.get_filename());
	m_emulator->setParams(m_entry_params.get_text());
	m_emulator->setIcon(m_image_icon.get_pixbuf());
	m_emulator->setIconFile(m_icon_file);

	this->response(Gtk::RESPONSE_OK);
}

void CDialogEmulatorEdit::onIconClicked(void){
	Gtk::FileChooserDialog l_filechooser(_("Please select an image"));
	Glib::RefPtr<Gdk::Pixbuf> l_pixbuf;

	// Configuramos el selector del fichero de icono y lo mostramos
	l_filechooser.set_action(Gtk::FILE_CHOOSER_ACTION_OPEN);
	l_filechooser.set_transient_for(*this);
	l_filechooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	l_filechooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	l_filechooser.set_filename(m_icon_file);

	if(l_filechooser.run() == Gtk::RESPONSE_OK){
		try{
			m_icon_file = l_filechooser.get_filename();
			l_pixbuf = Gdk::Pixbuf::create_from_file(m_icon_file)->scale_simple(32, 32, Gdk::INTERP_BILINEAR);
			m_image_icon.set(l_pixbuf);
		}
		catch(const Glib::Exception& l_exception){
			return;
		}
	}
}

void CDialogEmulatorEdit::loadConfig(void){
	int l_x, l_y;

	// Obtenemos la posición almacenada
	//m_config->getEmulatorEditDialogPos(l_x, l_y);
	m_config->getKey(GELIDE_CFG_DIALOGS, "emulatoredit_dialog_x", l_x);
	m_config->getKey(GELIDE_CFG_DIALOGS, "emulatoredit_dialog_y", l_y);
	if((l_x!= -1) && (l_y != -1))
		this->move(l_x, l_y);
}

void CDialogEmulatorEdit::saveConfig(void){
	int l_x, l_y;

	// Guardamos la posición del dialogo
	this->get_position(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "emulatoredit_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "emulatoredit_dialog_y", l_y);
	//m_config->setEmulatorEditDialogPos(l_x, l_y);
}

