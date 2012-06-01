
#ifndef LIGHT_H_
#define LIGHT_H_

#include <engine/vector3D.h>
#include <engine/object.h>
#include <engine/color.h>
#include <GL/glut.h>


namespace engine {

class Light : Object {
public:
    enum LightType {
        DIRECTIONAL,    POINT,     SPOTLIGHT
    };

    Light(LightType type);
    virtual ~Light();

    virtual void Update(double dt) {}
	virtual void Render();

    void Activate();
    void Deactivate();

    void SetType(LightType type) { type_ = type; }
    LightType type() { return type_; }

    /*angle in degrees, in range [0, 90], or special value 180.0 (no spotlight)*/
    void SetSpotlightParameters(double angle = 180.0, double exponent = 0.0) { spot_angle_ = angle; spot_exponent_ = exponent; }
    void SetAttenuationParameters(double constant = 1.0, double linear = 0.0, double quadratic = 0.0) {
        atte_constant_ = constant;
        atte_linear_ = linear;
        atte_quadratic_ = quadratic;
    }

    Vector3D direction() { return direction_; }
	void set_direction(Vector3D& dir) { direction_ = dir; }

protected:
	GLenum light_id_;
    GLuint gl_list_;
    bool active_;
    LightType type_;
    Vector3D direction_;
    Color ambient_, diffuse_, specular_;
    double spot_angle_, spot_exponent_;
    double atte_constant_, atte_linear_, atte_quadratic_;

    void buildGLlist();
};

}
#endif
