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

#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include "treeview_games.hpp"
#include "gelide_ui.hpp"
#include "dialog_emulator_launcher.hpp"
#include "../utils/tokenizer.hpp"
#include "../utils/utils.hpp"


CTreeViewGames::CTreeViewGames(void):
	Gtk::TreeView()
{
	Gtk::TreeView::Column* l_column;
	Gtk::CellRendererPixbuf* l_renderer_pixbuf;
	Gtk::CellRendererText* l_renderer_text;
	Gtk::TreeModel::iterator l_treemodel_iter;
	Gtk::TreeModel::Row l_row;
	int l_index;

	m_config = CConfig::getInstance();
	m_smanager = CSystemManager::getInstance();
	m_system = NULL;

	m_filter_type = 0;
	m_filter_value = 0;
	//m_filter_str = "";
	m_apply_filter_regex = false;

	initFlagsIcons();
	initActions();

	this->set_rules_hint();
	this->set_headers_visible(true);
	this->set_headers_clickable(true);
	this->set_reorderable(false);
	this->set_enable_search(true);
	this->set_search_column(m_columns.m_name);
	this->get_selection()->set_mode(Gtk::SELECTION_SINGLE);

	// Creación de la columna Favorito
	l_column = Gtk::manage( new Gtk::TreeView::Column(_("Favorite")));
	// Guardamos el puntero de la columna para identificarla
	m_columns_id[TREEVIEW_FAVORITE_COLUMN_ID] = l_column;
	// Renderizador para el icono favorito
	l_renderer_pixbuf = Gtk::manage(new Gtk::CellRendererPixbuf());
#ifdef GLIBMM_PROPERTIES_ENABLED
	l_renderer_pixbuf->property_xalign() = 0.5;
#else
	l_renderer_pixbuf->set_property("xalign", 0.5);
#endif
	l_column->pack_start(*l_renderer_pixbuf, false);
	// Indicamos el callback y asociamos vários parámetros con sigc::bind
	l_column->set_cell_data_func(*l_renderer_pixbuf, sigc::mem_fun(*this, &CTreeViewGames::renderFavorite));
	l_column->set_sort_column(m_columns.m_favorite);
	l_column->set_sizing(Gtk::TREE_VIEW_COLUMN_AUTOSIZE);
	l_column->set_resizable(false);
	l_column->set_reorderable(true);
	this->append_column(*l_column);

	// Creación de la columna Nombre
	l_column = Gtk::manage(new Gtk::TreeViewColumn(_("Name")));
	// Guardamos el puntero de la columna para identificarla
	m_columns_id[TREEVIEW_NAME_COLUMN_ID] = l_column;
	// Añadimos la columna del nombre al final, al principio va el icono
	l_column->pack_start(m_columns.m_name);
	// Insertamos la columna en el treeview
	l_index = this->append_column(*l_column);
	// Establecemos algunas propiedades
	l_column->set_sort_column(m_columns.m_name);
	l_column->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
	l_column->set_fixed_width(250);
	l_column->set_resizable(true);
	l_column->set_reorderable(true);
	// Obtenemos el renderizador de texto de la columna
	l_renderer_text = dynamic_cast<Gtk::CellRendererText*> (this->get_column_cell_renderer(l_index -1));
#ifdef GLIBMM_PROPERTIES_ENABLED
	l_renderer_text->property_ellipsize() = Pango::ELLIPSIZE_END;
#else
	l_renderer_text->set_property("ellipsize", Pango::ELLIPSIZE_END);
#endif

	// Creación de la columna Flags
	l_column = Gtk::manage( new Gtk::TreeView::Column(_("Flags")));
	// Guardamos el puntero de la columna para identificarla
	m_columns_id[TREEVIEW_FLAGS_COLUMN_ID] = l_column;
	// Renderizador para el icono favorito
	l_renderer_pixbuf = Gtk::manage(new Gtk::CellRendererPixbuf());
	l_column->pack_start(*l_renderer_pixbuf, false);
	// Indicamos el callback y asociamos vários parámetros con sigc::bind
	l_column->set_cell_data_func(*l_renderer_pixbuf,
		sigc::bind< int, Glib::RefPtr<Gdk::Pixbuf>, Glib::RefPtr<Gdk::Pixbuf> >(
			sigc::mem_fun(*this, &CTreeViewGames::renderFlag), 0x08, m_flag_played[0], m_flag_played[1]
		)
	);
	// Renderizador para el icono funcional
	l_renderer_pixbuf = Gtk::manage(new Gtk::CellRendererPixbuf());
	l_column->pack_start(*l_renderer_pixbuf, false);
	l_column->set_cell_data_func(*l_renderer_pixbuf,
		sigc::bind<int, Glib::RefPtr<Gdk::Pixbuf>, Glib::RefPtr<Gdk::Pixbuf> >(
			sigc::mem_fun(*this, &CTreeViewGames::renderFlag), 0x04, m_flag_working[0], m_flag_working[1]
		)
	);
	// Renderizador para el icono presente
	l_renderer_pixbuf = Gtk::manage(new Gtk::CellRendererPixbuf());
	l_column->pack_start(*l_renderer_pixbuf, false);
	l_column->set_cell_data_func(*l_renderer_pixbuf,
		sigc::bind<int, Glib::RefPtr<Gdk::Pixbuf>, Glib::RefPtr<Gdk::Pixbuf> >(
			sigc::mem_fun(*this, &CTreeViewGames::renderFlag), 0x02, m_flag_available[0], m_flag_available[1]
		)
	);
	// Renderizador para el icono desconocido
	l_renderer_pixbuf = Gtk::manage(new Gtk::CellRendererPixbuf());
	l_column->pack_start(*l_renderer_pixbuf, false);
	l_column->set_cell_data_func(*l_renderer_pixbuf,
		sigc::bind<int, Glib::RefPtr<Gdk::Pixbuf>, Glib::RefPtr<Gdk::Pixbuf> >(
			sigc::mem_fun(*this, &CTreeViewGames::renderFlag), 0x01, m_flag_unknown[0], m_flag_unknown[1]
		)
	);
	// Establecemos algunas propiedades para la columna
	l_column->set_sort_column(m_columns.m_flags);
	l_column->set_sizing(Gtk::TREE_VIEW_COLUMN_AUTOSIZE);
	l_column->set_resizable(false);
	l_column->set_reorderable(true);
	this->append_column(*l_column);

	// Creación de la columna puntuación
	// CHECKME: Se podría hacer la puntuación con imagenes de estrellas
	l_column = Gtk::manage(new Gtk::TreeViewColumn(_("Rank")));
	m_columns_id[TREEVIEW_RANK_COLUMN_ID] = l_column;
	l_column->pack_start(m_columns.m_rank);
	l_column->set_sort_column(m_columns.m_rank);
	l_column->set_sizing(Gtk::TREE_VIEW_COLUMN_AUTOSIZE);
	l_column->set_resizable(false);
	l_column->set_reorderable(true);
	l_index = this->append_column(*l_column);
	l_renderer_text = dynamic_cast<Gtk::CellRendererText*> (this->get_column_cell_renderer(l_index -1));
#ifdef GLIBMM_PROPERTIES_ENABLED
	l_renderer_text->property_xalign() = 0.5;
#else
	l_renderer_text->set_property("xalign", 0.5);
#endif

	// Creación de la columna Año
	l_column = Gtk::manage(new Gtk::TreeViewColumn(_("Year")));
	m_columns_id[TREEVIEW_YEAR_COLUMN_ID] = l_column;
	l_column->pack_start(m_columns.m_year);
	l_column->set_sort_column(m_columns.m_year);
	l_column->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
	l_column->set_fixed_width(100);
	l_column->set_resizable(true);
	l_column->set_reorderable(true);
	l_index = this->append_column(*l_column);
	// Obtenemos el renderizador de texto de la columna
	l_renderer_text = dynamic_cast<Gtk::CellRendererText*> (this->get_column_cell_renderer(l_index -1));
#ifdef GLIBMM_PROPERTIES_ENABLED
	l_renderer_text->property_ellipsize() = Pango::ELLIPSIZE_END;
	l_renderer_text->property_xalign() = 0.5;
#else
	l_renderer_text->set_property("ellipsize", Pango::ELLIPSIZE_END);
	l_renderer_text->set_property("xalign", 0.5);
#endif
	// Callback para el renderizado del texto en el nombre
	l_column->set_cell_data_func(*l_renderer_text, sigc::mem_fun(*this, &CTreeViewGames::renderYearCell));

	// Creación de la columna Descripción
	l_column = Gtk::manage(new Gtk::TreeViewColumn(_("Description")));
	m_columns_id[TREEVIEW_DESCRIPTION_COLUMN_ID] = l_column;
	l_column->pack_start(m_columns.m_description);
	l_column->set_sort_column(m_columns.m_description);
	l_column->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
	l_column->set_fixed_width(225);
	l_column->set_resizable(true);
	l_column->set_reorderable(true);
	l_index = this->append_column(*l_column);
	// Obtenemos el renderizador de texto de la columna
	l_renderer_text = dynamic_cast<Gtk::CellRendererText*> (this->get_column_cell_renderer(l_index -1));
#ifdef GLIBMM_PROPERTIES_ENABLED
	l_renderer_text->property_ellipsize() = Pango::ELLIPSIZE_END;
#else
	l_renderer_text->set_property("ellipsize", Pango::ELLIPSIZE_END);
#endif

	// Creación de la columna Fabricante
	l_column = Gtk::manage(new Gtk::TreeViewColumn(_("Manufacturer")));
	m_columns_id[TREEVIEW_MANUFACTURER_COLUMN_ID] = l_column;
	l_column->pack_start(m_columns.m_manufacturer);
	l_column->set_sort_column(m_columns.m_manufacturer);
	l_column->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
	l_column->set_fixed_width(150);
	l_column->set_resizable(true);
	l_column->set_reorderable(true);
	l_index = this->append_column(*l_column);
	// Obtenemos el renderizador de texto de la columna
	l_renderer_text = dynamic_cast<Gtk::CellRendererText*> (this->get_column_cell_renderer(l_index -1));
#ifdef GLIBMM_PROPERTIES_ENABLED
	l_renderer_text->property_ellipsize() = Pango::ELLIPSIZE_END;
#else
	l_renderer_text->set_property("ellipsize", Pango::ELLIPSIZE_END);
#endif

	// CHECKME: De momento no se han añadido las columnas path y rom_crc

	// Conexión de la señal de cambio
	this->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &CTreeViewGames::onGameChanged));

	// Cargamos la configuración
	loadConfig();

	// Mostramos todos los widgets
	this->show_all_children();
}

CTreeViewGames::~CTreeViewGames(){
	saveConfig();
}

sigc::signal<void> CTreeViewGames::signalChanged(void){
	return m_signal_changed;
}

void CTreeViewGames::setSystem(Gtk::TreeModel::iterator& p_system){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	// Obtenemos la columna de ordenación y su tipo de orden
	if(m_sort_list)
		m_sort_list->get_sort_column_id(m_sort_column, m_sort_type);

	this->unset_model();
	m_filter_list.reset();
	m_sort_list.reset();
	m_system = NULL;

	if(p_system){
		m_system = m_smanager->getSystem(p_system);
		m_filter_list = Gtk::TreeModelFilter::create(m_system->getGameList());
		m_sort_list = Gtk::TreeModelSort::create(m_filter_list);
		m_sort_list->set_sort_column(m_sort_column, m_sort_type);
		m_filter_list->set_visible_func(sigc::mem_fun(*this, &CTreeViewGames::visibleFunction));
		this->set_model(m_sort_list);
		this->set_search_column(m_columns.m_name);
	}
}

Gtk::TreeModel::iterator CTreeViewGames::getSelected(void){
	// TODO: Esto puede dar error, hay que cambiarlo para recibirlo como parámetro
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	return l_iter;
}

void CTreeViewGames::initFlagsIcons(void){
	m_flag_favorite[0] = this->render_icon(Gtk::StockID("gelide-favorite"), Gtk::ICON_SIZE_MENU);
	m_flag_played[0] = this->render_icon(Gtk::StockID("gelide-played"), Gtk::ICON_SIZE_MENU);
	m_flag_working[0] = this->render_icon(Gtk::StockID("gelide-working"), Gtk::ICON_SIZE_MENU);
	m_flag_available[0] = this->render_icon(Gtk::StockID("gelide-available"), Gtk::ICON_SIZE_MENU);
	m_flag_unknown[0] = this->render_icon(Gtk::StockID("gelide-unknown"), Gtk::ICON_SIZE_MENU);

	m_flag_favorite[1] = this->render_icon(Gtk::StockID("gelide-favorite-neg"), Gtk::ICON_SIZE_MENU);
	m_flag_played[1] = this->render_icon(Gtk::StockID("gelide-played-neg"), Gtk::ICON_SIZE_MENU);
	m_flag_working[1] = this->render_icon(Gtk::StockID("gelide-working-neg"), Gtk::ICON_SIZE_MENU);
	m_flag_available[1] = this->render_icon(Gtk::StockID("gelide-available-neg"), Gtk::ICON_SIZE_MENU);
	m_flag_unknown[1] = this->render_icon(Gtk::StockID("gelide-unknown-neg"), Gtk::ICON_SIZE_MENU);
}

void CTreeViewGames::loadConfig(void){
	std::vector<bool> l_visibility;
	std::vector<int> l_widths;
	std::vector<int> l_order;
	Gtk::TreeViewColumn* l_column;
	int l_ind, l_sort_type;

	// Obtenemos los valores de visibilidad, y anchuras y orden
	//m_config->getUIColumns(l_visibility, l_widths, l_order);
	m_config->getKey(GELIDE_CFG_UI, "columns_visibility", l_visibility);
	m_config->getKey(GELIDE_CFG_UI, "columns_widths", l_widths);
	m_config->getKey(GELIDE_CFG_UI, "columns_order", l_order);

	// Comprobamos si la configuración tiene todos los datos
	if(!l_visibility.size() || !l_widths.size() || !l_order.size())
		return;

	for(l_ind = 0; l_ind < TREEVIEW_COLUMNS; l_ind++){
		m_columns_id[l_ind]->set_visible(l_visibility[l_ind]);
		if(l_widths[l_ind] > 0)
			m_columns_id[l_ind]->set_fixed_width(l_widths[l_ind]);
	}

	// Colocamos la primera columna
	l_column = m_columns_id[l_order[0]];
	this->move_column_to_start(*l_column);
	// Colocamos el resto
	for(l_ind = 1; l_ind < TREEVIEW_COLUMNS; l_ind++){
		// Obtenemos la columna que se debe colocar a continuación
		l_column = m_columns_id[l_order[l_ind]];
		this->move_column_after(*l_column, *(m_columns_id[l_order[l_ind - 1]]));
	}

	// Obtenemos la columna de ordenación y su tipo de orden
	//m_config->getUISortColumn(m_sort_column, l_sort_type);
	m_config->getKey(GELIDE_CFG_UI, "sort_column", m_sort_column);
	m_config->getKey(GELIDE_CFG_UI, "sort_type", l_sort_type);
	m_sort_type = l_sort_type == 0 ? Gtk::SORT_ASCENDING : Gtk::SORT_DESCENDING;

}

void CTreeViewGames::saveConfig(void){
	std::vector<bool> l_visibility;
	std::vector<int> l_widths;
	std::vector<int> l_order;
	std::list<Gtk::TreeViewColumn*> l_columns;
	std::list<Gtk::TreeViewColumn*>::iterator l_iter;
	int l_ind;

	// Construimos los listados de visibilidad, anchuras
	l_columns = this->get_columns();
	for(l_ind = 0; l_ind < TREEVIEW_COLUMNS; l_ind++){
		l_visibility.push_back(m_columns_id[l_ind]->get_visible());
		l_widths.push_back(m_columns_id[l_ind]->get_width());
	}
	// Construimos la lista de orden. Almacenamos en cada posición del orden
	// el id de la columna que se debe colocar
	for(l_iter = l_columns.begin(); l_iter != l_columns.end(); l_iter++){
		// Buscamos la columna que está en esta posición
		for(l_ind = 0; l_ind < TREEVIEW_COLUMNS; l_ind++){
			if((*l_iter) == m_columns_id[l_ind]){
				l_order.push_back(l_ind);
				break;
			}
		}
	}

	//m_config->setUIColumns(l_visibility, l_widths, l_order);
	m_config->setKey(GELIDE_CFG_UI, "columns_visibility", l_visibility);
	m_config->setKey(GELIDE_CFG_UI, "columns_widths", l_widths);
	m_config->setKey(GELIDE_CFG_UI, "columns_order", l_order);
	// Obtenemos la columna de ordenación y su tipo de orden
	if(m_sort_list)
		m_sort_list->get_sort_column_id(m_sort_column, m_sort_type);
	//m_config->setUISortColumn(m_sort_column, m_sort_type == Gtk::SORT_ASCENDING ? 0 : 1);
	m_config->setKey(GELIDE_CFG_UI, "sort_column", m_sort_column);
	m_config->setKey(GELIDE_CFG_UI, "sort_type", m_sort_type);
}

unsigned int CTreeViewGames::getGames(void){
	if(m_system)
		return (unsigned int) m_sort_list->children().size();
	else
		return 0;
}

void CTreeViewGames::setFilter(const int p_type, const unsigned int p_value){
	m_filter_type = p_type;
	m_filter_value = p_value;
	if(m_system)
		m_filter_list->refilter();
}

void CTreeViewGames::setNameFilter(Glib::ustring p_str){
	int l_ind;

	// La aplicación del filtro dependerá de esta variable
	m_apply_filter_regex = (p_str != "");

	// Crearemos la expresión regular solo si es necesario
	if(m_apply_filter_regex){
		// Escapamos caracteres especiales en la cadena y añadimos marcador de inicio
		p_str = "^" + Glib::Regex::escape_string(p_str);
		// Restauramos las apariciones de * para permitir la sustitución de 0 o
		// más caracteres
		l_ind = p_str.find("\\*");
		while(l_ind > -1){
			p_str = p_str.replace(l_ind, 2, ".*");
			l_ind = p_str.find("\\*", l_ind + 1);
		}
		// Restauramos las apariciones de ? para permitir la sustitución de 0 o
		// 1 caracter
		l_ind = p_str.find("\\?");
		while(l_ind > -1){
			p_str = p_str.replace(l_ind, 2, ".?");
			l_ind = p_str.find("\\?", l_ind + 1);
		}

		// Creamos la expresión regular para el filtrado por nombre
		m_filter_regex = Glib::Regex::create(p_str, Glib::REGEX_CASELESS|Glib::REGEX_OPTIMIZE);
	}

	m_filter_list->refilter();
}

void CTreeViewGames::renderFavorite(Gtk::CellRenderer *p_renderer, const Gtk::TreeModel::iterator& p_iter){
	Gtk::TreeModel::Row l_row;
	Gtk::CellRendererPixbuf* l_renderer;

	assert(p_renderer);

	l_row = *p_iter;
	l_renderer = dynamic_cast<Gtk::CellRendererPixbuf*>(p_renderer);
	if ((l_row[m_columns.m_favorite])){
#ifdef GLIBMM_PROPERTIES_ENABLED
		l_renderer->property_pixbuf() = m_flag_favorite[0];
#else
		l_renderer->set_property("pixbuf", m_flag_favorite[0]);
#endif
	}
	else{
#ifdef GLIBMM_PROPERTIES_ENABLED
		l_renderer->property_pixbuf() = m_flag_favorite[1];
#else
		l_renderer->set_property("pixbuf", m_flag_favorite[1]);
#endif
	}
}

void CTreeViewGames::renderFlag(Gtk::CellRenderer *p_renderer, const Gtk::TreeModel::iterator& p_iter,
		int p_mask, Glib::RefPtr<Gdk::Pixbuf> p_active, Glib::RefPtr<Gdk::Pixbuf> p_inactive){
	Gtk::TreeModel::Row l_row;
	Gtk::CellRendererPixbuf* l_renderer;

	assert(p_renderer);

	l_row = *p_iter;
	l_renderer = dynamic_cast<Gtk::CellRendererPixbuf*>(p_renderer);

	if (l_row[m_columns.m_flags] & p_mask){
#ifdef GLIBMM_PROPERTIES_ENABLED
		l_renderer->property_pixbuf() = p_active;
#else
		l_renderer->set_property("pixbuf", p_active);
#endif
	}
	else{
#ifdef GLIBMM_PROPERTIES_ENABLED
		l_renderer->property_pixbuf() = p_inactive;
#else
		l_renderer->set_property("pixbuf", p_inactive);
#endif
	}
}

void CTreeViewGames::renderYearCell(Gtk::CellRenderer* p_renderer, const Gtk::TreeModel::iterator& p_iter){
	Gtk::TreeModel::Row l_row;
	Gtk::CellRendererText* l_renderer;

	if(p_iter){
		l_row = *p_iter;
		l_renderer = dynamic_cast<Gtk::CellRendererText*>(p_renderer);

		if(l_row[m_columns.m_year] == 0){
#ifdef GLIBMM_PROPERTIES_ENABLED
			l_renderer->property_text() = _("Unknown");
#else
			l_renderer->set_property("text", _("Unknown"));
#endif
		}
	}
}

bool CTreeViewGames::visibleFunction(const Gtk::TreeModel::const_iterator& p_iter){
	int l_ret;
	Glib::ustring l_name;

	if(!m_system)
		return false;

	switch(m_filter_type){
	case FILTER_TYPE_FAVORITE:
		l_ret = (*p_iter)[m_columns.m_favorite];
		l_ret = ((bool)l_ret == m_filter_value);
		break;
	case FILTER_TYPE_PLAYED:
		l_ret = ((*p_iter)[m_columns.m_flags] & 0x08);
		l_ret = ((bool)l_ret == m_filter_value);
		break;
	case FILTER_TYPE_WORKING:
		l_ret = ((*p_iter)[m_columns.m_flags] & 0x04);
		l_ret = ((bool)l_ret == m_filter_value);
		break;
	case FILTER_TYPE_AVAILABLE:
		l_ret = ((*p_iter)[m_columns.m_flags] & 0x02);
		l_ret = ((bool)l_ret == m_filter_value);
		break;
	case FILTER_TYPE_UNKNOWN:
		l_ret = ((*p_iter)[m_columns.m_flags] & 0x01);
		l_ret = ((bool)l_ret == m_filter_value);
		break;
	case FILTER_TYPE_RANK:
		l_ret = ((*p_iter)[m_columns.m_rank] == m_filter_value);
		break;
	case FILTER_TYPE_YEAR:
		l_ret = ((*p_iter)[m_columns.m_year] == m_filter_value);
		break;
	default:
		l_ret = 1;
	}
	if((!l_ret) ||  (!m_apply_filter_regex))
		return l_ret;
	else{
		l_name = (*p_iter)[m_columns.m_description];
		return (m_filter_regex->match(l_name));
	}
}

void CTreeViewGames::initActions(){
	Glib::ustring l_ui_info;

	// Creamos las acciones
	m_action_group = Gtk::ActionGroup::create();

	// Creamos las acciones del popup de los juegos
	m_action_playgame = Gtk::Action::create("PlayGame", Gtk::Stock::MEDIA_PLAY,	_("_Launch game"));
	m_action_favorite = Gtk::ToggleAction::create("Favorite", _("_Favorite"));
	m_action_played = Gtk::ToggleAction::create("Played", _("_Played"));
	m_action_working = Gtk::ToggleAction::create("Working", _("_Working"));
	m_action_rank = Gtk::Action::create("MenuRank", _("Ran_k"));
	m_action_remove = Gtk::Action::create("Remove",  Gtk::Stock::REMOVE,_("_Remove"));
	// Creamos las acciones para el submenú rank
	Gtk::RadioAction::Group l_rank_group;
	m_action_ranks[0] = Gtk::RadioAction::create(l_rank_group, "Rank0", _("_0"));
	m_action_ranks[1] = Gtk::RadioAction::create(l_rank_group, "Rank1", _("_1"));
	m_action_ranks[2] = Gtk::RadioAction::create(l_rank_group, "Rank2", _("_2"));
	m_action_ranks[3] = Gtk::RadioAction::create(l_rank_group, "Rank3", _("_3"));
	m_action_ranks[4] = Gtk::RadioAction::create(l_rank_group, "Rank4", _("_4"));
	m_action_ranks[5] = Gtk::RadioAction::create(l_rank_group, "Rank5", _("_5"));

	// Insertamos las acciones en el grupo de acciones
	m_action_group->add(m_action_playgame, sigc::mem_fun(*this, &CTreeViewGames::onActionPlaygame) );
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
	m_action_group->add(m_action_remove, sigc::mem_fun(*this, &CTreeViewGames::onActionRemove) );

	// Conectamos las acciones y guardamos la conexión
	m_conn_favorite = m_action_favorite->signal_activate().connect(sigc::mem_fun(*this, &CTreeViewGames::onActionFavorite));
	m_conn_played = m_action_played->signal_activate().connect(sigc::mem_fun(*this, &CTreeViewGames::onActionPlayed));
	m_conn_working = m_action_working->signal_activate().connect(sigc::mem_fun(*this, &CTreeViewGames::onActionWorking));
	m_conn_ranks[0] = m_action_ranks[0]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CTreeViewGames::onActionRank), 0));
	m_conn_ranks[1] = m_action_ranks[1]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CTreeViewGames::onActionRank), 1));
	m_conn_ranks[2] = m_action_ranks[2]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CTreeViewGames::onActionRank), 2));
	m_conn_ranks[3] = m_action_ranks[3]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CTreeViewGames::onActionRank), 3));
	m_conn_ranks[4] = m_action_ranks[4]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CTreeViewGames::onActionRank), 4));
	m_conn_ranks[5] = m_action_ranks[5]->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*this, &CTreeViewGames::onActionRank), 5));

	m_ui_manager = Gtk::UIManager::create();
	m_ui_manager->insert_action_group(m_action_group);

	l_ui_info =
		"<ui>"
		"	<popup name='GameListPopup'>"
		"		<menuitem action='PlayGame'/>"
		"		<separator />"
		"		<menuitem action='Favorite'/>"
		"		<menuitem action='Played'/>"
		"		<menuitem action='Working'/>"
		"		<menu action='MenuRank'>"
		"			<menuitem action='Rank0' />"
		"			<menuitem action='Rank1' />"
		"			<menuitem action='Rank2' />"
		"			<menuitem action='Rank3' />"
		"			<menuitem action='Rank4' />"
		"			<menuitem action='Rank5' />"
		"		</menu>"
		"		<separator />"
		"		<menuitem action='Remove'/>"
		"	</popup>"
		"</ui>";

	try{
		m_ui_manager->add_ui_from_string(l_ui_info);
        m_gamelist_popup = static_cast<Gtk::Menu*>(m_ui_manager->get_widget("/GameListPopup"));
		assert(m_gamelist_popup);
	}
	catch (Glib::Error& l_exception)
	{
		GELIDE_ERROR("Generating games popup (" << l_exception.what().c_str() << ")");
		throw l_exception;
	}
}


void CTreeViewGames::blockSignals(void){
	int l_i;

	m_conn_favorite.block();
	m_conn_played.block();
	m_conn_working.block();
	for(l_i = 0; l_i <= 5; l_i++)
		m_conn_ranks[l_i].block();
}

void CTreeViewGames::unblockSignals(void){
	int l_i;

	m_conn_favorite.unblock();
	m_conn_played.unblock();
	m_conn_working.unblock();
	for(l_i = 0; l_i <= 5; l_i++)
		m_conn_ranks[l_i].unblock();
}

bool CTreeViewGames::on_button_press_event(GdkEventButton* p_event){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;

	Gtk::TreeView::on_button_press_event(p_event);
	if((p_event->type == GDK_BUTTON_PRESS) && (p_event->button == 3) ) {
		// Actualizamos el menú dependiendo de la selección
		l_selection = this->get_selection();
		l_iter = l_selection->get_selected();
		if(l_iter){
			blockSignals();
			m_action_playgame->set_sensitive(true);
			m_action_favorite->set_sensitive(true);
			m_action_favorite->set_active((*l_iter)[m_columns.m_favorite]);
	  		m_action_played->set_sensitive(true);
	  		m_action_played->set_active((*l_iter)[m_columns.m_flags] & 0x08);
			m_action_working->set_sensitive(true);
	  		m_action_working->set_active((*l_iter)[m_columns.m_flags] & 0x04);
	  		m_action_rank->set_sensitive(true);
	  		m_action_ranks[(*l_iter)[m_columns.m_rank]]->set_active(true);
			m_action_remove->set_sensitive(true);
			unblockSignals();
		}
		else{
			blockSignals();
			m_action_playgame->set_sensitive(false);
			m_action_favorite->set_sensitive(false);
			m_action_favorite->set_active(false);
	  		m_action_played->set_sensitive(false);
			m_action_played->set_active(false);
			m_action_working->set_sensitive(false);
			m_action_working->set_active(false);
	  		m_action_rank->set_sensitive(false);
			m_action_remove->set_sensitive(false);
			unblockSignals();
		}

		m_gamelist_popup->popup(p_event->button, p_event->time);
		return true;
	}
	else if((p_event->type == GDK_2BUTTON_PRESS) && (p_event->button == 1) ) {
		playGame();
		return true;
	}
	else
		return false;
}

void CTreeViewGames::onGameChanged(void){
	// Emitimos una señal de cambio
	m_signal_changed.emit();
}

void CTreeViewGames::onActionPlaygame(void){
	playGame();
	m_signal_changed.emit();
}

void CTreeViewGames::onActionFavorite(void){
	setFavorite(m_action_favorite->get_active());
	m_signal_changed.emit();
}

void CTreeViewGames::onActionPlayed(void){
	setPlayed(m_action_played->get_active());
	m_signal_changed.emit();
}

void CTreeViewGames::onActionWorking(void){
	setWorking(m_action_working->get_active());
	m_signal_changed.emit();
}

void CTreeViewGames::onActionRank(const int p_rank){
	setRank(p_rank);
	m_signal_changed.emit();
}

void CTreeViewGames::onActionRemove(void){
	Gtk::TreeModel::iterator l_iter;
	Glib::RefPtr<Gtk::TreeSelection> l_selection;
	Gtk::MessageDialog l_dialog(*static_cast<Gtk::Window*>(CGelideUI::getInstance()),
		_("This operation can not be undone."), false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL);

	assert(m_system);

	//"Si continua eliminará el juego del listado actual."
	l_dialog.set_secondary_text( _("Game data will be deleted if you continue."));
	// Mostramos un mensaje avisando de la operación
	if(l_dialog.run() == Gtk::RESPONSE_CANCEL)
		return;

	// Actualizamos el menú dependiendo de la selección
	l_selection = this->get_selection();
	l_iter = l_selection->get_selected();
	if(l_iter){
		l_iter = m_sort_list->convert_iter_to_child_iter(l_iter);
		l_iter = m_filter_list->convert_iter_to_child_iter(l_iter);
		m_system->removeGame(l_iter);
	}
}

void CTreeViewGames::playGame(void){
	CDialogEmulatorLauncher l_launcher;
	Gtk::TreeModel::iterator l_iter;
	CGame* l_game;

	l_iter = this->getSelected();

	if(!l_iter)
		return;
	l_game = m_system->getGame(l_iter);

	l_launcher.set_transient_for(*static_cast<Gtk::Window*>(CGelideUI::getInstance()));
	l_launcher.run(m_system, l_game);
	l_iter = m_sort_list->convert_iter_to_child_iter(l_iter);
	l_iter = m_filter_list->convert_iter_to_child_iter(l_iter);
	(*l_iter)[m_columns.m_flags] = (*l_iter)[m_columns.m_flags] | 0x08;
}

void CTreeViewGames::setFavorite(bool p_favorite){
	Gtk::TreeModel::iterator l_iter;

	// Actualizamos el menú dependiendo de la selección
	l_iter = this->get_selection()->get_selected();
	if(l_iter){
		l_iter = m_sort_list->convert_iter_to_child_iter(l_iter);
		l_iter = m_filter_list->convert_iter_to_child_iter(l_iter);
		(*l_iter)[m_columns.m_favorite] = p_favorite;
	}
}

void CTreeViewGames::setPlayed(bool p_played){
	Gtk::TreeModel::iterator l_iter;

	// Actualizamos el menú dependiendo de la selección
	l_iter = this->get_selection()->get_selected();
	if(l_iter){
		l_iter = m_sort_list->convert_iter_to_child_iter(l_iter);
		l_iter = m_filter_list->convert_iter_to_child_iter(l_iter);
		if(p_played)
			(*l_iter)[m_columns.m_flags] = (*l_iter)[m_columns.m_flags] | 0x08;
		else
			(*l_iter)[m_columns.m_flags] = (*l_iter)[m_columns.m_flags] & 0xF7;
	}
}

void CTreeViewGames::setWorking(bool p_working){
	Gtk::TreeModel::iterator l_iter;

	// Actualizamos el menú dependiendo de la selección
	l_iter = this->get_selection()->get_selected();
	if(l_iter){
		l_iter = m_sort_list->convert_iter_to_child_iter(l_iter);
		l_iter = m_filter_list->convert_iter_to_child_iter(l_iter);
		if(p_working)
			(*l_iter)[m_columns.m_flags] = (*l_iter)[m_columns.m_flags] | 0x04;
		else
			(*l_iter)[m_columns.m_flags] = (*l_iter)[m_columns.m_flags] & 0xFB;
	}
}

void CTreeViewGames::setRank(const int p_rank){
	Gtk::TreeModel::iterator l_iter;

	// Actualizamos el menú dependiendo de la selección
	l_iter = this->get_selection()->get_selected();
	if(l_iter){
		l_iter = m_sort_list->convert_iter_to_child_iter(l_iter);
		l_iter = m_filter_list->convert_iter_to_child_iter(l_iter);
		(*l_iter)[m_columns.m_rank] = p_rank;
	}
}
