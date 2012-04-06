#include <math.h>
#include <engine/vector3D.h>

namespace engine {

double Vector3D::Length() const {
    return sqrt( (x*x) + (y*y) + (z*z) );
}

void Vector3D::Normalize() {
    double norm = Length();
    if (norm != 0.0) {
        x = x / norm;
        y = y / norm;
        z = z / norm;
    }
    else {
        x = y = z = 0.0;
    }
}

void Vector3D::Scale(const Vector3D &scale) {
    x *= scale.x;
    y *= scale.y;
    z *= scale.z;
}

Vector3D& Vector3D::operator+=(const Vector3D &other) {
    x += other.x;  
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3D Vector3D::operator+(const Vector3D &right) const {
    return Vector3D::Add(*this, right);
}
Vector3D Vector3D::operator-(const Vector3D &right) const {
    return Vector3D::Subtract(*this, right);
}
Vector3D Vector3D::operator-() const {
    return Vector3D(-this->x, -this->y, -this->z);
}

Vector3D Vector3D::operator*(const double &scalar) const {
    return Vector3D::Multiply(*this, scalar);
}
Vector3D Vector3D::operator/(const double &scalar) const {
    return Vector3D::Multiply(*this, 1.0 / scalar);
}
double Vector3D::operator*(const Vector3D &right) const {
    return Vector3D::InnerProduct(*this, right);
}
Vector3D Vector3D::CrossProduct(const Vector3D &right) const {
    return Vector3D::CrossProduct(*this, right);
}

Vector3D operator*(const double &scalar, const Vector3D &right) {
    return Vector3D::Multiply(right, scalar);
}

}
