
#ifndef PLAYER_H_
#define PLAYER_H_

#include <mundoime/objects/model.h>
#include <engine/eventhandler.h>


namespace mundoime {
namespace objects {

class Player : public Model, public engine::EventHandler {
public:
    Player(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape = 0);
    ~Player();

	void Update(double dt);
	void Render();
	
    virtual void MouseHandler(int btn, int state, int x, int y);
	virtual void MouseMotionHandler(int btn, int dx, int dy);
	virtual void KeyboardHandler(unsigned char key, int x, int y);

    void MoveForward(double amount, bool forward);
    void MoveSideways(double amount, bool right);

	engine::Vector3D eye_position();
	engine::Vector3D up() { return up_; }

    double speed() { return speed_; }

protected:
	engine::Vector3D up_;
	double vertical_angle_, horizontal_angle_;
	bool mouse_warped_;
	bool forward_move_[2], side_move_[2];
	double speed_;
	double min_speed_, max_speed_;
    bool jetpack_;
};

}
}
#endif
