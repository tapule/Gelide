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

#include "info_pannel.hpp"
#include <gtkmm/stock.h>
#include <gtkmm/icontheme.h>
#include "../utils/utils.hpp"
#include <sstream>

CInfoPannel::CInfoPannel(void):
	Gtk::VBox(),
	m_table_description(5, 6), m_table_system(2,2), m_table_emulator(2,2)
{
	// Obtenermos las instancias
	m_config = CConfig::getInstance();
	m_smanager = CSystemManager::getInstance();

	m_system = NULL;

	m_label_description.set_markup((Glib::ustring) "<b><big><i>" +	_("...") + "</i></big></b>");
	m_label_description.set_ellipsize(Pango::ELLIPSIZE_END);
	m_label_description.set_padding(0, 5);

	this->set_spacing(5);
	this->pack_start(m_label_description,false, false);
	this->pack_start (m_table_description,false, false);
	this->pack_start(m_notebook_images, true, true);
	this->pack_start(m_vbox_system, false, false);
	this->pack_start(m_vbox_emulator, false, false);
	this->pack_start(m_button_run, false, false);

	// todo
	initFlagsIcons();
	initDescription();
	initImages();
	initSystem();
	initEmulator();
	initRun();

	this->show_all_children();

	loadConfig();
}

CInfoPannel::~CInfoPannel(){
	saveConfig();
}

sigc::signal<void> CInfoPannel::signalPlayGame(void){
	return m_signal_play_game;
}

void CInfoPannel::setGame(Gtk::TreeModel::iterator& p_system, Gtk::TreeModel::iterator& p_game){
	CGame* l_game = NULL;
	CEmulator* l_emulator = NULL;
	Gtk::TreeModel::iterator l_iter;
	int l_ind, l_rank;
	std::stringstream l_str;

	if(!p_system || !p_game){
		clear();
		return;
	}

	m_system = m_smanager->getSystem(p_system);
	m_game = p_game;
	l_game = m_system->getGame(p_game);

	// CHECKME: cagada ¿dos llamadas para sacar el iterador?
	l_iter = m_system->getEmulator(m_system->getEmulatorId());
	if(l_iter)
		l_emulator = m_system->getEmulator(l_iter);

	// Actualización de la parte superior del info-panel
	m_label_description.set_markup((Glib::ustring) "<b><big><i>" +	Glib::Markup::escape_text(l_game->getDescription()) + "</i></big></b>");
	// Actualización de los flags del juego
	m_image_favorite.set(l_game->getFavorite()? m_flag_favorite[0]:m_flag_favorite[1]);
	m_image_played.set(l_game->getPlayed()? m_flag_played[0]:m_flag_played[1]);
	m_image_working.set(l_game->getWorking()? m_flag_working[0]:m_flag_working[1]);
	m_image_available.set(l_game->getAvailable()? m_flag_available[0]:m_flag_available[1]);
	m_image_unknown.set(l_game->getUnknown()? m_flag_unknown[0]:m_flag_unknown[1]);
	// Actualización de la puntuación del juego
	l_rank = l_game->getRank();
	for(l_ind = 1; l_ind < 6; l_ind++){
		if(l_rank < l_ind)
			m_image_rank[l_ind - 1].set(m_flag_rank[1]);
		else
			m_image_rank[l_ind - 1].set(m_flag_rank[0]);
	}
	m_label_manufacturer_value.set_label(l_game->getManufacturer());
	l_str << l_game->getYear();
	m_label_year_value.set_label(l_str.str() );
	m_label_crc_value.set_label(l_game->getRomCrc());

	// Actualización de las imágenes
	updateImages(m_system, l_game);

	// Actualización del sistema
	m_image_system.set(m_system->getIcon());
	m_label_system_name.set_markup("<b>" +	m_system->getName() + "</b>");
	m_label_system_description.set_label(m_system->getDescription());

	// Actualización del sistema
	if(l_emulator){
		m_image_emulator.set(l_emulator->getIcon());
		m_label_emulator_name.set_markup("<b>" + l_emulator->getName() + "</b>");
		m_label_emulator_description.set_label(l_emulator->getDescription());
	}
	else{
		m_image_emulator.set(m_pixbuf_emulator);
		m_label_emulator_name.set_label("");
		m_label_emulator_description.set_label("");
	}

	m_button_run.set_sensitive(true);

	delete l_game;
	delete l_emulator;
}

void CInfoPannel::clear(void){

	m_system = NULL;
	m_button_run.set_sensitive(false);
	m_label_description.set_markup((Glib::ustring) "<b><big><i>" +	_("...") + "</i></big></b>");
	m_image_favorite.set(m_flag_favorite[1]);
	m_image_played.set(m_flag_played[1]);
	m_image_working.set(m_flag_working[1]);
	m_image_available.set(m_flag_available[1]);
	m_image_unknown.set(m_flag_unknown[1]);
	m_image_rank[0].set(m_flag_rank[1]);
	m_image_rank[1].set(m_flag_rank[1]);
	m_image_rank[2].set(m_flag_rank[1]);
	m_image_rank[3].set(m_flag_rank[1]);
	m_image_rank[4].set(m_flag_rank[1]);
	m_label_manufacturer_value.set_label("");
	m_label_year_value.set_label("");
	m_label_crc_value.set_label("");

	m_pixbuf_snapshot = m_default->copy();
	m_pixbuf_tittle = m_default->copy();
	m_pixbuf_front = m_default->copy();
	m_pixbuf_back = m_default->copy();
	m_pixbuf_media = m_default->copy();

	m_image_system.set(m_pixbuf_system);
	m_label_system_name.set_label("");
	m_label_system_description.set_label("");
	m_image_emulator.set(m_pixbuf_emulator);
	m_label_emulator_name.set_label("");
	m_label_emulator_description.set_label("");
}

// CHECKME: Esto es una mala solución. El método no localiza bien los ficheros
void CInfoPannel::updateImages(CSystem* p_system, CGame* p_game){
	Glib::ustring l_exts[] = {"png", "PNG", "jpg", "JPG", "jpeg", "JPEG", "gif", "GIF", "xpm", "XPM", "pnm", "tif", "tiff"};
	int l_tab, l_ind;
	//Gtk::Image* l_image = NULL;
	Glib::RefPtr<Gdk::Pixbuf>* l_pixbuf = NULL;
	Glib::ustring l_path;
	bool l_find = false;

	for(l_tab = 0; l_tab < 5; l_tab++)
	{
		// Seleccionamos la imagen en cada ciclo
		switch(l_tab){
		case 0:
			l_pixbuf = &m_pixbuf_snapshot;
			l_path = m_system->getSnapDir();
			break;
		case 1:
			l_pixbuf = &m_pixbuf_tittle;
			l_path = m_system->getTitleDir();
			break;
		case 2:
			l_pixbuf = &m_pixbuf_front;
			l_path = m_system->getFrontDir();
			break;
		case 3:
			l_pixbuf = &m_pixbuf_back;
			l_path = m_system->getBackDir();
			break;
		case 4:
			l_pixbuf = &m_pixbuf_media;
			l_path = m_system->getMediaDir();
			break;
		}
		l_find = false;
		for(l_ind = 0; l_ind < 8; l_ind++ ){
			// comprobamos nombre.ext
			if(Glib::file_test(Glib::build_filename(l_path, p_game->getName() + "." + l_exts[l_ind]), Glib::FILE_TEST_EXISTS)){
				(*l_pixbuf) = Gdk::Pixbuf::create_from_file(Glib::build_filename(l_path, p_game->getName() + "." + l_exts[l_ind]));
				//l_image->set(l_path + "/" + p_game->getName() + "." + l_exts[l_ind]);
				l_find = true;
				break;
			}
			// comprobamos crc.ext
			else if(Glib::file_test(Glib::build_filename(l_path, p_game->getRomCrc().lowercase() + "." + l_exts[l_ind]), Glib::FILE_TEST_EXISTS)){
				//l_image->set(l_path + "/" + p_game->getRomCrc().lowercase() + "." + l_exts[l_ind]);
				(*l_pixbuf) = Gdk::Pixbuf::create_from_file(Glib::build_filename(l_path, p_game->getRomCrc().lowercase() + "." + l_exts[l_ind]));
				l_find = true;
				break;
			}
			// comprobamos CRC.ext
			else if(Glib::file_test(Glib::build_filename(l_path,p_game->getRomCrc().uppercase() + "." + l_exts[l_ind]), Glib::FILE_TEST_EXISTS)){
				//l_image->set(l_path + "/" + p_game->getRomCrc().uppercase() + "." + l_exts[l_ind]);
				(*l_pixbuf) = Gdk::Pixbuf::create_from_file(Glib::build_filename(l_path, p_game->getRomCrc().uppercase() + "." + l_exts[l_ind]));
				l_find = true;
				break;
			}
		}
		if(!l_find)
			(*l_pixbuf) = m_default->copy();
	}
}

void CInfoPannel::on_size_allocate(Gtk::Allocation & p_allocation){
	int l_awidth, l_aheight;
	int l_rwidth, l_rheight;
	int l_swidth, l_sheight;
	int l_nwidth, l_nheight;
	int l_width_diff, l_height_diff;

    Glib::RefPtr<Gdk::Pixbuf> l_pixbuf;
    Gtk::Image* l_image = NULL;

    Gtk::VBox::on_size_allocate(p_allocation);

   if(!m_sized){
		// Seleccionamos la imagen dependiendo del pannel
		switch(m_notebook_images.get_current_page()){
		case 0:
			l_pixbuf = m_pixbuf_snapshot;
			l_image =  &m_image_snapshot;
			break;
		case 1:
			l_pixbuf = m_pixbuf_tittle;
			l_image =  &m_image_tittle;
			break;
		case 2:
			l_pixbuf = m_pixbuf_front;
			l_image =  &m_image_front;
			break;
		case 3:
			l_pixbuf = m_pixbuf_back;
			l_image =  &m_image_back;
			break;
		case 4:
			l_pixbuf = m_pixbuf_media;
			l_image =  &m_image_media;
			break;
		}
		// Tamaño total que tenemos para la imagen
    	//l_awidth = p_allocation.get_width() - 20;
    	//l_aheight = p_allocation.get_height() - 20;
		l_awidth = l_image->get_width();
		l_aheight = l_image->get_height();

		if(l_awidth <= 1 || l_aheight <= 1){
			Gtk::VBox::on_size_allocate(p_allocation);
			return;
		}

    	// Tamaño de la imagen real
    	l_rwidth = l_pixbuf->get_width();
    	l_rheight = l_pixbuf->get_height();
    	// Tamaños en proporción que necesitaremos para meter la imagen
    	l_sheight = (l_awidth * l_rheight) / l_rwidth;
    	l_swidth = (l_aheight * l_rwidth) / l_rheight;

    	// Calculamos las diferencias con el espacio total que tenemos
    	// para comprobar cual de los dos se ajusta mejor
    	l_height_diff = l_aheight - l_sheight;
    	l_width_diff = l_awidth - l_swidth;

		// la imágen entrará tanto en anchura como en altura, cogemos el mayor
    	if((l_width_diff >= 0) && (l_height_diff >= 0)) {
			if(l_width_diff < l_height_diff) {
				l_nwidth = l_swidth;
				l_nheight = l_aheight;
			}
			else {
				l_nwidth = l_awidth;
				l_nheight = l_sheight;
			}
		}
    	else if(l_width_diff < 0){
			l_nwidth = l_awidth;
			l_nheight = l_sheight;
		}
    	else if(l_height_diff < 0){
			l_nwidth = l_swidth;
    		l_nheight = l_aheight;
		}
    	//GELIDE_DEBUG("aw " << l_awidth << " ah " <<l_aheight << " / rw " << l_rwidth << " rh " << l_rheight << " / sw " << l_swidth << " sh " << l_sheight);
    	//GELIDE_DEBUG("Nw " << l_nwidth << " nh " <<l_nheight);
    	//GELIDE_DEBUG("Bw " << m_notebook_images.get_width() << " Bh " << m_notebook_images.get_height());

    	// Al establecer la imagen se provoca que se entre de nuevo en el método
    	// con m_sized controlamos que no se vuelva a recalcular en la siguiente
    	// entrada
		l_image->set(l_pixbuf->scale_simple(l_nwidth, l_nheight, Gdk::INTERP_BILINEAR));
		m_sized = true;
    }
    else
    	m_sized = false;
}

void CInfoPannel::loadConfig(void){
	int l_pannel;

	// Obtenemos la posición almacenada
	//m_config->getUIInfoPannel(l_pannel);
	m_config->getKey(GELIDE_CFG_UI, "info_pannel_selected", l_pannel);
	if(l_pannel != -1)
		m_notebook_images.set_current_page(l_pannel);
}

void CInfoPannel::saveConfig(void){
	//m_config->setUIInfoPannel(m_notebook_images.get_current_page());
	m_config->setKey(GELIDE_CFG_UI, "info_pannel_selected", m_notebook_images.get_current_page());
}

void CInfoPannel::initFlagsIcons(void){
	m_flag_favorite[0] = this->render_icon(Gtk::StockID("gelide-favorite"), Gtk::ICON_SIZE_BUTTON);
	m_flag_played[0] = this->render_icon(Gtk::StockID("gelide-played"), Gtk::ICON_SIZE_BUTTON);
	m_flag_working[0] = this->render_icon(Gtk::StockID("gelide-working"), Gtk::ICON_SIZE_BUTTON);
	m_flag_available[0] = this->render_icon(Gtk::StockID("gelide-available"), Gtk::ICON_SIZE_BUTTON);
	m_flag_unknown[0] = this->render_icon(Gtk::StockID("gelide-unknown"), Gtk::ICON_SIZE_BUTTON);
	m_flag_rank[0] = this->render_icon(Gtk::StockID("gelide-rank"), Gtk::ICON_SIZE_BUTTON);

	m_flag_favorite[1] = this->render_icon(Gtk::StockID("gelide-favorite-neg"), Gtk::ICON_SIZE_BUTTON);
	m_flag_played[1] = this->render_icon(Gtk::StockID("gelide-played-neg"), Gtk::ICON_SIZE_BUTTON);
	m_flag_working[1] = this->render_icon(Gtk::StockID("gelide-working-neg"), Gtk::ICON_SIZE_BUTTON);
	m_flag_available[1] = this->render_icon(Gtk::StockID("gelide-available-neg"), Gtk::ICON_SIZE_BUTTON);
	m_flag_unknown[1] = this->render_icon(Gtk::StockID("gelide-unknown-neg"), Gtk::ICON_SIZE_BUTTON);
	m_flag_rank[1] = this->render_icon(Gtk::StockID("gelide-rank-neg"), Gtk::ICON_SIZE_BUTTON);
}

void CInfoPannel::initDescription(void){
	m_label_flags.set_markup((Glib::ustring) "<b>" +	_("Flags:") + "</b>");
	m_label_flags.set_alignment(0, 0.50);
	m_label_rank.set_markup((Glib::ustring) "<b>" +	_("Rank:") + "</b>");
	m_label_rank.set_alignment(0, 0.50);
	m_label_manufacturer.set_markup((Glib::ustring) "<b>" +	_("Manufacturer:") + "</b>");
	m_label_manufacturer.set_alignment(0, 0.50);
	m_label_year.set_markup((Glib::ustring) "<b>" +	_("Year:") + "</b>");
	m_label_year.set_alignment(0, 0.50);
	m_label_crc.set_markup((Glib::ustring) "<b>" +	_("CRC:") + "</b>");
	m_label_crc.set_alignment(0, 0.50);
	m_label_manufacturer_value.set_ellipsize(Pango::ELLIPSIZE_END);
	m_label_year_value.set_ellipsize(Pango::ELLIPSIZE_END);
	m_label_crc_value.set_ellipsize(Pango::ELLIPSIZE_END);

	m_image_favorite.set(m_flag_favorite[1]);
	m_image_played.set(m_flag_played[1]);
	m_image_working.set(m_flag_working[1]);
	m_image_available.set(m_flag_available[1]);
	m_image_unknown.set(m_flag_unknown[1]);

	m_image_rank[0].set(m_flag_rank[1]);
	m_image_rank[1].set(m_flag_rank[1]);
	m_image_rank[2].set(m_flag_rank[1]);
	m_image_rank[3].set(m_flag_rank[1]);
	m_image_rank[4].set(m_flag_rank[1]);

	//m_table_images.set_row_spacings(5);
	m_table_description.attach(m_label_flags, 0, 1, 0, 1, Gtk::FILL);
	m_table_description.attach(m_image_favorite, 1, 2, 0, 1, Gtk::SHRINK);
	m_table_description.attach(m_image_played, 2, 3, 0, 1, Gtk::SHRINK);
	m_table_description.attach(m_image_working, 3, 4, 0, 1,Gtk::SHRINK );
	m_table_description.attach(m_image_available, 4, 5, 0, 1,Gtk::SHRINK );
	m_table_description.attach(m_image_unknown, 5, 6, 0, 1, Gtk::SHRINK);

	m_table_description.attach(m_label_rank, 0, 1, 1, 2, Gtk::FILL);
	m_table_description.attach(m_image_rank[0], 1, 2, 1, 2, Gtk::SHRINK);
	m_table_description.attach(m_image_rank[1], 2, 3, 1, 2, Gtk::SHRINK);
	m_table_description.attach(m_image_rank[2], 3, 4, 1, 2, Gtk::SHRINK);
	m_table_description.attach(m_image_rank[3], 4, 5, 1, 2, Gtk::SHRINK);
	m_table_description.attach(m_image_rank[4], 5, 6, 1, 2, Gtk::SHRINK);

	m_table_description.attach(m_label_manufacturer, 0, 1, 2, 3, Gtk::FILL);
	m_table_description.attach(m_label_manufacturer_value, 1, 6, 2, 3, Gtk::FILL);
	m_table_description.attach(m_label_year, 0, 1, 3, 4, Gtk::FILL);
	m_table_description.attach(m_label_year_value, 1, 6, 3, 4, Gtk::FILL);
	m_table_description.attach(m_label_crc, 0, 1, 4, 5, Gtk::FILL);
	m_table_description.attach(m_label_crc_value, 1, 6, 4, 5, Gtk::FILL);

	m_table_description.set_col_spacings(10);
}

void CInfoPannel::initImages(void){
	Glib::RefPtr<Gtk::IconTheme> l_icon_theme;
	Gtk::Image* l_i;
	bool l_continue;

	l_i = Gtk::manage(new Gtk::Image());

	m_notebook_images.set_size_request(200, -1);
	m_notebook_images.append_page(m_image_snapshot, _("Snapshot"));
	m_notebook_images.append_page(m_image_tittle, _("Title"));
	m_notebook_images.append_page(m_image_front, _("Front"));
	m_notebook_images.append_page(m_image_back, _("Back"));
	m_notebook_images.append_page(m_image_media, _("Media"));

	//m_default = Gdk::Pixbuf::create_from_file(GELIDE_DEFAULT_IMAGE);
	//m_default = Gtk::IconTheme::get_default()->load_icon("gtk-missing-image", 200, Gtk::ICON_LOOKUP_USE_BUILTIN);


	l_icon_theme = Gtk::IconTheme::get_default();
	l_continue = false;
	// Comprobamos si se pudo obtener el icontheme
	if(l_icon_theme){
		// Intentamos cargar el icono
		try{
			m_default = l_icon_theme->load_icon("gtk-missing-image", 200, Gtk::ICON_LOOKUP_USE_BUILTIN);
			l_continue = true;
		}
		catch(...){
			l_continue = false;
		}
	}
	// Si no se pudo añadir el icono del tema, lo añadimos del fichero
	if(!l_continue){
		try
		{
			m_default = Gdk::Pixbuf::create_from_file(GELIDE_UI_ICONS "gelide-no-image.png");
		}
		catch(const Glib::Exception& l_exception)
		{
			GELIDE_WARNING("InfoPannel loading default image (" << l_exception.what().c_str() << ")");
			return;
		}
	}

	m_pixbuf_snapshot = m_default->copy();
	m_pixbuf_tittle = m_default->copy();
	m_pixbuf_front = m_default->copy();
	m_pixbuf_back = m_default->copy();
	m_pixbuf_media = m_default->copy();

}
void CInfoPannel::initSystem(void){
	m_label_system_title.set_markup((Glib::ustring) "<b>" +	_("System:") + "</b>");
	m_label_system_title.set_alignment(0, 0.50);
	m_label_system_name.set_alignment(0, 1);
	m_label_system_name.set_ellipsize(Pango::ELLIPSIZE_END);
	m_label_system_description.set_alignment(0, 0);
	m_label_system_description.set_ellipsize(Pango::ELLIPSIZE_END);
	m_pixbuf_system = utils::createNegativePixbuf(m_smanager->getSystemDefaultIcon());
	m_image_system.set(m_pixbuf_system);
	m_image_system.set_padding(10,10);

	m_table_system.attach(m_image_system, 0, 1, 0, 2, Gtk::FILL);
	m_table_system.attach(m_label_system_name, 1, 2, 0, 1, Gtk::FILL|Gtk::EXPAND);
	m_table_system.attach(m_label_system_description, 1, 2, 1, 2, Gtk::FILL|Gtk::EXPAND);

	m_alignment_system.set(0, 0, 1, 1);
	m_alignment_system.set_padding(0, 0, 12, 0);
	m_alignment_system.add(m_table_system);

	m_vbox_system.pack_start(m_label_system_title, true, false);
	m_vbox_system.pack_start(m_alignment_system, true, true);

}

void CInfoPannel::initEmulator(void){
	m_label_emulator_title.set_markup((Glib::ustring) "<b>" +	_("Emulator:") + "</b>");
	m_label_emulator_title.set_alignment(0, 0.50);
	m_label_emulator_name.set_alignment(0, 1);
	m_label_emulator_name.set_ellipsize(Pango::ELLIPSIZE_END);
	m_label_emulator_description.set_alignment(0, 0);
	m_label_emulator_description.set_ellipsize(Pango::ELLIPSIZE_END);
	m_pixbuf_emulator = utils::createNegativePixbuf(m_smanager->getEmulatorDefaultIcon());
	m_image_emulator.set(m_pixbuf_emulator);
	m_image_emulator.set_padding(10,10);

	m_table_emulator.attach(m_image_emulator, 0, 1, 0, 2, Gtk::FILL);
	m_table_emulator.attach(m_label_emulator_name, 1, 2, 0, 1, Gtk::FILL|Gtk::EXPAND);
	m_table_emulator.attach(m_label_emulator_description, 1, 2, 1, 2, Gtk::FILL|Gtk::EXPAND);

	m_alignment_emulator.set(0, 0, 1, 1);
	m_alignment_emulator.set_padding(0, 0, 12, 0);
	m_alignment_emulator.add(m_table_emulator);

	m_vbox_emulator.pack_start(m_label_emulator_title, false, false);
	m_vbox_emulator.pack_start(m_alignment_emulator, true, false);
}

void CInfoPannel::initRun(void){
	m_image_run.set(Gtk::Stock::MEDIA_PLAY, Gtk::ICON_SIZE_BUTTON);
	m_image_run.set_alignment(1, 0.5);
	m_label_run.set_label(_("Launch game"));
	m_label_run.set_alignment(0, 0.5);

	m_hbox_run.pack_start (m_image_run,true,true);
	m_hbox_run.pack_start (m_label_run,true,true);

	m_button_run.add(m_hbox_run);
	m_button_run.signal_clicked().connect(sigc::mem_fun(*this, &CInfoPannel::onRunActivated));
	m_button_run.set_sensitive(false);
}

void CInfoPannel::onRunActivated(void){
	m_signal_play_game.emit();
}
