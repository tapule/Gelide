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
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "tokenizer.hpp"

#define TK_DEFAULT_DELIMITERS	" \n\r\t"
#define TK_DEFAULT_DELIMITERS_SIZE	4

CTokenizer::CToken::CToken(const int p_type, const unsigned int p_value, char* p_string){
	m_type = p_type;
	m_value = p_value;
	m_string = p_string;
}

int CTokenizer::CToken::getType(void){
	return m_type;
}

unsigned int CTokenizer::CToken::getValue(void){
	return m_value;
}

char* CTokenizer::CToken::getString(void){
	return m_string;
}

void CTokenizer::CToken::setType(const int p_type){
	m_type = p_type;
}

void CTokenizer::CToken::setValue(const unsigned int p_value){
	m_value = p_value;
}

void CTokenizer::CToken::setString(char* p_string){
	static char l_zero[] = "";

	if(!p_string)
		m_string = l_zero;
	else
		m_string = p_string;
}

/*---------------------------------------------------------------------------*/

CTokenizer::CTokenizer(void){
	// Por defecto detección de cadenas y números activada
	m_detect_strings = true;
	m_detect_numbers = true;
	m_words = NULL;
	m_words_count = 0;
	m_delimiters = NULL;
	m_delimiters_count = 0;
	m_buff = NULL;
	m_buff_pos = NULL;
	m_init = false;
	// Establecemos los delimitadores por defecto
	setDelimiters((char *)TK_DEFAULT_DELIMITERS, TK_DEFAULT_DELIMITERS_SIZE);
}

CTokenizer::~CTokenizer(){
	if(m_buff)
		delete[] m_buff;
	if(m_delimiters)
		delete[] m_delimiters;
	if(m_words)
		delete[] m_words;
}

bool CTokenizer::initFromFile(const Glib::ustring& p_file){
	std::ifstream m_file;
	int l_fsize;
	int l_readed;

	// Nos aseguramos de liberar la memoria y hacer nulos los punteros
	if(m_buff)
		delete[] m_buff;
	m_buff = NULL;
	m_buff_pos = NULL;
	m_init = false;

	m_file.open(p_file.data());
	// Comprobamos si la apertura fue correcta
	if(!m_file.good()){
		m_file.close();
		return false;
	}
	// Obtenemos el tamaño del fichero
	m_file.seekg (0, std::ios::end);
	l_fsize = m_file.tellg();
	m_file.seekg (0, std::ios::beg);
	// Reservamos la memoria necesaria
	m_buff = new char[l_fsize + 1];
	if(!m_buff){
		m_file.close();
		return false;
	}
	// Cargamos el contenido completo del fichero en memoria
	m_file.read (m_buff, l_fsize);
	l_readed = m_file.gcount();
	m_file.close();
	// Reiniciamos el puntero de lectura
	m_buff_pos = m_buff;
	m_buff[l_readed] = '\0';
	m_init = true;
	findNextToken();
	return true;
}

bool CTokenizer::initFromString(const Glib::ustring& p_str){
	std::ifstream m_file;
	int l_ssize;
	int l_readed;

	// Nos aseguramos de liberar la memoria y hacer nulos los punteros
	if(m_buff)
		delete[] m_buff;
	m_buff = NULL;
	m_buff_pos = NULL;
	m_init = false;

	// Obtenemos el tamaño de la cadena
	l_ssize = p_str.size();
	// Reservamos la memoria necesaria
	m_buff = new char[l_ssize + 1];
	if(!m_buff){
		m_file.close();
		return false;
	}
	// Cargamos el contenido completo de la cadena en el buffer
	l_readed = p_str.copy(m_buff, l_ssize, 0);
	// Reiniciamos el puntero de lectura
	m_buff_pos = m_buff;
	m_buff[l_readed] = '\0';
	m_init = true;
	findNextToken();
	return true;
}

bool CTokenizer::initFromMemory(const char *p_buffer, int p_size){
	int l_ssize;

	// Nos aseguramos de liberar la memoria y hacer nulos los punteros
	if(m_buff)
		delete[] m_buff;
	m_buff = NULL;
	m_buff_pos = NULL;
	m_init = false;

	// Reservamos la memoria necesaria
	m_buff = new char[p_size + 1];
	if(!m_buff)
		return false;

	// Copiamos el contenido del buffer
	memcpy(m_buff, p_buffer, p_size);

	// Reiniciamos el puntero de lectura
	m_buff_pos = m_buff;
	m_buff[p_size] = '\0';
	m_init = true;
	findNextToken();
	return true;
}

void CTokenizer::setDelimiters(char* p_delimiters, unsigned int p_count){
	// Si no se especifican delimitadores, asignamos los internos
	if(!p_delimiters){
		p_delimiters = (char *) TK_DEFAULT_DELIMITERS;
		p_count = (unsigned int) TK_DEFAULT_DELIMITERS_SIZE;
	}
	// Limpiamos la memoria de los anteriores delimitadores e iniciamos contador
	delete[] m_delimiters;
	m_delimiters = NULL;
	m_delimiters_count = 0;

	// Solicitamos memoria para copiar los delimitadores
	m_delimiters = new char[p_count];
	if(m_delimiters){
		strncpy(m_delimiters, p_delimiters, p_count);
		m_delimiters_count = p_count;
	}
}

void CTokenizer::setReservedWords(TReservedWord p_words[], unsigned int p_count){
	unsigned int l_ind;

	// Limpiamos la memoria de las anteriores palabras e iniciamos el contador
	if(m_words)
		delete[] m_words;
	m_words = NULL;
	m_words_count = 0;

	// Solicitamos memoria para las nuevas palabras reservadas
	m_words = new TWord[p_count];
	if(m_words){
		m_words_count = p_count;
		// Copiamos las palabras reservadas pasadas
		for(l_ind = 0; l_ind < p_count; l_ind++){
			m_words[l_ind].m_type = p_words[l_ind].m_type;
			m_words[l_ind].m_size = strlen(p_words[l_ind].m_name);
			strcpy(m_words[l_ind].m_name, p_words[l_ind].m_name);
		}
	}
	std::sort(m_words, m_words + p_count, CTokenizer::compareWords);
}

void CTokenizer::setStringsDetection(const bool p_detect){
	m_detect_strings = p_detect;
}

void CTokenizer::setNumbersDetection(const bool p_detect){
	m_detect_numbers = p_detect;
}

void CTokenizer::nextToken(CToken& p_token){
	if(!m_init)
		return;
	// Valor de control para token
	p_token.m_type = TK_NOTK;
	p_token.m_value = -1;
	p_token.m_string = NULL;
	while(p_token.m_type == TK_NOTK){
		switch(*m_buff_pos){
		// Fin de fichero
		case '\0':
			p_token.m_type = TK_EOF;
			p_token.m_value = 0;
			// Hacemos que apunte a "/0", para evitar segfaults
			p_token.m_string = m_buff_pos;
			break;
		// Comienzo de una cadena de caracteres
		case '"':
			if(m_detect_strings){
				// Obtenemos la cadena
				p_token.m_type = TK_STR;
				p_token.m_value = 0;
				p_token.m_string = this->getString();
				break;
			}
		default:
			// Llegados a este punto, como minimo será un identificador
			p_token.m_type = TK_ID;
			p_token.m_string = this->getIdentifier();

			// Comprobamos si se trata de un número
			if(m_detect_numbers){
				if(checkNum(p_token))
					break;
			}

			checkReservedWords(p_token);
		}
		// Posicionamos al comienzo del siguiente token
		findNextToken();
	}
}

bool CTokenizer::hasMoreTokens(void){
	return *m_buff_pos != '\0';
}

void CTokenizer::reset(){
	// Nos aseguramos de liberar la memoria y hacer nulos los punteros
	if(m_buff)
		delete[] m_buff;
	m_buff = NULL;
	m_buff_pos = NULL;

	m_init = false;
}

bool CTokenizer::isDelimiter(void){
	unsigned int l_ind = 0;

	// Buscamos nuestra cadena hasta agotar todos los separadores
	while((l_ind < m_delimiters_count) && (*m_buff_pos != m_delimiters[l_ind]))
		l_ind++;
	// Comprobamos si se encontró alguno
	if(l_ind < m_delimiters_count)
		return true;
	return false;
}

bool CTokenizer::checkReservedWords(CToken& p_token){
	int l_pos;

	l_pos = binarySearchWord(p_token.m_string);
	if(l_pos != -1){
		p_token.m_type = m_words[l_pos].m_type;
		p_token.m_string = m_words[l_pos].m_name;
		return true;
	}
	return false;
}

bool CTokenizer::checkNum(CToken& p_token){
	char* l_string = NULL;
	char* l_ind = NULL;

	l_string = p_token.m_string;
	l_ind = l_string;

	// Avanzamos mientras tengamos números
	while(isdigit((int) *l_ind))
		l_ind++;
	// Si no se avanza, no tenemos ningún número
	if(l_ind == l_string)
		return false;
	if(*l_ind == '\0'){
		p_token.m_type = TK_NUM;
		p_token.m_value = atoi(l_string);
		return true;
	}
	return false;
}

bool CTokenizer::findNextToken(void){
	while(isDelimiter())
		m_buff_pos++;
	if(*m_buff_pos == '\0')
		return false;
	else
		return true;
}

char* CTokenizer::getString(void){
	char* l_string;

	// Saltamos las comillas iniciales y colocamos el inicio de cadena
	m_buff_pos++;
	l_string = m_buff_pos;

	// Ignoramos caracteres escapados iniciales
	while(*m_buff_pos == '\\')
		m_buff_pos += 2;

	// Consideramos que la cadena finaliza en " o final de línea
	while( (*m_buff_pos != '"') && (*m_buff_pos != '\0') ){
		m_buff_pos++;
		// Ignoramos caracteres escapados
		while(*m_buff_pos == '\\')
			m_buff_pos += 2;

	}

	// Si no llegamos a eof, marcamos el fin de la cadena y avanzamos
	// para evitar un eof erroneo en la siguiente lectura
	if(*m_buff_pos == '"'){
		*m_buff_pos = '\0';
		m_buff_pos++;
	}

	return l_string;
}

char* CTokenizer::getIdentifier(void){
	char* l_string;
	// Asignamos el puntero de la cadena en el comienzo del identificador
	l_string = m_buff_pos;
	while((!isDelimiter()) && (*m_buff_pos != '\0'))
		m_buff_pos++;

	// Si no llegamos a eof, marcamos el fin de la cadena y avanzamos
	// para evitar un eof erroneo en la siguiente lectura
	if(*m_buff_pos != '\0'){
		*m_buff_pos = '\0';
		m_buff_pos++;
	}
	return l_string;
}

bool CTokenizer::compareWords(const TWord& p_word0, const TWord& p_word1){
	return (strcmp(p_word0.m_name, p_word1.m_name) < 0);
}

int CTokenizer::binarySearchWord(char* p_key)
{
	int l_top = m_words_count-1;
	int l_bottom = 0;
	int l_center;
	int l_str_cmp;

	while (l_bottom <= l_top){
		l_center = (l_top + l_bottom)/2;
		l_str_cmp = strcmp(m_words[l_center].m_name, p_key);
		if(l_str_cmp == 0)
			return l_center;
		else{
			if(l_str_cmp > 0)
				l_top = l_center - 1;
			else
				l_bottom = l_center + 1;
			}
	}
	return -1;
}
