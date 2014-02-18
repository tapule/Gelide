/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gelide
 * Copyright (C) 2008 - 2014 Juan Ángel Moreno Fernández
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
#include "clrmamepro_reader.hpp"
#include "logiqxxml_reader.hpp"
#include "mamexml_reader.hpp"
#include "hyperspinxml_reader.hpp"


namespace gelide{

DatReader* DatReaderFactory::getDatReader(const Glib::ustring& file)
{
	char* buffer = NULL;
	std::ifstream file_stream;
	unsigned int size;
	DatReader* reader = NULL;

	assert(!file.empty());

	file_stream.open(file.data());
	// Comprobamos si la apertura fue correcta
	if (!file_stream.good())
	{
		file_stream.close();
		return NULL;
	}
	// Obtenemos el tamaño del fichero
	file_stream.seekg(0, std::ios::end);
	size = file_stream.tellg();
	file_stream.seekg(0, std::ios::beg);

	// Reservamos la memoria necesaria
	buffer = new char[size];
	if (!buffer)
	{
		file_stream.close();
		return NULL;
	}
	// Cargamos el contenido completo del fichero en memoria
	file_stream.read(buffer, size);
	file_stream.close();
	// Buscamos el lector adecuado
	// Chequeamos CMPro
	reader = new ClrMameProReader();
	if (reader->load(buffer, size))
	{
		delete[] buffer;
		return reader;
	}
	delete reader;
	// Chequeamos Logiqx xml
	reader = new LogiqxXmlReader();
	if (reader->load(buffer, size))
	{
		delete[] buffer;
		return reader;
	}
	delete reader;
	// Chequeamos Mame xml
	reader = new MameXmlReader();
	if (reader->load(buffer, size))
	{
		delete[] buffer;
		return reader;
	}
	delete reader;
	// Chequeamos Hyperspin xml
	reader = new HyperspinXmlReader();
	if (reader->load(buffer, size))
	{
		delete[] buffer;
		return reader;
	}
	delete reader;

	// No encontramos ningún lector adecuado para el fichero
	delete[] buffer;
	return NULL;
}

} // namespace gelide
