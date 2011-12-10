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

#include "dat_reader_clrmamepro.hpp"
#include "../gelide.hpp"

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
	// Tokens para los bloques rom y disk
	TK_SIZE,
	TK_CRC,
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

// Palabras reservadas del parser
CTokenizer::TReservedWord g_words_cmpro[] = {
	// Tokens para los bloques del dat
	TK_CLRMAMEPRO,		"clrmamepro",
	TK_EMULATOR,		"emulator",
	TK_BIOSSET,			"biosset",
	TK_GAME,			"game",
	TK_RESOURCE,		"resource",
	TK_ROM,				"rom",
	TK_DISK,			"disk",
	// Tokens en el bloque clrmamepro y comunes
	TK_NAME,			"name",
	TK_DESCRIPTION,		"description",
	TK_CATEGORY,		"category",
	TK_VERSION,			"version",
	TK_AUTHOR,			"author",
	TK_COMMENT,			"comment",
	// Tokens para el bloque game
	TK_YEAR,			"year",
	TK_MANUFACTURER,	"manufacturer",
	TK_CLONEOF,			"cloneof",
	TK_ROMOF,			"romof",
	// Tokens para los bloques rom y disk
	TK_SIZE,			"size",
	TK_CRC,				"crc",
	TK_MERGE,			"merge",
	TK_FLAGS,			"flags",
	TK_BADDUMP,			"baddump",
	TK_NODUMP,			"nodump",
	// Tokens para los parentesis
	TK_LPAR, 			"(",
	TK_RPAR, 			")"
};

CDatReaderClrMamePro::CDatReaderClrMamePro(void):
	CDatReader(){
	m_tokenizer.setReservedWords(g_words_cmpro, NUM_TK_WORDS);
}

CDatReaderClrMamePro::~CDatReaderClrMamePro(void){}

bool CDatReaderClrMamePro::open(const Glib::ustring& p_file){
	CTokenizer::CToken l_token;

	// El fichero debe ser válido
	assert(p_file.size());

	// Reseteamos si es necesario
	if(m_loaded)
		reset();

	if(m_tokenizer.initFromFile(p_file)){
		// Comprobamos si el primer token es "clrmamepro"
		m_tokenizer.nextToken(l_token);
		if(l_token.getType() == TK_CLRMAMEPRO){
			m_loaded = true;
			parseHeaderBlock();
			return true;
		}
	}
	return false;
}

bool CDatReaderClrMamePro::load(const char* p_buffer, int p_size){
	CTokenizer::CToken l_token;

	// Los datos tienen que ser válidos
	assert(p_buffer);
	assert(p_size);

	// Reseteamos si es necesario
	if(m_loaded)
		reset();

	if(m_tokenizer.initFromMemory(p_buffer, p_size)){
		// Comprobamos si el primer token es "clrmamepro"
		m_tokenizer.nextToken(l_token);
		if(l_token.getType() == TK_CLRMAMEPRO){
			m_loaded = true;
			parseHeaderBlock();
			return true;
		}
	}
	return false;
}

bool CDatReaderClrMamePro::read(std::map<Glib::ustring, CGame*>& p_sets){
	CTokenizer::CToken l_token;
	CGame* l_game = NULL;

	if(!m_loaded)
		return false;

	/*
	 * Estructura principal de un dat en formato ClrMamePro:
	 *
	 * clrmamepro( ... )
	 * emulator( ... )
	 * biosset( ... )
	 * game( ... )
	 * resource( ... )
	 *
	 */
	while(m_tokenizer.hasMoreTokens()){
		m_tokenizer.nextToken(l_token);
		switch(l_token.getType()){
			case TK_GAME:
				l_game = this->parseGameBlock();
				if(l_game)
					p_sets[l_game->getName()] = l_game;
				break;
			case TK_RESOURCE:
				if(m_ignore_bios){
					skipBlock();
					break;
				}
				l_game = this->parseGameBlock();
				if(l_game){
					//l_game->setIsBios(true);
					p_sets[l_game->getName()] = l_game;
				}
				break;
			default:
				skipBlock();
		}
	}
	return true;
}

void CDatReaderClrMamePro::reset(void){
	CDatReader::reset();
	m_tokenizer.reset();
}

Glib::ustring CDatReaderClrMamePro::getType(){
	static Glib::ustring l_type = "ClrMamePro Dat File";

	return l_type;
}

void CDatReaderClrMamePro::parseHeaderBlock(void){
	CTokenizer::CToken l_token;

	/*
	 * Estructura de un bloque clrmamepro:
	 *
	 * clrmamepro (
	 *	name 			""
	 * 	description 	""
	 * 	category 		""
	 * 	version 		"YYYYMMDD"
	 * 	author 			""
	 * 	forcemerging 	none/split/full
	 * 	forcezipping	yes/no
	 * 	header 			""
	 * 	comment 		""
	 * )
	 *
	 */
	// Comenzamos justo en el parentesis izquierdo, lo leemos primero
	m_tokenizer.nextToken(l_token);
	if(l_token.getType() == TK_LPAR){
		while(m_tokenizer.hasMoreTokens() && l_token.getType() != TK_RPAR){
			m_tokenizer.nextToken(l_token);
			switch(l_token.getType()){
				case TK_NAME:
					m_tokenizer.nextToken(l_token);
					m_name = l_token.getString();
					break;
				case TK_DESCRIPTION:
					m_tokenizer.nextToken(l_token);
					m_description = l_token.getString();
					break;
				case TK_CATEGORY:
					m_tokenizer.nextToken(l_token);
					m_category = l_token.getString();
					break;
				case TK_VERSION:
					m_tokenizer.nextToken(l_token);
					m_version = l_token.getString();
					break;
				case TK_AUTHOR:
					m_tokenizer.nextToken(l_token);
					m_author = l_token.getString();
					break;
				case TK_COMMENT:
					m_tokenizer.nextToken(l_token);
					m_comment = l_token.getString();
					break;
				case TK_RPAR:
					break;
				default:
					// Saltamos los campos que no nos interesan
					//m_tokenizer.nextToken(l_token);
					break;
			}
		}
	}
}

CGame* CDatReaderClrMamePro::parseGameBlock(void){
	CTokenizer::CToken l_token;
	CGame* l_game = NULL;

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
	 *	biosset( ... )
	 *	rom( ... )
	 *	...
	 *	rom( ... )
	 *	disk( ... )
	 *	...
	 *	disk( ... )
	 *	sampleof		""
	 *	sample			""
	 *	...
	 *	sample			""
	 * )
	 *
	 */
	// Comenzamos justo en el parentesis izquierdo, lo leemos primero
	m_tokenizer.nextToken(l_token);
	if(l_token.getType() == TK_LPAR){
		l_game = new CGame();
		l_game->setUnknown(false);
		while(m_tokenizer.hasMoreTokens() && (l_token.getType() != TK_RPAR)){
			m_tokenizer.nextToken(l_token);
			switch(l_token.getType()){
				case TK_NAME:
					m_tokenizer.nextToken(l_token);
					l_game->setName(l_token.getString());
					break;
				case TK_DESCRIPTION:
					m_tokenizer.nextToken(l_token);
					l_game->setDescription(l_token.getString());
					break;
				case TK_YEAR:
					m_tokenizer.nextToken(l_token);
					l_game->setYear(l_token.getValue());
					break;
				case TK_MANUFACTURER:
					m_tokenizer.nextToken(l_token);
					l_game->setManufacturer(l_token.getString());
					break;
				case TK_CLONEOF:
					m_tokenizer.nextToken(l_token);
					//l_game->setCloneOf(l_token.getString());
					break;
				case TK_ROMOF:
					m_tokenizer.nextToken(l_token);
					//l_game->setRomOf(l_token.getString());
					break;
				// Solamente leemos la primera rom o disk
				case TK_ROM:
					// Si ya hemos leido la rom, saltamos el bloque
					if(l_game->getRomCrc().size()){
						skipBlock();
						break;
					}
					l_game->setRomCrc(parseRomBlock());
					break;
				case TK_DISK:
					/*l_rom = this->parseRomBlock();
					if(l_rom != NULL){
						l_rom->setType(CRom::ROM_TYPE_DISK);
						l_game->addRom(l_rom);
					}*/
					skipBlock();
					break;
				/*
				 * CHECKME: Estos bloques los saltamos porque no tenemos claro
				 * cual es su utilidad. Parece que solo aparecen en dat's viejos
				 * de MAME y además parecen estar desapareciendo de los dat's.
				 */
				case TK_EMULATOR:
					skipBlock();
					break;

				case TK_BIOSSET:
					skipBlock();
					break;

				case TK_RPAR:
					break;

				default:
					//m_tokenizer.nextToken(l_token);
					break;
			}
		}
		return l_game;
	}
	return NULL;
}

Glib::ustring CDatReaderClrMamePro::parseRomBlock(void){
	CTokenizer::CToken l_token;
	Glib::ustring l_crc;
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
	m_tokenizer.nextToken(l_token);
	if(l_token.getType() == TK_LPAR){
		while(m_tokenizer.hasMoreTokens() && l_token.getType() != TK_RPAR){
			m_tokenizer.nextToken(l_token);
			switch(l_token.getType()){
				case TK_NAME:
					m_tokenizer.nextToken(l_token);
					break;
				case TK_SIZE:
					m_tokenizer.nextToken(l_token);
					break;
				case TK_CRC:
					m_tokenizer.nextToken(l_token);
					l_crc = l_token.getString();
					break;
				case TK_MERGE:
					m_tokenizer.nextToken(l_token);
					break;
				case TK_BADDUMP:
					break;
				case TK_NODUMP:
					break;
				case TK_RPAR:
					break;
				default:
					//m_tokenizer.nextToken(l_token);
					break;
			}
		}
	}
	return l_crc;
}

void CDatReaderClrMamePro::skipBlock(void){
	CTokenizer::CToken l_token;
	int l_open_par;

	// Leemos el paréntesis izquierdo
	m_tokenizer.nextToken(l_token);
	if(l_token.getType() == TK_LPAR){
		// Indicamos que se ha leido un parentesis izquierdo
		l_open_par = 1;
		while(m_tokenizer.hasMoreTokens() && (l_open_par > 0)){
			m_tokenizer.nextToken(l_token);
			// Si encontramos un parentesis derecho restamos uno al número
			// de parentesis abiertos
			if(l_token.getType() == TK_RPAR){
				l_open_par--;
			}
			// Si encontramos un parentesis izquierdo sumamos uno al número de
			// parentesis abiertos
			else if(l_token.getType() == TK_LPAR){
				l_open_par++;
			}
		}
	}
}
