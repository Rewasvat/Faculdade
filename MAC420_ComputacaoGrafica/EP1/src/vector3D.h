
#ifndef VECTOR3D_H_
#define VECTOR3D_H_

namespace math {

class Vector3D {
  public:

    Vector3D() : x(0.0), y(0.0), z(0.0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    ~Vector3D() { }

    // Fields are accessible as x/y/z or val[0], val[1] or val[2].
    union {
        struct { double x, y, z; };
        struct { double val[3]; };
    };

    /// Returns the norm-2 of this vector
    /** The norm-2 of a vector (x,y) is sqrt( x^2 + y^2 )    */
    double Length() const;
    
    /// Returns a new vector equal to this vector normalized
    /** The normalized vector (x,y) is (x,y) / Lenght(x,y)    */
    void Normalize();

    /// Returns a new vector which is this vector scaled coordinate by coordinate with "scale".
    void Scale(const Vector3D &scale);

    Vector3D& operator+=(const Vector3D &other);
    Vector3D& operator-=(const Vector3D &other);

    Vector3D operator+(const Vector3D &right) const; // operator to add
    Vector3D operator-() const; // negation operator (return opposite vector)
    Vector3D operator-(const Vector3D &right) const; // operator to subtract
    Vector3D operator*(const double &scalar) const; // operator to scale
    Vector3D operator/(const double &scalar) const; // operator to inverse scale (scale by 1.0/scalar)
    double operator*(const Vector3D &right) const; // operator to inner product
    Vector3D CrossProduct(const Vector3D &right) const; 
    
    // Static methods

    /* Returns the sum of two vectors as a new vector. */
    static Vector3D Add(const Vector3D &a, const Vector3D &b) {
        return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
    }
    
    /* Subtracts the two vectors, returning a new one. */
    static Vector3D Subtract(const Vector3D &a, const Vector3D &b) {
        return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
    }
    
    /* Returns a new vector, equal to A scaled by the scalar value. */
    static Vector3D Multiply(const Vector3D &a, const double &scalar) {
        return Vector3D(a.x * scalar, a.y * scalar, a.z * scalar);
    }
    
    /* Returns the inner product of two vectors A and B, which is directly related to the length and angle between the vectors.
      So A * B = |A| * |B| * cos(angle between A and B) */
    static double InnerProduct(const Vector3D &a, const Vector3D &b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }
    
    /* Returns the cross product of two vectors A and B, which contrasts with the InnerProduct.
       This returns a vector which is perpendicular to both A and B (the plane defined by A and B),
       with a direction given by the right-hand rule and a magnitude equal to the area of the parallelogram
       that A and B span (the area of the AB plane). */
    static Vector3D CrossProduct(const Vector3D &a, const Vector3D &b) {
        return Vector3D( (a.y * b.z) - (a.z * b.y),
                         (a.z * b.x) - (a.x * b.z),
                         (a.x * b.y) - (a.y * b.x) );
    }
};

 /// Method that returns a vector equal to the a vector multiplied by a scalar
    /** A vector (x,y) multiplied by a scalar a is equal to the vector (x * scalar, y*scalar)
    *  @return Scalar Multiplied Vector
    */
Vector3D operator*(const double  &scalar, const Vector3D &right);

} 
#endif
