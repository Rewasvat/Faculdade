
#ifndef SUN_H_
#define SUN_H_

#include <engine/light.h>

namespace mundoime {
namespace objects {

class Sun : public engine::Light {
public:
    Sun(double radius, double distance);
    ~Sun();

	virtual void Update(double dt);
	virtual void Render();

	void IncreateTimeRate();
	void DecreaseTimeRate();

protected:
	double radius_;
	double distance_;
	double angle_; //directly related to time of day ==>  angle=0 -> sun coming up |  angle=PI/2 -> midday ...
	double time_rate_; // rate at which time passes by, in radians per second (this affects angle_ which is proportional to time of day)
	float sun_color_[4];

	void updatePosAndDir();
	void updateColors();
};

}
}
#endif

