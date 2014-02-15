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

#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_

#include <string>
#include <vector>


// Delimitadores por defecto: Espacio, Salto de línea, Retorno de carro,
// Tabulador horizontal, Tabulador vertical y Avance de página
static const std::string DEFAULT_DELIMITERS = " \n\r\t\v\f";

/**
 * Tokenizador de cadenas simple.
 *
 * Permite trocear cadenas de texto o el contenido de un fichero en sus
 * sub-bloques.
 * Permite establecer los caracteres que se detectarán como delimitadores. Por
 * defecto detecta como delimitadores los caracteres de Espacio, Salto de línea,
 * Retorno de carro, Tabulador horizontal, Tabulador vertical y Avance de
 * página.
 * Incluye soporte para detectar cadenas de texto entrecomilladas activado por
 * defecto.
 */
class Tokenizer
{
public:
	/**
	 * Constructor básico de la clase. Se encarga de establecer los valores
	 * por defecto que utilizará el tokenizador.
	 */
	Tokenizer(void);

	/**
	 * Constructor parametrizado
	 * @param delimiters Delimitadores a usar por defecto
	 * @param detect_str Indica si se deben detectar las cadenas de texto
	 */
	Tokenizer(const std::string& delimiters, const bool detect_str = true);

	/**
	 * Destructor
	 */
	~Tokenizer();

	/**
	 * Inicializa el tokenizador sobre un fichero determinado
	 * @param file nombre del fichero con el contenido a tokenizar
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromFile(const std::string& file);

	/**
	 * Inicializa el tokenizador sobre una cadena de texto
	 * @param str Cadena de texto a tokenizar
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromString(const std::string& str);

	/**
	 * Inicializa el tokenizador a partir de un buffer de memoria
	 * @param buffer Puntero al buffer donde se almacenan los datos
	 * @param size Tamaño total del buffer
	 * @return true si se realizó la carga correctamente
	 */
	bool initFromMemory(const char *buffer, const unsigned int size);

	/**
	 * Establece los delimitadores que debe reconocer el tokenizador para
	 * separar tokens. Los delimitadores por defecto son ' ', '\n', '\r', '\t',
	 * '\v', \f'
	 * @param delimiters Nuevos delimitadores a usar por el tokenizador
	 * @note El tokenizador continuará por donde estaba, para comenzar de nuevo
	 * con los nuevos delimitadores, usar reset()
	 */
	void setDelimiters(const std::string& delimiters);

	/**
	 * Activa o desactiva la detección de cadenas en el tokenizador.
	 * Se consideran cadenas las siguientes: "..." ó "...\0
	 * @param detect true para activar la detección, false para desactivarla
	 */
	void setStringDetection(const bool detect = true);

	/**
	 * Obtiene el siguiente token
	 * @return Token leído o "" si no quedan más tokens
	 */
	std::string nextToken(void);

	/**
	 * Indica si quedan tokens por leer en el tokenizador
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
	 * Resetea el tokenizador para comenzar el proceso de nuevo
	 */
	void reset(void);

	/**
	 * Obtiene todos los tokens restantes de golpe
	 * @return Vector con los tokens restantes
	 */
	std::vector<std::string> split(void);

private:

	/**
	 * Comprueba si la posición de lectura del buffer está sobre un delimitador
	 * @return true si se encontró un delimitador, false en otro caso
	 */
	bool isDelimiter(void);

	/**
	 * Obtiene una cadena del buffer del tokenizador
	 * @return cadena leída
	 * @note Se consideran cadenas las siguientes: "..." ó "...\0
	 * @note trata entidades como un solo caracter (\t, \n, etc)
	 */
	std::string getString(void);

	std::string m_buff;						/**< Buffer del tokenizador */
	std::string::const_iterator m_buff_pos;	/**< Posición de lectura del buffer */
	std::string m_delimiters;				/**< Lista de delimitadores */
	std::string m_token;					/**< Token leído */
	char m_last_delimiter;					/**< Ultimo delimitador encontrado */
	bool m_detect_strings;					/**< Estado de la detección de cadenas */
};

#endif // _TOKENIZER_HPP_
