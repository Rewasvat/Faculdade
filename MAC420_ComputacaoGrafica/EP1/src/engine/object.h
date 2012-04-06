
#ifndef OBJECT_H_
#define OBJECT_H_

namespace engine {

class Object {
public:
    Object();
    ~Object();

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
};

}
#endif
