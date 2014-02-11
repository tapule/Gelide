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

#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include "gelide.hpp"
#include <glibmm/ustring.h>
#include <map>
#include "utils/utils.hpp"

/**
 * Sistema de configuración centralizado.
 *
 * Se encarga de gestionar el almacen de claves de configuración formadas por
 * tripetas "Grupo, clave, valor".
 *
 * Esta definido como un singleton, por lo que para acceder a su instancia
 * única, tendremos que llamar a getInstance()
 */
class CConfig
{
public:

	/* Alias de un mapa de pares [clave, valor] indexado por la clave */
	typedef std::map<Glib::ustring, Glib::ustring> key_map_t;
	/* Alias de un mapa de pares [grupo, claves] indexado por el nombre de grupo */
	typedef std::map<Glib::ustring, key_map_t> group_map_t;

protected:

	/**
	 * Constructor real de la clase
	 */
	CConfig(void);

	/**
	 * Destructor de la clase
	 */
	virtual ~CConfig(void);

private:
	/**
	 * Limpia el almacen interno de claves
	 */
	void clear(void);

public:

	/**
	 * Obtiene la instancia única de la clase que se define como un singleton
	 * @return Instancia única de la clase
	 */
	static CConfig* getInstance(void){
		static CConfig m_instance;
		return &m_instance;
	}

	/**
	 * Carga la configuración desde un fichero XML
	 * @param p_file Path del fichero XML con la configuración
	 * @return true Si se pudo realizar la carga, false en otro caso.
	 * @note Internamente se resetea el almacen de claves, por lo que se pierde
	 * el estado en el que estuviera.
	 */
	bool load(const Glib::ustring& p_file);

	/**
	 * Guarda la configuración en un fichero especificado
	 * @param p_file Path del fichero donde guardar la configuración
	 * @return true si se pudo guardar la configuración, false en otro caso
	 */
	bool save(const Glib::ustring& p_file);

	/**
	 * Guarda la configuración en el mismo fichero del que se cargó previamente
	 * @return true si se pudo guardar la configuración, false en otro caso
	 */
	bool save(void){
		if(m_file.size())
			return save(m_file);
		else
			return false;
	}

	/**
	 * Obtiene una clave de la configuración especificada por su tripleta
	 * @param p_group Grupo al que pertenece la clave
	 * @param p_key Nombre de la clave
	 * @param p_value Tipo y lugar de retorno para la clave obtenida
	 * @return true si se obtuvo la clave, false en otro caso
	 * @pre El grupo y la clave deben ser válidos
	 * @note Aunque virtualmente el método puede leer cualquier tipo de
	 * valor, depende directamente de gelide::utils::toStr y por lo tanto
	 * está orientado a la lectura de tipos básicos y vectores de tipos
	 * básicos (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool getKey(const Glib::ustring& p_group, const Glib::ustring& p_key, T& p_value){
		group_map_t::iterator l_giter;
		key_map_t::iterator l_kiter;
		Glib::ustring l_value_str;

		assert(p_group.size());
		assert(p_key.size());

		// Buscamos el grupo
		l_giter = m_groups.find(p_group);
		if(l_giter != m_groups.end()){
			// Buscamos la clave dentro del grupo
			l_kiter = l_giter->second.find(p_key);
			if(l_kiter != l_giter->second.end()){
				l_value_str = l_kiter->second;
				// Pasamos la cadena al tipo adecuado
				utils::strTo(l_value_str, p_value);
				return true;
			}
			else
				return false;
		}
		return false;
	}

	/**
	 * Establece una clave de configuración especificada por su tripleta
	 * @param p_group Grupo al que pertenece la clave
	 * @param p_key Nombre de la clave
	 * @param p_value Tipo y valor para la clave
	 * @pre El grupo y la clave deben ser válidos
	 * @note Aunque virtualmente el método puede escribir cualquier tipo de
	 * valor, depende directamente de gelide::utils::toStr y por lo tanto está
	 * orientada a la escritura de tipos básicos y vectores de tipos básicos
	 * (char, int, bool, double, string, etc).
	 */
	template<class T>
	void setKey(const Glib::ustring& p_group, const Glib::ustring& p_key, const T& p_value){
		group_map_t::iterator l_giter;
		key_map_t l_key;
		Glib::ustring l_value_str;

		assert(p_group.size());
		assert(p_key.size());

		// Pasamos el valor a string
		l_value_str = utils::toStr(p_value);

		// Buscamos el grupo
		l_giter = m_groups.find(p_group);
		if(l_giter != m_groups.end())
			// Si existe, establecemos la key
			(l_giter->second)[p_key] = l_value_str;
		else{
			// Si no existe creamos el grupo y añadimos una key con el valor
			l_key[p_key] = l_value_str;
			m_groups[p_group] = l_key;
		}
	}

	/**
	 * Comprueba si existe un grupo en el almacen interno, con el nombre dado
	 * @param p_group Nombre del grupo a buscar
	 * @return true si el grupo existe, false en otro caso
	 */
	bool hasGroup(const Glib::ustring& p_group);

	/**
	 * Comprueba si existe un clave en el almacen interno, con el nombre dado
	 * @param p_group Nombre del grupo al que pertenece la clave
	 * @param p_key Nombre de la clave buscada.
	 * @return true si la clave existe, false en otro caso
	 */
	bool hasKey(const Glib::ustring& p_group, const Glib::ustring& p_key);

private:
	group_map_t m_groups;					/**< Almacen de grupos con las claves del sistema */
	Glib::ustring m_file;					/**< Fichero de configuración usado */
	static const Glib::ustring m_version;	/**< Versión del sistema de configuración */
};

#endif // _CONFIG_HPP_
