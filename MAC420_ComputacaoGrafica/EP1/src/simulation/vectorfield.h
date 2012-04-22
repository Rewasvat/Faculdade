
#ifndef VECTORFIELD_H_
#define VECTORFIELD_H_

#include <engine/vector3D.h>
#include <utility>
#include <vector>

namespace simulation {

typedef std::pair<engine::Vector3D, engine::Vector3D> Force3D; /* Force3D = (position, force), where force is the direction/magnitude vector*/

typedef std::vector< std::vector< std::vector<Force3D> > > VectorCube;

/* X Y Z entre cada vetor (step_size_)
   N1 N2 N3 qtos N tem em cada dimensao
   cubo de vetores
   funcao de interpolacao trilinear
*/
class VectorField {
public:
    VectorField(double stepX, double stepY, double stepZ, int nX, int nY, int nZ);
    ~VectorField();
    
    /** Sets the base vector of the field in the given coordinates (x, y, z) as v. Remember that base vectors of the field are
        places in the positions along a discrete 3D grid in the field. */
    void SetVectorAtPos(int x, int y, int z, engine::Vector3D& v);
    
    /** Returns the 3D vector (force) of the field in the given position.
        Result is based on the vectors at key positions in the field and interpolation between them.
    */
    engine::Vector3D GetVectorAtPos(const engine::Vector3D& pos);

	/*Gets the base vector of the grid at the given coordinates. Mostly used so that outside code can iterate over the grid.*/
	Force3D GetBaseVectorInGrid(int x, int y, int z) { return field_[x][y][z]; }

    /** Checks if the given position is inside the vector field */
    bool IsInField(const engine::Vector3D& pos);

    /** Normalizes the vectors in the field according to the step_size and so on */
    void Normalize();
    
    int nX() { return nX_; }
    int nY() { return nY_; }
    int nZ() { return nZ_; }

	engine::Vector3D GetFieldCenterPos();
    engine::Vector3D GetMinimumVector() { return min_vector_; }
	engine::Vector3D GetMaximumVector() { return max_vector_; }
	double GetMinimumDimensionStep() { 
		double min1 = (step_size_.x < step_size_.y) ? step_size_.x : step_size_.y;
		return (step_size_.z < min1) ? step_size_.z : min1;
	}

protected:
    int nX_, nY_, nZ_;
    engine::Vector3D step_size_;
    VectorCube field_;

	engine::Vector3D max_vector_; //used to help normalization
    engine::Vector3D min_vector_;

    engine::Vector3D getBaseVectorForInterpolation(int i, int j, int k);
	engine::Vector3D interpolateAtPos(const engine::Vector3D& pos);
};

}
#endif
