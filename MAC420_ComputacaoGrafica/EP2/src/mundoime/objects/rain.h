/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: rain.h                                             **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/

#ifndef RAIN_H_
#define RAIN_H_

#include <engine/object.h>
#include <engine/objectcontainer.h>
#include <engine/color.h>
#include <engine/eventhandler.h>

#define MIN_INTENSITY 5.0
#define MAX_INTENSITY 150.0

namespace engine {
class Light; 
}

namespace mundoime {
namespace objects {

/*****/
class Drop : public engine::Object {
public:
	Drop(engine::Vector3D& pos, engine::Color& start_color, engine::Color& end_color, float line_width, engine::Vector3D& fall_speed);
    ~Drop();

	virtual void Update(double dt);
	virtual void Render();

	bool finished() { return finished_; }

protected:
	engine::Color start_color_;
	engine::Color end_color_;
	float line_width_;
	engine::Vector3D fall_speed_;
	bool finished_;
};

/*****/
class Rain : public engine::Object, public engine::ObjectContainer, public engine::EventHandler {
public:
    Rain();
    ~Rain();

	virtual void Update(double dt);
	virtual void Render();

	bool raining() { return raining_; }

	double intensity_factor() { return actual_intensity_/intensity_; }
    double real_intensity_factor() { return actual_intensity_/MAX_INTENSITY; }

	virtual void MouseHandler(int btn, int state, int x, int y) {}
	virtual void MouseMotionHandler(int btn, int dx, int dy) {}
	virtual void KeyboardHandler(unsigned char key, int x, int y);

protected:
	bool raining_;
	double intensity_;
	double actual_intensity_;
	double max_factor_;
	double factor_mean_;
	double factor_variance_;
	double rain_time_;
	double time_elapsed_;
	double drop_time_;
	engine::Vector3D rain_direction_;
	engine::Vector3D min_coords_;
	engine::Vector3D max_coords_;

    enum RainCurve {
        NORMAL, DESCENDING_GAMMA, ASCENDING_GAMMA
    };
    RainCurve curve_;

	void startRain();
	void waitForRain();

	double RandExpTime();
	double Random();
};

}
}
#endif
