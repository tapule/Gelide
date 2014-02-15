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

#include "parser.hpp"
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>


Parser::Parser(void):
	m_detect_numbers(true)
{
	// Por defecto detección de cadenas y números activada
	m_tokenizer.setStringDetection(true);
}

Parser::~Parser()
{
	m_words.clear();
}

bool Parser::initFromFile(const std::string& file)
{
	return m_tokenizer.initFromFile(file);
}

bool Parser::initFromString(const std::string& str)
{
	return m_tokenizer.initFromString(str);
}

bool Parser::initFromMemory(const char *buffer, const unsigned int size)
{
	return m_tokenizer.initFromMemory(buffer, size);
}

void Parser::setDelimiters(const std::string& delimiters)
{
	m_tokenizer.setDelimiters(delimiters);
}

void Parser::setReservedWords(const std::vector<ReservedWord>& words)
{
	m_words = words;
	std::sort(m_words.begin(), m_words.end(), Parser::compareWords);
}

void Parser::setStringDetection(const bool detect)
{
	m_tokenizer.setStringDetection(detect);
}

void Parser::setNumberDetection(const bool detect)
{
	m_detect_numbers = detect;
}

Token Parser::nextToken(void)
{
	Token token;

	// Inicialmente marcamos como token no válido
	token.type = TK_NOTK;
	token.value = 0;

	if (m_tokenizer.hasMoreTokens())
	{
		// De momento marcamos como cadena y obtenemos esta del tokenizador
		token.type = TK_STR;
		token.string = m_tokenizer.nextToken();
		// Comprobamos si se trata de un número
		if (m_detect_numbers)
		{
			checkNum(token);
		}
		checkReservedWords(token);
	}
	else
	{
		// Si no quedan tokens, devolvemos el token fin de fichero
		token.type = TK_EOF;
	}

	return token;
}

bool Parser::hasMoreTokens(void)
{
	return m_tokenizer.hasMoreTokens();
}

char Parser::lastDelimiter(void)
{
	return m_tokenizer.lastDelimiter();
}

void Parser::reset()
{
	m_tokenizer.reset();
}

bool Parser::checkReservedWords(Token& token)
{
	int pos;

	pos = binarySearchWord(token.string);
	if (pos != -1)
	{
		token.type = m_words[pos].type;
		token.string = m_words[pos].name;
		return true;
	}
	return false;
}

bool Parser::checkNum(Token& token)
{
	std::string::const_iterator iter;

	if (token.string.empty())
	{
		return false;
	}

	iter = token.string.begin();
	while (iter != token.string.end() && std::isdigit(*iter))
	{
		++iter;
	}

	if (iter == token.string.end())
	{
		token.type = TK_NUM;
		token.value = atoi(token.string.c_str());
		return true;
	}

	return false;
}

bool Parser::compareWords(const ReservedWord& word0, const ReservedWord& word1)
{
	return (word0.name.compare(word1.name) < 0);
}

int Parser::binarySearchWord(const std::string& key)
{
	int top = m_words.size() - 1;
	int bottom = 0;
	int center;
	int str_cmp;

	while (bottom <= top)
	{
		center = (top + bottom)/2;
		str_cmp = m_words[center].name.compare(key);
		if (str_cmp == 0)
		{
			return center;
		}
		else
		{
			if (str_cmp > 0)
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
