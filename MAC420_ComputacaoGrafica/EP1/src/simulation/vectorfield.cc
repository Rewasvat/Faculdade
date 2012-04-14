#include <simulation/vectorfield.h>
#include <engine/vector3D.h>
#include <math.h>

using namespace engine;

namespace simulation {

VectorField::VectorField(double stepX, double stepY, double stepZ, int nX, int nY, int nZ) :
    nX_(nX), nY_(nY), nZ_(nZ), step_size_(Vector3D(stepX,stepY,stepZ)), max_vector_(Vector3D()) 
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

	if (v.Length() > max_vector_.Length())
		max_vector_ = v;
}

/** Returns the 3D vector (force) of the field in the given position.
    Result is based on the vectors at key positions in the field and interpolation between them.
*/
Vector3D VectorField::GetVectorAtPos(const Vector3D& pos) {
    bool x_check = ( ((pos.x / step_size_.x) - (int)(pos.x / step_size_.x)) == 0) && (0 < pos.x) && (pos.x < nX_ * step_size_.x);
    bool y_check = ( ((pos.y / step_size_.y) - (int)(pos.y / step_size_.y)) == 0) && (0 < pos.y) && (pos.y < nY_ * step_size_.y);
    bool z_check = ( ((pos.z / step_size_.z) - (int)(pos.z / step_size_.z)) == 0) && (0 < pos.z) && (pos.z < nZ_ * step_size_.z);
    
    int base_x = (int) (pos.x / step_size_.x);
    int base_y = (int) (pos.y / step_size_.y);
    int base_z = (int) (pos.z / step_size_.z);
    
    if (x_check && y_check && z_check) {
        return field_[base_x][base_y][base_z].second;
    }
    else {
        /*interpolate near vector to get result*/
		if (! this->IsInField(pos) )
			return Vector3D();
        return this->interpolateAtPos(pos);
    }
}

/** Checks if the given position is inside the vector field */
bool VectorField::IsInField(const Vector3D& pos) {
    bool in = false;
    if (0 < pos.x && pos.x < nX_ * step_size_.x)
        if (0 < pos.y && pos.y < nY_ * step_size_.y)
            if (0 < pos.z && pos.z < nZ_ * step_size_.z)
                in = true;
    return in;
}

/** Normalizes the vectors in the field according to the step_size and so on */
void VectorField::Normalize() {
	double scale = step_size_.Length() / max_vector_.Length();

	int i, j, k;
	for (i=0; i < nX_; i++)
		for (j=0; j < nY_; j++)
			for (k=0; k < nZ_; k++)
				field_[i][j][k].second.Scale(scale);
}


Vector3D VectorField::interpolateAtPos(const Vector3D& pos) {
	/*  TRILINEAR INTERPOLATION:
		Vxyz =	V000.(1 - x).(1 - y).(1 - z) +
				V100.x.(1 - y).(1 - z) + 
				V010.(1 - x).y.(1 - z) + 
				V001.(1 - x).(1 - y).z +
				V101.x.(1 - y).z + 
				V011.(1 - x).y.z + 
				V110.x.y.(1 - z) + 
				V111.x.y.z 
				
		Where:
			Vxyz = the resulting interpolated vector
			V000 to V111 = the 8 vectors representing the cube vertexes from which the interpolation will occur
			x, y and z = values in the range [0,1] that dictates how far along each axis (inside the cube) is the point
						 where the interpolated vector will be calculated
			'.' in the above formulas = multiplication

		In our vector/force field case here, Vxyz is the resulting interpolated force vector; V000 to V111 will be the surrounding 8
		force vectors from the grid;  x/y/z will be calculated according to the position (to see where inside the cube it is) */

	//Getting the 8 surrounding points in the grid
	int baseX_down, baseX_up, baseY_down, baseY_up, baseZ_down, baseZ_up;
	baseX_down = floor(pos.x / step_size_.x);
	baseX_up = ceil(pos.x / step_size_.x);
	baseY_down = floor(pos.y / step_size_.y);
	baseY_up = ceil(pos.y / step_size_.y);
	baseZ_down = floor(pos.z / step_size_.z);
	baseZ_up = ceil(pos.z / step_size_.z);

	Vector3D V000 = field_[baseX_down][baseY_down][baseZ_down].second;
	Vector3D V001 = field_[baseX_down][baseY_down][baseZ_up].second;
	Vector3D V010 = field_[baseX_down][baseY_up][baseZ_down].second;
	Vector3D V011 = field_[baseX_down][baseY_up][baseZ_up].second;
	Vector3D V100 = field_[baseX_up][baseY_down][baseZ_down].second;
	Vector3D V101 = field_[baseX_up][baseY_down][baseZ_up].second;
	Vector3D V110 = field_[baseX_up][baseY_up][baseZ_down].second;
	Vector3D V111 = field_[baseX_up][baseY_up][baseZ_up].second;


	//Calculating X,Y and Z from the formula (relative position of pos inside the cube scaled to [0,1] range)
	double rx, ry, rz;
	rx = (pos.x / step_size_.x) - baseX_down;
	ry = (pos.y / step_size_.y) - baseY_down;
	rz = (pos.z / step_size_.z) - baseZ_down;


	//Interpolating...
	Vector3D Vxyz = (V000 * ((1.0 - rx) * (1.0 - ry) * (1.0 - rz))) +
					(V100 * (rx * (1.0 - ry) * (1.0 - rz))) + 
					(V010 * ((1.0 - rx) * ry * (1.0 - rz))) + 
					(V001 * ((1.0 - rx) * (1.0 - ry) * rz)) +
					(V101 * (rx * (1.0 - ry) * rz)) + 
					(V011 * ((1.0 - rx) * ry * rz)) + 
					(V110 * (rx * ry * (1.0 - rz))) + 
					(V111 * (rx * ry * rz));

	return Vxyz;
}

}
