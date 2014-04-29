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

#include "../../defines.hpp"
#include <vector>
#include "clrmamepro_reader.hpp"


namespace gelide{

// Tipo enumerado para identificar los tokens
enum
{
	// Tokens para los bloques del dat
	TK_CLRMAMEPRO,
	TK_EMULATOR,
	TK_BIOSSET,
	TK_GAME,
	TK_RESOURCE,
	TK_ROM,
	TK_DISK,
	// Tokens en el bloque clrmamepro y comunes
	TK_NAME,
	TK_DESCRIPTION,
	TK_CATEGORY,
	TK_VERSION,
	TK_AUTHOR,
	TK_COMMENT,
	// Tokens para el bloque game
	TK_YEAR,
	TK_MANUFACTURER,
	TK_CLONEOF,
	TK_ROMOF,
	TK_SOURCEFILE,
	// Tokens para los bloques rom y disk
	TK_SIZE,
	TK_CRC,
	TK_MD5,
	TK_SHA1,
	TK_MERGE,
	TK_FLAGS,
	TK_BADDUMP,
	TK_NODUMP,
	// Tokens para los parentesis
	TK_LPAR,				// Token Parentesis izquierdo
	TK_RPAR,				// Token Parentesis derecho
	// Contador de palabras reservadas
	NUM_TK_WORDS
};

ClrMameProReader::ClrMameProReader(void):
	DatReader()
{
	std::vector<ReservedWord> words;
	ReservedWord word;

	// Tokens para los bloques del dat
	word = {TK_CLRMAMEPRO, "clrmamepro"};
	words.push_back(word);
	word = {TK_EMULATOR, "emulator"};
	words.push_back(word);
	word = {TK_BIOSSET, "biosset"};
	words.push_back(word);
	word = {TK_GAME, "game"};
	words.push_back(word);
	word = {TK_GAME, "game"};
	words.push_back(word);
	word = {TK_RESOURCE, "resource"};
	words.push_back(word);
	word = {TK_ROM, "rom"};
	words.push_back(word);
	word = {TK_DISK, "disk"};
	words.push_back(word);
	// Tokens en el bloque clrmamepro y comunes
	word = {TK_NAME, "name"};
	words.push_back(word);
	word = {TK_DESCRIPTION, "description"};
	words.push_back(word);
	word = {TK_CATEGORY, "category"};
	words.push_back(word);
	word = {TK_VERSION, "version"};
	words.push_back(word);
	word = {TK_AUTHOR, "author"};
	words.push_back(word);
	word = {TK_COMMENT, "comment"};
	words.push_back(word);
	// Tokens para el bloque game
	word = {TK_YEAR, "year"};
	words.push_back(word);
	word = {TK_MANUFACTURER, "manufacturer"};
	words.push_back(word);
	word = {TK_CLONEOF, "cloneof"};
	words.push_back(word);
	word = {TK_ROMOF, "romof"};
	words.push_back(word);
	word = {TK_SOURCEFILE, "sourcefile"};
	words.push_back(word);
	// Tokens para los bloques rom y disk
	word = {TK_SIZE, "size"};
	words.push_back(word);
	word = {TK_CRC, "crc"};
	words.push_back(word);
	word = {TK_MD5, "md5"};
	words.push_back(word);
	word = {TK_SHA1, "sha1"};
	words.push_back(word);
	word = {TK_MERGE, "merge"};
	words.push_back(word);
	word = {TK_FLAGS, "flags"};
	words.push_back(word);
	word = {TK_BADDUMP, "baddump"};
	words.push_back(word);
	word = {TK_NODUMP, "nodump"};
	words.push_back(word);
	// Tokens para los parentesis
	word = {TK_LPAR, "("};
	words.push_back(word);
	word = {TK_RPAR, ")"};
	words.push_back(word);

	m_parser.setReservedWords(words);
}

ClrMameProReader::~ClrMameProReader(void)
{
	clean();
}

bool ClrMameProReader::load(const char* buffer, const unsigned int size)
{
	Token token;

	assert(buffer);
	assert(size);

	if (m_parser.initFromMemory(buffer, size))
	{
		// Comprobamos si el primer token es "clrmamepro"
		token = m_parser.nextToken();
		if (token.type == TK_CLRMAMEPRO)
		{
			// Saltamos la cabecera ya que no haremos nada con ella
			skipBlock();
			m_loaded = true;
			return true;
		}
	}
	return false;
}

bool ClrMameProReader::read(std::map<Glib::ustring, DatSet>& set_collection)
{
	Token token;
	DatSet set;

	assert(m_loaded);

	set_collection.clear();

	m_parser.reset();

	/*
	 * Estructura principal de un dat en formato ClrMamePro:
	 *
	 * clrmamepro ( ... )
	 * emulator ( ... )
	 * biosset ( ... )
	 * game ( ... )
	 * resource ( ... )
	 *
	 */
	while (m_parser.hasMoreTokens())
	{
		token = m_parser.nextToken();
		switch (token.type)
		{
		case TK_GAME:
			set.is_bios = false;
			if (this->parseGameBlock(set))
			{
				set_collection[set.name] = set;
			}
			break;
		// Los set bios están en este bloque en vez de biosset
		case TK_RESOURCE:
			set.is_bios = true;
			if (this->parseGameBlock(set))
			{
				set_collection[set.name] = set;
			}
			break;
		default:
			skipBlock();
		}
	}
	clean();
	return true;
}

bool ClrMameProReader::read(std::vector<DatSet>& set_collection)
{
	Token token;
	DatSet set;

	assert(m_loaded);

	set_collection.clear();

	m_parser.reset();

	/*
	 * Estructura principal de un dat en formato ClrMamePro:
	 *
	 * clrmamepro ( ... )
	 * emulator ( ... )
	 * biosset ( ... )
	 * game ( ... )
	 * resource ( ... )
	 *
	 */
	while (m_parser.hasMoreTokens())
	{
		token = m_parser.nextToken();
		switch (token.type)
		{
		case TK_GAME:
			set.is_bios = false;
			if (this->parseGameBlock(set))
			{
				set_collection.push_back(set);
			}
			break;
		// Los set bios están en este bloque en vez de biosset
		case TK_RESOURCE:
			set.is_bios = true;
			if (this->parseGameBlock(set))
			{
				set_collection.push_back(set);
			}
			break;
		default:
			skipBlock();
		}
	}
	clean();
	return true;
}

Glib::ustring ClrMameProReader::getType(void)
{
	static const Glib::ustring type = "ClrMamePro Dat File";

	return type;
}

bool ClrMameProReader::parseGameBlock(DatSet& set)
{
	Token token;

	/*
	 * Estructura de un bloque game o resource:
	 *
	 * game / resource (
	 *	name			""
	 *	description		""
	 *	year			"YYYYMMDD"
	 *	manufacturer	""
	 *	cloneof			""
	 *	romof			""
	 *	sourcefile		""
	 *	biosset ( ... )
	 *	rom ( ... )
	 *	...
	 *	rom ( ... )
	 *	disk ( ... )
	 *	...
	 *	disk ( ... )
	 *	sampleof		""
	 *	sample			""
	 *	...
	 *	sample			""
	 * )
	 *
	 */
	// Comenzamos justo en el parentesis izquierdo, lo leemos primero
	token = m_parser.nextToken();
	if (token.type == TK_LPAR)
	{
		set.name.clear();
		set.description.clear();
		set.year.clear();
		set.manufacturer.clear();
		set.clone_of.clear();
		set.crc.clear();
		while (m_parser.hasMoreTokens() && (token.type != TK_RPAR))
		{
			token = m_parser.nextToken();
			switch (token.type)
			{
			case TK_NAME:
				token = m_parser.nextToken();
				set.name = token.string;
				// HACK: Detectar bios en dats de no-intro
				if (set.name.substr(0,6) == "[BIOS]")
				{
					set.is_bios = true;
				}
				break;
			case TK_DESCRIPTION:
				token = m_parser.nextToken();
				set.description = token.string;
				break;
			case TK_YEAR:
				token = m_parser.nextToken();
				set.year = token.string;
				break;
			case TK_MANUFACTURER:
				token = m_parser.nextToken();
				set.manufacturer = token.string;
				break;
			case TK_CLONEOF:
				token = m_parser.nextToken();
				set.clone_of = token.string;
				break;
			// Estos bloques no los necesitamos, los pasamos
			case TK_ROMOF:
			case TK_SOURCEFILE:
				// Leemos un parámetro para llegar al siguiente bloque
				token = m_parser.nextToken();
				break;
			// Solamente leemos la primera rom o disk
			case TK_ROM:
				// Si ya hemos leido la rom, saltamos el bloque
				if (!set.crc.empty())
				{
					skipBlock();
					break;
				}
				set.crc = parseRomBlock();
				break;
			/*
			 * CHECKME: Estos bloques del dat los saltamos.
			 * No está claro cual es su utilidad. Parece que solo aparecen en
			 * ficheros dat muy viejos y únicamente de MAME.
			 */
			case TK_DISK:
			case TK_EMULATOR:
			case TK_BIOSSET:
				skipBlock();
				break;
			case TK_RPAR:
				break;
			default:
				break;
			}
		}
		return true;
	}
	return false;
}

Glib::ustring ClrMameProReader::parseRomBlock(void)
{
	Token token;
	Glib::ustring crc;
	/*
	 * Estructura de un bloque rom o disk:
	 *
	 * rom / disk (
	 * 	name			""
	 *	size			int
	 *	crc				id8
	 *	md5				id32
	 *	sha1			id40
	 *	merge			""
	 *	flags			baddump/nodump
	 * )
	 *
	 */
	// Comenzamos justo en el parentesis izquierdo, lo leemos primero
	token = m_parser.nextToken();
	if (token.type == TK_LPAR)
	{
		while (m_parser.hasMoreTokens() && token.type != TK_RPAR)
		{
			token = m_parser.nextToken();
			switch (token.type)
			{
			// Solamente nos interesa el campo con el crc
			case TK_CRC:
				token = m_parser.nextToken();
				crc = token.string;
				break;
			case TK_NAME:
			case TK_SIZE:
			case TK_MD5:
			case TK_SHA1:
			case TK_MERGE:
			case TK_FLAGS:
				token = m_parser.nextToken();
				break;
			case TK_RPAR:
				break;
			default:
				break;
			}
		}
	}
	return crc;
}

void ClrMameProReader::skipBlock(void)
{
	Token token;
	int open_par;

	// Leemos el paréntesis izquierdo
	token = m_parser.nextToken();
	if (token.type == TK_LPAR)
	{
		// Indicamos que se ha leido un parentesis izquierdo
		open_par = 1;
		while (m_parser.hasMoreTokens() && (open_par > 0))
		{
			token = m_parser.nextToken();
			// Si encontramos un parentesis derecho restamos uno al número
			// de parentesis abiertos
			if (token.type == TK_RPAR)
			{
				open_par--;
			}
			// Si encontramos un parentesis izquierdo sumamos uno al número de
			// parentesis abiertos
			else if (token.type == TK_LPAR)
			{
				open_par++;
			}
		}
	}
}

void ClrMameProReader::clean(void)
{
	//m_parser.reset();
	m_loaded = false;
}

} // namespace gelide
