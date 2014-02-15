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

#include "utils.hpp"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include "log.hpp"
//#include <glibmm/fileutils.h>
//#include <glibmm.h>
//#include <glib/gstdio.h>


namespace utils{

bool createDir(const Glib::ustring& path)
{

	if (g_mkdir_with_parents((path).c_str(), 0755))
	{
		return false;
	}
	return true;
}

bool checkOrCreateDir(const Glib::ustring& path)
{

	if (!Glib::file_test(path, Glib::FILE_TEST_EXISTS))
	{
		if (!createDir(path))
		{
			return false;
		}
	}
	return true;
}

bool removeDir(const Glib::ustring& path)
{
	Glib::Dir::iterator dir_iter;
	Glib::ustring file;

	assert(!path.empty());

	try
	{
		// Abrimos el directorio y vamos eliminando ficheros y directorios
		Glib::Dir dir(path);
		for (dir_iter = dir.begin(); dir_iter != dir.end(); dir_iter++)
		{
			file = Glib::build_filename(path, (*dir_iter));
			if (Glib::file_test(file.c_str(), Glib::FILE_TEST_IS_DIR))
			{
				removeDir(file);
			}
			else
			{
				if (remove(file.c_str()) != 0)
				{
					return false;
				}
			}
		}
		if (remove(path.c_str()) != 0)
		{
			return false;
		}
		return true;
	}
	catch (Glib::Error& e)
	{
		LOG_ERROR("Utils: Removing directory (" << e.what().c_str() << ")");
	}
	return false;
}

bool copyFile(const Glib::ustring&orig, const Glib::ustring& dest)
{
  std::ifstream in_file(orig.c_str(), std::ios::in | std::ios::binary);
  std::ofstream out_file(dest.c_str(),  std::ios::out | std::ios::binary | std::ios::trunc);

  if (!in_file || !out_file)
  {
	  return false;
  }

  out_file << in_file.rdbuf();

  in_file.close();
  out_file.close();
  return true;
}

bool getFiles(const Glib::ustring& path, std::vector<Glib::ustring>& files)
{
	Glib::Dir::iterator dir_iter;
	Glib::ustring file;

	assert(!path.empty());

	try
	{
		// Abrimos el directorio y vamos obteniendo los ficheros
		Glib::Dir dir(path);
		files.clear();
		for (dir_iter = dir.begin(); dir_iter != dir.end(); dir_iter++)
		{
			file = Glib::build_filename(path, (*dir_iter));
			if (Glib::file_test(file.c_str(), Glib::FILE_TEST_IS_REGULAR))
			{
				files.push_back((*dir_iter));
			}
		}
		return true;
	}
	catch (Glib::Error& e)
	{
		LOG_ERROR("Utils: Reading files (" << e.what().c_str() << ")");
	}
	return false;
}

bool getDirectories(const Glib::ustring& path, std::vector<Glib::ustring>& directories)
{
	Glib::Dir::iterator dir_iter;
	Glib::ustring file;

	assert(!path.empty());

	try
	{
		// Abrimos el directorio y vamos obteniendo los directorios
		Glib::Dir dir(path);
		directories.clear();
		for (dir_iter = dir.begin(); dir_iter != dir.end(); dir_iter++)
		{
			file = Glib::build_filename(path, (*dir_iter));
			if (Glib::file_test(file.c_str(), Glib::FILE_TEST_IS_DIR))
			{
				directories.push_back((*dir_iter));
			}
		}
		return true;
	}
	catch (Glib::Error& e)
	{
		LOG_ERROR("Utils: Reading files (" << e.what().c_str() << ")");
	}
	return false;
}

bool findFiles(const Glib::ustring& path, std::vector<Glib::ustring>& files, const Glib::ustring& pattern)
{
	Glib::Dir::iterator dir_iter;
	Glib::ustring file;

	assert(!path.empty());

	try
	{
		// Abrimos el directorio y vamos obteniendo los directorios
		Glib::Dir dir(path);
		files.clear();
		for (dir_iter = dir.begin(); dir_iter != dir.end(); dir_iter++)
		{
			file = Glib::build_filename(path, (*dir_iter));
			if (Glib::file_test(file.c_str(), Glib::FILE_TEST_IS_REGULAR) && (*dir_iter).find(pattern) == 0)
			{
				files.push_back(file);
			}
		}
		return true;
	}
	catch (Glib::Error& e)
	{
		LOG_ERROR("Utils: Finding files (" << e.what().c_str() << ")");
	}
	return false;
}

/**
 * Obtiene todos los ficheros de un directorio que comienzar por pattern de
 * forma recursiva
 * @param path Ruta al directorio donde leer los ficheros
 * @param files Vector donde se guardan las rutas a los ficheros
 * @param pattern Patrón de comienzo para los nombres de los ficheros o ""
 * 		  para cualquier fichero
 * @return True si se pudo realizar la operación, false en otro caso
 * @note El vector p_files no será vaciado en esta función
 */
bool findFilesRecursiveExt(const Glib::ustring& path, std::vector<Glib::ustring>& files, const Glib::ustring& pattern)
{
	Glib::Dir::iterator dir_iter;
	Glib::ustring file;

	try
	{
		Glib::Dir dir(path);
		for (dir_iter = dir.begin(); dir_iter != dir.end(); dir_iter++)
		{
			file = Glib::build_filename(path, (*dir_iter));
			if (Glib::file_test(file.c_str(), Glib::FILE_TEST_IS_DIR))
			{
				findFilesRecursiveExt(file, files, pattern);
			}
			else if((*dir_iter).find(pattern) == 0)
			{
				files.push_back(file);
			}
		}
		return true;
	}
	catch (Glib::Error& e)
	{
		LOG_ERROR("Utils: Finding files (" << e.what().c_str() << ")");
	}
	return false;
}

bool findFilesRecursive(const Glib::ustring& path, std::vector<Glib::ustring>& files, const Glib::ustring& pattern)
{

	assert(!path.empty());

	files.clear();
	return findFilesRecursiveExt(path, files, pattern);
}

Glib::ustring getRndWord(const int min_chars, const int max_chars)
{
	int min, max, len;
	Glib::ustring ret;
	char c;

	// Ordenamos los límites
	if (min_chars > max_chars)
	{
		min = max_chars;
		max = min_chars;
	}
	else
	{
		min = min_chars;
		max = max_chars;
	}

	// Ajustamos los límites
	if (min <= 0)
	{
		min = 1;
	}
	if (max <= 0)
	{
		max = 1;
	}

	// Obtenemos la longitud de la palabra
	len = (rand() % (max - min + 1)) + min;

	// Generamos la palabra
	while (len)
	{
		c = 'a' + rand() % 26;
		ret.push_back(c);
		--len;
	}

	return ret;
}

Glib::ustring getRndSentence(const int min_words, const int max_words)
{
	int min, max, len;
	Glib::ustring ret;

	if (min_words > max_words)
	{
		min = max_words;
		max = min_words;
	}
	else
	{
		min = min_words;
		max = max_words;
	}

	if (min <= 0)
	{
		min = 1;
	}
	if (max <= 0)
	{
		max = 1;
	}

	len = (rand() % (max - min + 1)) + min;

	while (len)
	{
		ret.append(getRndWord(1, 8));
		--len;
		// Insertamos un espacio si no es la última
		if (len > 0){
			ret.push_back(' ');
		}
	}

	return ret;
}

unsigned int getYear(void)
{
	time_t raw_time;
	struct tm* time_info;

	time( &raw_time );
	time_info = localtime ( &raw_time );

	return time_info->tm_year + 1900;
}

unsigned int getTimeID(void)
{
	time_t raw_time;
	struct tm* time_info;
	unsigned int year;

	time(&raw_time );
	time_info = localtime(&raw_time);

	year = ((time_info->tm_year + 1900) % 10);
	return (year * 1000000000) + ((time_info->tm_mon+1) * 10000000) + (time_info->tm_mday * 100000) + (time_info->tm_hour * 3600) +	(time_info->tm_min * 60) + (time_info->tm_sec);
}

Glib::ustring getTextFirstLine(const Glib::ustring& text)
{
	int pos;

	// Buscamos la primera aparición del salto de linea
	pos = text.find("\n" , 0);
	// Si se encuentra devolvemos la porción hasta el salto
	if (pos > -1)
	{
		return text.substr(0, pos);
	}
	else
	{
		// Si no hay salto de linea, devolvemos la descripción completa
		return text;
	}
}

} // namespace utils
