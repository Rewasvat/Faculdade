/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: light.h                                            **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/

#ifndef LIGHT_H_
#define LIGHT_H_

#include <engine/vector3D.h>
#include <engine/object.h>
#include <engine/color.h>
#include <GL/glut.h>


namespace engine {

class Light : public Object {
public:
    enum LightType {
        DIRECTIONAL,    POINT,     SPOTLIGHT
    };

    Light(unsigned id, LightType type);
    virtual ~Light();

    virtual void Update(double dt) {}
	virtual void Render();

    void Activate();
    void Deactivate();
    bool active() { return active_; }
	bool casts_shadow() { return casts_shadow_; }
	void set_casts_shadow(bool casts_shadow) { casts_shadow_ = casts_shadow; }

    void SetType(LightType type);
    LightType type() { return type_; }

    /*angle in degrees, in range [0, 90], or special value 180.0 (no spotlight)*/
    void SetAmbientColor(Color ambient);
    void SetDiffuseColor(Color diffuse);
    void SetSpecularColor(Color specular);
    void SetColors(Color ambient, Color diffuse, Color specular);
    void SetSpotlightParameters(float angle = 180.0, float exponent = 0.0);
    void SetAttenuationParameters(float constant = 1.0, float linear = 0.0, float quadratic = 0.0);

    Vector3D direction() { return direction_; }
	void set_direction(Vector3D& dir);

	virtual void set_position(Vector3D& pos);

    static void SetGlobalAmbientLight(Color color);
    static void SetUseLocalViewport(bool use);
    static void SetUseTwoSidedLighting(bool use);

protected:
	GLenum light_id_;
    bool active_;
	bool casts_shadow_;
    LightType type_;
    Vector3D direction_;
    Color ambient_, diffuse_, specular_;
    float spot_angle_, spot_exponent_;
    float atte_constant_, atte_linear_, atte_quadratic_;

	void applyAmbientColor();
	void applyDiffuseColor();
	void applySpecularColor();
	void applySpotlightParameters();
	void applyAttenuation();
	void applyDirection();
	void applyPosition();
	void applyAllSettings();
};

}
#endif
