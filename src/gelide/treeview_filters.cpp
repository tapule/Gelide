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

#include <stdio.h>
#include <gtkmm/icontheme.h>
#include "treeview_filters.hpp"
#include "../utils/utils.hpp"


CTreeViewFilters::CTreeViewFilters(void):
	Gtk::TreeView()
{
	Gtk::TreeModel::iterator l_iter, l_all, l_child;
	Glib::RefPtr<Gdk::Pixbuf> l_pixbuf;
	Glib::RefPtr<Gtk::TreeSelection> l_tree_selection;
	Gtk::CellRendererText* l_renderer_text;
	int l_index;
	char l_buffer[5];
	int l_year;

	// Obtenermos las instancias
	m_config = CConfig::getInstance();

	// Creación del listado y configuración del treeview
	m_tree = Gtk::TreeStore::create(m_columns);
	this->set_model(m_tree);
	this->set_rules_hint();
	this->set_headers_visible(false);
	this->set_enable_search(true);
	this->set_search_column(m_columns.m_name);
	//this->set_enable_tree_lines();
	this->get_selection()->set_mode(Gtk::SELECTION_SINGLE);

	// Creación de la columna Icono y Nombre
	this->append_column(_("Icon"), m_columns.m_icon);
	l_index = this->append_column(_("Name"), m_columns.m_name);
	l_renderer_text = (Gtk::CellRendererText*) this->get_column_cell_renderer(l_index -1);
	if(l_renderer_text){
#ifdef GLIBMM_PROPERTIES_ENABLED
		l_renderer_text->property_ellipsize() = Pango::ELLIPSIZE_END;
#else
		l_renderer_text->set_property("ellipsize", Pango::ELLIPSIZE_END);
#endif
	}

	GELIDE_DEBUG("Generating filters tree...");
	// Filtro todos
	l_all = addFilter(FILTER_TYPE_ALL, 0, this->render_icon(Gtk::StockID("gelide-all"), Gtk::ICON_SIZE_BUTTON), _("All"));
	// Filtro favoritos
	addFilter(FILTER_TYPE_FAVORITE, 1, 	this->render_icon(Gtk::StockID("gelide-favorite"), Gtk::ICON_SIZE_BUTTON), _("Favorites"));
	// Filtro jugados
	//l_pixbuf = this->render_icon(Gtk::StockID("gelide-played"), Gtk::ICON_SIZE_LARGE_TOOLBAR);
	addFilter(FILTER_TYPE_PLAYED, 1, this->render_icon(Gtk::StockID("gelide-played"), Gtk::ICON_SIZE_BUTTON), _("Played"));
	// Filtro no jugados
	addFilter(FILTER_TYPE_PLAYED, 0, this->render_icon(Gtk::StockID("gelide-played-neg"), Gtk::ICON_SIZE_BUTTON), _("Non played"));
	// Filtro funcionales
	//l_pixbuf = Gtk::IconTheme::get_default()->load_icon("gtk-ok", 48, Gtk::ICON_LOOKUP_USE_BUILTIN);
	addFilter(FILTER_TYPE_WORKING, 1, this->render_icon(Gtk::StockID("gelide-working"), Gtk::ICON_SIZE_BUTTON), _("Working"));
	// Filtro no funcionales
	//l_pixbuf = this->render_icon(Gtk::StockID("gelide-working-neg"), Gtk::ICON_SIZE_LARGE_TOOLBAR);
	addFilter(FILTER_TYPE_WORKING, 0, this->render_icon(Gtk::StockID("gelide-working-neg"), Gtk::ICON_SIZE_BUTTON), _("Non working"));
	// Filtro disponibles
	//l_pixbuf = Gtk::IconTheme::get_default()->load_icon("gtk-apply", 48, Gtk::ICON_LOOKUP_USE_BUILTIN);
	addFilter(FILTER_TYPE_AVAILABLE, 1, this->render_icon(Gtk::StockID("gelide-available"), Gtk::ICON_SIZE_BUTTON), _("Availables"));
	// Filtro no disponibles
	//l_pixbuf = this->render_icon(Gtk::StockID("gelide-available-neg"), Gtk::ICON_SIZE_LARGE_TOOLBAR);
	addFilter(FILTER_TYPE_AVAILABLE, 0, this->render_icon(Gtk::StockID("gelide-available-neg"), Gtk::ICON_SIZE_BUTTON), _("Non availables"));
	// Filtro desconocidos
	//l_pixbuf = Gtk::IconTheme::get_default()->load_icon("dialog-warning", 48, Gtk::ICON_LOOKUP_USE_BUILTIN);
	addFilter(FILTER_TYPE_UNKNOWN, 1, this->render_icon(Gtk::StockID("gelide-unknown"), Gtk::ICON_SIZE_BUTTON), _("Unknown"));
	// Filtro no desconocidos
	//l_pixbuf = this->render_icon(Gtk::StockID("gelide-unknown-neg"), Gtk::ICON_SIZE_LARGE_TOOLBAR);
	addFilter(FILTER_TYPE_UNKNOWN, 0, this->render_icon(Gtk::StockID("gelide-unknown-neg"), Gtk::ICON_SIZE_BUTTON), _("Non unknown"));
	// Contenedor para filtros de puntuación y filtros
	l_pixbuf = this->render_icon(Gtk::StockID("gelide-rank"), Gtk::ICON_SIZE_BUTTON);
	l_iter = addFilter(FILTER_TYPE_NOFILTER, 1, l_pixbuf, _("Rank"));
	addChildFilter(l_iter, FILTER_TYPE_RANK, 0, this->render_icon(Gtk::StockID("gelide-rank-neg"), Gtk::ICON_SIZE_BUTTON), "0");
	for(l_index = 1; l_index <= 5; l_index++){
		sprintf(l_buffer, "%i", l_index);
		addChildFilter(l_iter, FILTER_TYPE_RANK, l_index, l_pixbuf, l_buffer);
	}
	// Contenedor para filtros de años y filtros
	l_pixbuf = this->render_icon(Gtk::StockID("gelide-year"), Gtk::ICON_SIZE_BUTTON);
	l_iter = addFilter(FILTER_TYPE_NOFILTER, 2, l_pixbuf, _("Year"));
	addChildFilter(l_iter, FILTER_TYPE_YEAR, 0, l_pixbuf, _("Unknown"));
	l_year = utils::getYear();
	for(l_index = 1975; l_index <= l_year; l_index++){
		sprintf(l_buffer, "%i", l_index);
		addChildFilter(l_iter, FILTER_TYPE_YEAR, l_index, l_pixbuf, l_buffer);
	}

	// Forzamos la selección del filtro todos
	l_tree_selection = this->get_selection();
	l_tree_selection->select(l_all);
	// Conectamos la señal de cambio de filtro
	l_tree_selection->signal_changed().connect(sigc::mem_fun(*this, &CTreeViewFilters::onFilterChanged) );

	loadConfig();

}

CTreeViewFilters::~CTreeViewFilters(){
	saveConfig();
}

Gtk::TreeModel::iterator CTreeViewFilters::addFilter(const int p_type,
		const unsigned int p_value, Glib::RefPtr<Gdk::Pixbuf> p_pixbuf,
		Glib::ustring p_name)
{
	Gtk::TreeModel::iterator l_iter;
	Gtk::TreeModel::Row l_row;

	l_iter = m_tree->append();
	l_row = *l_iter;
	l_row[m_columns.m_icon] = p_pixbuf;
	l_row[m_columns.m_type] = p_type;
	l_row[m_columns.m_value] = p_value;
	l_row[m_columns.m_name] = p_name;

	return l_iter;
}

void CTreeViewFilters::addChildFilter(Gtk::TreeModel::iterator p_iter, const int p_type,
		const unsigned int p_value, Glib::RefPtr<Gdk::Pixbuf> p_pixbuf,
		Glib::ustring p_name)
{
	Gtk::TreeModel::iterator l_child;

	l_child = m_tree->append((*p_iter).children());
	(*l_child)[m_columns.m_icon] = p_pixbuf;
	(*l_child)[m_columns.m_type] = p_type;
	(*l_child)[m_columns.m_value] = p_value;
	(*l_child)[m_columns.m_name] = p_name;
}

void CTreeViewFilters::getFilter(int* p_type, unsigned int* p_value){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	if(l_iter){
		*p_type = (*l_iter)[m_columns.m_type];
		*p_value = (*l_iter)[m_columns.m_value];
	}
}

void CTreeViewFilters::onFilterChanged(void){
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::TreeModel::iterator l_iter;

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	if(l_iter && (*l_iter)[m_columns.m_type] != FILTER_TYPE_NOFILTER){
		m_signal_filter_changed.emit((*l_iter)[m_columns.m_type], (*l_iter)[m_columns.m_value]);
	}
}

// Señales emitida por el treeview al cambiar de filtro
sigc::signal<void, int, unsigned int> CTreeViewFilters::signalFilterChanged(void){
	return m_signal_filter_changed;
}

void CTreeViewFilters::loadConfig(void){
/*	Glib::ustring l_path;
	Gtk::TreeModel::Path l_tree_path;

	// Obtenemos la posición almacenada

	m_config->getKey(GELIDE_CFG_UI, "selected_filter", l_path);
	// Comprobamos que es válida
	if(l_path != ""){
		l_tree_path = m_tree->get_iter(l_path);
		this->expand_to_path(l_tree_path);
		this->get_selection()->select(l_tree_path);
		this->scroll_to_row(l_tree_path);
	}*/
}

void CTreeViewFilters::saveConfig(void){
//	m_config->setKey(GELIDE_CFG_UI, "selected_filter", m_tree->get_string(this->get_selection()->get_selected()));
}
