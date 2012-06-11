#include <mundoime/objects/sun.h>
#include <GL/glut.h>
#include <cstdlib>
#include <string>

namespace mundoime {
namespace objects {

Sun::Sun() {

}

Sun::~Sun() {

}

void Sun::Update(double dt) {

}

void Sun::Render() {

    glColor4d(1.0, 1.0, 1.0, 1.0);
    
    glutSolidSphere(2.0, 6, 6);

}

}
}

