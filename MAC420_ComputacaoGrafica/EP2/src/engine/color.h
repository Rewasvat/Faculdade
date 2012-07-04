/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: color.h                                            **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/

#ifndef COLOR_H_
#define COLOR_H_

namespace engine {

class Color {
  public:

    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color(float _r, float _g, float _b, float _a = 1.0) : r(_r), g(_g), b(_b), a(_a) {}

    ~Color() { }

    union {
        struct { float r, g, b, a; };
        struct { float val[4]; };
    };

};

static const Color WHITE(1.0, 1.0, 1.0);
static const Color GRAY(0.5, 0.5, 0.5);
static const Color RED(1.0,0.0,0.0);
static const Color BLACK(0.0, 0.0, 0.0);

} 
#endif
