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

#include "tokenizer.hpp"
#include <fstream>
/*#include <stdlib.h>
#include <string.h>
#include <algorithm>
*/


Tokenizer::Tokenizer(void):
	m_delimiters(DEFAULT_DELIMITERS),
	m_last_delimiter('\0'),
	m_detect_strings(true)
{
	m_buff_pos = m_buff.begin();
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::initFromFile(const std::string& file)
{
	std::ifstream file_stream;
	unsigned int size;

	file_stream.open(file.data());
	if (!file_stream.good())
	{
		file_stream.close();
		return false;
	}
	// Obtenemos el tamaño del fichero para redimensionar la cadena
	file_stream.seekg (0, std::ios::end);
	size = file_stream.tellg();
	file_stream.seekg (0, std::ios::beg);
	m_buff.resize(size);
	// Cargamos el contenido completo del fichero en la cadena
	file_stream.read(&m_buff[0], size);
	file_stream.close();
	// Reiniciamos el puntero de lectura y los datos internos
	m_buff_pos = m_buff.begin();
	m_token.clear();
	m_last_delimiter = '\0';

	return true;
}

bool Tokenizer::initFromString(const std::string& str)
{
	m_buff = str;
	m_buff_pos = m_buff.begin();
	m_token.clear();
	m_last_delimiter = '\0';

	return true;
}

bool Tokenizer::initFromMemory(const char *buffer, const unsigned int size)
{
	m_buff.resize(size);
	m_buff.assign(buffer, size);
	m_buff_pos = m_buff.begin();
	m_token.clear();
	m_last_delimiter = '\0';

	return true;
}

void Tokenizer::setDelimiters(const std::string& delimiters)
{
	m_delimiters = delimiters;
}

void Tokenizer::setStringDetection(const bool detect)
{
	m_detect_strings = detect;
}

std::string Tokenizer::nextToken(void)
{
	m_token.clear();

	if (hasMoreTokens())
	{
		// Comprobamos si estamos en una cadena y hay que extraerla
		if ((*m_buff_pos == '"') && m_detect_strings)
		{
			m_token = getString();
			return m_token;
		}
		while ((m_buff_pos != m_buff.end()) && !isDelimiter())
		{
			m_token += *m_buff_pos;
			++m_buff_pos;
		}
	}
	return m_token;
}

bool Tokenizer::hasMoreTokens(void)
{
	// Saltamos delimitadores iniciales y guardamos el último delimitador
	while ((m_buff_pos != m_buff.end()) && isDelimiter())
	{
		m_last_delimiter = *m_buff_pos;
		++m_buff_pos;
	}
	if (m_buff_pos != m_buff.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

char Tokenizer::lastDelimiter(void)
{
	return m_last_delimiter;
}

void Tokenizer::reset(void)
{
	m_buff_pos = m_buff.begin();
	m_token.clear();
	m_last_delimiter = '\0';
}

std::vector<std::string> Tokenizer::split(void)
{
	std::vector<std::string> tokens;

	while (hasMoreTokens())
	{
		tokens.push_back(nextToken());
	}

	return tokens;
}

bool Tokenizer::isDelimiter(void)
{
	return(m_delimiters.find(*m_buff_pos) != std::string::npos);
}

std::string Tokenizer::getString(void)
{

	std::string str;

	// Saltamos las comillas iniciales donde hemos sido llamados
	++m_buff_pos;

	// Comprobamos caracteres escapados iniciales
	while (*m_buff_pos == '\\')
	{
		str += *m_buff_pos;
		str += *(m_buff_pos + 1);
		m_buff_pos += 2;
	}

	// Consideramos que la cadena finaliza en " o al final del buffer
	while ((*m_buff_pos != '"') && (m_buff_pos != m_buff.end()) )
	{
		str += *m_buff_pos;
		++m_buff_pos;
		// Caracteres escapados internos
		while (*m_buff_pos == '\\')
		{
			str += *m_buff_pos;
			str += *(m_buff_pos + 1);
			m_buff_pos += 2;
		}
	}

	// Si paramos en unas comillas, pasamos a la siguiente posición para evitar
	// que se vuelva a interpretar la posición como cadena
	if (*m_buff_pos == '"')
	{
		++m_buff_pos;
	}
	return str;
}
