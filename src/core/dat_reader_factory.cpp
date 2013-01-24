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
#include "dat_reader_factory.hpp"
#include "dat_reader_clrmamepro.hpp"
#include "dat_reader_logiqxxml.hpp"
#include "dat_reader_mamexml.hpp"

CDatReader* CDatReaderFactory::getDatReader(const Glib::ustring& p_file){
	char* l_buffer = NULL;
	std::ifstream l_file;
	int l_fsize;
	CDatReader* l_reader = NULL;

	// El fichero debe ser válido y no puede ser un directorio
	if(!p_file.size() || Glib::file_test(p_file, Glib::FILE_TEST_IS_DIR))
		return NULL;

	l_file.open(p_file.data());
	// Comprobamos si la apertura fue correcta
	if(!l_file.good()){
		l_file.close();
		return NULL;
	}
	// Obtenemos el tamaño del fichero
	l_file.seekg (0, std::ios::end);
	l_fsize = l_file.tellg();
	l_file.seekg (0, std::ios::beg);

	// Reservamos la memoria necesaria
	l_buffer = new char[l_fsize];
	if(!l_buffer){
		l_file.close();
		return NULL;
	}

	// Cargamos el contenido completo del fichero en memoria
	l_file.read (l_buffer, l_fsize);
	l_file.close();

	// Buscamos el lector adecuado
	// Chequeamos CMPro
	GELIDE_DEBUG("Checking ClrMamePro dat format...");
	l_reader = new CDatReaderClrMamePro();
	if(l_reader->load(l_buffer, l_fsize)){
		delete[] l_buffer;
		return l_reader;
	}
	delete l_reader;
	// Chequeamos Logiqx xml
	GELIDE_DEBUG("Checking Logiqx xml dat format...");
	l_reader = new CDatReaderLogiqxXml();
	if(l_reader->load(l_buffer, l_fsize)){
		delete[] l_buffer;
		return l_reader;
	}
	delete l_reader;
	// Chequeamos Mame xml
	GELIDE_DEBUG("Checking Mame xml dat format...");
	l_reader = new CDatReaderMameXml();
	if(l_reader->load(l_buffer, l_fsize)){
		delete[] l_buffer;
		return l_reader;
	}
	delete l_reader;

	// No encontramos ningún lector adecuado para el fichero
	delete[] l_buffer;
	return NULL;
}

/*CDatReader* CDatReaderFactory::getDatReader(const Glib::ustring& p_file){
	CDatReader* l_reader = NULL;

	// El fichero debe ser válido
	assert(p_file.size());

	// Buscamos el lector adecuado
	// Chequeamos CMPro
	l_reader = new CDatReaderClrMamePro();
	if(l_reader->open(p_file)){
		return l_reader;
	}
	delete l_reader;
	// Chequeamos Logiqx xml
	l_reader = new CDatReaderLogiqxXml();
	if(l_reader->open(p_file)){
		return l_reader;
	}
	delete l_reader;
	// Chequeamos Mame xml
	l_reader = new CDatReaderMameXml();
	if(l_reader->open(p_file)){
		return l_reader;
	}
	delete l_reader;

	return NULL;
}*/
