/*
Phont - générateur de caractères pour OpenGL
Copyright (C) 2010  Philippe Proulx <philippe.proulx@polymtl.ca>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _PHONT_H
#define _PHONT_H

#define _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>
#include "glew.h"
#include <cstdarg>

/**
 * @class Phont
 * Gère l'affichage de texte basé sur une image de caractères générés par
 * Bitmap Font Generator v1.12 (http://www.angelcode.com/products/bmfont/).
 * L'image doit être de type Windows bitmap 8-bit, et les pixels "allumés" doivent
 * être de couleur à l'index 0x00 de la palette du fichier. Le fichier de description
 * ".fnt" doit être de type texte. Ne gère pas le kerning inter-caractère pour l'instant.
 * @author Philippe Proulx
 */
class Phont {
	public:
	struct bmp_magic {
		unsigned char magic[2];
	};
	 
	struct bmp_header {
		unsigned int filesz;
		unsigned short creator1;
		unsigned short creator2;
		unsigned int bmp_offset;
	};

	struct bmp_dib_v3_header {
		unsigned int header_sz;
		unsigned int width;
		unsigned int height;
		unsigned short nplanes;
		unsigned short bitspp;
		unsigned int compress_type;
		unsigned int bmp_bytesz;
		unsigned int hres;
		unsigned int vres;
		unsigned int ncolors;
		unsigned int nimpcolors;
	};

	struct phont_bfg_info {
		char face [128];
		int size;
		int bold;
		int italic;
		char charset [16];
		int unicode;
		int stretch_h;
		int smooth;
		int aa;
		int padding [4];
		int spacing [2];
		int outline;
	};

	struct phont_bfg_common {
		int line_height;
		int base;
		int scale_w;
		int scale_h;
		int pages;
		int packed;
		int alpha_chan;
		int red_chan;
		int green_chan;
		int blue_chan;
	};

	struct phont_bfg_char {
		int id;
		bool active;
		int x;
		int y;
		int w;
		int h;
		int x_off;
		int y_off;
		int x_adv;
		int page;
		int chan;
	};	
	Phont();
	~Phont();

	//change la taille du texte. (custom added)
	void scaleText(float pourcentage);
	float getScale() {return scale_;}

	bool load(const char* pref, const char* name);
	bool unload(void);
	bool isLoaded(void) const;
	GLuint getCLBase(void) const;
	struct phont_bfg_info getBFGInfo(void) const;
	struct phont_bfg_common getBFGCommon(void) const;
	struct phont_bfg_char getCharInfo(const unsigned int index) const;
	unsigned int getCharCount(void) const;
	unsigned int getTextLength(const char* text) const;
	bool print(GLfloat vp_x, GLfloat vp_y, bool keep_gl, const char* format, ...) const;
	
	private:
		float scale_;
	Phont(const Phont& copy) {}
	Phont& operator=(const Phont& copy) {}
	bool load_bfg(const char* filename);
	bool create_cl(const char* filename);
	inline unsigned int dib_pos(const unsigned int y, const unsigned int ph,
		const unsigned int ih);
	inline unsigned int pixoff(const unsigned int x, const unsigned int y,
		const unsigned int w);
	inline unsigned int pmod(const unsigned int pad, const unsigned int tot);
	bool read_8bit_bmp(const char* filename, unsigned char** pixmap,
		struct bmp_dib_v3_header* dibhead);
	bool cut_in_buf(const unsigned char* pixmap, const struct bmp_dib_v3_header* dibhead,
		unsigned char* newbuf, const unsigned int x_off, const unsigned int y_off,
		const unsigned int wi, const unsigned int he);
	unsigned int pixmap_to_bitmap(const unsigned char* pixmap,
		const unsigned int width, const unsigned int height, unsigned char** bitmap,
		const unsigned int pad, const unsigned char p_on, const unsigned char pad_ch);
	
	struct phont_bfg_info info_;
	struct phont_bfg_common common_;
	struct phont_bfg_char* chars_;
	GLuint cl_base_;
	unsigned int count_;
	bool loaded_;
};

/**
 * Calcule la position en y d'un pixel sachant que le tampon est inversé verticalement,
 * pour un sous-tampon découpé.
 * @author Philippe Proulx
 *
 * @param y		Position en y du pixel
 * @param ph		Hauteur du tampon total
 * @param ih		Hauteur du tampon découpé
 * @return		Nouvelle position en y
 */
inline unsigned int Phont::dib_pos(const unsigned int y, const unsigned int ph,
const unsigned int ih) {
	return ph - y - ih;
}

/**
 * Calcule l'index d'un pixel dans un tapon de pixels, selon un position et la largeur
 * de l'image contenue dans le tampon.
 * @author Philippe Proulx
 *
 * @param x	Position en x
 * @param y	Position en y
 * @param w	Largeur de l'image comprise dans le tampon de pixels
 * @return	Index dans le tampon du pixel recherché
 */
unsigned int Phont::pixoff(const unsigned int x, const unsigned int y, const unsigned int w) {
	return y * w + x;
}

/**
 * Calcule, pour un padding donné, le nombre d'unité manquante pour l'atteindre.
 * @author Philippe Proulx
 *
 * @param pad		Padding
 * @param tot		Nombre total d'unités
 * @return		Résultat du calcul
 */
unsigned int Phont::pmod(const unsigned int pad, const unsigned int tot) {
	return (pad - (tot % pad)) % pad;
}

#endif // _PHONT_H
