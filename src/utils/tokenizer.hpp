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

#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_

#include "../gelide.hpp"
#include <glibmm/ustring.h>

#define TOKENIZER_RES_WORD_SIZE	25	/**< Tamaño máximo de las palabras reservadas */

/**
 * Clase encargada del particionamiento de de un fichero de texto en bloques
 * fundamentales (tokens) de un conjunto dado como parámetro y teniendo en
 * cuenta unos delimitadores determinados.
 */
class CTokenizer
{
public:

	/**
	 * Tokens genericos devueltos por el tokenizador. Se definen negativos, para
	 * utilizar los positivos con las palabras reservadas del usuario.
	 */
	enum TTokenTypes{
		TK_NOTK	= -1,		/**< Token Notoken */
		TK_EOF = -2,		/**< Token final de fichero */
		TK_UNK = -3,		/**< Token desconocido */
		TK_ID = -4,			/**< Token identificador */
		TK_NUM = -5,		/**< Token número */
		TK_STR = -6			/**< Token cadena */
	};

	/** Tipo para especificar las palabras reservadas */
	typedef struct
	{
		int m_type;								/**< Tipo de token */
		char m_name[TOKENIZER_RES_WORD_SIZE];	/**< Identificador de la palabra */
	}TReservedWord;

	/**
	 * Encapsula a un bloque representativo de un conjunto especial
	 * de bloques de texto.
	 */
	class CToken
	{
	public:

		/**
		 * Constructor de la clase
		 * @param p_type Identificador del tipo de token
		 * @param p_value Valor numérico del token
		 * @param p_string Cadena de texto token
		 */
		CToken(const int p_type = 0, const unsigned int p_value = -1,
			   char* p_string = NULL);
		/**
		 * Destructor de la clase
		 */
		virtual ~CToken(){};

		/**
		 * Establece el tipo del token
		 * @param p_type Nuevo tipo que se asigna al token
		 */
		void setType(const int p_type = 0);

		/**
		 * Establece el valor numérico del token
		 * @param p_value Nuevo valor numérico para el token
		 */
		void setValue(const unsigned int p_value = -1);

		/**
		 * Establece el valor textual del token
		 * @param p_string Nuevo valor textual del token
		 */
		void setString(char* p_string = NULL);

		/**
		 * Devuelve el tipo de token
		 * @return Identificador del tipo de token
		 */
		int getType(void);

		/**
		 * Devuelve el valor numérico del token
		 * @return Valor numérico del token
		 */
		unsigned int getValue(void);

		/**
		 * Devuelve el valor textual del token
		 * @return Puntero al valor textual del token
		 */
		char* getString(void);

	private:
		friend class CTokenizer;

		int m_type;				/**< Identificador del tipo de token */
		unsigned int m_value;	/**< Valor numérico del token */
		char* m_string;			/**< Valor textual del token */
	};

	/**
	 * Constructor básico de la clase. Se encarga de establecer los valores
	 * por defecto que utilizará el tokenizador.
	 */
	CTokenizer(void);

	/**
	 * Destructor, se encarga de limpiar la memoria interna del tokenizador
	 */
	~CTokenizer();

	/**
	 * Inicializa el tokenizador sobre un fichero determinado
	 * @param p_file nombre del fichero con el contenido a tokenizar
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromFile(const Glib::ustring& p_file);

	/**
	 * Inicializa el tokenizador sobre una cadena de texto
	 * @param p_str Cadena de texto a tokenizar
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromString(const Glib::ustring& p_str);

	/**
	 * Inicializa el tokenizador a partir de un buffer de memoria
	 * @param p_buffer Puntero al buffer donde se almacenan los datos
	 * @param p_size Tamaño total del buffer
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromMemory(const char *p_buffer, int p_size);

	/**
	 * Establece los delimitadores que debe reconocer el tokenizador para
	 * separar tokens. Los delimitadores por defecto son ' ', '\t', '\r', '\n'
	 * @param p_delimiters Array de caracteres delimitadores
	 * @param p_count Número de caracteres delimitadores en el array
	 * @note Cuando se llama al método para establecer los delimitadores, los
	 * delimitadores por defecto son desactivados por lo que cualquiera de ellos
	 * que deba ser detectado, deberá estár en los nuevos delimitadores.
	 */
	void setDelimiters(char* p_delimiters, unsigned int p_count);

	/**
	 * Establece las palabras reservadas que identificará el tokenizador
	 * @param p_reserved Array de palabras reservadas
	 * @param p_count Contador de palabras reservadas
	 */
	void setReservedWords(TReservedWord p_reserved[], unsigned int p_count);

	/**
	 * Activa o desactiva la detección de cadenas en el tokenizador.
	 * Se consideran cadenas las siguientes: "..." ó "...\0
	 * @param p_detect true para activar la detección, false para desactivarla
	 * @note Por defecto la detección de cadenas está activada.
	 */
	void setStringsDetection(const bool p_detect = true);

	/**
	 * Activa o desactiva la detección de números en el tokenizador.
	 * @param p_detect true para activar la detección, false para desactivarla
	 * @note De momento solo se detectan enteros positivos sin signo
	 */
	void setNumbersDetection(const bool p_detect = true);

	/**
	 * Obtiene el siguiente token
	 * @param p_token Token donde dejar el siguiente token
	 */
	void nextToken(CToken& p_token);

	/**
	 * Indica si quedan tokens por leer en el tokenizador
	 * @return true si quedan más tokens por leer
	 */
	bool hasMoreTokens(void);

	/**
	 * Libera la memoria utilizada por el tokenizador y lo deja como al inicio
	 */
	void reset(void);

private:
	/** Tipo para especificar las palabras reservadas internamente */
	typedef struct
	{
		int m_type;								/**< Tipo de token */
		int m_size;
		char m_name[TOKENIZER_RES_WORD_SIZE];	/**< Identificador de la palabra */
	}TWord;

	/**
	 * Comprueba si el puntero de lectura del buffer está sobre un delimitador
	 * @return true si se encontró un delimitador
	 */
	bool isDelimiter(void);

	/**
	 * Búsca en las palabras reservadas un token y lo rellena si lo encuentra
	 * @param p_token Token de destino
	 */
	bool checkReservedWords(CToken& p_token);

	/**
	 * Comprueba si token dado dado es un número
	 * @param p_token token a comprobar
	 * @return true si el token dado es un número entero sin signo
	 */
	bool checkNum(CToken& p_token);

	/**
	 * Establece el puntero de lectura del buffer sobre el siguiente token
	 * @return true si se pudo posicionar, false si se llega al final del texto
	 */
	bool findNextToken(void);

	/**
	 * Obtiene una cadena del buffer del tokenizador
	 * @return puntero a la cadena
	 * @note Se consideran cadenas las siguientes: "..." ó "...\0
	 */
	char* getString(void);

	/**
	 * Obtiene un identificador del buffer del tokenizador
	 * @return puntero al identificador
	 */
	char* getIdentifier(void);

	/**
	 * Compara dos palabras internas e indica si la primera es menor
	 * @param p_word0 Primera palabra interna
	 * @param p_word1 Segunda palabra interna
	 * @return true si la primera palabra es menor que la segunda
	 * @note: Este método se utiliza internamente para realizar una ordenación
	 * de las palabras reservadas.
	 */
	static inline bool compareWords(const TWord& p_word0, const TWord& p_word1);

	/**
	 * Realiza una busqueda binaria en la lista de palabras reservadas
	 * @param p_key Cadena de texto a buscar en las palabras reservadas
	 * @return Posición de la palabra reservada o -1 si no se encuentra
	 */
	inline int binarySearchWord(char* p_key);


	bool m_init;						/**< Indica si el tokenizador se inicio */
	bool m_detect_strings;				/**< Estado de la detección de cadenas */
	bool m_detect_numbers;				/**< Estado de la detección de números */
	char* m_buff;						/**< Buffer del tokenizador */
	char* m_buff_pos;					/**< Puntero de lectura del buffer */
	char* m_delimiters;					/**< Lista de delimitadores */
	unsigned int m_delimiters_count;	/**< Número de delimitadores */
	char m_default_delimiters[4];		/**< Delimitadores por defecto */
	TWord* m_words;						/**< Lista de palabras reservadas */
	unsigned int m_words_count;			/**< Número de palabras reservadas */

};

#endif // _TOKENIZER_HPP_
