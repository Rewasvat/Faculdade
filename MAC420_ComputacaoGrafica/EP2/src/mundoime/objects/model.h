
#ifndef MODEL_H_
#define MODEL_H_

#include <engine/object.h>
#include <engine/objload/objload.h>

class btCollisionShape;
class btRigidBody;
class btTriangleIndexVertexArray;

namespace engine {
class Light; 
}

namespace mundoime {
namespace objects {

struct Prism {
	engine::Vector3D p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

	void render();
};

class Model : public engine::Object {
public:
    Model(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape = 0);
    ~Model();

	virtual void Update(double dt);
	virtual void Render();
	void RenderShadowXXX(engine::Light* light);
	void RenderShadow(engine::Light* light);
	void RenderVisibleFaces(engine::Light* light, bool visible);
	
	engine::Vector3D direction() { return direction_; }
	void set_direction(engine::Vector3D& dir) { direction_ = dir; }

	void set_position(engine::Vector3D& pos);
	
	void set_mesh_visible(bool visible) { mesh_visible_ = visible; }
	bool mesh_visible() { return mesh_visible_; }

	btRigidBody* body() { return body_; }

protected:
	engine::Vector3D direction_;
    engine::Vector3D rot_axis_;
    double rot_angle_;
	Obj::VertexBuffer mesh_;
	bool mesh_visible_;
	
	double mass_;
	btCollisionShape* shape_;
    btTriangleIndexVertexArray* btTIVA_;
	btRigidBody* body_;

	std::vector<engine::Vector3D> frontFaceVertices;
	std::vector<engine::Vector3D> backFaceVertices;
	std::vector<Prism> prism;
};

}
}
#endif
