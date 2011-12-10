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

#ifndef _XML_READER_HPP_
#define _XML_READER_HPP_

#include "../gelide.hpp"
#include "utils.hpp"
#include <glibmm/ustring.h>
#include <libxml2/libxml/tree.h>


// Declaración fordward del XmlReader
class CXmlReader;

/**
 * Encapsula de la gestión y funcionamiento de un nodo en un documento xml.
 * Permite obtener sus atributos y su contenido en diferentes tipos. También
 * soporta la iteración sobre sus nodos hijos mediante el uso de iteradores.
 */
class CXmlNode
{
	// Necesario para el acceso a setXmlNode
	friend class CXmlReader;
public:

	/**
	 * Constructor básico de la clase
	 */
	CXmlNode(void);

	/**
	 * Constructor copia
	 * @param p_node Nodo del xml con el que se inicializará el nuevo nodo
	 */
	CXmlNode(xmlNodePtr p_node);

	/**
	 * Destructor de la clase
	 */
	~CXmlNode(){};

	/**
	 * Obtiene el nombre del nodo actual
	 * @return Nombre del nodo
	 */
	Glib::ustring getName(void) const;

	/**
	 * Obtiene un atributo del nodo en tipo determinado.
	 * @param p_name Nombre del atributo
	 * @param p_value Tipo y lugar de retorno para el atributo obtenido
	 * @return true si se obtuvo el atributo, false en otro caso
	 * @pre El nodo y el nombre del atributo deben ser válidos
	 * @note Aunque virtualmente el método puede leer cualquier tipo de
	 * valor, depende directamente de gelide::utils::toStr y por lo tanto
	 * está orientado a la lectura de tipos básicos y vectores de tipos
	 * básicos (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool getAttribute(const Glib::ustring& p_name, T& p_value) const{
		xmlChar* l_attribute;
		Glib::ustring l_value_str;

		assert(m_node);
		assert(p_name.size());

		// Obtenemos el atributo
		l_attribute =xmlGetProp(m_node, BAD_CAST p_name.c_str());

		// Si no se obtuvo el atributo lo indicamos en la salida
		if(!l_attribute)
			return false;
		// Pasamos el atributo a string si es válido
		l_value_str = reinterpret_cast<const char*>(l_attribute);
		// Pasamos la cadena al tipo adecuado
		utils::strTo(l_value_str, p_value);
		// Liberamos la memoria ocupada por el atributo
		xmlFree(l_attribute);
		return true;
	}

	/**
	 * Obtiene contenido del nodo en tipo determinado.
	 * @param p_value Tipo y lugar de retorno para el contenido obtenido
	 * @return true si se obtuvo el contenido, false en otro caso
	 * @pre El nodo debe ser válido
	 * @note Aunque virtualmente el método puede leer cualquier tipo de
	 * valor, depende directamente de gelide::utils::toStr y por lo tanto
	 * está orientado a la lectura de tipos básicos y vectores de tipos
	 * básicos (char, int, bool, double, string, etc).
	 */
	template<class T>
	bool getContent(T& p_value) const{
		xmlChar* l_content;
		Glib::ustring l_value_str;

		assert(m_node);
		// Obtenemos el atributo
		l_content = xmlNodeListGetString(m_node->doc, m_node->xmlChildrenNode, 1);

		// Si no se obtuvo el contenido lo indicamos en la salida
		if(!l_content)
			return true;

		// Pasamos el contenido a string si es válido
		l_value_str = reinterpret_cast<const char*>(l_content);
		// Pasamos la cadena al tipo adecuado
		utils::strTo(l_value_str, p_value);
		// Liberamos la memoria ocupada por el atributo
		xmlFree(l_content);
		return true;
	}

	// Declaración fordware de iterator
	class iterator;

	/**
	 * Obtiene un iterador al primer elemento hijo del nodo
	 * @return iterador al primer hijo o a end si no contine ninguno
	 * @pre El nodo debe ser válido
	 * @note end() apunta al propio elemento
	 */
	iterator begin();

	/**
	 * Obtiene un iterador considerado como final de los nodos hijo
	 * @return iterador apuntando al final de los nodos hijo
	 * @note end() apunta al propio elemento
	 */
	iterator end();

protected:
	/**
	 * Establece el valor del puntero interno al nodo del elemento
	 * @param p_node Nuevo puntero al nodo
	 */
	void setXmlNode(const xmlNodePtr p_node){
		m_node = p_node;
	}

	/**
	 * Obtiene el valor del puntero interno al nodo del elemento
	 * @return Puntero interno al nodo del elemento
	 */
	xmlNodePtr getXmlNode(void) const{
		return m_node;
	}

private:
	xmlNodePtr m_node;		/**< Nodo xml interno */
};


/**
 * Iterador sobre nodos de un documento xml.
 */
class CXmlNode::iterator: public std::iterator<std::forward_iterator_tag, CXmlNode*>
{
public:

	/**
	 * Constructor básico
	 */
	iterator();

	/**
	 * Constructor a partir de un nodo xml
	 * @param p_node Nodo xml real
	 */
	iterator(xmlNodePtr p_node);

	/**
	 * Destructor de la clase
	 */
	~iterator(){};

	/**
	 * Operador copia
	 * @param p_iter Iterador del que se realizará la copia
	 * @return Un nuevo iterador copia.
	 */
	iterator& operator=(const iterator& p_iter);

	/**
	 * Comprueba si dos iteradores son iguales
	 * @param p_iter Iterador con el que se compara
	 * @return true si los iteradores son iguales
	 */
	bool operator==(const iterator& p_iter) const;

	/**
	 * Comprueba si dos iteradores son distintos
	 * @param p_iter Iterador con el que se compara
	 * @return true si los iteradores son distintos
	 */
	bool operator!=(const iterator& p_iter) const;

	/**
	 * Operación de pre-incremento o avance
	 * @return Iterador incrementado
	 */
	iterator& operator++(void);

	/**
	 * Operación de post-incremento
	 * @return Iterador actual, y posteriormente se incrementa
	 */
	iterator operator++(int);

	/**
	 * Operador *
	 * @return Referencia al objeto al que apunta el iterador
	 */
	CXmlNode& operator*(void);

	/**
	 * Operador de acceso
	 * @return Obtiene un puntero al objeto al que apunta el iterador
	 */
	CXmlNode* operator->(void);

private:
	CXmlNode m_xml_node;		/**< Elemento interno del iterador */
};


/**
 * Implementación de un parser XML basado en DOM para la lectura de ficheros xml
 * de forma sencilla.
 *
 * A partir de un elemento principal, permite iterar sobre sus hijos, obteniendo
 * atributos y contenidos en prácticamente cualquier tipo de datos.
 */
class CXmlReader
{
public:
	/**
	 * Constructor básico
	 */
	CXmlReader();

	/**
	 * Constructor con apertura directa de fichero para lectura del xml
	 * @param p_file Nombre del fichero a abrir
	 *
	 */
	CXmlReader(const Glib::ustring& p_file);

	/**
	 * Destructor de la clase
	 */
	~CXmlReader();

	/**
	 * Carga un archivo especificado creando el árbol xml
	 * @param p_file Fichero xml a cargar
	 * @pre El nombre del fichero debe ser válidos
	 * @return true si se pudo realizar la carga sin problemas
	 */
	bool open(const Glib::ustring& p_file);

	/**
	 * Crea un árbol xml a partir de un buffer de memoria
	 * @param p_buffer Puntero al buffer donde se almacenan los datos
	 * @param p_size Tamaño total del buffer
	 * @return true si se pudo realizar la carga sin problemas
	 */
	bool load(const char *p_buffer, int p_size);

	/**
	 * Resetea el reader liberando la memoria utilizada
	 */
	void close(void);

	/**
	 * Obtiene el elemento o nodo maestro del documento xml.
	 * @return Un elemento conteniendo el nodo maestro del xml
	 * @note Una vez obtenido el nodo maestro, podemos movernos por el xml
	 * haciendo uso de iteradores sobre elementos.
	 */
	CXmlNode getRootElement() const{
		return m_root;
	}

private:
	xmlDocPtr m_doc;		/**< Documento xml de uso interno */
	CXmlNode m_root;		/**< Elemento root del xml */
};


#endif // _XML_READER_HPP_
