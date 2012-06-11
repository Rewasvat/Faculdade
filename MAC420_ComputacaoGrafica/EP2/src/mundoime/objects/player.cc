#include <mundoime/objects/player.h>
#include <btBulletDynamicsCommon.h>
#include <engine/vector3D.h>
#include <engine/engine.h>
#include <math.h>
#include <cstdlib>
#include <GL/glut.h>

#define PI 3.14159265358979323846

using namespace engine;

namespace mundoime {
namespace objects {

Player::Player(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape) : Model(pos,direction,mesh,mass,shape) {
	body_->setAngularFactor(btVector3(0.0, 0.0, 0.0));
	up_ = Vector3D(0.0, 1.0, 0.0);
	vertical_angle_ = horizontal_angle_ = 0.0;
	mouse_warped_ = false;
	forward_move_[0] = forward_move_[1] = side_move_[0] = side_move_[1] = false;
}

Player::~Player() {
}

void Player::Update(double dt) {
    Model::Update(dt);

	double speed = 4.0 * dt;
	if (forward_move_[0])
		MoveForward(speed, true);
	if (forward_move_[1])
		MoveForward(speed, false);
	if (side_move_[0])
		MoveSideways(speed, false);
	if (side_move_[1])
		MoveSideways(speed, true);

	forward_move_[0] = forward_move_[1] = side_move_[0] = side_move_[1] = false;
}

void Player::Render() {
    Model::Render();
}

void Player::MouseHandler(int btn, int state, int x, int y) {
}

void Player::MouseMotionHandler(int btn, int dx, int dy) {
	if (!mouse_warped_) {
		horizontal_angle_ -= dx*0.01;
		vertical_angle_ -= dy*0.01;

		direction_.x = cos(vertical_angle_) * sin(horizontal_angle_);
		direction_.y = sin(vertical_angle_);
		direction_.z = cos(vertical_angle_) * cos(horizontal_angle_);

		Vector3D right = Vector3D( sin(horizontal_angle_ - PI/2.0),	0, cos(horizontal_angle_ - PI/2.0) );

		right.Normalize();
		direction_.Normalize();
		up_ = right.CrossProduct(direction_);

		if (dx != 0.0 || dy != 0.0) {
			mouse_warped_ = true;
#ifdef WIN32
			//apparently this works on Windows but not on Linux...
			glutWarpPointer(Engine::reference()->window_width()/2, Engine::reference()->window_height()/2);
#endif
		}
	}
	else
		mouse_warped_ = false;

}

void Player::KeyboardHandler(unsigned char key, int x, int y) {
	forward_move_[0] = (key == 'w');
	forward_move_[1] = (key == 's');
	side_move_[0] = (key == 'a');
	side_move_[1] = (key == 'd');
}

void Player::MoveForward(double amount, bool forward) {
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);

    engine::Vector3D delta = direction_;
    delta.Normalize();
    delta.Scale(amount);

	if (!forward)
        delta.Scale(-1.0);

	body_->translate(btVector3(delta.x, delta.y, delta.z));

	//printf("Moving forward[%d]\n", forward);
}

void Player::MoveSideways(double amount, bool right) {
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);

    engine::Vector3D delta = direction_.CrossProduct(up_);
    delta.Normalize();
    delta.Scale(amount);

    if (!right)
        delta.Scale(-1.0);

	body_->translate(btVector3(delta.x, delta.y, delta.z));

	//printf("Moving sideways[%d]\n", right);
}

Vector3D Player::eye_position() {
	return position_ + Vector3D(0.0, 0.225, 0.0);
}

}
}
