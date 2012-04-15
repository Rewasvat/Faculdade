
#ifndef CYLINDER_H_
#define CYLINDER_H_

#include <engine/object.h>

namespace simulation {
namespace objects {

class Cylinder : public engine::Object {
public:
    Cylinder();
    ~Cylinder();

	void Update(double dt);
	void Render();
};

}
}
#endif
