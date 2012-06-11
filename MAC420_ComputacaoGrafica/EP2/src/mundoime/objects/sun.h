
#ifndef SUN_H_
#define SUN_H_

#include <engine/object.h>

namespace mundoime {
namespace objects {

class Sun : public engine::Object {
public:
    Sun();
    ~Sun();

	virtual void Update(double dt);
	virtual void Render();

protected:
	
};

}
}
#endif

