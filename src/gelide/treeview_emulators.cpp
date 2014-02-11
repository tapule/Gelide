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

#include "treeview_emulators.hpp"
#include "gelide_ui.hpp"
#include "dialog_emulator_edit.hpp"
#include "../utils/utils.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/aboutdialog.h>


CTreeViewEmulators::CTreeViewEmulators(void):
	Gtk::TreeView()
{
	Gtk::CellRendererText* l_renderer_text;
	Gtk::TreeViewColumn* l_column;
	int l_column_index;

	// Obtenermos la instancia del smanager
	m_smanager = CSystemManager::getInstance();

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
		l_column->set_cell_data_func(*l_renderer_text, sigc::mem_fun(*this, &CTreeViewEmulators::onSystemCellData));
#ifdef GLIBMM_PROPERTIES_ENABLED
		l_renderer_text->property_ellipsize() = Pango::ELLIPSIZE_END;
#else
		l_renderer_text->set_property("ellipsize", Pango::ELLIPSIZE_END);
#endif
	}
	// Conexión de la señal de cambio
	this->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &CTreeViewEmulators::onEmulatorChanged));

	initActions();
}

sigc::signal<void> CTreeViewEmulators::signalChanged(void){
	return m_signal_changed;
}

void CTreeViewEmulators::setSystem(Gtk::TreeModel::iterator& p_system){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	this->unset_model();

	if(p_system){
		m_system = m_smanager->getSystem(p_system);
		this->set_model(m_system->getEmulatorList());
		// Establecemos el emulador seleccionado en el sistema
		l_iter = m_system->getEmulator(m_system->getEmulatorId());
		l_selection = this->get_selection();
		if(l_iter)
	  		l_selection->select(l_iter);
		else
			l_selection->unselect_all();
	}
}

void CTreeViewEmulators::initActions(){
	Glib::ustring l_ui_info;

	// Creamos las acciones
	m_action_group = Gtk::ActionGroup::create();

	// Creamos las acciones del popup
	m_action_add = Gtk::Action::create("AddEmulator", Gtk::Stock::ADD);
	m_action_remove = Gtk::Action::create("RemoveEmulator", Gtk::Stock::REMOVE);
	m_action_edit = Gtk::Action::create("EditEmulator", Gtk::Stock::EDIT);
	m_action_about = Gtk::Action::create("AboutEmulator", Gtk::Stock::ABOUT);
	m_action_move = Gtk::Action::create("MenuMove", _("_Move"));
	m_action_first = Gtk::Action::create("MoveFirst", Gtk::Stock::GOTO_TOP, _("_First"));
	m_action_up = Gtk::Action::create("MoveUp", Gtk::Stock::GO_UP, _("_Up"));
	m_action_down = Gtk::Action::create("MoveDown", Gtk::Stock::GO_DOWN, _("_Down"));
	m_action_last = Gtk::Action::create("MoveLast", Gtk::Stock::GOTO_BOTTOM, _("_Last"));

	// Deshabilitamos remove, edit y acerca de
	m_action_remove->set_sensitive(false);
	m_action_edit->set_sensitive(false);
	m_action_about->set_sensitive(false);
	m_action_first->set_sensitive(false);
	m_action_up->set_sensitive(false);
	m_action_down->set_sensitive(false);
	m_action_last->set_sensitive(false);

	// Insertamos las acciones en el grupo de acciones
	m_action_group->add(m_action_add, sigc::mem_fun(*this, &CTreeViewEmulators::onActionAdd) );
	m_action_group->add(m_action_remove, sigc::mem_fun(*this, &CTreeViewEmulators::onActionRemove) );
	m_action_group->add(m_action_edit, sigc::mem_fun(*this, &CTreeViewEmulators::onActionEdit) );
	m_action_group->add(m_action_about, sigc::mem_fun(*this, &CTreeViewEmulators::onActionAbout) );
	m_action_group->add(m_action_move);
	m_action_group->add(m_action_first, sigc::mem_fun(*this, &CTreeViewEmulators::onActionMoveFirst) );
	m_action_group->add(m_action_up, sigc::mem_fun(*this, &CTreeViewEmulators::onActionMoveUp) );
	m_action_group->add(m_action_down, sigc::mem_fun(*this, &CTreeViewEmulators::onActionMoveDown) );
	m_action_group->add(m_action_last, sigc::mem_fun(*this, &CTreeViewEmulators::onActionMoveLast) );

	m_ui_manager = Gtk::UIManager::create();
	m_ui_manager->insert_action_group(m_action_group);

	l_ui_info =
		"<ui>"
		"	<popup name='EmulatorsPopup'>"
		"		<menuitem action='AddEmulator'/>"
		"		<menuitem action='RemoveEmulator'/>"
		"		<menuitem action='EditEmulator'/>"
		"		<separator />"
		"		<menu action='MenuMove'>"
		"			<menuitem action='MoveFirst' />"
		"			<menuitem action='MoveUp' />"
		"			<menuitem action='MoveDown' />"
		"			<menuitem action='MoveLast' />"
		"		</menu>"
		"		<separator />"
		"		<menuitem action='AboutEmulator'/>"
		"	</popup>"
		"</ui>";

	try{
		m_ui_manager->add_ui_from_string(l_ui_info);
        m_popup_menu = static_cast<Gtk::Menu*>(m_ui_manager->get_widget("/EmulatorsPopup"));
		assert(m_popup_menu);
	}
	catch (Glib::Error& l_exception)
	{
		GELIDE_ERROR("Generating emulators popup (" << l_exception.what().c_str() << ")");
		throw l_exception;
	}
}

void CTreeViewEmulators::onSystemCellData(Gtk::CellRenderer* p_renderer,
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

void CTreeViewEmulators::onEmulatorChanged(void){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	if(m_system){
		l_selection = this->get_selection();
		l_iter = l_selection->get_selected();
		if(l_iter)
			m_system->setEmulatorId((*l_iter)[m_columns.m_id]);
	}

	// Emitimos una señal de cambio
	m_signal_changed.emit();
}

bool CTreeViewEmulators::on_button_press_event(GdkEventButton* p_event){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	Gtk::TreeView::on_button_press_event(p_event);

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	if((p_event->type == GDK_BUTTON_PRESS) && (p_event->button == 3) ) {
		// Actualizamos el menú dependiendo de la selección y del sistema
		if(m_system)
			m_action_add->set_sensitive(true);
		else
			m_action_add->set_sensitive(false);

		if(l_iter){
			m_action_remove->set_sensitive(true);
			m_action_edit->set_sensitive(true);
			m_action_about->set_sensitive(true);
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
			m_action_about->set_sensitive(false);
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
			onActionAbout();
			return true;
		}
	}

	return false;
}

void CTreeViewEmulators::onActionAdd(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	CDialogEmulatorEdit* l_emu_edit;
	Gtk::TreeModel::iterator l_iter;
	CEmulator* l_emulator;

	l_emu_edit = new CDialogEmulatorEdit();
	l_emu_edit->set_transient_for(*static_cast<Gtk::Window*>(CGelideUI::getInstance()));
	l_emulator = new CEmulator();
	l_emulator->setIcon(m_smanager->getEmulatorDefaultIcon());
	if(l_emu_edit->run(l_emulator) == Gtk::RESPONSE_OK){
		l_iter = m_system->addEmulator(*l_emulator);
		if(l_iter){
			l_selection = this->get_selection();
			if(l_selection){
				l_selection->select(l_iter);
				m_action_remove->set_sensitive(true);
				m_action_edit->set_sensitive(true);
			}
		}
	}
	else
		delete l_emulator;
	delete l_emu_edit;
}

void CTreeViewEmulators::onActionRemove(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;
	Gtk::MessageDialog l_dialog(*static_cast<Gtk::Window*>(CGelideUI::getInstance()),
		_("This operation can not be undone."), false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL);

	//"Si continua eliminará los datos del emulador."
	l_dialog.set_secondary_text( _("Emulator data will be lost if you continue."));
	// Mostramos un mensaje avisando de la operación
	if(l_dialog.run() == Gtk::RESPONSE_CANCEL)
		return;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter){
			m_system->removeEmulator(l_iter);
			l_iter = m_system->getEmulatorList()->children().begin();
			// Comprobamos si hay algún elemento y lo seleccionamos
			if(l_iter)
				l_selection->select(l_iter);
			else{
				m_action_remove->set_sensitive(false);
				m_action_edit->set_sensitive(false);
			}
		}
	}
}

void CTreeViewEmulators::onActionEdit(void){
	CDialogEmulatorEdit* l_emu_edit;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;
	CEmulator* l_emulator;
	CEmulator l_new;

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	if(l_iter){
		l_emulator = m_system->getEmulator(l_iter);
		if(!l_emulator)
			return;
		l_new = *l_emulator;
		l_emu_edit = new CDialogEmulatorEdit();
		l_emu_edit->set_transient_for(*static_cast<Gtk::Window*>(CGelideUI::getInstance()));
		if(l_emu_edit->run(&l_new) == Gtk::RESPONSE_OK){
			m_system->updateEmulator(l_iter, l_new);
			// Emitimos una señal de cambio
			m_signal_changed.emit();
		}
		delete l_emu_edit;
		delete l_emulator;
	}
}

void CTreeViewEmulators::onActionAbout(void){
	Gtk::AboutDialog l_about;
	Gtk::TreeModel::iterator l_iter;
	std::vector<Glib::ustring> l_authors;
	Glib::RefPtr<Gdk::Pixbuf> l_logo;

	l_iter = this->get_selection()->get_selected();
	if(!l_iter)
		return;

	l_about.set_transient_for(*static_cast<Gtk::Window*>(CGelideUI::getInstance()));
	l_about.set_size_request(300, -1);
	l_about.set_resizable(true);
	l_about.set_name((*l_iter)[m_columns.m_name]);
	l_about.set_version((*l_iter)[m_columns.m_version]);
	if((*l_iter)[m_columns.m_author] != ""){
		l_about.set_copyright("Copyright © " + (*l_iter)[m_columns.m_author]);
		l_authors.push_back((*l_iter)[m_columns.m_author]);
		l_about.set_authors(l_authors);
	}

	l_about.set_comments((*l_iter)[m_columns.m_description]);
	l_about.set_website((*l_iter)[m_columns.m_homepage]);
	l_about.set_website_label((*l_iter)[m_columns.m_homepage]);

	if((*l_iter)[m_columns.m_icon_file] != ""){
		try{
			l_logo = Gdk::Pixbuf::create_from_file((Glib::ustring)(*l_iter)[m_columns.m_icon_file])->scale_simple(128, 128, Gdk::INTERP_BILINEAR);
			l_about.set_logo(l_logo);
		}
		catch (const Glib::Error& l_exception){
			GELIDE_WARNING("Loading emulator icon (" << l_exception.what().c_str() << ")");
			l_about.set_logo((*l_iter)[m_columns.m_icon]);
		}
	}
	else
		l_about.set_logo( (*l_iter)[m_columns.m_icon]);

	l_about.run();
}


void CTreeViewEmulators::onActionMoveFirst(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter){
			// Intercambiamos las filas en el listado de columnas
			l_dest = this->get_model()->children().begin();
			m_system->getEmulatorList()->move(l_iter, l_dest);
		}
	}
}

void CTreeViewEmulators::onActionMoveUp(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_orig;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter){
			// Movemos arriba en el modelo de sistemas
			l_orig = l_iter--;
			m_system->getEmulatorList()->move(l_orig, l_iter);
		}
	}
}

void CTreeViewEmulators::onActionMoveDown(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_orig;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter){
			// Movemos arriba en el modelo de sistemas
			l_orig = l_iter++;
			++l_iter;
			m_system->getEmulatorList()->move(l_orig, l_iter);
		}
	}
}

void CTreeViewEmulators::onActionMoveLast(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter, l_dest;

	l_selection = this->get_selection();
	if(l_selection){
		l_iter = l_selection->get_selected();
		if(l_iter){
			// Intercambiamos las filas en el listado de columnas
			l_dest = this->get_model()->children().end();
			m_system->getEmulatorList()->move(l_iter, l_dest);
		}
	}
}
