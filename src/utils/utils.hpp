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

#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif /* HAVE_CONFIG_H */

// Si no está definido el modo debug, desactivamos los asserts
#ifndef ENABLE_DEBUG_MODE
	#define NDEBUG
#endif

#include <cassert>
#include <sstream>
#include <vector>
#include <glibmm.h>


namespace utils{

/**
 * Crea un nuevo directorio con un nombre dado
 * @param path Ruta al nuevo directorio
 * @return True si se pudo crear el directorio, false en otro caso
 */
bool createDir(const Glib::ustring& path);

/**
 * Comprueba si existe un directorio y en caso negativo lo crea
 * @param path Ruta al directorio a comprobar
 * @return True si existe o se creó sin problemas, false en otro caso.
 */
bool checkOrCreateDir(const Glib::ustring& path);

/**
 * Elimina un directorio de forma recursiva
 * @param path Ruta al directorio a eliminar
 * @return True si se pudo realizar la operación, false en otro caso
 */
bool removeDir(const Glib::ustring& path);

/**
 * Realiza la copia de un fichero
 * @param orig Ruta al fichero origen
 * @param dest Ruta al fichero destino
 * @return True si se pudo realizar la copia, false en otro caso
 * @note Si el fichero destino existe, será sobreescrito
 */
bool copyFile(const Glib::ustring& orig, const Glib::ustring& dest);

/**
 * Obtiene todos los nombres de ficheros de un directorio ignorando directorios
 * @param path Ruta al directorio donde leer los ficheros
 * @param files Vector donde almacenar los nombres de los ficheros
 * @return True si se pudo realizar la operación, false en otro caso
 * @note El vector de destino será vaciado previamente
 */
bool getFiles(const Glib::ustring& path, std::vector<Glib::ustring>& files);

/**
 * Obtiene todos los nombres de directorios de un directorio ignorando ficheros
 * @param path Ruta al directorio donde leer los directorios
 * @param directories Vector donde almacenar los nombres de los directorios
 * @return True si se pudo realizar la operación, false en otro caso
 * @note El vector de destino será vaciado previamente
 * @note Se ignoran las entradas "." y ".."
 */
bool getDirectories(const Glib::ustring& path, std::vector<Glib::ustring>& directories);

/**
 * Obtiene todos los ficheros de un directorio que comienzar por pattern
 * @param path Ruta al directorio donde leer los ficheros
 * @param files Vector donde se guardan las rutas a los ficheros
 * @param pattern Patrón de comienzo para los nombres de los ficheros o ""
 * 		  para cualquier fichero
 * @return True si se pudo realizar la operación, false en otro caso
 */
bool findFiles(const Glib::ustring& path, std::vector<Glib::ustring>& files, const Glib::ustring& pattern);

/**
 * Obtiene todos los ficheros de un directorio que comienzar por pattern de
 * forma recursiva
 * @param path Ruta al directorio donde leer los ficheros
 * @param files Vector donde se guardan las rutas a los ficheros
 * @param pattern Patrón de comienzo para los nombres de los ficheros o ""
 * 		  para cualquier fichero
 * @return True si se pudo realizar la operación, false en otro caso
 */
bool findFilesRecursive(const Glib::ustring& path, std::vector<Glib::ustring>& files, const Glib::ustring& pattern);

/**
 * Genera una palabra aleatoriamente
 * @param min_chars Número mínimo de caracteres que tendrá la palabra
 * @param max_chars Número máximo de caracteres que tendrá la palabra
 * @return Palabra generada
 */
Glib::ustring getRndWord(const int min_chars, const int max_chars);

/**
 * Genera una frase de palabras aleatorias
 * @param min_words Número mínimo de palabras que tendrá la frase
 * @param max_words Número máximo de palabras que tendrá la frase
 * @return Frase generada
 */
Glib::ustring getRndSentence(const int min_words, const int max_words);

/**
 * Obtiene el año actual del sistema
 * @return Año actual
 */
unsigned int getYear(void);

/**
 * Obtiene un identificador en base a la fecha y hora actuales
 * @return Representación de la fecha en el siguiente formato "AMMDDSSSSS" donde
 * A = último dígito del año, MM = Més del año, DD = Día del més, SSSSS = Hora
 * actual en formato de segundos [(h * 3600) + [m * 60] + s]
 * @nota Este ID se puede utilizar como uid teniendo en cuenta que cambia cada
 * segundo y que serán ID's únicos hasta 2020.
 */
unsigned int getTimeID(void);

/**
 * Obtiene la primera linea de un texto dado
 * @param text Texto del que obtener la primera linea delimitada por "\n"
 * @return Primera linea del texto dado
 */
Glib::ustring getTextFirstLine(const Glib::ustring& text);

/**
 * Divide una cadena de texto en partes según un delimitador
 * @param str Cadena que dividir en partes
 * @return delim Carácter usado para la división de la cadena
 */
std::vector<std::string> explode(const std::string& str, char delim);

/**
 * Realiza la conversión de un tipo básico a cadena
 * @param value Tipo básico a convertir a cadena
 * @return Cadena con la conversión textual
 * @example
 * Glib::ustring cad = toStr(127);
 * Glib::ustring cad = toStr(true);
 */
template<class T>
Glib::ustring toStr(const T& value)
{
	std::stringstream str;

	str << value;
	return str.str();
}

/**
 * Especialización de toStr para cadenas, de forma que solo copie
 * @param str Cadena de texto con el valor original
 * @return Cadena con el valor de la entrada
 */
template<>
inline Glib::ustring toStr(const Glib::ustring& value)
{
	return value;
}

/**
 * Realiza la conversión de un rango de valores a una cadena
 * @param begin Iterador al comienzo del rango
 * @param end Iterador al final del rango (uno después del último)
 * @return Cadena de texto con la conversión en formato "val, val, ..."
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad;
 * vec.push_back(123);
 * vec.push_back(63);
 * cad = toStr(vec.begin(), vec.end());
 */
template<class T>
Glib::ustring toStr(T begin, T end)
{
	std::stringstream str;

	// Nos aseguramos de que hay algún elemento
	if (begin != end)
	{
		// Añadimos el primer elemento que sabemos debe existir
		str << *begin++;
		// Añadimos el resto de elementos y las comas necesarias
		while (begin != end)
		{
			str<< ", " << *begin++;
		}
	}
	return str.str();
}

/**
 * Realiza la conversión de un vector de valores a una cadena de texto
 * @param vec vector con los valores
 * @return Cadena de texto con la conversión en formato "val, val, ..."
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad;
 * vec.push_back(123);
 * vec.push_back(63);
 * cad = vectorToStr(vec);
 */
template<class T>
Glib::ustring toStr(const std::vector<T>& vec)
{
	return toStr(vec.begin(), vec.end());
}

/**
 * Realiza la conversión de una cadena a un tipo básico
 * @param str Cadena de texto con el valor a extraer
 * @return Valor convertido al tipo básico
 * @example
 * Glib::ustring cad = "127";
 * int val = strTo<int>(cad);
 */
template<class T>
void strTo(const Glib::ustring& str, T& value)
{
	std::stringstream str_stream;

	str_stream << str.c_str();
	str_stream >> value;
}

/**
 * Especialización de strTo para cadenas, de forma que solo copie
 * @param str Cadena de texto con el valor original
 * @param value Cadena de destino para realizar la copia
 */
template<>
inline void strTo(const Glib::ustring& str, Glib::ustring& value)
{
	value = str;
}

/**
 * Extrae valores de una cadena de valores (del mismo tipo) separados por comas
 * y los va insertando en p_out (output iterator)
 * @param str Cadena con la lista de valores separados por comas
 * @param out Iterador de salida en el que se van insertando los valores
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad = "123, 34, 54";
 * strTo<int>(cad, std::back_inserter(vec));
 */
template<class Q, class T>
void strTo(const Glib::ustring& str, T out)
{
	int pos;
	unsigned int old_pos;
	std::stringstream str_stream;
	Q ret;

	// Si la cadena no contiene nada, terminamos
	if (str.empty()){
		return;
	}

	// Puntero de cabecera y cola en la cadena
	old_pos = 0;
	pos = str.find("," , 0);
	// Si hay cadenas
	while (pos > -1){
		// Convertimos y añadimos la substring
		str_stream.clear();
		str_stream << str.substr(old_pos, pos - old_pos).c_str();
		str_stream >> ret;
		*out++ = ret;

		// Saltamos , o ", "
		if (str[pos + 1] == ' ')
		{
			old_pos = pos + 2;
		}
		else
		{
			old_pos = pos + 1;
		}
		// Buscamos la siguiente ","
		pos = str.find("," , old_pos);
	}

	if (old_pos != str.size())
	{
		// Convertimos y añadimos la substring
		str_stream.clear();
		str_stream << str.substr(old_pos, str.size()).c_str();
		str_stream >> ret;
		*out = ret;
	}
}

/**
 * Extrae valores de una cadena de valores (del mismo tipo) separados por comas
 * y los va insertando en un vector
 * @param str Cadena con la lista de valores separados por comas
 * @param out Vector donde se dejarán los datos extraidos
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad = "123, 34, 54";
 * strTo(cad, vec);
 */
template<class T>
void strTo(const Glib::ustring& str, std::vector<T>& vec)
{
	vec.clear();
	strTo<T>(str, std::back_inserter(vec));
}

/**
 * Implementación genérica del algoritmo de búsqueda binaria en un vector
 * @param vec Vector en el que realizar la búsqueda
 * @param key Elemento a buscar dentro del vector
 * @param cmp_fun Callback utilizado para realizar la comparación de elementos
 * @return Posición en el vector del elemento si se encuentra, -1 en otro caso
 * @note p_cmp_fun debe ser una función que devuelve 0 si los elementos son
 * considerados iguales, menor que 0 si p_val0 se considera menor que p_val1,
 * mayor que 0 si p_val0 se considera mayor que p_val1
 * Definimos dos parámetros en el template, para permitir comparación de los
 * elementos del vector con otro tipo de elementos
 * @note La función necesita que el vector esté ordenado por el campo que se
 * comparará.
 */
template<class T, class Q>
inline int binarySearch(const std::vector<T>& vec, const Q& key, int cmp_fun(T val0, Q val1))
{
	int top = vec.size() - 1;
	int bottom = 0;
	int center;
	int cmp;

	while (bottom <= top)
	{
		center = (top + bottom)/2;
		cmp = cmp_fun(vec[center], key);
		if (cmp == 0)
		{
			return center;
		}
		else
		{
			if (cmp > 0)
			{
				top = center - 1;
			}
			else
			{
				bottom = center + 1;
			}
		}
	}
	return -1;
}


} // namespace utils

#endif // _UTILS_HPP_
