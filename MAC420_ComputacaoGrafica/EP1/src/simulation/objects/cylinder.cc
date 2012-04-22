#include <simulation/objects/cylinder.h>
#include <GL/glut.h>
#include <math.h>
#include <simulation/loaderutil.h>
#include <simulation/vectorfield.h>

#include <cstdio>

#define PI 3.14159265358979323846

namespace simulation {
namespace objects {

Cylinder::Cylinder(engine::Vector3D& pos, engine::Vector3D& direction) {
	position_ = pos;

	base_radius_ = 0.1;
	top_radius_ = 0.0;  //YES, It is a cone.
	height_ = direction.Length();
	direction_ = direction;

	double cm;
	switch (LoaderUtil::reference()->GetVectorColorMode()) {
	case VCM_MAGNETUDE:
		cm = direction.Length() / LoaderUtil::reference()->GetVectorField()->GetMaximumVector().Length();
		color_[0] = cm;
		color_[1] = 1 - cm;
		color_[2] = 1 - cm;
		break;
	case VCM_DIRECTION:
		color_[0] = direction.x / height_;
		color_[1] = direction.y / height_;
		color_[2] = direction.z / height_;
		break;
	case VCM_RANDOM:
		color_[0] = (double)rand()/(double)RAND_MAX;
		color_[1] = (double)rand()/(double)RAND_MAX;
		color_[2] = (double)rand()/(double)RAND_MAX;
		break;
	}
	quadric_ = gluNewQuadric();
	
	buildRenderList();
}

Cylinder::~Cylinder() {
	gluDeleteQuadric(quadric_);
}

void Cylinder::Update(double dt) {
}

void Cylinder::Render() {
    glCallList(render_list_);
}

void Cylinder::buildRenderList() {
    render_list_ = glGenLists(1);
    
    glNewList(render_list_, GL_COMPILE);
    
    gluQuadricDrawStyle(quadric_, GLU_FILL);
	gluQuadricNormals(quadric_, GLU_SMOOTH);
	
	glColor3d(color_[0], color_[1], color_[2]);
	glTranslated(position_.x, position_.y, position_.z);

    engine::Vector3D dir = direction_;
    engine::Vector3D z_axis (0.0, 0.0, 1.0);
    dir.Normalize();
    
    
    double angle = acos( dir * z_axis );
    if ( fabs(angle) > 0.0 ) {
        engine::Vector3D rot_axis = dir.CrossProduct(z_axis);
        rot_axis.Normalize();
        rot_axis.Scale(-1.0);

        double angle_deg = (180.0 * angle) / PI;
        glRotated(angle_deg, rot_axis.x, rot_axis.y, rot_axis.z);
    }

	int resolution = LoaderUtil::reference()->GetCylinderResolution();
	gluCylinder(quadric_, base_radius_, top_radius_, height_, resolution, resolution/2);
	gluCylinder(quadric_, base_radius_, 0.0, 0.0, resolution, resolution/2); /*This creates the circle base polygons, thus closing the cone in a solid 3D model.*/
	
    glEndList();
}

}
}
