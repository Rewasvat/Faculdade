/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: skybox.h                                           **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/

#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <engine/object.h>

namespace mundoime {
namespace objects {

class Sun;

class Skybox : public engine::Object {
public:
    Skybox(const char* day_base_tex_name, double size);
    ~Skybox();

	virtual void Update(double dt);
	virtual void Render();

	void set_related_sun(Sun* sun) { related_sun_ = sun; }

protected:
	double size_;
	unsigned top_tex_;
	unsigned bottom_tex_;
	unsigned right_tex_;
	unsigned left_tex_;
	unsigned forward_tex_;
	unsigned back_tex_;
	Sun* related_sun_;
};

}
}
#endif

