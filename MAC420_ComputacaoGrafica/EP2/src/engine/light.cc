#include <engine/light.h>
#include <engine/vector3D.h>
#include <engine/color.h>
#include <GL/glut.h>

#define LIGHT_ID(id) GL_LIGHT##id

namespace engine {

Light::Light(LightType type) : active_(false), type_(type) {
/*
guardar um vetor de indices de luzes -> bool, pra saber quais luzes estao disponiveis
escolher um
suportar estado em que nao tem luz disponivel
*/
}
~Light::Light() { Deactivate(); }

void Light::Render() {
    if (active_)
        glCallList(gl_list_);
}

void Light::Activate() {
    active_ = true;
    this->buildGLlist();
    glEnable(light_id_);
}

void Light::Deactivate() {
    active_ = false;
    glDisable(light_id_);
}

void Light::buildGLlist() {
    gl_list_ = glGenLists(1);
    glNewList(gl_list_, GL_COMPILE);
    
    glLightfv(light_id_, GL_AMBIENT, &ambient_.val);
    glLightfv(light_id_, GL_DIFFUSE, &ambient_.val);
    glLightfv(light_id_, GL_SPECULAR, &ambient_.val);

    glLightf(light_id_, GL_CONSTANT_ATTENUATION, atte_linear_);
    glLightf(light_id_, GL_LINEAR_ATTENUATION, atte_linear_);
    glLightf(light_id_, GL_QUADRATIC_ATTENUATION, atte_quadratic_);

    float x, y, z, w;
    if (type_ == LightType::DIRECTIONAL) {
        x = direction_.x; y = direction_.y; z = direction_.z; w = 0.0;
    }
    else if (type_ == LightType::POINT) {
        x = position_.x; y = position_.y; z = position_.z; w = 1.0;
    }
    else if (type_ == LightType::SPOTLIGHT) {
        x = position_.x; y = position_.y; z = position_.z; w = 1.0;
        glLightf(light_id_, GL_SPOT_CUTOFF, spot_angle_);
        glLightf(light_id_, GL_SPOT_EXPONENT, spot_exponent_);
        float dir[] = {direction_.x, direction_.y, direction_.z}
        glLightfv(light_id_, GL_SPOT_DIRECTION, dir);
    }
    float pos[] = {x, y, z, w}
    glLightfv(light_id_, GL_POSITION, pos);
   
    glEndList();
}

}
