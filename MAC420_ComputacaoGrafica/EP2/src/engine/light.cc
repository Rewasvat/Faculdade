#include <engine/light.h>
#include <engine/vector3D.h>
#include <engine/color.h>
#include <GL/glut.h>
#include <cmath>

#define LIGHT_ID(id) GL_LIGHT##id

#define PI 3.14159265358979323846

namespace engine {

Light::Light(unsigned id, LightType type) : light_id_(id), active_(false), type_(type) {
/*
guardar um vetor de indices de luzes -> bool, pra saber quais luzes estao disponiveis
escolher um
suportar estado em que nao tem luz disponivel
*/
}

Light::~Light() { 
    Deactivate(); 
}

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
    
    glLightfv(light_id_, GL_AMBIENT, ambient_.val);
    glLightfv(light_id_, GL_DIFFUSE, diffuse_.val);
    glLightfv(light_id_, GL_SPECULAR, specular_.val);

    glLightf(light_id_, GL_CONSTANT_ATTENUATION, atte_linear_);
    glLightf(light_id_, GL_LINEAR_ATTENUATION, atte_linear_);
    glLightf(light_id_, GL_QUADRATIC_ATTENUATION, atte_quadratic_);
    

    double x, y, z, w;
    x=y=z=w=0.0;
    if (type_ == DIRECTIONAL) {
        x = direction_.x; y = direction_.y; z = direction_.z; w = 0.0;
    }
    else if (type_ == POINT) {
        x = position_.x; y = position_.y; z = position_.z; w = 1.0;
        glTranslated(position_.x, position_.y, position_.z);
    }
    else if (type_ == SPOTLIGHT) {
        x = position_.x; y = position_.y; z = position_.z; w = 1.0;
        glLightf(light_id_, GL_SPOT_CUTOFF, spot_angle_);
        glLightf(light_id_, GL_SPOT_EXPONENT, spot_exponent_);
        float dir[] = {static_cast<float>(direction_.x), static_cast<float>(direction_.y), static_cast<float>(direction_.z)};
        glLightfv(light_id_, GL_SPOT_DIRECTION, dir);
        //glTranslated(position_.x, position_.y, position_.z);
		//rotateToDirection();
    }
    float pos[] = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)};
    glLightfv(light_id_, GL_POSITION, pos);
   
    glEndList();
}

void Light::rotateToDirection() {
	Vector3D z_axis (0.0, 0.0, 1.0);
    double angle = acos( direction_ * z_axis );
    if ( fabs(angle) > 0.0 ) {
        Vector3D rot_axis = direction_.CrossProduct(z_axis);
        rot_axis.Normalize();
        rot_axis.Scale(-1.0);
        double angle_deg = (180.0 * angle) / PI;
        glRotated(angle_deg, rot_axis.x, rot_axis.y, rot_axis.z);
    }
}

}
