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

#include "dialog_columns.hpp"
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>

CDialogColumns::CDialogColumns():
	Gtk::Dialog(),
	// Inicialización de botones
	m_button_column_up(Gtk::Stock::GO_UP), m_button_column_down(Gtk::Stock::GO_DOWN),
	m_button_column_add(Gtk::Stock::ADD), m_button_column_remove(Gtk::Stock::REMOVE),
	m_button_close(Gtk::Stock::CLOSE)//, m_button_help(Gtk::Stock::HELP),
{
	// Obtenermos las instancias
	m_config = CConfig::getInstance();
	m_tree = NULL;

	// Configuración visual del dialogo
	this->set_title( _("Columns selector") );
	this->set_resizable(false);
	this->set_size_request( 400, -1 );
	this->set_border_width(5);

	// Inicializamos el cuerpo
	initBody();

	// Configuración de los botones principales
	//m_button_help.signal_clicked().connect( sigc::mem_fun(*this,
	//	&CDialogColumns::onHelpClicked) );
	m_button_close.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogColumns::onCloseClicked) );

	// Añadimos el notebook y los botones al dialogo
	this->get_vbox()->set_spacing(5);
	this->get_vbox()->pack_start(m_label_title, false, false);
	this->get_vbox()->pack_start(m_alignment_body, true, true);

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

CDialogColumns::~CDialogColumns(){
	saveConfig();
}

void CDialogColumns::initBody(void){
	Gtk::CellRendererToggle* l_renderer_toggle;
	Glib::RefPtr<Gtk::TreeSelection> l_tree_selection;
	int l_column_index;

	// Configuración de la etiqueta superior
	m_label_title.set_markup((Glib::ustring) "<span weight=\"bold\">" + _("Columns") + "</span>");
	m_label_title.set_alignment(0,0.5);
	// Configuración de un alineamiento para tabular
	//m_alignment_body.set(0, 0, 1, 1);
	m_alignment_body.set_padding(0, 0, 12, 0);
	m_alignment_body.add(m_vbox_alignment);
	// Empaquetamos etiqueta y alineamiento
	//m_vbox_body.set_spacing(5);
	//m_vbox_body.pack_start(m_label_title, false, false);
	//m_vbox_body.pack_start(m_alignment_body, true, true);

	// Configuramos la etiqueta de descripción
	m_label_description.set_label(_("Please select games list columns"));
	m_label_description.set_alignment(0,0.5);
	// Configuramos el cuerpo del tree
	m_hbox_columns.set_spacing(10);
	m_hbox_columns.pack_start(m_scroll_columns, true, true);
	m_hbox_columns.pack_start(m_vbox_controls, false, false);
	// Empaquetamos en el alineamiento
	m_vbox_alignment.set_spacing(10);
	m_vbox_alignment.pack_start(m_label_description, false, true);
	m_vbox_alignment.pack_start(m_hbox_columns, true, true );

	// Configuración del scroll que contiene la lista
	m_scroll_columns.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scroll_columns.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll_columns.add(m_treeview_columns);

	// Configuración de los botones de control sobre la lista
	m_button_column_up.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogColumns::onColumnUpClicked) );
	m_button_column_up.set_sensitive(false);
	m_button_column_down.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogColumns::onColumnDownClicked) );
	m_button_column_down.set_sensitive(false);
	m_button_column_add.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogColumns::onColumnAddClicked) );
	m_button_column_add.set_sensitive(false);
	m_button_column_remove.signal_clicked().connect( sigc::mem_fun(*this,
		&CDialogColumns::onColumnRemoveClicked) );
	m_button_column_remove.set_sensitive(false);
	// Configuración del contenedor de los botones
	m_vbox_controls.set_spacing(10);
	m_vbox_controls.set_size_request(150,-1);
	m_vbox_controls.pack_start(m_button_column_up, false, false);
	m_vbox_controls.pack_start(m_button_column_down, false, false);
	m_vbox_controls.pack_start(m_button_column_add, false, false);
	m_vbox_controls.pack_start(m_button_column_remove, false, false);

	// Creación del listado y configuración del treeview
	m_columns_list = Gtk::ListStore::create(m_columns);
	m_treeview_columns.set_model(m_columns_list);
	m_treeview_columns.set_rules_hint();
	m_treeview_columns.set_headers_visible(false);
	m_treeview_columns.set_enable_search(true);
	m_treeview_columns.set_search_column(m_columns.m_name);
	m_treeview_columns.get_selection()->set_mode(Gtk::SELECTION_SINGLE);

	// Creación de la columna Activo (Editable) y configuración
	l_column_index = m_treeview_columns.append_column_editable(_("Active"),	m_columns.m_active);
	l_renderer_toggle = (Gtk::CellRendererToggle*) m_treeview_columns.get_column_cell_renderer(l_column_index -1);
	if(l_renderer_toggle){
		// Callback para activar/desactivar la columna
		l_renderer_toggle->signal_toggled().connect(sigc::mem_fun(*this, &CDialogColumns::onColumnToggled));
	}
	// Creación de la columna nombre
	m_treeview_columns.append_column(_("Name"), m_columns.m_name);
	l_tree_selection = m_treeview_columns.get_selection();
	l_tree_selection->signal_changed().connect(sigc::mem_fun(*this, &CDialogColumns::onColumnChanged));

}

void CDialogColumns::populateColumns(void){
	Glib::RefPtr<Gtk::TreeSelection> l_tree_selection;
	Gtk::TreeModel::iterator l_tree_iter;
	Gtk::TreeModel::Row l_row;
	std::vector<Gtk::TreeViewColumn*> l_columns;
	std::vector<Gtk::TreeViewColumn*>::iterator l_columns_iter;

	assert(m_tree);

	m_columns_list->clear();

	// Construimos los listados de visibilidad, anchuras
	l_columns = m_tree->get_columns();
	for(l_columns_iter = l_columns.begin(); l_columns_iter != l_columns.end(); l_columns_iter++){
		l_tree_iter = m_columns_list->append();
		l_row = *l_tree_iter;
		l_row[m_columns.m_active] = (*l_columns_iter)->get_visible();
		l_row[m_columns.m_name] = (*l_columns_iter)->get_title();
		l_row[m_columns.m_column] = (*l_columns_iter);
	}
	// Si hay elementos en el listado, forzamos a que se seleccione el primero
	l_tree_iter = m_columns_list->children().begin();
	if(l_tree_iter){
		l_tree_selection = m_treeview_columns.get_selection();
  		l_tree_selection->select(l_tree_iter);
		// Activamos los botones si hay selección
  		m_button_column_up.set_sensitive(false);
  		m_button_column_down.set_sensitive(true);
  		m_button_column_add.set_sensitive( ! (*l_tree_iter)[m_columns.m_active] );
  		m_button_column_remove.set_sensitive( (*l_tree_iter)[m_columns.m_active] );
	}
	else{
  		m_button_column_up.set_sensitive(false);
  		m_button_column_down.set_sensitive(false);
  		m_button_column_add.set_sensitive(false);
  		m_button_column_remove.set_sensitive(false);
	}
}

int CDialogColumns::run(void){

	// No se puede ejecutar sin un treeview
	assert(m_tree);
	// Llenamos el listado
	populateColumns();
	return Gtk::Dialog::run();
}

void CDialogColumns::setTreeView(Gtk::TreeView* p_tree){
	assert(p_tree);
	m_tree = p_tree;
}

int CDialogColumns::run(Gtk::TreeView* p_tree){
	setTreeView(p_tree);
	return run();
}

/*void CDialogColumns::onHelpClicked(void){
	Gtk::MessageDialog l_dialog(*this, _("No implementado."));

	l_dialog.set_secondary_text(
		_("Esta acción está en proceso de desarrollo."));
	l_dialog.run();
}*/


void CDialogColumns::onCloseClicked(void){
	this->response(Gtk::RESPONSE_CLOSE);
}

void CDialogColumns::loadConfig(void){
	int l_x, l_y;

	// Obtenemos la posición almacenada
	//m_config->getColumnsDialogPos(l_x, l_y);
	m_config->getKey(GELIDE_CFG_DIALOGS, "columns_dialog_x", l_x);
	m_config->getKey(GELIDE_CFG_DIALOGS, "columns_dialog_y", l_y);
	if((l_x!= -1) && (l_y != -1))
		this->move(l_x, l_y);
}

void CDialogColumns::saveConfig(void){
	int l_x, l_y;

	// Guardamos la posición del dialogo
	this->get_position(l_x, l_y);
	//m_config->setColumnsDialogPos(l_x, l_y);
	m_config->setKey(GELIDE_CFG_DIALOGS, "columns_dialog_x", l_x);
	m_config->setKey(GELIDE_CFG_DIALOGS, "columns_dialog_y", l_y);
}

void CDialogColumns::onColumnUpClicked(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;
	Gtk::TreeViewColumn* l_column;
	std::vector<Gtk::TreeViewColumn*> l_columns;
	std::vector<Gtk::TreeViewColumn*>::iterator l_columns_iter;

	assert(m_tree);

	l_selection = m_treeview_columns.get_selection();
	l_iter = l_selection->get_selected();
	l_column = (*l_iter)[m_columns.m_column];
	l_columns = m_tree->get_columns();
	// Buscamos la columna anterior en el treeview editado
	for(l_columns_iter = l_columns.begin(); l_columns_iter != l_columns.end(); l_columns_iter++){
		if((*l_columns_iter) == l_column){
			l_columns_iter--;
			break;
		}
	}
	// Intercambiamos las columnas en el treeview editado
	m_tree->move_column_after(*(*l_columns_iter), *l_column);
	// Intercambiamos las filas en el listado de columnas
	l_dest = l_selection->get_selected();
	l_dest--;
	m_columns_list->move(l_iter, l_dest);
	// Generamos un cambio de fila falso para que se actualicen los controles
	onColumnChanged();
}

void CDialogColumns::onColumnDownClicked(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;
	Gtk::TreeViewColumn* l_column;
	std::vector<Gtk::TreeViewColumn*> l_columns;
	std::vector<Gtk::TreeViewColumn*>::iterator l_columns_iter;

	assert(m_tree);

	l_selection = m_treeview_columns.get_selection();
	l_iter = l_selection->get_selected();
	l_column = (*l_iter)[m_columns.m_column];
	l_columns = m_tree->get_columns();
	// Buscamos la columna siguiente en el treeview editado
	for(l_columns_iter = l_columns.begin(); l_columns_iter != l_columns.end(); l_columns_iter++){
		if((*l_columns_iter) == l_column){
			l_columns_iter++;
			break;
		}
	}
	// Intercambiamos las columnas en el treeview editado
	m_tree->move_column_after(*l_column, *(*l_columns_iter));
	// Intercambiamos las filas en el listado de columnas
	l_dest = l_selection->get_selected();
	l_dest++;
	m_columns_list->move(l_dest, l_iter);
	// Generamos un cambio de fila falso para que se actualicen los controles
	onColumnChanged();
}

void CDialogColumns::onColumnAddClicked(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;
	Gtk::TreeViewColumn* l_column;

	l_selection = m_treeview_columns.get_selection();
	l_iter = l_selection->get_selected();
	l_column = (*l_iter)[m_columns.m_column];
	(*l_iter)[m_columns.m_active] = true;
	l_column->set_visible(true);
	m_button_column_add.set_sensitive(false);
	m_button_column_remove.set_sensitive(true);
}

void CDialogColumns::onColumnRemoveClicked(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;
	Gtk::TreeViewColumn* l_column;

	l_selection = m_treeview_columns.get_selection();
	l_iter = l_selection->get_selected();
	l_column = (*l_iter)[m_columns.m_column];
	(*l_iter)[m_columns.m_active] = false;
	l_column->set_visible(false);
	m_button_column_add.set_sensitive(true);
	m_button_column_remove.set_sensitive(false);
}

void CDialogColumns::onColumnToggled(const Glib::ustring& p_path){
	Gtk::TreeViewColumn* l_column;
	Gtk::TreePath* l_path;
	Gtk::TreeModel::iterator l_iter;

	l_path = new Gtk::TreePath(p_path);
	l_iter = m_columns_list->get_iter(*l_path);

	l_column = (*l_iter)[m_columns.m_column];
	l_column->set_visible((*l_iter)[m_columns.m_active]);
	if((*l_iter)[m_columns.m_active]){
		m_button_column_add.set_sensitive(false);
		m_button_column_remove.set_sensitive(true);
	}
	else{
		m_button_column_add.set_sensitive(true);
		m_button_column_remove.set_sensitive(false);
	}

}

void CDialogColumns::onColumnChanged(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;

	l_selection = m_treeview_columns.get_selection();
	l_iter = l_selection->get_selected();
	// Ninguno seleccionado
	if(!l_iter){
 		m_button_column_up.set_sensitive(false);
  		m_button_column_down.set_sensitive(false);
  		m_button_column_add.set_sensitive(false);
  		m_button_column_remove.set_sensitive(false);
  		return;
	}
	// Establecemos los valores por defecto
	m_button_column_up.set_sensitive(true);
	m_button_column_down.set_sensitive(true);
	m_button_column_add.set_sensitive(!(*l_iter)[m_columns.m_active]);
	m_button_column_remove.set_sensitive((*l_iter)[m_columns.m_active]);
	// Comprobamos si se trata del primero
	if(l_iter == m_columns_list->children().begin()){
 		m_button_column_up.set_sensitive(false);
  		return;
	}
	// Comprobamos si se trata del último
	l_iter++;
	if(l_iter == m_columns_list->children().end()){
		l_iter--;
  		m_button_column_down.set_sensitive(false);
  		return;
	}
}

