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

#ifndef _GELIDE_UTILS_HPP_
#define _GELIDE_UTILS_HPP_

#include <sstream>
#include <vector>
#include <glibmm/ustring.h>
#include <glibmm/miscutils.h>
#include "../gelide.hpp"
#include <gdkmm/pixbuf.h>


namespace utils{


// CHECKME: ¿Esta función no debería pasarla al iconManager?
Glib::RefPtr<Gdk::Pixbuf> createNegativePixbuf(Glib::RefPtr<Gdk::Pixbuf> p_pixbuf);

bool createDir(const Glib::ustring& p_path);

/**
 * Obtiene el año actual del sistema
 * @return Año actual
 */
unsigned int getYear(void);

/**
 * Obtiene el path al directorio home de Gelide
 * @return Path al directorio home de Gelide
 */
Glib::ustring getGelideDir(void);

/**
 * Obtiene el path a la lista de sistemas del usuario
 * @return Path a la lista de sistemas del usuario
 */
Glib::ustring getSystemsList(void);

/**
 * Realiza la conversión de un tipo básico a cadena
 * @param p_value Tipo básico a convertir a cadena
 * @return Cadena con la conversión textual
 * @example
 * Glib::ustring cad = toStr(127);
 * Glib::ustring cad = toStr(true);
 */
template<class T>
Glib::ustring toStr(const T& p_value){
	std::stringstream l_str;

	l_str << p_value;
	return l_str.str();
}

/**
 * Especialización de toStr para cadenas, de forma que solo copie
 * @param p_str Cadena de texto con el valor original
 * @return Cadena con el valor de la entrada
 */
template<>
inline Glib::ustring toStr(const Glib::ustring& p_value){

	return p_value;
}


/**
 * Realiza la conversión de un rango de valores a una cadena
 * @param p_begin Iterador al comienzo del rango
 * @param p_end Iterador al final del rango (uno después del último)
 * @return Cadena de texto con la conversión en formato "val, val, ..."
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad;
 * vec.push_back(123);
 * vec.push_back(63);
 * cad = toStr(vec.begin(), vec.end());
 */
template<class T>
Glib::ustring toStr(T p_begin, T p_end){
	std::stringstream l_str;

	// Nos aseguramos de que hay algún elemento
	if(p_begin != p_end){
		// Añadimos el primer elemento que sabemos debe existir
		l_str << *p_begin++;
		// Añadimos el resto de elementos y las comas necesarias
		while(p_begin != p_end)
			l_str<< ", " << *p_begin++;
	}
	return l_str.str();
}

/**
 * Realiza la conversión de un vector de valores a una cadena de texto
 * @param p_vec vector con los valores
 * @return Cadena de texto con la conversión en formato "val, val, ..."
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad;
 * vec.push_back(123);
 * vec.push_back(63);
 * cad = vectorToStr(vec);
 */
template<class T>
Glib::ustring toStr(const std::vector<T>& p_vec){
	return toStr(p_vec.begin(), p_vec.end());
}

/**
 * Realiza la conversión de una cadena a un tipo básico
 * @param p_str Cadena de texto con el valor a extraer
 * @return Valor convertido al tipo básico
 * @example
 * Glib::ustring cad = "127";
 * int val = strTo<int>(cad);
 */
template<class T>
void strTo(const Glib::ustring& p_str, T& p_value){
	std::stringstream l_str;

	l_str << p_str.c_str();
	l_str >> p_value;
}

/**
 * Especialización de strTo para cadenas, de forma que solo copie
 * @param p_str Cadena de texto con el valor original
 * @param p_value Cadena de destino para realizar la copia
 */
template<>
inline void strTo(const Glib::ustring& p_str, Glib::ustring& p_value){
	p_value = p_str;
}

/**
 * Extrae valores de una cadena de valores (del mismo tipo) separados por comas
 * y los va insertando en p_out (output iterator)
 * @param p_str Cadena con la lista de valores separados por comas
 * @param p_out Iterador de salida en el que se van insertando los valores
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad = "123, 34, 54";
 * strTo<int>(cad, std::back_inserter(vec));
 */
template<class Q, class T>
void strTo(const Glib::ustring& p_str, T p_out){
	int l_pos;
	unsigned int l_oldpos;
	std::stringstream l_str;
	Q l_ret;

	// Si la cadena no contiene nada, terminamos
	if(p_str.size() == 0)
		return;

	// Puntero de cabecera y cola en la cadena
	l_oldpos = 0;
	l_pos = p_str.find("," , 0);
	// Si hay cadenas
	while(l_pos > -1){
		// Convertimos y añadimos la substring
		l_str.clear();
		l_str << p_str.substr(l_oldpos, l_pos - l_oldpos).c_str();
		l_str >> l_ret;
		*p_out++ = l_ret;

		// Saltamos , o ", "
		if(p_str[l_pos + 1] == ' ')
			l_oldpos = l_pos + 2;
		else
			l_oldpos = l_pos + 1;
		// Buscamos la siguiente ","
		l_pos = p_str.find("," , l_oldpos);
	}

	if(l_oldpos != p_str.size()){
		// Convertimos y añadimos la substring
		l_str.clear();
		l_str << p_str.substr(l_oldpos, p_str.size()).c_str();
		l_str >> l_ret;
		*p_out = l_ret;
	}
}

/**
 * Extrae valores de una cadena de valores (del mismo tipo) separados por comas
 * y los va insertando en un vector
 * @param p_str Cadena con la lista de valores separados por comas
 * @param p_out Vector donde se dejarán los datos extraidos
 * @example
 * std::vector<int> vec;
 * Glib::ustring cad = "123, 34, 54";
 * strTo(cad, vec);
 */
template<class T>
void strTo(const Glib::ustring& p_str, std::vector<T>& p_vec){
	p_vec.clear();
	strTo<T>(p_str, std::back_inserter(p_vec));
}

/**
 * Implementación genérica del algoritmo de búsqueda binaria en un vector
 * @param p_vec Vector en el que realizar la búsqueda
 * @param p_key Elemento a buscar dentro del vector
 * @param p_cmp_fun Callback utilizado para realizar la comparación de elementos
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
inline int binarySearch(const std::vector<T>& p_vec, const Q& p_key,
						int p_cmp_fun(T p_val0, Q p_val1))
{
	int l_top = p_vec.size() - 1;
	int l_bottom = 0;
	int l_center;
	int l_cmp;

	while (l_bottom <= l_top){
		l_center = (l_top + l_bottom)/2;
		l_cmp = p_cmp_fun(p_vec[l_center], p_key);
		if(l_cmp == 0)
			return l_center;
		else{
			if(l_cmp > 0)
				l_top = l_center - 1;
			else
				l_bottom = l_center + 1;
			}
	}
	return -1;
}


} // namespace utils



#endif // _GELIDE_UTILS_HPP_
