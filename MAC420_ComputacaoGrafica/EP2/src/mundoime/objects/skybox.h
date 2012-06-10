
#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <engine/object.h>

namespace mundoime {
namespace objects {

class Skybox : public engine::Object {
public:
    Skybox(const char* base_tex_name, double size);
    ~Skybox();

	virtual void Update(double dt);
	virtual void Render();

protected:
	double size_;
	unsigned top_tex_;
	unsigned bottom_tex_;
	unsigned right_tex_;
	unsigned left_tex_;
	unsigned forward_tex_;
	unsigned back_tex_;

};

}
}
#endif

