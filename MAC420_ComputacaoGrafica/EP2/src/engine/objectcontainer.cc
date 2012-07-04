/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: objectcontainer.cc                                 **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#include <engine/objectcontainer.h>
#include <engine/object.h>
#include <GL/glut.h>

namespace engine {

ObjectContainer::ObjectContainer() {
}

ObjectContainer::~ObjectContainer() {
	ChildList to_delete;
	for(ChildList::iterator it = childs_.begin(); it != childs_.end(); ++it)
		to_delete.push_front(*it);

	for(ChildList::iterator it = to_delete.begin(); it != to_delete.end(); ++it) {
		delete (*it);
		childs_.remove(*it);
	}

	childs_.clear();
}

void ObjectContainer::UpdateChilds(double dt) {
	ChildList::iterator it;
	for (it = childs_.begin(); it != childs_.end(); ++it) {
		Object* obj = (*it);
		obj->Update(dt);
	}
}

void ObjectContainer::RenderChilds() {
	ChildList::iterator it;
    for (it = childs_.begin(); it != childs_.end(); ++it) {
        Object* obj = (*it);
        
        glPushMatrix(); /*glPushMatrix copies the current matrix (top of the matrix stack), and places the copy at the top*/
        obj->Render();
        glPopMatrix(); /*glPopMatrix removes the top matrix of the stack*/
    }
}

void ObjectContainer::RenderShadowChilds(Light* light) {
	ChildList::iterator it;
    for (it = childs_.begin(); it != childs_.end(); ++it) {
        Object* obj = (*it);
        
		if (obj->casts_shadow()) {
			glPushMatrix(); /*glPushMatrix copies the current matrix (top of the matrix stack), and places the copy at the top*/
			obj->RenderShadow(light);
			glPopMatrix(); /*glPopMatrix removes the top matrix of the stack*/
		}
    }
}

}
