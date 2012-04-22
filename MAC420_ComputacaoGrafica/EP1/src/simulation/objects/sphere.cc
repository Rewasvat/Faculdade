#include <simulation/objects/sphere.h>
#include <simulation/vectorfield.h>
#include <simulation/loaderutil.h>

namespace simulation {
namespace objects {

Sphere::Sphere(engine::Vector3D& pos) {
	position_ = pos;
	field_ = LoaderUtil::reference()->GetVectorField();
    handles_mouse_ = false;
	radius_ = LoaderUtil::reference()->GetSphereRadiusFactor() * (field_->GetMinimumDimensionStep() / 2.0);
    active_ = true;

	color_[0] = 0.0;
	color_[1] = 0.0;
	color_[2] = 0.0;
	quadric_ = gluNewQuadric();
	
	buildRenderList();
}

Sphere::~Sphere() {
    gluDeleteQuadric(quadric_);
}

void Sphere::Update(double dt) {
    if (active_) {
        engine::Vector3D delta_pos = field_->GetVectorAtPos(position_);
        delta_pos.Normalize();
        delta_pos.Scale(dt);

        engine::Vector3D new_pos = position_ + delta_pos;
        
        if ( field_->IsInField(new_pos) )
            position_ = new_pos;
    }
}

void Sphere::Render() {
	if (active_) {
		glTranslated(position_.x, position_.y, position_.z);
		glCallList(render_list_);
	}
}

void Sphere::KeyboardHandler(unsigned char key, int x, int y) {
    if (key == 's')
        active_ = !active_;
}

void Sphere::buildRenderList() {
    render_list_ = glGenLists(1);
    
    glNewList(render_list_, GL_COMPILE);
    
    gluQuadricDrawStyle(quadric_, GLU_FILL);
	gluQuadricNormals(quadric_, GLU_SMOOTH);
	
	glColor3d(color_[0], color_[1], color_[2]);

	int resolution = LoaderUtil::reference()->GetSphereResolution();
	gluSphere(quadric_, radius_, resolution, resolution);
	
    glEndList();
}

}
}
