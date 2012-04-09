#include "scene.h"
#include <engine/object.h>
#include <engine/engine.h>

namespace engine {

Scene::Scene() : is_visible_(true), finished_(false) {
}

Scene::~Scene() {
}

void Scene::Update(double dt) {
    if (is_visible_) {
        ObjectList::iterator it;
        for (it = objects_.begin(); it != objects_.end(); ++it) {
            Object* obj = (*it);
            obj->Update(dt);
        }
    }
}

void Scene::Render() {
    if (is_visible_) {
        ObjectList::iterator it;
        for (it = objects_.begin(); it != objects_.end(); ++it) {
            Object* obj = (*it);
            obj->Render();
        }
    }
}

}
