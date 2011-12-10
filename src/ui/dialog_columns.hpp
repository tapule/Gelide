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

#ifndef _DIALOG_COLUMNS_HPP_
#define _DIALOG_COLUMNS_HPP_

#include "../gelide.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/alignment.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/buttonbox.h>
#include "../interface_configurable.hpp"
#include "../config.hpp"

class CDialogColumns: public Gtk::Dialog, public IConfigurable
{
public:
	// Constructor y destructor de la clase
	CDialogColumns();
	virtual ~CDialogColumns();

	// Establece el treeview a editar
	void setTreeView(Gtk::TreeView* p_tree);
	// Ejecuta el dialogo
	int run(void);
	// Ejecuta el dialogo estableciendo primero el treeview
	int run(Gtk::TreeView* p_tree);

	// Implementación de Iconfigurable
	virtual void loadConfig(void);
	virtual void saveConfig(void);
private:
	// Instancia del configurador
	CConfig* m_config;
	Gtk::TreeView* m_tree;

	// Cuerpo principal
	Gtk::VBox m_vbox_body;
	Gtk::Label m_label_title;
	Gtk::Alignment m_alignment_body;
	Gtk::VBox m_vbox_alignment;
	Gtk::Label m_label_description;
	Gtk::HBox m_hbox_columns;
	Gtk::ScrolledWindow m_scroll_columns;
	Gtk::TreeView m_treeview_columns;
	Gtk::VBox m_vbox_controls;
	// Botones de control de columnas
	Gtk::Button m_button_column_up;
	Gtk::Button m_button_column_down;
	Gtk::Button m_button_column_add;
	Gtk::Button m_button_column_remove;
	// Botones del dialogo
	//Gtk::Button m_button_help;
	Gtk::Button m_button_close;

	// Columnas utilizadas en el dialogo de preferencias
	class CColumnsColumns:public Gtk::TreeModel::ColumnRecord
	{
	public:
		CColumnsColumns(){
			add(m_active);
			add(m_name);
			add(m_column);
		}
		Gtk::TreeModelColumn<bool> m_active;
		Gtk::TreeModelColumn<Glib::ustring> m_name;
		Gtk::TreeModelColumn<Gtk::TreeViewColumn*> m_column;
	};

	Glib::RefPtr<Gtk::ListStore> m_columns_list;
	CColumnsColumns m_columns;
	// Funciones callbacks para las columnas
	void onColumnUpClicked(void);
	void onColumnDownClicked(void);
	void onColumnAddClicked(void);
	void onColumnRemoveClicked(void);
	void onColumnToggled(const Glib::ustring& p_path);
	void onColumnChanged(void);

	// Callbacks para el cuerpo principal
	//void onHelpClicked(void);
	void onCloseClicked(void);

	void initBody(void);
	void populateColumns(void);
};

#endif // _DIALOG_COLUMNS_HPP_
