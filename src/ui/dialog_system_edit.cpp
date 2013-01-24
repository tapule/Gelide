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
#include "dialog_system_edit.hpp"

CDialogSystemEdit::CDialogSystemEdit():
	Gtk::Dialog(),
	// Inicialización de widgets
	m_table_info(5, 2),	m_table_images(5, 2),
	m_button_cancel(Gtk::Stock::CANCEL),m_button_help(Gtk::Stock::HELP),
	m_button_accept(Gtk::Stock::OK),
	m_button_home(Gtk::Stock::OPEN), m_button_roms(Gtk::Stock::OPEN),
	m_button_snap(Gtk::Stock::OPEN), m_button_tittles(Gtk::Stock::OPEN),
	m_button_front(Gtk::Stock::OPEN), m_button_back(Gtk::Stock::OPEN),
	m_button_media(Gtk::Stock::OPEN)
{
	// Obtenermos las instancias
	m_config = CConfig::getInstance();
	m_smanager = CSystemManager::getInstance();

	m_icon_file = "";

	// Configuración visual del dialogo
	this->set_title( _("Edit systems") );
	this->set_resizable(false);
	this->set_size_request( 450, -1 );
	this->set_border_width(5);

	// Inicializamos los paneles
	initInfoTab();
	initPathsTab();

	// Configuración del Notebook
	//m_notebook_body.set_border_width(10);
	m_notebook_body.append_page(m_vbox_info, _("Information"), _("Information"));
	m_notebook_body.append_page(m_vbox_paths, _("Directories"), _("Directories"));

	// Configuración de los botones principales
	m_button_help.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogSystemEdit::onHelpClicked) );
	m_button_cancel.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogSystemEdit::onCancelClicked) );
	m_button_accept.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogSystemEdit::onAcceptClicked) );

	// Añadimos el notebook y los botones al dialogo
	this->get_vbox()->set_spacing(5);
	this->get_vbox()->pack_start(m_notebook_body);
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

CDialogSystemEdit::~CDialogSystemEdit(){
	saveConfig();
}

void CDialogSystemEdit::initInfoTab(void){
	std::vector<CEmulator*>::iterator l_emulators_iter;
	Gtk::TreeModel::iterator l_treemodel_iter;
	Gtk::TreeModel::Row l_row;
	Glib::RefPtr<Gtk::TreeSelection> l_tree_selection;

	// Configuración de la etiqueta superior
	m_label_info.set_markup((Glib::ustring) "<b>" +	_("System's general information") + "</b>");
	m_label_info.set_alignment(0,0.5);
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
	m_label_dat.set_label(_("Dat File:"));
	m_label_dat.set_alignment(0, 0.50);
	m_label_bios.set_label(_("Bios File:"));
	m_label_bios.set_alignment(0, 0.50);
	m_label_icon.set_label(_("Icon:"));
	m_label_icon.set_alignment(0, 0);
	// Configuración del boton para el icono
	m_button_icon.signal_clicked().connect( sigc::mem_fun(*this, &CDialogSystemEdit::onIconClicked) );
	m_button_icon.add(m_image_icon);
	m_image_icon.set_size_request(48, 48);
	m_image_icon.set(m_smanager->getSystemDefaultIcon());
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
	m_table_info.attach(m_label_dat, 0, 1, 2, 3, Gtk::FILL);
	m_table_info.attach(m_button_dat, 1, 2, 2, 3);
	m_table_info.attach(m_label_bios, 0, 1, 3, 4, Gtk::FILL);
	m_table_info.attach(m_button_bios, 1, 2, 3, 4);
	m_table_info.attach(m_label_icon, 0, 1, 4, 5, Gtk::FILL);
	m_table_info.attach(m_button_icon, 1, 2, 4, 5, Gtk::SHRINK);
	// Configuración de un alineamiento para tabular un poco
	m_alignment_info.set(0, 0, 1, 1);
	m_alignment_info.set_padding(0, 0, 12, 0);
	m_alignment_info.add(m_table_info);
	// Empaquetado de todos los elementos
	m_vbox_info.set_spacing(10);
	m_vbox_info.set_border_width(10);
	m_vbox_info.pack_start(m_label_info,false,false);
	m_vbox_info.pack_start(m_alignment_info, false, false);
}

void CDialogSystemEdit::initPathsTab(void){
	// Configuración de los botones de selección de directorio
	m_button_home.signal_clicked().connect( sigc::bind<Gtk::Entry*>(
		sigc::mem_fun(*this, &CDialogSystemEdit::onSelectFolderClicked),
		&m_entry_home) );
	m_button_roms.signal_clicked().connect( sigc::bind<Gtk::Entry*>(
		sigc::mem_fun(*this, &CDialogSystemEdit::onSelectFolderClicked),
		&m_entry_roms) );
	m_button_snap.signal_clicked().connect( sigc::bind<Gtk::Entry*>(
		sigc::mem_fun(*this, &CDialogSystemEdit::onSelectFolderClicked),
		&m_entry_snap) );
	m_button_tittles.signal_clicked().connect( sigc::bind<Gtk::Entry*>(
		sigc::mem_fun(*this, &CDialogSystemEdit::onSelectFolderClicked),
		&m_entry_tittles) );
	m_button_front.signal_clicked().connect( sigc::bind<Gtk::Entry*>(
		sigc::mem_fun(*this, &CDialogSystemEdit::onSelectFolderClicked),
		&m_entry_front) );
	m_button_back.signal_clicked().connect( sigc::bind<Gtk::Entry*>(
		sigc::mem_fun(*this, &CDialogSystemEdit::onSelectFolderClicked),
		&m_entry_back) );
	m_button_media.signal_clicked().connect( sigc::bind<Gtk::Entry*>(
		sigc::mem_fun(*this, &CDialogSystemEdit::onSelectFolderClicked),
		&m_entry_media) );

	// Configuración de la etiqueta home
	m_label_home.set_markup((Glib::ustring) "<b>" + _("Home directory") + "</b>");
	m_label_home.set_alignment(0,0.5);
	// Configuración de la etiqueta superior
	m_label_roms.set_markup((Glib::ustring) "<b>" + _("Roms directory") + "</b>");
	m_label_roms.set_alignment(0,0.5);
	// Configuración del buscador del home
	m_hbox_home.set_spacing(10);
	m_hbox_home.pack_start(m_entry_home,true,true);
	m_hbox_home.pack_start(m_button_home, false, false);
	// Configuración de un alineamiento para tabular un poco
	m_alignment_home.set(0, 0, 1, 1);
	m_alignment_home.set_padding(0, 0, 12, 0);
	m_alignment_home.add(m_hbox_home);
	// Configuración del buscador de roms
	m_hbox_roms.set_spacing(10);
	m_hbox_roms.pack_start(m_entry_roms,true,true);
	m_hbox_roms.pack_start(m_button_roms, false, false);
	// Configuración de un alineamiento para tabular un poco
	m_alignment_roms.set(0, 0, 1, 1);
	m_alignment_roms.set_padding(0, 0, 12, 0);
	m_alignment_roms.add(m_hbox_roms);
	// Configuración de la etiqueta de directorios
	m_label_images.set_markup((Glib::ustring) "<b>" + _("Images directory") + "</b>");
	m_label_images.set_alignment(0,0.5);
	// Configuración de las etiquetas
	m_label_snap.set_label(_("Snapshots:"));
	m_label_snap.set_alignment(0, 0.50);
	m_label_tittles.set_label(_("Titles:"));
	m_label_tittles.set_alignment(0, 0.50);
	m_label_front.set_label(_("Fronts:"));
	m_label_front.set_alignment(0, 0.50);
	m_label_back.set_label(_("Backs:"));
	m_label_back.set_alignment(0, 0.50);
	m_label_media.set_label(_("Media:"));
	m_label_media.set_alignment(0, 0.50);
	// Configuración de los buscadores
	m_hbox_snap.set_spacing(10);
	m_hbox_snap.pack_start(m_entry_snap,true,true);
	m_hbox_snap.pack_start(m_button_snap, false, false);
	m_hbox_tittles.set_spacing(10);
	m_hbox_tittles.pack_start(m_entry_tittles,true,true);
	m_hbox_tittles.pack_start(m_button_tittles, false, false);
	m_hbox_front.set_spacing(10);
	m_hbox_front.pack_start(m_entry_front,true,true);
	m_hbox_front.pack_start(m_button_front, false, false);
	m_hbox_back.set_spacing(10);
	m_hbox_back.pack_start(m_entry_back,true,true);
	m_hbox_back.pack_start(m_button_back, false, false);
	m_hbox_media.set_spacing(10);
	m_hbox_media.pack_start(m_entry_media,true,true);
	m_hbox_media.pack_start(m_button_media, false, false);
	// Configuración de la tabla
	m_table_images.set_col_spacings(10);
	//m_table_images.set_row_spacings(5);
	m_table_images.attach(m_label_snap, 0, 1, 0, 1, Gtk::FILL);
	m_table_images.attach(m_hbox_snap, 1, 2, 0, 1);
	m_table_images.attach(m_label_tittles, 0, 1, 1, 2, Gtk::FILL);
	m_table_images.attach(m_hbox_tittles, 1, 2, 1, 2);
	m_table_images.attach(m_label_front, 0, 1, 2, 3, Gtk::FILL);
	m_table_images.attach(m_hbox_front, 1, 2, 2, 3);
	m_table_images.attach(m_label_back, 0, 1, 3, 4, Gtk::FILL);
	m_table_images.attach(m_hbox_back, 1, 2, 3, 4);
	m_table_images.attach(m_label_media, 0, 1, 4, 5, Gtk::FILL);
	m_table_images.attach(m_hbox_media, 1, 2, 4, 5);
	// Configuración de un alineamiento para tabular un poco
	m_alignment_images.set(0, 0, 1, 1);
	m_alignment_images.set_padding(0, 0, 12, 0);
	m_alignment_images.add(m_table_images);
	// Empaquetado de todos los elementos
	m_vbox_paths.set_spacing(5);
	m_vbox_paths.set_border_width(10);
	m_vbox_paths.pack_start(m_label_home,false,false);
	m_vbox_paths.pack_start(m_alignment_home, false, false);
	m_vbox_paths.pack_start(m_label_roms,false,false);
	m_vbox_paths.pack_start(m_alignment_roms, false, false);
	m_vbox_paths.pack_start(m_label_images,false,false);
	m_vbox_paths.pack_start(m_alignment_images, false, false);
}

int CDialogSystemEdit::run(void){
	Gtk::TreeModel::Children::iterator l_iter;
	Gtk::TreeModel::Row l_row;

	// No se puede ejecutar sin un sistema que editar
	assert(m_system);

	// Establecemos los campos
	m_entry_name.set_text(m_system->getName());
	m_description_buf->set_text(m_system->getDescription());


	m_button_dat.set_filename(m_system->getDatFile());
	m_button_bios.set_filename(m_system->getBiosFile());
	m_image_icon.set(m_system->getIcon());
	m_icon_file = m_system->getIconFile();
	m_entry_home.set_text(m_system->getHomeDir());
	m_entry_roms.set_text(m_system->getRomsDir());
	m_entry_snap.set_text(m_system->getSnapDir());
	m_entry_tittles.set_text(m_system->getTitleDir());
	m_entry_front.set_text(m_system->getFrontDir());
	m_entry_back.set_text(m_system->getBackDir());
	m_entry_media.set_text(m_system->getMediaDir());

	return Gtk::Dialog::run();
}

void CDialogSystemEdit::setSystem(CSystem* p_system){
	assert(p_system);
	m_system = p_system;
}

int CDialogSystemEdit::run(CSystem* p_system){
	setSystem(p_system);
	return run();
}


void CDialogSystemEdit::onHelpClicked(void){
	Gtk::MessageDialog l_dialog(*this, _("Non implemented."));

	l_dialog.set_secondary_text(
		_("This action is work in progress."));
	l_dialog.run();
}

void CDialogSystemEdit::onCancelClicked(void){
	this->response(Gtk::RESPONSE_CANCEL);
}

void CDialogSystemEdit::onAcceptClicked(void){
	Gtk::TreeModel::iterator l_iter;
	Gtk::MessageDialog l_dialog(*this, _("Name field is not correct"));

	// Comprobamos si se especificó el nombre ya que es necesario
	if(m_entry_name.get_text() == ""){
		l_dialog.set_secondary_text(
			//"Este campo es esencial para un sistema,\ndebe contener algún valor."
			_("This field can not be empty."));
		l_dialog.run();
		return;
	}

	// Pasamos la configuración al sistema
	m_system->setName(m_entry_name.get_text());
	m_system->setDescription(m_description_buf->get_text());


	if(!Glib::file_test(m_button_dat.get_filename(), Glib::FILE_TEST_IS_DIR))
		m_system->setDatFile(m_button_dat.get_filename());
	if(!Glib::file_test(m_button_bios.get_filename(), Glib::FILE_TEST_IS_DIR))
		m_system->setBiosFile(m_button_bios.get_filename());

	m_system->setIcon(m_image_icon.get_pixbuf());
	m_system->setIconFile(m_icon_file);
	m_system->setHomeDir(m_entry_home.get_text());
	m_system->setRomsDir(m_entry_roms.get_text());
	m_system->setSnapDir(m_entry_snap.get_text());
	m_system->setTitleDir(m_entry_tittles.get_text());
	m_system->setFrontDir(m_entry_front.get_text());
	m_system->setBackDir(m_entry_back.get_text());
	m_system->setMediaDir(m_entry_media.get_text());

	this->response(Gtk::RESPONSE_OK);
}

void CDialogSystemEdit::onSelectFolderClicked(Gtk::Entry* p_entry){
	Gtk::FileChooserDialog l_filechooser(_("Please select a directory"));

	// Configuramos el selector de directorio y lo mostramos
	l_filechooser.set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	l_filechooser.set_transient_for(*this);
	l_filechooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	l_filechooser.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	l_filechooser.set_current_folder(p_entry->get_text());
	if(l_filechooser.run() == Gtk::RESPONSE_OK){
		p_entry->set_text(l_filechooser.get_filename());
	}
}

void CDialogSystemEdit::onIconClicked(void){
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

void CDialogSystemEdit::loadConfig(void){
	int l_x, l_y;

	// Obtenemos la posición almacenada
	//m_config->getSystemEditDialogPos(l_x, l_y);
	m_config->getKey(GELIDE_CFG_DIALOGS, "systemedit_dialog_x", l_x);
	m_config->getKey(GELIDE_CFG_DIALOGS, "systemedit_dialog_y", l_y);
	if((l_x!= -1) && (l_y != -1))
		this->move(l_x, l_y);
}

void CDialogSystemEdit::saveConfig(void){
	int l_x, l_y;

	// Guardamos la posición del dialogo
	this->get_position(l_x, l_y);
	//m_config->setSystemEditDialogPos(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "systemedit_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "systemedit_dialog_y", l_y);
}


