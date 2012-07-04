/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: scene.h                                            **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#ifndef SCENE_H_
#define SCENE_H_

#include <list>
#include <engine/objectcontainer.h>

namespace engine {

class Object;
class Light;

class Scene : public ObjectContainer {
public:
	Scene() : is_visible_(true), finished_(false) {}
	virtual ~Scene() {}

    virtual void Start() {}
	virtual void Update(double dt) { if (is_visible_)	UpdateChilds(dt); }
	virtual void Render() { if (is_visible_)	RenderChilds(); }
	virtual void RenderShadows(Light* light) { if (is_visible_)	RenderShadowChilds(light); }
	virtual void End() {};
	virtual void WindowResize(int w, int h) { }
	
	void Finish() { End(); finished_ = true; }

	bool is_visible() { return is_visible_; }
	void set_visible(bool visible) { is_visible_ = visible; }
	bool finished() { return finished_; }

protected:
	bool is_visible_;
	bool finished_;
};

}
#endif
