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

#include "treeview_systems.hpp"
#include "gelide_ui.hpp"
#include "dialog_system_edit.hpp"
#include "dialog_gamelist_generator.hpp"
#include "../utils/utils.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>


CTreeViewSystems::CTreeViewSystems(void):
	Gtk::TreeView()
{
	Gtk::CellRendererText* l_renderer_text;
	Gtk::TreeViewColumn* l_column;
	int l_column_index;

	// Obtenermos la instancia del smanager
	m_smanager = CSystemManager::getInstance();

	this->set_model(m_smanager->getSystemList());
	this->set_rules_hint();
	this->set_headers_visible(false);
	this->set_enable_search(true);
	this->set_search_column(m_columns.m_name);
	this->get_selection()->set_mode(Gtk::SELECTION_SINGLE);

	// Creación de la columna Icono y configuración
	this->append_column(_("Icon"), m_columns.m_icon);
	// Creación de la columna Descripción y configuración
	l_column_index = this->append_column(_("Description"), m_columns.m_name);
	l_column = this->get_column(l_column_index -1);
	l_renderer_text = (Gtk::CellRendererText*) this->get_column_cell_renderer(l_column_index -1);
	if(l_renderer_text){
		// Callback para el renderizado del texto en dos lineas
		l_column->set_cell_data_func(*l_renderer_text, sigc::mem_fun(*this, &CTreeViewSystems::onSystemCellData));
#ifdef GLIBMM_PROPERTIES_ENABLED
		l_renderer_text->property_ellipsize() = Pango::ELLIPSIZE_END;
#else
		l_renderer_text->set_property("ellipsize", Pango::ELLIPSIZE_END);
#endif
	}

	this->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &CTreeViewSystems::onSystemChanged));

	initActions();
}

sigc::signal<void> CTreeViewSystems::signalChanged(void){
	return m_signal_changed;
}

void CTreeViewSystems::setSelected(Gtk::TreeModel::iterator& p_system){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::Path l_path;

	l_selection = this->get_selection();
	if(p_system){
  		l_selection->select(p_system);
  		l_path = p_system;
		this->scroll_to_row(l_path);
	}
	else{
		// Forzamos a que se seleccione el primero si existe
		if(l_iter = m_smanager->getSystemList()->children().begin())
			l_selection->select(l_iter);
	}
}

Gtk::TreeModel::iterator CTreeViewSystems::getSelected(void){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	return l_iter;
}

void CTreeViewSystems::initActions(){
	Glib::ustring l_ui_info;

	// Creamos las acciones
	m_action_group = Gtk::ActionGroup::create();

	// Creamos las acciones del popup
	m_action_add = Gtk::Action::create("AddSystem", Gtk::Stock::ADD);
	m_action_remove = Gtk::Action::create("RemoveSystem", Gtk::Stock::REMOVE);
	m_action_edit = Gtk::Action::create("EditSystem", Gtk::Stock::EDIT);
	m_action_refresh = Gtk::Action::create("RefreshSystem", Gtk::Stock::REFRESH);
	m_action_move = Gtk::Action::create("MenuMove", _("_Move"));
	m_action_first = Gtk::Action::create("MoveFirst", Gtk::Stock::GOTO_TOP, _("_First"));
	m_action_up = Gtk::Action::create("MoveUp", Gtk::Stock::GO_UP, _("_Up"));
	m_action_down = Gtk::Action::create("MoveDown", Gtk::Stock::GO_DOWN, _("_Down"));
	m_action_last = Gtk::Action::create("MoveLast", Gtk::Stock::GOTO_BOTTOM, _("_Last"));


	// Deshabilitamos remove, edit, refresh y las acciones de movimiento
	m_action_remove->set_sensitive(false);
	m_action_edit->set_sensitive(false);
	m_action_refresh->set_sensitive(false);
	m_action_first->set_sensitive(false);
	m_action_up->set_sensitive(false);
	m_action_down->set_sensitive(false);
	m_action_last->set_sensitive(false);

	// Insertamos las acciones en el grupo de acciones
	m_action_group->add(m_action_add, sigc::mem_fun(*this, &CTreeViewSystems::onActionAdd) );
	m_action_group->add(m_action_remove, sigc::mem_fun(*this, &CTreeViewSystems::onActionRemove) );
	m_action_group->add(m_action_edit, sigc::mem_fun(*this, &CTreeViewSystems::onActionEdit) );
	m_action_group->add(m_action_refresh, sigc::mem_fun(*this, &CTreeViewSystems::onActionRefresh) );
	m_action_group->add(m_action_move);
	m_action_group->add(m_action_first, sigc::mem_fun(*this, &CTreeViewSystems::onActionMoveFirst) );
	m_action_group->add(m_action_up, sigc::mem_fun(*this, &CTreeViewSystems::onActionMoveUp) );
	m_action_group->add(m_action_down, sigc::mem_fun(*this, &CTreeViewSystems::onActionMoveDown) );
	m_action_group->add(m_action_last, sigc::mem_fun(*this, &CTreeViewSystems::onActionMoveLast) );

	m_ui_manager = Gtk::UIManager::create();
	m_ui_manager->insert_action_group(m_action_group);

	l_ui_info =
		"<ui>"
		"	<popup name='SystemsPopup'>"
		"		<menuitem action='AddSystem'/>"
		"		<menuitem action='RemoveSystem'/>"
		"		<menuitem action='EditSystem'/>"
		"		<separator />"
		"		<menu action='MenuMove'>"
		"			<menuitem action='MoveFirst' />"
		"			<menuitem action='MoveUp' />"
		"			<menuitem action='MoveDown' />"
		"			<menuitem action='MoveLast' />"
		"		</menu>"
		"		<separator />"
		"		<menuitem action='RefreshSystem'/>"
		"	</popup>"
		"</ui>";

	try{
		m_ui_manager->add_ui_from_string(l_ui_info);
        m_popup_menu = static_cast<Gtk::Menu*>(m_ui_manager->get_widget("/SystemsPopup"));
		assert(m_popup_menu);
	}
	catch (Glib::Error& l_exception)
	{
		GELIDE_ERROR("Generating systems popup (" << l_exception.what().c_str() << ")");
		throw l_exception;
	}
}

void CTreeViewSystems::onSystemCellData(Gtk::CellRenderer* p_renderer,
								const Gtk::TreeModel::iterator& p_iter)
{
	Gtk::TreeModel::Row l_row;
	Glib::ustring l_markup;

	if(p_iter){
		l_row = *p_iter;
		l_markup = (Glib::ustring) "<b><span size=\"medium\">" + l_row[m_columns.m_name]
			+ "</span></b>\n" +"<span size=\"small\"><i>" + l_row[m_columns.m_description] + "</i></span>";
	#ifdef GLIBMM_PROPERTIES_ENABLED
		((Gtk::CellRendererText*) p_renderer)->property_markup() = l_markup;
	#else
		((Gtk::CellRendererText*) p_renderer)->set_property("markup", l_markup);
	#endif
	}
}

void CTreeViewSystems::onSystemChanged(void){
	// Emitimos una señal de cambio
	m_signal_changed.emit();
}

bool CTreeViewSystems::on_button_press_event(GdkEventButton* p_event){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	Gtk::TreeView::on_button_press_event(p_event);

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	if((p_event->type == GDK_BUTTON_PRESS) && (p_event->button == 3) ) {
		// Actualizamos el menú dependiendo de la selección
		if(l_iter){
			m_action_remove->set_sensitive(true);
			m_action_edit->set_sensitive(true);
	  		m_action_refresh->set_sensitive(true);
	  		m_action_first->set_sensitive(true);
	  		m_action_up->set_sensitive(true);
	  		m_action_down->set_sensitive(true);
	  		m_action_last->set_sensitive(true);
	  		// Comprobamos si se trata del primero
	  		if(l_iter == this->get_model()->children().begin()){
	  	 		m_action_first->set_sensitive(false);
	  	 		m_action_up->set_sensitive(false);
	  		}
	  		// Comprobamos si se trata del último
	  		l_iter++;
	  		if(l_iter == this->get_model()->children().end()){
	  			l_iter--;
	  	  		m_action_last->set_sensitive(false);
	  	  		m_action_down->set_sensitive(false);
	  		}
		}
		else{
			m_action_remove->set_sensitive(false);
			m_action_edit->set_sensitive(false);
	  		m_action_refresh->set_sensitive(false);
	  		m_action_first->set_sensitive(false);
	  		m_action_up->set_sensitive(false);
	  		m_action_down->set_sensitive(false);
	  		m_action_last->set_sensitive(false);
		}

		m_popup_menu->popup(p_event->button, p_event->time);
		return true;
	}
	else if((p_event->type == GDK_2BUTTON_PRESS) && (p_event->button == 1) ) {
		if(l_iter){
			onActionEdit();
			return true;
		}
	}

	return false;
}

void CTreeViewSystems::onActionAdd(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	CDialogSystemEdit* l_sys_edit;
	Gtk::TreeModel::iterator l_iter;
	CSystem* l_system;

	l_sys_edit = new CDialogSystemEdit();
	l_sys_edit->set_transient_for(*static_cast<Gtk::Window*>(CGelideUI::getInstance()));
	l_system = new CSystem();
	l_system->setIcon(m_smanager->getSystemDefaultIcon());
	if(l_sys_edit->run(l_system) == Gtk::RESPONSE_OK){
		l_iter = m_smanager->addSystem(*l_system);
		if(l_iter){
			l_selection = this->get_selection();
			if(l_selection)
				l_selection->select(l_iter);
		}
	}
	else
		delete l_system;
	delete l_sys_edit;
}

void CTreeViewSystems::onActionRemove(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;
	Gtk::MessageDialog l_dialog(*static_cast<Gtk::Window*>(CGelideUI::getInstance()),
		_("This operation can not be undone."), false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL);

	//"Si continua eliminará los datos del sistema,\naunque no sus archivos."
	l_dialog.set_secondary_text( _("System's data will be lost if you continue."));
	// Mostramos un mensaje avisando de la operación
	if(l_dialog.run() == Gtk::RESPONSE_CANCEL)
		return;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter){
			m_smanager->removeSystem(l_iter);
			l_iter = m_smanager->getSystemList()->children().begin();
			// Comprobamos si hay algún elemento y lo seleccionamos
			if(l_iter)
				l_selection->select(l_iter);
		}
	}
}

void CTreeViewSystems::onActionEdit(void){
	CDialogSystemEdit* l_sys_edit;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;
	CSystem* l_system;
	CSystem l_new;

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	if(l_iter){
		l_system = m_smanager->getSystem(l_iter);
		if(!l_system)
			return;
		l_new = *l_system;
		l_sys_edit = new CDialogSystemEdit();
		l_sys_edit->set_transient_for(*static_cast<Gtk::Window*>(CGelideUI::getInstance()));
		if(l_sys_edit->run(&l_new) == Gtk::RESPONSE_OK){
			m_smanager->updateSystem(l_iter, l_new);
			m_signal_changed.emit();
		}
		delete l_sys_edit;
	}
}

void CTreeViewSystems::onActionRefresh(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;
	Gtk::MessageDialog l_dialog(*static_cast<Gtk::Window*>(CGelideUI::getInstance()),
		_("This operation can not be undone."), false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL);
	CDialogGamelistGenerator* l_generator;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter){
			//Lanzar dialogo de progreso y actualización
			l_generator = new CDialogGamelistGenerator();
			l_generator->set_transient_for(*static_cast<Gtk::Window*>(CGelideUI::getInstance()));
			l_generator->run(m_smanager->getSystem(l_iter));
			m_signal_changed.emit();
			delete l_generator;
		}
	}
}

void CTreeViewSystems::onActionMoveFirst(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter)
			m_smanager->moveSystemFirst(l_iter);
	}
}

void CTreeViewSystems::onActionMoveUp(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter)
			m_smanager->moveSystemUp(l_iter);
	}
}

void CTreeViewSystems::onActionMoveDown(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter)
			m_smanager->moveSystemDown(l_iter);
	}
}

void CTreeViewSystems::onActionMoveLast(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter)
			m_smanager->moveSystemLast(l_iter);
	}
}
