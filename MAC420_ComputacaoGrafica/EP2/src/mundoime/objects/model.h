
#ifndef MODEL_H_
#define MODEL_H_

#include <engine/object.h>
#include <engine/objload/objload.h>

namespace mundoime {
namespace objects {

/*Model:  Object, VertexBuffer, BulletCollisionObject|RigidBody
*/
class btCollisionShape;
class btRigidBody;

class Model : public engine::Object {
public:
    Model(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape = NULL);
    ~Model();

	void Update(double dt);
	void Render();
	
	engine::Vector3D direction() { return direction_; }
	void set_direction(engine::Vector3D& dir) { direction_ = dir; }
	
	btRigidBody* body() { return body_; }

protected:
	engine::Vector3D direction_;
	Obj::VertexBuffer mesh_;
	
	double mass_;
	btCollisionShape* shape_;
	btRigidBody* body_;
};

}
}
#endif
