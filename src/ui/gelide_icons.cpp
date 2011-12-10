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

#include "gelide_icons.hpp"
#include "../utils/utils.hpp"
#include <gtkmm/image.h>
#include <gtkmm/icontheme.h>


CGelideIcons::CGelideIcons(void){
	// Creamos el icon Factory para Gelide y añadimos los iconos por defecto
	m_icon_factory = Gtk::IconFactory::create();
	m_icon_factory->add_default();
	//Gtk::IconTheme::get_default()->signal_changed().connect(sigc::mem_fun(*this, &CGelideIcons::updateDefaultIcons));
}

/*
void CGelideIcons::updateDefaultIcons(void){
}
*/

void CGelideIcons::addStockIcon(const Glib::ustring& p_file,
		const Glib::ustring& p_id, const Glib::ustring& p_label){
	Gtk::IconSource l_icon_source;
	Gtk::IconSet l_icon_set;
	Gtk::StockID l_stock_id;
	Glib::RefPtr<Gdk::Pixbuf> l_pixbuf;

	try
	{
		l_pixbuf = Gdk::Pixbuf::create_from_file(p_file);
	}
	catch(const Glib::Exception& l_exception)
	{
		GELIDE_WARNING("Loading icon (" << l_exception.what().c_str() << ")");
		return;
	}

	l_stock_id = Gtk::StockID(p_id);
	l_icon_set = Gtk::IconSet(l_pixbuf);

	m_icon_factory->add(l_stock_id, l_icon_set);
	Gtk::Stock::add(Gtk::StockItem(l_stock_id, p_label));
}

void CGelideIcons::addStockIcon(Glib::RefPtr<Gdk::Pixbuf> p_pixbuf,
		const Glib::ustring& p_id, const Glib::ustring& p_label){
	Gtk::IconSource l_icon_source;
	Gtk::IconSet l_icon_set;
	Gtk::StockID l_stock_id;

	l_stock_id = Gtk::StockID(p_id);
	l_icon_set = Gtk::IconSet(p_pixbuf);

	m_icon_factory->add(l_stock_id, l_icon_set);
	Gtk::Stock::add(Gtk::StockItem(l_stock_id, p_label));
}

void CGelideIcons::addThemeIcon(const Glib::ustring& p_icon, const Glib::ustring& p_id,
		const Glib::ustring& p_label, const Glib::ustring& p_file, const bool p_neg){
	Gtk::Image l_image;
	Glib::RefPtr<Gdk::Pixbuf> l_pixbuf, l_pixbuf_neg;
	Glib::RefPtr<Gtk::IconTheme> l_icon_theme;
	bool l_continue;

	l_icon_theme = Gtk::IconTheme::get_default();

	l_continue = false;
	// Comprobamos si se pudo obtener el icontheme
	if(l_icon_theme){
		// Intentamos cargar el icono
		try{
			l_pixbuf = l_icon_theme->load_icon(p_icon, 48, Gtk::ICON_LOOKUP_USE_BUILTIN);
			if(l_pixbuf){
				// Añadimos el icono y su negativo si es necesario
				addStockIcon(l_pixbuf, p_id, p_label);
				if(p_neg){
					l_pixbuf_neg = utils::createNegativePixbuf(l_pixbuf);
					addStockIcon(l_pixbuf_neg, p_id + "-neg", p_label + "-neg");
				}
				// Indicamos que el icono se añadió del tema correctamente
				l_continue = true;
			}
		}
		catch(...){
			l_continue = false;
		}

	}
	// Si no se pudo añadir el icono del tema, lo añadimos del fichero
	if(!l_continue){
		addStockIcon(p_file , p_id, p_label);
		l_pixbuf = l_image.render_icon(Gtk::StockID(p_id), Gtk::ICON_SIZE_DND);
		if(p_neg){
			l_pixbuf_neg = utils::createNegativePixbuf(l_pixbuf);
			addStockIcon(l_pixbuf_neg, p_id + "-neg", p_label + "-neg");
		}
	}
}


void CGelideIcons::initDefaultIcons(void){

	GELIDE_DEBUG("Adding icons...");
	GELIDE_DEBUG("Toolbar icons");
	addStockIcon(GELIDE_UI_ICONS "gelide-export-list.png", "gelide-export-list", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-export-systems.png", "gelide-export-systems", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-import-systems.png", "gelide-import-systems", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-columns.png", "gelide-columns", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-show-filters.png", "gelide-show-filters", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-show-images.png", "gelide-show-images", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-show-browser.png", "gelide-show-browser", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-vertical-browser.png", "gelide-vertical-browser", "");
	addStockIcon(GELIDE_UI_ICONS "gelide-horizontal-browser.png", "gelide-horizontal-browser", "");

	GELIDE_DEBUG("Filter icons...");
	addThemeIcon("emblem-favorite", "gelide-favorite", "", GELIDE_UI_ICONS "gelide-favorite.png", true);
	addThemeIcon("joystick", "gelide-played", "", GELIDE_UI_ICONS "gelide-played.png", true);
	addThemeIcon("binary", "gelide-working", "", GELIDE_UI_ICONS "gelide-working.png", true);
	addThemeIcon("gtk-apply", "gelide-available", "", GELIDE_UI_ICONS "gelide-available.png", true);
	addThemeIcon("dialog-warning", "gelide-unknown", "", GELIDE_UI_ICONS "gelide-unknown.png", true);
	addThemeIcon("", "gelide-rank", "", GELIDE_UI_ICONS "gelide-rank.png", true);
	addThemeIcon("gtk-home", "gelide-all", "", GELIDE_UI_ICONS "gelide-all.png", false);
	addThemeIcon("stock_calendar", "gelide-year", "", GELIDE_UI_ICONS "gelide-year.png", false);

	GELIDE_DEBUG("Other icons...");
	addThemeIcon("applications-system", "gelide-system", "", GELIDE_UI_ICONS "gelide-system.png", false);
	addThemeIcon("applications-other", "gelide-emulator", "", GELIDE_UI_ICONS "gelide-emulator.png", false);
	GELIDE_DEBUG("Icons added correctly.");
}


