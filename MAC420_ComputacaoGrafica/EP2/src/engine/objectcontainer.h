/*******************************************************************/
/**   MAC 420 - Introdu��o � Computa��o Gr�fica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exerc�cio-Programa                                  **/
/**   Arquivo: objectcontainer.h                                  **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/

#ifndef OBJECTCONTAINER_H_
#define OBJECTCONTAINER_H_

#include <list>
#include <engine/vector3D.h>

namespace engine {

class Object;
class Light;

typedef std::list<Object*> ChildList;

class ObjectContainer {
public:
	ObjectContainer();
	virtual ~ObjectContainer();

	void UpdateChilds(double dt);
	void RenderChilds();
	void RenderShadowChilds(Light* light);

	/* If needed, this should return the Position of this entity in the 3D space,
	   as given to the childs.*/
	virtual Vector3D parent_position() { return Vector3D(); }

protected:
	ChildList childs_;

	void AddChild(Object* obj) { childs_.push_back(obj); }
 	void RemoveChild(Object* obj) { childs_.remove(obj); }

	friend class Object;
};

}
#endif
