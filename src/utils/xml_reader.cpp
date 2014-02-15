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

#include "xml_reader.hpp"


XmlNode::XmlNode(void):
	m_node(NULL)
{
}

XmlNode::XmlNode(xmlNodePtr node) :
	m_node(node)
{
}

Glib::ustring XmlNode::getName(void) const
{
	if (m_node->name != NULL)
	{
		return(reinterpret_cast<const char*>(m_node->name));
	}
	else
	{
		return "";
	}
}

XmlNode::iterator XmlNode::begin()
{
	xmlNodePtr node;

	assert(m_node);

	node = m_node->children;
	// Saltamos los nodos no válidos
	while ((node != NULL) && (node->type != XML_ELEMENT_NODE))
	{
		node = node->next;
	}
	if (node)
	{
		return iterator(node);
	}
	else
	{
		return iterator(m_node);
	}
}

XmlNode::iterator XmlNode::end()
{
	assert(m_node);
	// Consideramos al nodo padre como "uno pasado el último", por lo que
	// devolvemos como end el propio nodo
	return iterator(m_node);
}


XmlNode::iterator::iterator()
{
}

XmlNode::iterator::iterator(xmlNodePtr node)
{
	m_xml_node.setXmlNode(node);
}


XmlNode::iterator& XmlNode::iterator::operator=(const iterator& iter)
{
	m_xml_node = iter.m_xml_node;
	return(*this);
}

bool XmlNode::iterator::operator==(const iterator& iter) const
{
	return(m_xml_node.getXmlNode() == iter.m_xml_node.getXmlNode());
}

bool XmlNode::iterator::operator!=(const iterator& iter) const
{
	return(m_xml_node.getXmlNode() != iter.m_xml_node.getXmlNode());
}

XmlNode::iterator& XmlNode::iterator::operator++(void)
{
	xmlNodePtr node;

	// Obtenemos el siguiente nodo
	node = m_xml_node.getXmlNode()->next;

	// Saltamos los nodos no válidos
	while ((node != NULL) && (node->type != XML_ELEMENT_NODE) )
	{
		node = node->next;
	}

	// Comprobamos si no estamos en el último nodo hijo
	if (node)
	{
		m_xml_node.setXmlNode(node);
	}
	else
	{
		// Consideramos al nodo padre como "uno pasado el último" y lo asignamos
		// desde el nodo original, ya que l_node sera nulo
		m_xml_node.setXmlNode(m_xml_node.getXmlNode()->parent);
	}

	return(*this);
}

XmlNode::iterator XmlNode::iterator::operator++(int)
{
	iterator iter(this->m_xml_node.getXmlNode());

	++(*this);
	return(iter);
}

XmlNode& XmlNode::iterator::operator*(void)
{
	return m_xml_node;
}

XmlNode* XmlNode::iterator::operator->(void)
{
	return &m_xml_node;
}


XmlReader::XmlReader():
	m_doc(NULL)
{
}

XmlReader::XmlReader(const Glib::ustring& file):
	m_doc(NULL)
{
	open(file);
}

XmlReader::~XmlReader()
{
	close();
}

bool XmlReader::open(const Glib::ustring& file)
{
	assert(file.size());

	// Liberamos la memoria si fuera necesario
	close();
	m_doc = xmlParseFile(file.c_str());
	if (m_doc == NULL)
	{
		// Anulamos el nodo root
		m_root.setXmlNode(NULL);
		return false;
	}
	// Establecemos el nodo root
	m_root.setXmlNode(xmlDocGetRootElement(m_doc));
	return true;
}

bool XmlReader::load(const char* buffer, const unsigned int size)
{
	// Liberamos la memoria si fuera necesario
	close();
	m_doc = xmlParseMemory(buffer, size);
	if (m_doc == NULL)
	{
		// Anulamos el nodo root
		m_root.setXmlNode(NULL);
		return false;
	}
	// Establecemos el nodo root
	m_root.setXmlNode(xmlDocGetRootElement(m_doc));
	return true;
}

bool XmlReader::load(const Glib::ustring& str)
{
	return(load(str.c_str(), str.bytes()));
}

void XmlReader::close(void)
{
	// Anulamos el nodo root
	m_root.setXmlNode(NULL);
	// Liberamos la memoria ocupada por el reader
	xmlFreeDoc(m_doc);
	m_doc = NULL;
}
