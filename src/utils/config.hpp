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

#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif /* HAVE_CONFIG_H */

// Si no está definido el modo debug, desactivamos los asserts
#ifndef ENABLE_DEBUG_MODE
	#define NDEBUG
#endif

#include <cassert>
#include <map>
#include <glibmm.h>
#include "utils.hpp"
#include "log.hpp"


// Nombre por defecto para el nodo root
#define CFG_DEFAULT_ROOT "cfgroot"

/**
 * Sistema de configuración genérico.
 *
 * Se encarga de gestionar un almacen de claves de configuración en formato xml
 * formadas por tripletas "Grupo, clave, valor".
 *
 * El fichero de configuración tendrá el siguiente formato:
 * <[nombre_root|'cfgroot'] [version="ver_str"]>
 *		<group name="mygrup_1">
 *			<key name="mykey_1" value="myvalue">
 *
 *			<key name="mykey_n" value="myvalue">
 *		</group>
 *
 *		<group name="mygrup_n">
 *			<key name="mykey_1" value="myvalue">
 *
 *			<key name="mykey_n" value="myvalue">
 *		</group>
 * </[m_root|'cfgroot']>
 *
 */
class Config
{
public:
	/* Alias de un mapa de pares [clave, valor] indexado por la clave */
	typedef std::map<Glib::ustring, Glib::ustring> KeyMap;
	/* Alias de un mapa de pares [grupo, claves] indexado por el nombre de grupo */
	typedef std::map<Glib::ustring, KeyMap> GroupMap;

	/**
	 * Constructor de la clase
	 */
	Config(void);

	/**
	 * Destructor de la clase
	 */
	virtual ~Config(void);

	/**
	 * Establece el nombre del nodo root de la configuración así como la versión
	 * de esta
	 * @param root Nuevo nombre para el nodo root
	 * @param version Nueva versión para la configuración
	 */
	void setRoot(const Glib::ustring& root, const Glib::ustring& version)
	{
		m_root = root;
		if (m_root.empty())
		{
			m_root = CFG_DEFAULT_ROOT;
		}
		LOG_INFO("Config: Setting root node \"" << root << "\"");
		m_version = version;
		LOG_INFO("Config: Setting root version \"" << version << "\"");
	}

	/**
	 * Carga la configuración desde un fichero XML
	 * @param file Path del fichero XML con la configuración
	 * @return true Si se pudo realizar la carga, false en otro caso.
	 * @note Internamente se resetea el almacen de claves, por lo que se pierde
	 * el estado en el que estuviera.
	 */
	bool load(const Glib::ustring& file);

	/**
	 * Guarda la configuración en un fichero especificado
	 * @param file Path del fichero donde guardar la configuración
	 * @return true si se pudo guardar la configuración, false en otro caso
	 */
	bool save(const Glib::ustring& file);

	/**
	 * Guarda la configuración en el mismo fichero del que se cargó previamente
	 * @return true si se pudo guardar la configuración, false en otro caso
	 */
	bool save(void)
	{
		if (!m_file.empty())
		{
			return save(m_file);
		}
		else
		{
			return false;
		}
	}

	/**
	 * Obtiene una clave de la configuración especificada por su tripleta
	 * @param group Grupo al que pertenece la clave
	 * @param key Nombre de la clave
	 * @param value Tipo y lugar de retorno para la clave obtenida
	 * @return true si se obtuvo la clave, false en otro caso
	 * @pre El grupo y la clave deben ser válidos
	 * @note Aunque virtualmente el método puede leer cualquier tipo de
	 * valor, depende directamente de utils::toStr y por lo tanto
	 * está orientado a la lectura de tipos básicos y vectores de tipos
	 * básicos (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool getKey(const Glib::ustring& group, const Glib::ustring& key, T& value)
	{
		GroupMap::iterator group_iter;
		KeyMap::iterator key_iter;
		Glib::ustring value_str;

		assert(!group.empty());
		assert(!key.empty());

		// Buscamos el grupo
		group_iter = m_groups.find(group);
		if (group_iter != m_groups.end())
		{
			// Buscamos la clave dentro del grupo
			key_iter = group_iter->second.find(key);
			if (key_iter != group_iter->second.end())
			{
				value_str = key_iter->second;
				// Pasamos la cadena al tipo adecuado
				utils::strTo(value_str, value);
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	}

	/**
	 * Establece una clave de configuración especificada por su tripleta
	 * @param group Grupo al que pertenece la clave
	 * @param key Nombre de la clave
	 * @param value Tipo y valor para la clave
	 * @pre El grupo y la clave deben ser válidos
	 * @note Aunque virtualmente el método puede escribir cualquier tipo de
	 * valor, depende directamente de utils::toStr y por lo tanto está
	 * orientada a la escritura de tipos básicos y vectores de tipos básicos
	 * (char, int, bool, double, string, etc).
	 */
	template<class T>
	void setKey(const Glib::ustring& group, const Glib::ustring& key, const T& value)
	{
		GroupMap::iterator group_iter;
		KeyMap key_map;
		Glib::ustring value_str;

		assert(!group.empty());
		assert(!key.empty());

		value_str = utils::toStr(value);

		group_iter = m_groups.find(group);
		if (group_iter != m_groups.end())
		{
			// Si existe, establecemos la key
			(group_iter->second)[key] = value_str;
		}
		else
		{
			// Si no existe creamos el grupo y añadimos una key con el valor
			key_map[key] = value_str;
			m_groups[group] = key_map;
		}
	}

	/**
	 * Comprueba si existe un grupo en el almacen interno, con el nombre dado
	 * @param group Nombre del grupo a buscar
	 * @return true si el grupo existe, false en otro caso
	 */
	bool hasGroup(const Glib::ustring& group);

	/**
	 * Comprueba si existe un clave en el almacen interno, con el nombre dado
	 * @param group Nombre del grupo al que pertenece la clave
	 * @param key Nombre de la clave buscada.
	 * @return true si la clave existe, false en otro caso
	 */
	bool hasKey(const Glib::ustring& group, const Glib::ustring& key);

private:
	/**
	 * Limpia el almacen interno de claves
	 */
	void clear(void);

private:
	GroupMap m_groups;			/**< Almacen de grupos con las claves del sistema */
	Glib::ustring m_file;		/**< Fichero de configuración usado */
	Glib::ustring m_root;		/**< Nombre del nodo root de la configuración */
	Glib::ustring m_version;	/**< Versión del sistema de configuración */
};

#endif // _CONFIG_HPP_
