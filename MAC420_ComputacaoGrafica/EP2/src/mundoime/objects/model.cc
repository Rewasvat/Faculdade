#include <mundoime/objects/model.h>
#include <engine/objload/objload.h>
#include <math.h>
#include <GL/glut.h>

#define PI 3.14159265358979323846

namespace mundoime {
namespace objects {

Model::Model(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh) {
	position_ = pos;
	direction_ = direction;
	direction_.Normalize();
    mesh_ = mesh;
}

Model::~Model() {
}

void Model::Update(double dt) {
}

void Model::Render() {
    glTranslated(position_.x, position_.y, position_.z);
    
	engine::Vector3D z_axis (0.0, 0.0, 1.0);
    double angle = acos( direction_ * z_axis );
    if ( direction_.Length() > 0.0 && fabs(angle) > 0.0 ) {
        engine::Vector3D rot_axis = direction_.CrossProduct(z_axis);
        rot_axis.Normalize();
        rot_axis.Scale(-1.0);
        double angle_deg = (180.0 * angle) / PI;
        glRotated(angle_deg, rot_axis.x, rot_axis.y, rot_axis.z);
    }
    
    mesh_.gl();
}

}
}
