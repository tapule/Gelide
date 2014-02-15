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

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <vector>
#include "tokenizer.hpp"


/**
 * Tokens genericos devueltos por el parser. Se definen negativos, para utilizar
 * los positivos con las palabras reservadas del usuario.
 */
enum TokenTypes{
	TK_NOTK	= -1,		/**< Token Notoken */
	TK_EOF = -2,		/**< Token final de fichero */
	TK_UNK = -3,		/**< Token desconocido */
	TK_STR = -4,		/**< Token cadena */
	TK_NUM = -5,		/**< Token número */
};

/** Tipo para especificar las palabras reservadas */
struct ReservedWord
{
	int type;			/**< Tipo de token */
	std::string name;	/**< Identificador de la palabra */
};

/**
 * Bloque unitario parseado de la fuente.
 */
struct Token
{
	/**
	 * Constructor básico
	 */
	Token(void):
		type(TK_NOTK),
		value(0)
	{
	}

	/**
	 * Constructor de la clase
	 * @param type Identificador del tipo de token
	 * @param value Valor numérico del token
	 * @param string Cadena de texto token
	 */
	Token(const int p_type, const unsigned int p_value, const std::string& p_string):
		type(p_type),
		value(p_value),
		string(p_string)
	{
	}

	int type;				/**< Identificador del tipo de token */
	unsigned int value;		/**< Valor numérico del token */
	std::string string;		/**< Valor textual del token */
};

/**
 * Clase encargada del particionamiento de de una fuente de texto en bloques
 * de un conjunto dado y teniendo en cuenta unos delimitadores determinados.
 *
 * El parser devolverá Tokens con el contenido que se va leyendo de la fuente.
 * Los Tokens detectables, se puden ampliar agregando palabras reservadas.
 */
class Parser
{
public:
	/**
	 * Constructor básico de la clase. Se encarga de establecer los valores
	 * por defecto que utilizará el parser.
	 */
	Parser(void);

	/**
	 * Destructor, se encarga de limpiar la memoria interna del parser
	 */
	~Parser();

	/**
	 * Inicializa el parser sobre un fichero determinado
	 * @param file nombre del fichero con el contenido
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromFile(const std::string& file);

	/**
	 * Inicializa el parser sobre una cadena de texto
	 * @param str Cadena de texto fuente
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromString(const std::string& str);

	/**
	 * Inicializa el parser a partir de un buffer de memoria
	 * @param buffer Puntero al buffer donde se almacenan los datos
	 * @param size Tamaño total del buffer
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromMemory(const char *buffer, const unsigned int size);

	/**
	 * Establece los delimitadores que debe reconocer el parser para
	 * separar tokens. Los delimitadores por defecto son ' ', '\n', '\r', '\t',
	 * '\v', \f'
	 * @param delimiters Nuevos delimitadores a usar por el parser
	 * @note El parser continuará por donde estaba, para comenzar de nuevo
	 * con los nuevos delimitadores, usar reset()
	 */
	void setDelimiters(const std::string& delimiters);

	/**
	 * Establece las palabras reservadas que identificará el parser
	 * @param words Palabras reservadas que detectará el parser
	 */
	void setReservedWords(const std::vector<ReservedWord>& words);

	/**
	 * Activa o desactiva la detección de cadenas en el parser.
	 * Se consideran cadenas las siguientes: "..." ó "...\0
	 * @param detect true para activar la detección, false para desactivarla
	 */
	void setStringDetection(const bool detect = true);

	/**
	 * Activa o desactiva la detección de números en el parser.
	 * @param detect true para activar la detección, false para desactivarla
	 * @note De momento solo se detectan enteros positivos sin signo
	 */
	void setNumberDetection(const bool detect = true);

	/**
	 * Obtiene el siguiente token
	 * @return Siguiente token detectado
	 */
	Token nextToken(void);

	/**
	 * Indica si quedan tokens por leer
	 * @return true si quedan más tokens por leer
	 * @note Posiciona la lectura en el comienzo del siguiente token valido
	 */
	bool hasMoreTokens(void);

	/**
	 * Obtiene el último delimitador encontrado durante el proceso
	 * @return Carácter delimitador detectado
	 */
	char lastDelimiter(void);

	/**
	 * Resetea el parser para comenzar el proceso de nuevo
	 */
	void reset(void);

private:

	/**
	 * Búsca en las palabras reservadas un token y lo rellena si lo encuentra
	 * @param token Token de destino
	 */
	bool checkReservedWords(Token& token);

	/**
	 * Comprueba si token dado dado es un número
	 * @param token token a comprobar
	 * @return true si el token dado es un número entero sin signo
	 */
	bool checkNum(Token& token);

	/**
	 * Compara dos palabras reservadas e indica si la primera es menor
	 * @param word0 Primera palabra reservada
	 * @param word1 Segunda palabra reservada
	 * @return true si la primera palabra es menor que la segunda
	 * @note: Este método se utiliza internamente para realizar una ordenación
	 * de las palabras reservadas.
	 */
	static inline bool compareWords(const ReservedWord& word0, const ReservedWord& word1);

	/**
	 * Realiza una busqueda binaria en la lista de palabras reservadas
	 * @param key Cadena de texto a buscar en las palabras reservadas
	 * @return Posición de la palabra reservada o -1 si no se encuentra
	 */
	inline int binarySearchWord(const std::string& key);

	bool m_detect_numbers;				/**< Estado de la detección de números */
	std::vector<ReservedWord> m_words;	/**< Lista de palabras reservadas */
	Tokenizer m_tokenizer;				/**< Tokenizador de palabras */
};

#endif // #define _PARSER_HPP_
