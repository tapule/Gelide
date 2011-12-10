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

#include "xml_reader.hpp"


CXmlNode::CXmlNode(void):
	m_node(NULL)
{
}

CXmlNode::CXmlNode(xmlNodePtr p_node) :
	m_node(p_node)
{
}

Glib::ustring CXmlNode::getName(void) const{
	if (m_node->name != NULL)
		return( reinterpret_cast<const char*>(m_node->name));
	else
		return "";
}


CXmlNode::iterator CXmlNode::begin(){
	xmlNodePtr l_node;

	assert(m_node);

	l_node = m_node->children;
	// Saltamos los nodos no válidos
	while((l_node != NULL) && (l_node->type != XML_ELEMENT_NODE))
		l_node = l_node->next;
	if(l_node)
		return iterator(l_node);
	else
		return iterator(m_node);
}

CXmlNode::iterator CXmlNode::end(){

	assert(m_node);
	// Consideramos al nodo padre como "uno pasado el último", por lo que
	// devolvemos como end el propio nodo
	return iterator(m_node);
}


CXmlNode::iterator::iterator(){
}

CXmlNode::iterator::iterator(xmlNodePtr p_node){
	m_xml_node.setXmlNode(p_node);
}


CXmlNode::iterator& CXmlNode::iterator::operator=(const iterator& p_iter){
	m_xml_node = p_iter.m_xml_node;
	return(*this);
}

bool CXmlNode::iterator::operator==(const iterator& p_iter) const{
	return(m_xml_node.getXmlNode() == p_iter.m_xml_node.getXmlNode());
}

bool CXmlNode::iterator::operator!=(const iterator& p_iter) const{
	return(m_xml_node.getXmlNode() != p_iter.m_xml_node.getXmlNode());
}

CXmlNode::iterator& CXmlNode::iterator::operator++(void){
	xmlNodePtr l_node;

	// Obtenemos el siguiente nodo
	l_node = m_xml_node.getXmlNode()->next;

	// Saltamos los nodos no válidos
	while((l_node != NULL) && (l_node->type != XML_ELEMENT_NODE) )
		l_node = l_node->next;

	// Comprobamos si no estamos en el último nodo hijo
	if(l_node)
		m_xml_node.setXmlNode(l_node);
	else
		// Consideramos al nodo padre como "uno pasado el último" y lo asignamos
		// desde el nodo original, ya que l_node sera nulo
		m_xml_node.setXmlNode(m_xml_node.getXmlNode()->parent);

	return(*this);
}

CXmlNode::iterator CXmlNode::iterator::operator++(int){
	iterator l_iter(this->m_xml_node.getXmlNode());

	++(*this);
	return(l_iter);
}

CXmlNode& CXmlNode::iterator::operator*(void){
	return m_xml_node;
}

CXmlNode* CXmlNode::iterator::operator->(void){
	return &m_xml_node;
}


CXmlReader::CXmlReader():
	m_doc(NULL)
{
}

CXmlReader::CXmlReader(const Glib::ustring& p_file):
	m_doc(NULL)
{
	open(p_file);
}

CXmlReader::~CXmlReader(){
	close();
}

bool CXmlReader::open(const Glib::ustring& p_file){

	assert(p_file.size());

	// Liberamos la memoria si fuera necesario
	close();
	// Creamos el parser xml
	m_doc = xmlParseFile(p_file.c_str());

	if (m_doc == NULL){
		// Anulamos el nodo root
		m_root.setXmlNode(NULL);
		return false;
	}
	// Establecemos el nodo root
	m_root.setXmlNode(xmlDocGetRootElement(m_doc));
	return true;
}

bool CXmlReader::load(const char *p_buffer, int p_size){

	// Liberamos la memoria si fuera necesario
	close();
	// Creamos el parser xml
	m_doc = xmlParseMemory(p_buffer, p_size);

	if (m_doc == NULL){
		// Anulamos el nodo root
		m_root.setXmlNode(NULL);
		return false;
	}
	// Establecemos el nodo root
	m_root.setXmlNode(xmlDocGetRootElement(m_doc));
	return true;
}

void CXmlReader::close(void){
	// Anulamos el nodo root
	m_root.setXmlNode(NULL);
	// Liberamos la memoria ocupada por el reader
	xmlFreeDoc(m_doc);
	m_doc = NULL;
}


