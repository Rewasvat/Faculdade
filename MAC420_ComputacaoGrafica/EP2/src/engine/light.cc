#include <engine/light.h>
#include <engine/vector3D.h>
#include <engine/color.h>
#include <engine/engine.h>
#include <GL/glut.h>
#include <cmath>

#define LIGHT_ID(id) GL_LIGHT##id

#define PI 3.14159265358979323846

namespace engine {

Light::Light(unsigned id, LightType type) : light_id_(id), active_(false), casts_shadow_(true), type_(type) {
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
    if (active_) {
		applyAllSettings();
	}
}

void Light::Activate() {
    active_ = true;
	applyAllSettings();
    glEnable(light_id_);
	Engine::reference()->AddLight(this);
}

void Light::Deactivate() {
    active_ = false;
    glDisable(light_id_);
	Engine::reference()->RemoveLight(this);
}

void Light::SetType(LightType type) { 
	type_ = type;
}

void Light::SetAmbientColor(Color ambient) { 
	ambient_ = ambient;
}

void Light::SetDiffuseColor(Color diffuse) { 
	diffuse_ = diffuse;
}

void Light::SetSpecularColor(Color specular) { 
	specular_ = specular;
}

void Light::SetColors(Color ambient, Color diffuse, Color specular) { 
	this->SetAmbientColor(ambient);
	this->SetDiffuseColor(diffuse);
	this->SetSpecularColor(specular);
}

/*angle in degrees, in range [0, 90], or special value 180.0 (no spotlight)*/
void Light::SetSpotlightParameters(float angle, float exponent) { 
	spot_angle_ = angle; 
	spot_exponent_ = exponent;
}

void Light::SetAttenuationParameters(float constant, float linear, float quadratic) {
    atte_constant_ = constant;
    atte_linear_ = linear;
    atte_quadratic_ = quadratic;
}

void Light::set_direction(Vector3D& dir) { 
	direction_ = dir;
	direction_.Normalize();
}

void Light::set_position(Vector3D& pos) {
	Object::set_position(pos);
}

void Light::applyAmbientColor() { 
	glLightfv(light_id_, GL_AMBIENT, ambient_.val);
}

void Light::applyDiffuseColor() { 
	glLightfv(light_id_, GL_DIFFUSE, diffuse_.val);
}

void Light::applySpecularColor() { 
	glLightfv(light_id_, GL_SPECULAR, specular_.val);
}

void Light::applySpotlightParameters() {
	if (type_ == SPOTLIGHT) {
		glLightf(light_id_, GL_SPOT_CUTOFF, spot_angle_);
		glLightf(light_id_, GL_SPOT_EXPONENT, spot_exponent_);
	}
	else {
		glLightf(light_id_, GL_SPOT_CUTOFF, 180.0);
	}
}

void Light::applyAttenuation() {
    glLightf(light_id_, GL_CONSTANT_ATTENUATION, atte_linear_);
    glLightf(light_id_, GL_LINEAR_ATTENUATION, atte_linear_);
    glLightf(light_id_, GL_QUADRATIC_ATTENUATION, atte_quadratic_);
}

void Light::applyDirection() { 
    if (type_ == DIRECTIONAL) {
        float gldir[] = {static_cast<float>(direction_.x), static_cast<float>(direction_.y), static_cast<float>(direction_.z), 0.0};
        glLightfv(light_id_, GL_POSITION, gldir);
    }
    else if (type_ == SPOTLIGHT) {
        float gldir[] = {static_cast<float>(direction_.x), static_cast<float>(direction_.y), static_cast<float>(direction_.z)};
        glLightfv(light_id_, GL_SPOT_DIRECTION, gldir);
    }
}

void Light::applyPosition() {
    if (type_ == POINT) {
        float glpos[] = {static_cast<float>(position_.x), static_cast<float>(position_.y), static_cast<float>(position_.z), static_cast<float>(1.0)};
        glLightfv(light_id_, GL_POSITION, glpos);
    }
    else if (type_ == SPOTLIGHT) {
        float glpos[] = {static_cast<float>(position_.x), static_cast<float>(position_.y), static_cast<float>(position_.z), static_cast<float>(1.0)};
        glLightfv(light_id_, GL_POSITION, glpos);
    }
}

void Light::applyAllSettings() {
	applyAmbientColor();
	applyDiffuseColor();
	applySpecularColor();
	applySpotlightParameters();
	applyAttenuation();
	applyDirection();
	applyPosition();
}

void Light::SetGlobalAmbientLight(Color color) {
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color.val);
}

void Light::SetUseLocalViewport(bool use) {
	int value = (use) ? GL_TRUE : GL_FALSE;
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, value);
}

void Light::SetUseTwoSidedLighting(bool use) {
	int value = (use) ? GL_TRUE : GL_FALSE;
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, value);
}

}
