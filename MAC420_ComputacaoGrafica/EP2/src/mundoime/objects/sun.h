
#ifndef SUN_H_
#define SUN_H_

#include <engine/light.h>
#include <engine/color.h>

namespace mundoime {
namespace objects {

class Rain;

class Sun : public engine::Light {
public:
    Sun(double radius, double distance);
    ~Sun();

	virtual void Update(double dt);
	virtual void Render();

	void IncreateTimeRate();
	void DecreaseTimeRate();

    void GetCurrentTimeStr(char* str);
    bool IsDaytime();

	double time_factor() { return time_factor_; }
	engine::Color& color() { return sun_color_; }

	void set_related_rain(Rain* related_rain) { related_rain_ = related_rain; }

protected:
	double radius_;
	double distance_;
	double angle_; //directly related to time of day ==>  angle=0 -> sun coming up |  angle=PI/2 -> midday ...
	double time_rate_; // rate at which time passes by, in radians per second (this affects angle_ which is proportional to time of day)
	engine::Color sun_color_;
	double time_factor_;
	double max_factor_;
	double factor_mean_;
	double factor_variance_;
	Rain* related_rain_;

	void updatePosAndDir();
	void updateColors();
};

}
}
#endif

