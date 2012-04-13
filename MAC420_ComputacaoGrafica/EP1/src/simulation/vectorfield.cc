#include <simulation/vectorfield.h>
#include <vector>

using namespace engine;

namespace simulation {

VectorField::VectorField(double stepX, double stepY, double stepZ, int nX, int nY, int nZ) :
    nX_(nX), nY_(nY), nZ_(nZ), step_size_(Vector3D(stepX,stepY,stepZ)) 
{
    field_ = VectorCube();
    field_.resize(nX);
    int i, j;
    for (i=0; i<nX; i++) {
        field_[i].resize(nY);
        for (j=0; j<nY; j++) 
            field_[i][j].resize(nZ);
    }    
}

VectorField::~VectorField() {
}

/** Sets the base vector of the field in the given coordinates (x, y, z) as v. Remember that base vectors of the field are
    places in the positions along a discrete 3D grid in the field. */
void VectorField::SetVectorAtPos(int x, int y, int z, Vector3D& v) {
    Vector3D pos = Vector3D(x,y,z);
    pos.Scale(step_size_);
    field_[x][y][z] = Force3D(pos, v);
}

/** Returns the 3D vector (force) of the field in the given position.
    Result is based on the vectors at key positions in the field and interpolation between them.
*/
engine::Vector3D VectorField::GetVectorAtPos(const engine::Vector3D& pos) {
    bool x_check = (pos.x % step_size_.x == 0) && (0 < pos.x) && (pos.x < nX_ * step_size_.x);
    bool y_check = (pos.y % step_size_.y == 0) && (0 < pos.y) && (pos.y < nY_ * step_size_.y);
    bool z_check = (pos.z % step_size_.z == 0) && (0 < pos.z) && (pos.z < nZ_ * step_size_.z);
    
    int base_x = (int) (pos.x / step_size_.x);
    int base_y = (int) (pos.y / step_size_.y);
    int base_z = (int) (pos.z / step_size_.z);
    
    if (x_check && y_check && z_check) {
        return field_[base_x][base_y][base_z]->second;
    }
    else {
        /*interpolate near vector to get result*/
        //TODO: if not a base vector pos, interpolate to get the desired vector.
        return field_[base_x][base_y][base_z]->second;
    }
}

/** Checks if the given position is inside the vector field */
bool VectorField::IsInField(const engine::Vector3D& pos) {
    bool in = false;
    if (0 < pos.x && pos.x < nX_ * step_size_.x)
        if (0 < pos.y && pos.y < nY_ * step_size_.y)
            if (0 < pos.z && pos.z < nZ_ * step_size_.z)
                in = true;
    return in;
}

/** Normalizes the vectors in the field according to the step_size and so on */
void VectorField::Normalize() {
    //TODO
}

}
