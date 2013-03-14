/*
Phont - g�n�rateur de caract�res pour OpenGL
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
#include "Phont.h"

#include <string>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <cstring>
#define _WINSOCKAPI_
#include <windows.h>
#include "glew.h"
#include <GL/glu.h>

using std::string;

/**
 * Instancie un Phont sans rien charger.
 * @author Philippe Proulx
 */
Phont::Phont() : loaded_(false), cl_base_(0), count_(0), chars_(NULL), scale_(1) {
}

/**
 * D�truit un Phont et nettoie correctement.
 * @author Philippe Proulx
 */
Phont::~Phont() {
	unload();
}


void Phont::scaleText(float facteur)
{
	scale_=facteur;
}

/**
 * Accesseur du chargement.
 * @author Philippe Proulx
 *
 * @return	Vrai si le Phont est charg�
 */
bool Phont::isLoaded(void) const {
	return loaded_;
}

/**
 * Accesseur de la base des call lists de caract�res.
 * @author Philippe Proulx
 *
 * @return	Base des call lists (si chargement valide)
 */
GLuint Phont::getCLBase(void) const {
	return cl_base_;
}

/**
 * Accesseur des informations g�n�rales BFG.
 * @author Philippe Proulx
 *
 * @return	Informations g�n�rales BFG
 */
struct Phont::phont_bfg_info Phont::getBFGInfo(void) const {
	return info_;
}

/**
 * Accesseur des informations communes BFG.
 * @author Philippe Proulx
 *
 * @return	Informations communes BFG
 */
struct Phont::phont_bfg_common Phont::getBFGCommon(void) const {
	return common_;
}

/**
 * Accesseur du nombre de caract�res valides.
 * @author Philippe Proulx
 *
 * @return	Nombre de caract�res valides.
 */
unsigned int Phont::getCharCount(void) const {
	return count_;
}

/**
 * Calcule la taille (en pixels) d'une chaine selon les informations de chaque
 * caract�re. Les caract�res invalides assument une taille nulle.
 * @author Philippe Proulx
 *
 * @param text		Texte dont la longueur doit �tre v�rifi�e
 * @return		Taille en pixels de la chaine (incluant tout espace additionnel)
 */
unsigned int Phont::getTextLength(const char* text) const {
	if (!loaded_) {
		return 0;
	}
	assert(chars_ != NULL);
	assert(text != NULL);
	size_t text_sz = strlen(text);
	int acc = 0;
	for (size_t i = 0; i < text_sz; ++i) {
		char ch = text[i];
		if (chars_[ch].active) {
			acc += (chars_[ch].x_adv + chars_[ch].x_off);
		}
	}
	
	return (unsigned int) acc;
}

/**
 * Imprime un texte (format 'printf') avec le Phont actuellement charg� sur le viewport.
 * @author Philippe Proulx
 *
 * @param vp_x		Coordonn�es viewport en x du premier caract�re
 * @param vp_y		Coordonn�es viewport en y du bas du premier caract�re
 * @param keep_gl	Conserver les matrices OpenGL pr�c�dentes
 * @param format	Format 'printf'
 * @param ...		Arguments 'printf'
 * @return		Succ�s
 */
bool Phont::print(GLfloat vp_x, GLfloat vp_y, bool keep_gl, const char* format, ...) const {
	if (!loaded_) {
		return false;
	}
	
	char buf [4096]; // Tampon de 4 kiB
	va_list	ap; // Liste des arguments variants

	assert(format != NULL);

	// Traitement de la chaine style 'printf'
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);
	
	// Viewport et matrices � sauvegarder s'il y a lieu
	GLint viewport [4];
	GLfloat modelview [16];
	GLfloat projection [16];
	
	// �tats de la lumi�re et de la profondeur
	GLboolean lighting_state, depth_test_state;

	// Dimensions du viewport actuel
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Conserver les matrices pr�c�dentes s'il y a lieu
	if (keep_gl) {
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
		glGetFloatv(GL_PROJECTION_MATRIX, projection);
	}

	// Matrice de projection orthogonale 2D de m�me dimensions virtuelles que le viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) viewport[2] - 1.0, 0, (GLfloat) viewport[3] - 1.0);

	// Matrice de mod�le/vue identit�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// D�sactiver le test de profondeur et l'�clairage
	glGetBooleanv(GL_LIGHTING, &lighting_state);
	glGetBooleanv(GL_DEPTH_TEST, &depth_test_state);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	
	// Imprimer la chaine
	glPushMatrix();
		glTranslatef(0.0, (GLfloat) common_.line_height, 0.0);
		glTranslatef((GLfloat) vp_x * (viewport[2] - 1.0), (GLfloat) vp_y * (viewport[3] - 1.0), 0.0);
		glScalef(scale_,scale_,1);
		glListBase(cl_base_);
		glCallLists((GLsizei)strlen(buf), GL_UNSIGNED_BYTE, buf);
	glPopMatrix();
	
	// R�activer l'�clairage et le test de profondeur (s'il y a lieu)
	if (lighting_state == GL_TRUE) {
		glEnable(GL_LIGHTING);
	}
	if (depth_test_state == GL_TRUE) {
		glEnable(GL_DEPTH_TEST);
	}
	
	// Recharger les matrices pr�c�dentes s'il y a lieu
	if (keep_gl) {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(projection);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(modelview);
	}
	
	return true;
}

/**
 * Accesseur des informations d'un caract�re BFG.
 * @author Philippe Proulx
 *
 * @return	Information d'un caract�re BFG
 */
struct Phont::phont_bfg_char Phont::getCharInfo(const unsigned int index) const {
	if (loaded_) {
		if (index >= 0 && index < 256) {
			return chars_[index];
		}
	}
	struct phont_bfg_char ch;
	ch.active = false;
	return ch;
}

/**
 * Charge un Phont. L'image et la description doivent se trouver dans
 * le r�pertoire 'pref' et poss�der respectivement le m�me nom 'name'
 * avec les extensions ".bmp" et ".fnt".
 * @author Philippe Proulx
 *
 * @param pref		Pr�fixe du chemin relatif (doit inclure '\')
 * @param name		Nom sans extension
 * @return		Succ�s
 */
bool Phont::load(const char* pref, const char* name) {
	string desc = string(pref) + string(name) + string(".fnt");
	string image = string(pref) + string(name) + string(".bmp");
	DWORD f_attr_desc, f_attr_image;
	
	// V�rifier si le Phont est d�j� charg� (si oui, le d�charger)
	if (loaded_) {
		unload();
	}
	
	// V�rifier si l'image et la description existent
	f_attr_desc = GetFileAttributesA(desc.c_str());
	f_attr_image = GetFileAttributesA(image.c_str());
	if (f_attr_image == 0xffffffff || f_attr_desc == 0xffffffff) {
		return false;
	}
	
	// Charger la description
	if (!this->load_bfg(desc.c_str())) {
		return false;
	}

	// Charger l'image	
	if (!this->create_cl(image.c_str())) {
		free(chars_);
		chars_ = NULL;
		return false;
	}

	loaded_ = true;
	
	return true;
}

/**
 * D�charge un Phont et supprime ses call lists.
 * @author Philippe Proulx
 *
 * @return		Nouvelle valeur de chargement
 */
bool Phont::unload(void) {
	if (loaded_) {
		free(chars_);
		glDeleteLists(cl_base_, 256);
		count_ = 0;
		cl_base_ = 0;
		chars_ = NULL;
		loaded_ = false;
	}

	return loaded_;
}

/**
 * Charge une description de police BFG (Bitmap Font Generator de AngelCode).
 * @author Philippe Proulx
 *
 * @param filename	Chemin vers le fichier text ".fnt" � charger
 * @return		Succ�s
 */
bool Phont::load_bfg(const char* filename) {
	int ret;
	
	// Charger le fichier de description BFG:
	FILE* fh = fopen(filename, "r");
	assert(fh != NULL);

	// Lire les informations g�n�rales
	ret = fscanf(fh, "info face=\"%[^\"]\" size=%i bold=%i italic=%i "
		"charset=\"%[^\"]\" unicode=%i stretchH=%i smooth=%i aa=%i "
		"padding=%i,%i,%i,%i spacing=%i,%i outline=%i\n",
		&info_.face, &info_.size, &info_.bold,
		&info_.italic, &info_.charset, &info_.unicode,
		&info_.stretch_h, &info_.smooth, &info_.aa,
		&info_.padding[0], &info_.padding[1],
		&info_.padding[2], &info_.padding[3],
		&info_.spacing[0], &info_.spacing[1],
		&info_.outline);
	if (ret == EOF) {
		return false;
	}


	// Lire les informations communes
	ret = fscanf(fh, "common lineHeight=%i base=%i scaleW=%i scaleH=%i pages=%i "
		"packed=%i alphaChnl=%i redChnl=%i greenChnl=%i blueChnl=%i\n",
		&common_.line_height, &common_.base,
		&common_.scale_w, &common_.scale_h,
		&common_.pages, &common_.packed,
		&common_.alpha_chan, &common_.red_chan,
		&common_.green_chan, &common_.blue_chan);
	if (ret == EOF) {
		return false;
	}

	// Sauter les informations de page
	ret = fscanf(fh, "page id=%*s file=%*s\n");
	if (ret == EOF) {
		return false;
	}
	
	// Nombre de caract�res
	ret = fscanf(fh, "chars count=%u\n", &count_);
	if (ret == EOF) {
		return false;
	}

	// Lire les informations de chaque caract�re
	chars_ = (struct phont_bfg_char*)
		malloc(sizeof(struct phont_bfg_char) * 256);
	assert(chars_ != NULL);
	
	unsigned int cc;
	int id, x, y, w, h, x_off, y_off, x_adv, page, chan;
	for (cc = 0; cc < 256; ++cc) {
		chars_[cc].active = false;
	}
	for (cc = 0; cc < count_; ++cc) {
		ret = fscanf(fh, "char id=%i x=%i y=%i width=%i height=%i "
			"xoffset=%i yoffset=%i xadvance=%i page=%i chnl=%i\n",
			&id, &x, &y, &w, &h, &x_off, &y_off,
			&x_adv, &page, &chan);
		if (ret == EOF) {
			free(chars_);
			return false;
		}
		chars_[id].id = id;
		chars_[id].active = true;
		chars_[id].x = x;
		chars_[id].y = y;
		chars_[id].w = w;
		chars_[id].h = h;
		chars_[id].x_off = x_off;
		chars_[id].y_off = y_off;
		chars_[id].x_adv = x_adv;
		chars_[id].page = page;
		chars_[id].chan = chan;		
	}
	
	// On pourrait ici lire les autres informations (sur le kerning)

	// Fermer le fichier
	fclose(fh);
	
	return true;
}

/**
 * Cr�e les 256 call lists pour chaque caract�re.
 * @author Philippe Proulx
 *
 * @param filename	Chemin relatif vers le fichier bitmap Windows 8-bit contenant les caract�res
 * @return		Succ�s
 */
bool Phont::create_cl(const char* filename) {
	assert(filename != NULL);

	// Tampon de pixels qui sera rempli
	unsigned char* pixels;
	
	// Structure contenant les �ventuelles informations bitmap
	struct bmp_dib_v3_header dibhead;

	// Lire les pixels
	if (!read_8bit_bmp(filename, &pixels, &dibhead)) {
		return false;
	}

	// Obtenir 256 call lists
	cl_base_ = glGenLists(256);
	if (cl_base_ == 0) {
		free(pixels);
		return false;
	}

	// Cr�er chaque caract�re valide
	for (unsigned int i = 0; i < 256; ++i) {
		// Si c'est un caract�re invalide, cr�er une liste vide
		if (!chars_[i].active) {
			glNewList(cl_base_ + i, GL_COMPILE);
			glEndList();
			continue;
		}

		// D�couper les pixels du caract�re
		unsigned int x = (unsigned int) chars_[i].x,
			y = (unsigned int) chars_[i].y,
			w = (unsigned int) chars_[i].w,
			h = (unsigned int) chars_[i].h,
			x_adv = (unsigned int) chars_[i].x_adv;
		int x_off = chars_[i].x_off,
			y_off = chars_[i].y_off;

		// Allouer la m�moire pour les pixels du caract�re � d�couper et d�couper
		unsigned char* char_pixels = (unsigned char*)
			malloc(sizeof(unsigned char) * w * h);
		assert(char_pixels != NULL);
		cut_in_buf(pixels, &dibhead, char_pixels, x, dib_pos(y, dibhead.height, h), w, h);

		// Transformer les pixels du caract�re en image bitmap
		unsigned char* bitmap;
		unsigned int bmp_sz = pixmap_to_bitmap(char_pixels, w, h, &bitmap,
			4, 0x00, 0x00);

		// Cr�er la call list
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Appel� chaque fois par pr�caution
		glNewList(cl_base_ + i, GL_COMPILE);
			glPushMatrix();
				glTranslatef((GLfloat) x_off, (GLfloat) -y_off, 0.0);
				glRasterPos2i(0, 0);
				glBitmap((GLsizei) w, (GLsizei) h, 0.0f, (GLfloat) h,
					0.0f, 0.0f, bitmap);
			glPopMatrix();
			glTranslatef((GLfloat) x_adv, 0.0, 0.0);
		glEndList();

		// D�sallouer les pixels et le bitmap
		free(char_pixels);
		free(bitmap);
	}

	free(pixels);
	return true;
}

/**
 * Lit une image bitmap Windows 8-bit et met le contenu dans un tampon (le
 * tampon est allou� par la m�thode ; l'appelant doit la lib�rer).
 * @author Philippe Proulx
 *
 * @param filename	Nom du fichier bitmap Windows 8-bit � charger
 * @param pixmap	Tampon � allouer et remplir
 * @param dibhead	En-t�te bitmap � remplir
 * @return		Succ�s
 */
bool Phont::read_8bit_bmp(const char* filename, unsigned char** pixmap,
struct bmp_dib_v3_header* dibhead) {
	// Structures d'en-t�te
	struct bmp_magic rmagic;
	struct bmp_header rheader;
	
	// Ouvrir le fichier
	FILE* fh = fopen(filename, "rb");
	assert(fh != NULL);
	
	// Lire les structures
	fread(&rmagic, sizeof(rmagic), 1, fh);
	fread(&rheader, sizeof(rheader), 1, fh);
	fread(dibhead, sizeof(*dibhead), 1, fh);
	
	// Petite v�r�fication
	if (dibhead->bitspp != 8) {
		fclose(fh);
		return false;
	}
	
	// Allouer la m�moire pour les pixels
	*pixmap = (unsigned char*) malloc(sizeof(unsigned char) *
		dibhead->width * dibhead->height);
	
	// Sauter la description de palette
	fseek(fh, 256 * 4, SEEK_CUR);
	
	// Lire les pixels
	unsigned int col, row;
	for (row = 0; row < dibhead->height; ++row) {
		for (col = 0; col < dibhead->width; ++col) {
			fread(*pixmap + pixoff(col, row, dibhead->width),
				1, 1, fh);
		}
		fseek(fh, pmod(4, dibhead->width), SEEK_CUR);
	}
	
	// Fermer le fichier
	fclose(fh);
	
	return true;
}

/**
 * Coupe un morceau d'image dans un tampon.
 * @author Philippe Proulx
 *
 * @param pixmap	Tampon de pixels
 * @param dibhead	En-t�te bitmap (pour y lire la taille, entre autres)
 * @param newbuf	Tampon � remplir (allou� par l'appelant : (largeur * hauteur) octets)
 * @param x_off		Offset en x o� couper
 * @param y_off		Offset en y o� couper
 * @param wi		Largeur � couper
 * @param he		Hauteur � couper
 * @return		Succ�s
 */
bool Phont::cut_in_buf(const unsigned char* pixmap, const struct bmp_dib_v3_header* dibhead,
unsigned char* newbuf, const unsigned int x_off, const unsigned int y_off,
const unsigned int wi, const unsigned int he) {
	unsigned nb_off = 0;
	unsigned int row, col;
	const unsigned int pmap_w = dibhead->width;
	
	for (row = y_off; row < y_off + he; ++row) {
		for (col = x_off; col < x_off + wi; ++col) {
			newbuf[nb_off] = pixmap[pixoff(col, row, pmap_w)];
			++nb_off;
		}
	}
	
	return true;
}

/**
 * Cr�e un tampon bitmap � partir d'un tampon de pixels.
 * @author Philippe Proulx
 *
 * @param pixmap	Tampon de pixels
 * @param width		Largeur du tampon de pixels
 * @param height	Hauteur du tampon de pixels
 * @param bitmap	Tampon bitmap (allou� par la m�thode, � d�sallouer par l'appelant)
 * @param pad		Padding (en octets) pour chaque rang�e
 * @param p_on		Octet qui correspond � un pixel actif dans le tampon de pixels
 * @param pad_ch	Octet de padding
 * @return		Taille du tampon bitmap
 */
unsigned int Phont::pixmap_to_bitmap(const unsigned char* pixmap,
const unsigned int width, const unsigned int height, unsigned char** bitmap,
const unsigned int pad, const unsigned char p_on, const unsigned char pad_ch) {
	unsigned int tmp = 0x00;
	unsigned int row, col;
	unsigned int off = 0;
	
	// Calculer la taille du buffer bitmap
	unsigned int w_buf_sz = (unsigned int) ceil((float) width / 8.0);
	unsigned int r_pad = pmod(pad, w_buf_sz);
	w_buf_sz += r_pad;
	const unsigned int bitmap_sz = w_buf_sz * height;
	*bitmap = (unsigned char*) malloc(sizeof(unsigned char) * bitmap_sz);
	
	// Transf�rer pixels vers bits
	unsigned int pos;
	for (row = 0; row < height; ++row) {
		for (col = 0; col < width; ++col) {
			// Si le tampon est plein, le flusher
			if (col % 8 == 0 && col != 0) {
				(*bitmap)[off] = tmp;
				tmp = 0;
				++off;
			}
			
			// Ajout du prochain pixel au tampon
			pos = pixoff(col, row, width);
			tmp <<= 1;
			if (pixmap[pos] == p_on) {
				tmp |= 1;
			}
		}
		
		// Dernier octet de la rang�e
		tmp <<= pmod(8, width);
		(*bitmap)[off] = tmp;
		++off;
		
		// Padding impos�
		for (col = 0; col < r_pad; ++col) {
			(*bitmap)[off] = pad_ch;
			++off;
		}
	}
	
	assert(bitmap_sz == off);
	
	return bitmap_sz;
}