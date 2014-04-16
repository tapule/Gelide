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

#include <fstream>
#include <stdlib.h>
#include <sstream>
//#include <glib/gstdio.h>
//#include <glib.h>
#include <glibmm.h>
#include <gtkmm/image.h>
#include <giomm/file.h>
#include "system_manager.hpp"
#include "../utils/utils.hpp"
#include "../utils/xml_reader.hpp"
#include "../utils/xml_writer.hpp"
#include "../utils/tokenizer.hpp"


CSystemManager::CSystemManager(void){
	Gtk::Image l_image;

	// Tomamos la instancia del configurador
	m_config = CConfig::getInstance();
	// Renderizamos el icono por defecto de los sistemas y de los emuladores
	//m_system_icon = l_image.render_icon(Gtk::StockID("gnome-system"), Gtk::ICON_SIZE_DND);
	m_system_icon = l_image.render_icon_pixbuf(Gtk::StockID("gelide-system"), Gtk::ICON_SIZE_DND);
	m_emulator_icon = l_image.render_icon_pixbuf(Gtk::StockID("gelide-emulator"), Gtk::ICON_SIZE_DND);

	// Inicialización del listado de sistemas
	m_system_list = Gtk::ListStore::create(m_system_columns);
}

CSystemManager::~CSystemManager(void){
	std::vector<CSystem*>::iterator l_iter;

	// Liberamos la memoria ocupada por los sistemas
	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); l_iter++)
		delete *l_iter;
	m_systems.clear();
	m_system_list->clear();
}

Glib::RefPtr<Gdk::Pixbuf> CSystemManager::getSystemDefaultIcon(void){
	return m_system_icon;
}

Glib::RefPtr<Gdk::Pixbuf> CSystemManager::getEmulatorDefaultIcon(void){
	return m_emulator_icon;
}

bool CSystemManager::load(void){
	std::ifstream l_file;
	CTokenizer l_tokenizer;
	CTokenizer::CToken l_token;
	CSystem* l_system;
	bool l_ret = false;
	bool l_value;

	// Cargamos los sistemas del usuario.
	GELIDE_DEBUG("Loading systems...");
	if(l_tokenizer.initFromFile(utils::getSystemsList())){
		//l_tokenizer.setDelimiters((char *)CSV_SEPARATOR, 1);
		l_tokenizer.setDelimiters("\n\x3B", 2);
		while(l_tokenizer.hasMoreTokens()){
			// Preparamos el sistema
			l_system = new CSystem();
			l_tokenizer.nextToken(l_token);
			l_system->setId(l_token.getValue());			// Id
			l_tokenizer.nextToken(l_token);
			l_system->setName(Glib::strcompress(l_token.getString()));			// Nombre
			l_tokenizer.nextToken(l_token);
			l_system->setDescription(Glib::strcompress(l_token.getString()));	// Descripción
			l_tokenizer.nextToken(l_token);
			l_system->setIconFile(Glib::strcompress(l_token.getString()));		// Fichero de icono
			l_tokenizer.nextToken(l_token);
			l_system->setDatFile(Glib::strcompress(l_token.getString()));		// Fichero dat
			l_tokenizer.nextToken(l_token);
			l_system->setBiosFile(Glib::strcompress(l_token.getString()));		// Fichero bios
			l_tokenizer.nextToken(l_token);
			l_system->setHomeDir(Glib::strcompress(l_token.getString()));		// Directorio home
			l_tokenizer.nextToken(l_token);
			l_system->setRomsDir(Glib::strcompress(l_token.getString()));		// Directorio de roms
			l_tokenizer.nextToken(l_token);
			l_system->setSnapDir(Glib::strcompress(l_token.getString()));		// Directorio de snap
			l_tokenizer.nextToken(l_token);
			l_system->setTitleDir(Glib::strcompress(l_token.getString()));		// Directorio de tittles
			l_tokenizer.nextToken(l_token);
			l_system->setFrontDir(Glib::strcompress(l_token.getString()));		// Directorio de fronts
			l_tokenizer.nextToken(l_token);
			l_system->setBackDir(Glib::strcompress(l_token.getString()));		// Directorio de Backs
			l_tokenizer.nextToken(l_token);
			l_system->setMediaDir(Glib::strcompress(l_token.getString()));		// Directorio de Medias
			l_tokenizer.nextToken(l_token);
			l_system->setEmulatorId(l_token.getValue());						// Emulador asignado
			// Información del dat
			l_tokenizer.nextToken(l_token);
			l_system->setDatName(Glib::strcompress(l_token.getString()));		// Nombre del dat
			l_tokenizer.nextToken(l_token);
			l_system->setDatDescription(Glib::strcompress(l_token.getString()));// Descripción del dat
			l_tokenizer.nextToken(l_token);
			l_system->setDatCategory(Glib::strcompress(l_token.getString()));	// Categoría del dat
			l_tokenizer.nextToken(l_token);
			l_system->setDatVersion(Glib::strcompress(l_token.getString()));	// Versión del dat
			l_tokenizer.nextToken(l_token);
			l_system->setDatAuthor(Glib::strcompress(l_token.getString()));	// Autor del dat
			l_tokenizer.nextToken(l_token);
			l_system->setDatGames(l_token.getValue());		// Numero de juegos en dat
			// Intentamos añadirlo
			addSystem(*l_system);
		}
		l_ret = true;
	}

	// Cargamos los sistemas por defecto.
	//m_config->getPrefsImportSystems(l_value);
	m_config->getKey(GELIDE_CFG_PREFERENCES, "import_default_systems", l_value);
	if(l_value){
		GELIDE_DEBUG("Loading default systems...");
		generateDefaultSystems();
		//importSystems(GELIDE_SYSTEMS);
	}

	GELIDE_DEBUG("Loaded games: " << this->getGames());
	return l_ret;
}

bool CSystemManager::save(void){
	std::vector<CSystem*>::iterator l_iter;
	std::ofstream l_file;

	GELIDE_DEBUG("Saving systems...");
	l_file.open(utils::getSystemsList().data());
	if(l_file.good()){
		for(l_iter = m_systems.begin(); l_iter != m_systems.end(); l_iter++){
			l_file << (*l_iter)->getAsString();
			(*l_iter)->save();
		}
		l_file.close();
		return true;
	}
	l_file.close();
	return false;
}

bool CSystemManager::importSystems(const Glib::ustring& p_file){
	CXmlReader l_xml;
	CXmlNode l_snode;
	CXmlNode::iterator l_iter, l_iter_child;
	CSystem* l_system;
	CEmulator l_emulator;
	Glib::ustring l_path, l_file;
	unsigned int l_id;
	Glib::ustring l_str;


	GELIDE_DEBUG("Importing systems...");
	if(l_xml.open(p_file)){
		// Path  al directorio del listado de importación donde se supone
		// que estarán los directorios icons y dats
		l_path = Glib::path_get_dirname(p_file);

		l_snode = l_xml.getRootElement();		// <systems>
		if(l_snode.getName() == "systems"){
			for(l_iter = l_snode.begin(); l_iter != l_snode.end();++l_iter){
				// Preparamos el sistema y obtenemos la información preliminar
				l_system = new CSystem();
				l_iter->getAttribute("emuid", l_id);
				l_system->setEmulatorId(l_id);						// Emulador asignado
				l_iter->getAttribute("name", l_str);
				l_system->setName(l_str);							// Nombre
				l_iter->getAttribute("description", l_str);
				l_system->setDescription(l_str);					// Descripción
				l_iter->getAttribute("iconfile", l_str);
				if(l_str != "")
					l_system->setIconFile(Glib::build_filename(l_path,l_str));	// Fichero de icono
				l_iter->getAttribute("datfile", l_str);
				if(l_str != "")
					l_system->setDatFile(Glib::build_filename(l_path,l_str));			// Fichero de dat
				GELIDE_DEBUG("Importando sistema: " << l_system->getName());
				// Obtenemos los datos de los emuladores
				for(l_iter_child = l_iter->begin(); l_iter_child != l_iter->end(); ++l_iter_child){
					// Leemos la información del emulador
					l_iter_child->getAttribute("id", l_id);
					l_emulator.setId(l_id);
					l_iter_child->getAttribute("name", l_str);
					l_emulator.setName(l_str);
					l_iter_child->getAttribute("description", l_str);
					l_emulator.setDescription(l_str);
					l_iter_child->getAttribute("version", l_str);
					l_emulator.setVersion(l_str);
					l_iter_child->getAttribute("author", l_str);
					l_emulator.setAuthor(l_str);
					l_iter_child->getAttribute("homepage", l_str);
					l_emulator.setHomepage(l_str);
					l_iter_child->getAttribute("bindir", l_str);
					l_emulator.setPath(l_str);
					l_iter_child->getAttribute("params", l_str);
					l_emulator.setParams(l_str);
					l_iter_child->getAttribute("iconfile", l_str);
					if(l_str != "")
						l_emulator.setIconFile(Glib::build_filename(l_path,l_str));
					else
						l_emulator.setIconFile("");
					GELIDE_DEBUG("Importando emulador: " << l_emulator.getName());
					// Añadimos el emu al sistema
					l_system->addEmulator(l_emulator);
				}
				// Añadimos el sistema al manager
				addSystem(*l_system);
			}
		}
		return true;
	}
	return false;
}

bool CSystemManager::exportSystems(const Glib::ustring& p_file){
	std::vector<CSystem*>::iterator l_siter;
	Gtk::TreeModel::iterator l_eiter;
	CXmlWriter l_xml;
	std::stringstream l_str;
	CEmulator* l_emu;
	Glib::ustring l_fname, l_icons_path, l_dats_path;
	bool l_export_icons, l_export_dats;
	Glib::RefPtr<Gio::File> l_ifile, l_ofile;
	int l_ind;

	// Abrimos el xml
	if(!l_xml.open(p_file))
		return false;

	// Obtenemos la configuración de exportación
	//m_config->getPrefsExportIcons(l_export_icons);
	//m_config->getPrefsExportDats(l_export_dats);
	m_config->getKey(GELIDE_CFG_PREFERENCES, "export_icons", l_export_icons);
	m_config->getKey(GELIDE_CFG_PREFERENCES, "export_dats", l_export_dats);


	// Path a los directorios de iconos y dats
	l_icons_path = Glib::build_filename(Glib::path_get_dirname(p_file), SYSTEMS_EXPORT_ICONS_DIR);
	l_dats_path = Glib::build_filename(Glib::path_get_dirname(p_file), SYSTEMS_EXPORT_DATS_DIR);

	// Creamos los directorios si no existen
	if(l_export_icons){
		if(!Glib::file_test(l_icons_path, Glib::FILE_TEST_EXISTS)){
			if(!utils::createDir(l_icons_path))
				return false;
		}
	}
	if(l_export_dats){
		if(!Glib::file_test(l_dats_path, Glib::FILE_TEST_EXISTS)){
			if(!utils::createDir(l_dats_path))
				return false;
		}
	}

	GELIDE_DEBUG("Exporting systems...");
	l_xml.startElement("systems");
	for(l_siter = m_systems.begin(); l_siter != m_systems.end(); l_siter++){
		l_xml.startElement("system");
		l_xml.writeAttribute("emuid", (*l_siter)->getEmulatorId());
		l_xml.writeAttribute("name", (*l_siter)->getName());
		l_xml.writeAttribute("description", (*l_siter)->getDatDescription());
		// Comprobamos si se dan las condiciones para la copia del icono
		if(l_export_icons && Glib::file_test((*l_siter)->getIconFile(), Glib::FILE_TEST_EXISTS)){
			// Montamos el nombre del icono
			l_fname = (*l_siter)->getIconFile();
			l_ind = l_fname.find_last_of(".");
			if(l_ind)
				l_fname = l_fname.substr(l_ind, l_fname.size());
			else
				l_fname = "";
			l_str.str("");
			l_str << "s_" << (*l_siter)->getId() << l_fname;
			l_xml.writeAttribute("iconfile", Glib::build_filename(SYSTEMS_EXPORT_ICONS_DIR, l_str.str()));
			// Copiamos el fichero
			l_ifile = Gio::File::create_for_path((*l_siter)->getIconFile());
			l_ofile = Gio::File::create_for_path(Glib::build_filename(l_icons_path, l_str.str()));
			l_ifile->copy(l_ofile, Gio::FILE_COPY_OVERWRITE);
		}
		else
			l_xml.writeAttribute("iconfile", "");

		// Comprobamos si se dan las condiciones para la copia del dat
		if(l_export_dats && Glib::file_test((*l_siter)->getDatFile(), Glib::FILE_TEST_EXISTS)){
			// Montamos el nombre del dat
			l_str.str("");
			l_str << "s_" << (*l_siter)->getId() << ".dat";

			l_xml.writeAttribute("datfile", Glib::build_filename(SYSTEMS_EXPORT_DATS_DIR, l_str.str()));
			// Copiamos el fichero
			l_ifile = Gio::File::create_for_path((*l_siter)->getDatFile());
			l_ofile = Gio::File::create_for_path(Glib::build_filename(l_dats_path, l_str.str()));
			l_ifile->copy(l_ofile , Gio::FILE_COPY_OVERWRITE);
		}
		else
			l_xml.writeAttribute("datfile", "");

		for(l_eiter = (*l_siter)->getEmulatorList()->children().begin();
			l_eiter != (*l_siter)->getEmulatorList()->children().end(); l_eiter++){
			l_emu = (*l_siter)->getEmulator(l_eiter);
			l_xml.startElement("emulator");
			l_xml.writeAttribute("id", l_emu->getId());
			l_xml.writeAttribute("name", l_emu->getName());
			l_xml.writeAttribute("description", l_emu->getDescription());
			l_xml.writeAttribute("version", l_emu->getVersion());
			l_xml.writeAttribute("author", l_emu->getAuthor());
			l_xml.writeAttribute("homepage", l_emu->getHomepage());
			l_xml.writeAttribute("bindir", l_emu->getPath());
			l_xml.writeAttribute("params", l_emu->getParams());
			// Comprobamos si se dan las condiciones para la copia del icono
			if(l_export_icons && Glib::file_test(l_emu->getIconFile(), Glib::FILE_TEST_EXISTS)){
				// Montamos el nombre del icono
				l_fname = l_emu->getIconFile();
				l_ind = l_fname.find_last_of(".");
				if(l_ind)
					l_fname = l_fname.substr(l_ind, l_fname.size());
				else
					l_fname = "";
				l_str.str("");
				l_str << "s_" << (*l_siter)->getId()<< "_e_"<< l_emu->getId()<< l_fname;
				l_xml.writeAttribute("iconfile", Glib::build_filename(SYSTEMS_EXPORT_ICONS_DIR, l_str.str()));
				// Copiamos el fichero
				l_ifile = Gio::File::create_for_path(l_emu->getIconFile());
				l_ofile = Gio::File::create_for_path(Glib::build_filename(l_icons_path, l_str.str()));
				l_ifile->copy(l_ofile, Gio::FILE_COPY_OVERWRITE);
			}
			else
				l_xml.writeAttribute("iconfile", "");
			l_xml.endElement();
			delete l_emu;
		}
		l_xml.endElement();
	}
	l_xml.endElement();
	l_xml.close();
	return true;
}

bool CSystemManager::exportGameList(const Glib::ustring& p_file){
	std::vector<CSystem*>::iterator l_iter;
	Gtk::TreeModel::iterator l_giter;
	std::ofstream l_file;
	CGame* l_game;

	GELIDE_DEBUG("Exporting games list...");
	l_file.open(p_file.data());
	if(l_file.good()){
		l_file << _("Games list\n");
		l_file <<   "-----------------\n";
		for(l_iter = m_systems.begin(); l_iter != m_systems.end(); l_iter++){
			l_file << "* " << (*l_iter)->getName() << " ";
			l_file << "[" << (*l_iter)->getDescription() << "]:\n";
			for(l_giter = (*l_iter)->getGameList()->children().begin();
				l_giter != (*l_iter)->getGameList()->children().end(); l_giter++){
				// CHECKME: LENTO??? puede ser mejor acceder a la tabla directamente
				l_game = (*l_iter)->getGame(l_giter);
				l_file << "\t# " << l_game->getName() << " ";
				l_file << "(" << l_game->getDescription() << ")\n";
				l_file << _("\tFlags: ");
				l_file << (l_game->getFavorite()? _("F "): "- ");
				l_file << (l_game->getPlayed()? _("P "): "- ");
				l_file << (l_game->getWorking()? _("W "): "- ");
				l_file << (l_game->getAvailable()? _("A "): "- ");
				l_file << (l_game->getUnknown()? _("U "): "-");
				l_file << " | ";
				l_file << _("Rank: ");
				l_file << ((l_game->getRank() > 0)? _("* "): "- ");
				l_file << ((l_game->getRank() > 1)? _("* "): "- ");
				l_file << ((l_game->getRank() > 2)? _("* "): "- ");
				l_file << ((l_game->getRank() > 3)? _("* "): "- ");
				l_file << ((l_game->getRank() > 4)? _("* "): "-\n");
				l_file << "\n";
				delete l_game;
			}
			l_file << "\n";
		}
		l_file.close();
		return true;
	}
	l_file.close();
	return false;
}

Glib::RefPtr<Gtk::ListStore>& CSystemManager::getSystemList(void){
	return m_system_list;
}

std::vector<CSystem*>& CSystemManager::getSystems(void){
	return m_systems;
}

Gtk::TreeModel::iterator CSystemManager::getSystem(const unsigned int p_id){
	Gtk::TreeModel::Children::iterator l_iter;
	Gtk::TreeModel::Row l_row;

	// Obtenemos el iterador
	for(l_iter = m_system_list->children().begin();
		l_iter != m_system_list->children().end(); l_iter++){
		if((*l_iter)[m_system_columns.m_id] == p_id)
			break;
	}
	return l_iter;
}

CSystem* CSystemManager::getSystem(Gtk::TreeModel::iterator& p_iter){
	std::vector<CSystem*>::iterator l_iter;

	assert(p_iter);

	// Este listado no va a tener muchos elementos, por lo que no merece
	// la pena implementar otro método de busqueda.
	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); l_iter++){
		if((*l_iter)->getId() == (*p_iter)[m_system_columns.m_id])
			return *l_iter;
	}
	return NULL;
}

Gtk::TreeModel::iterator CSystemManager::addSystem(CSystem& p_system){
	std::vector<CSystem*>::iterator l_iter;
	Glib::RefPtr<Gdk::Pixbuf> l_pixbuf;
	Gtk::TreeModel::iterator l_tree_iter;
	unsigned int l_index;

	// Comprobamos que tiene un nombre válido
	if(p_system.getName() == "")
		return l_tree_iter;

	l_index = 1;
	// Comprobamos si existe un sistema con el mismo identificador o nombre
	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); l_iter++){
		if((*l_iter)->getId() == p_system.getId())
			return l_tree_iter;
		else if((*l_iter)->getName() == p_system.getName()){
			(*l_iter)->update(p_system);
			return l_tree_iter;
		}
		// Obtenemos el mayor índice de los sistemas
		l_index = l_index >= (*l_iter)->getId() ? l_index : (*l_iter)->getId();
	}

	// Si es un sistema nuevo (Id = 0), establecemos el id adecuado
	if(p_system.getId() == 0)
		p_system.setId(l_index + 1);

	GELIDE_DEBUG("Adding system " << p_system.getName().data() << "...");
	// Establecemos el icono del sistema
	try{
		p_system.setIcon(Gdk::Pixbuf::create_from_file(p_system.getIconFile())->scale_simple(32, 32, Gdk::INTERP_BILINEAR));
	}
	catch(const Glib::Exception& l_exception){
		GELIDE_WARNING("Loading system icon (" << l_exception.what().c_str() << ")");
		p_system.setIcon(m_system_icon);
	}
	// Establecemos su home si no se ha asignado
	if(p_system.getHomeDir() == "")
		p_system.setHomeDir(Glib::build_filename(utils::getGelideDir(), p_system.getName()));

	// Comprobamos su jerarquía de directorios
	checkSystemDirs(p_system);
	// Establecemos los directorios por defecto si son necesarios
	if(p_system.getRomsDir() == "")
		p_system.setRomsDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_ROMS));
	if(p_system.getSnapDir() == "")
		p_system.setSnapDir(Glib::build_filename(p_system.getHomeDir(),  SYSTEMS_IMAGES_SNAP));
	if(p_system.getTitleDir() == "")
		p_system.setTitleDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_TITLE));
	if(p_system.getFrontDir() == "")
		p_system.setFrontDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_FRONT));
	if(p_system.getBackDir() == "")
		p_system.setBackDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_BACK));
	if(p_system.getMediaDir() == "")
		p_system.setMediaDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_MEDIA));

	// Añadimos a la lista
	m_systems.push_back(&p_system);
	// Añadimos al modelo
	l_tree_iter = m_system_list->append();
	(*l_tree_iter)[m_system_columns.m_icon] = p_system.getIcon();
	(*l_tree_iter)[m_system_columns.m_id] = p_system.getId();
	(*l_tree_iter)[m_system_columns.m_name] = p_system.getName();
	(*l_tree_iter)[m_system_columns.m_description] = p_system.getDescription();

	// Indicamos al sistema que se puede cargar
	p_system.load();

	return l_tree_iter;
}

void CSystemManager::removeSystem(Gtk::TreeModel::iterator& p_iter){
	std::vector<CSystem*>::iterator l_iter;

	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); l_iter++){
		if((*l_iter)->getId() == (*p_iter)[m_system_columns.m_id]){
			GELIDE_DEBUG("Deleting system " << (*l_iter)->getId() << "...");
			// CHECKME: No se si esto elimina bien el sistema.
			delete *l_iter;
			m_systems.erase(l_iter);
			m_system_list->erase(p_iter);
			return;
		}
	}
}

void CSystemManager::updateSystem(Gtk::TreeModel::iterator& p_iter, CSystem& p_system){
	CSystem* l_system;

	l_system = getSystem(p_iter);
	if(l_system){
		GELIDE_DEBUG("Updating system " << l_system->getName().data() << "...");
		*l_system = p_system;
		// Establecemos su home si no se ha asignado
		if(l_system->getHomeDir() == "")
			l_system->setHomeDir(Glib::build_filename(utils::getGelideDir(), l_system->getName()));
		// Comprobamos estructura de directorios del sistema
		checkSystemDirs(*l_system);
		// Establecemos los directorios por defecto si son necesarios
		if(l_system->getRomsDir() == "")
			l_system->setRomsDir(Glib::build_filename(l_system->getHomeDir(), SYSTEMS_ROMS));
		if(l_system->getSnapDir() == "")
			l_system->setSnapDir(Glib::build_filename(l_system->getHomeDir(), SYSTEMS_IMAGES_SNAP));
		if(l_system->getTitleDir() == "")
			l_system->setTitleDir(Glib::build_filename(l_system->getHomeDir(), SYSTEMS_IMAGES_TITLE));
		if(l_system->getFrontDir() == "")
			l_system->setFrontDir(Glib::build_filename(l_system->getHomeDir(), SYSTEMS_IMAGES_FRONT));
		if(l_system->getBackDir() == "")
			l_system->setBackDir(Glib::build_filename(l_system->getHomeDir(), SYSTEMS_IMAGES_BACK));
		if(l_system->getMediaDir() == "")
			l_system->setMediaDir(Glib::build_filename(l_system->getHomeDir(), SYSTEMS_IMAGES_MEDIA));
		// Actualizamos su entrada en el listmodel
		(*p_iter)[m_system_columns.m_icon] = l_system->getIcon();
		(*p_iter)[m_system_columns.m_name] = l_system->getName();
		(*p_iter)[m_system_columns.m_description] = l_system->getDescription();
	}
}

void CSystemManager::moveSystemFirst(Gtk::TreeModel::iterator& p_iter){
	std::vector<CSystem*>::iterator l_iter;
	Gtk::TreeModel::iterator l_dest;
	CSystem* l_tmp;


	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); ++l_iter){
		if((*l_iter)->getId() == (*p_iter)[m_system_columns.m_id]){
			while(l_iter != m_systems.begin()){
				l_tmp = (*(l_iter - 1));
				*(l_iter-1) = *l_iter;
				*l_iter = l_tmp;
				--l_iter;
			}

			// Intercambiamos las filas en el listado de columnas
			l_dest = m_system_list->children().begin();
			m_system_list->move(p_iter, l_dest);

			return;
		}
	}
}

void CSystemManager::moveSystemUp(Gtk::TreeModel::iterator& p_iter){
	std::vector<CSystem*>::iterator l_iter;
	Gtk::TreeModel::iterator l_orig;
	CSystem* l_tmp;

	// Buscamos el systema a mover
	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); ++l_iter){
		if((*l_iter)->getId() == (*p_iter)[m_system_columns.m_id]){
			// Movemos arriba en el vector de sistemas
			l_tmp = (*(l_iter - 1));
			*(l_iter-1) = *l_iter;
			*l_iter = l_tmp;

			// Movemos arriba en el modelo de sistemas
			l_orig = p_iter--;
			m_system_list->move(l_orig, p_iter);

			return;
		}
	}
}

void CSystemManager::moveSystemDown(Gtk::TreeModel::iterator& p_iter){
	std::vector<CSystem*>::iterator l_iter;
	Gtk::TreeModel::iterator l_orig;
	CSystem* l_tmp;

	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); ++l_iter){
		if((*l_iter)->getId() == (*p_iter)[m_system_columns.m_id]){
			l_tmp = *(l_iter + 1);
			*(l_iter + 1) = *l_iter;
			*l_iter = l_tmp;

			// Movemos arriba en el modelo de sistemas
			l_orig = p_iter++;
			++p_iter;
			m_system_list->move(l_orig, p_iter);
			return;
		}
	}
}

void CSystemManager::moveSystemLast(Gtk::TreeModel::iterator& p_iter){
	std::vector<CSystem*>::iterator l_iter;
	Gtk::TreeModel::iterator l_dest;
	CSystem* l_tmp;


	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); ++l_iter){
		if((*l_iter)->getId() == (*p_iter)[m_system_columns.m_id]){
			while((l_iter + 1) != m_systems.end()){
				l_tmp = (*(l_iter + 1));
				*(l_iter+1) = *l_iter;
				*l_iter = l_tmp;
				++l_iter;
			}
			// Intercambiamos las filas en el listado de columnas
			l_dest = m_system_list->children().end();
			m_system_list->move(p_iter, l_dest);

			return;
		}
	}
}


unsigned int CSystemManager::getGames(void){
	std::vector<CSystem*>::iterator l_iter;
	unsigned int l_ret;

	l_ret = 0;

	for(l_iter = m_systems.begin(); l_iter != m_systems.end(); l_iter++){
		l_ret += (*l_iter)->getGames();
	}

	return l_ret;

}

bool CSystemManager::checkSystemDirs(CSystem& p_system){
	// Comprobamos su directorio home y si existe asumimos que toda la
	// jerarquía se creo con anterioridad
	if(!Glib::file_test(p_system.getHomeDir(),	Glib::FILE_TEST_EXISTS)){
		if(!utils::createDir(p_system.getHomeDir()))
			return false;
		// Debemos crear los directorios internos del sistema
		// Directorio de imágenes
		if(!utils::createDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES)))
			return false;
		// Directorio de roms
		if(!utils::createDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_ROMS)))
			return false;
		// Directorio de snapshot
		if(!utils::createDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_SNAP)))
			return false;
		// Directorio de titles
		if(!utils::createDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_TITLE)))
			return false;
		// Directorio de fronts
		if(!utils::createDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_FRONT)))
			return false;
		// Directorio de back
		if(!utils::createDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_BACK)))
			return false;
		// Directorio de Soportes
		if(!utils::createDir(Glib::build_filename(p_system.getHomeDir(), SYSTEMS_IMAGES_MEDIA)))
			return false;
	}
	return true;
}
