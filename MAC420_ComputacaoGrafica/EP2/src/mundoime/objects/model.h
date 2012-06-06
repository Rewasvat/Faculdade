
#ifndef MODEL_H_
#define MODEL_H_

#include <engine/object.h>
#include <engine/objload/objload.h>

class btCollisionShape;
class btRigidBody;
class btTriangleIndexVertexArray;

namespace mundoime {
namespace objects {

class Model : public engine::Object {
public:
    static const double STATIC_MASS = 0.0;

    Model(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape = 0);
    ~Model();

	void Update(double dt);
	void Render();
	
	engine::Vector3D direction() { return direction_; }
	void set_direction(engine::Vector3D& dir) { direction_ = dir; }
	
	btRigidBody* body() { return body_; }

protected:
	engine::Vector3D direction_;
    engine::Vector3D rot_axis_;
    double rot_angle_;
	Obj::VertexBuffer mesh_;
	
	double mass_;
	btCollisionShape* shape_;
    btTriangleIndexVertexArray* btTIVA_;
	btRigidBody* body_;
};

}
}
#endif
