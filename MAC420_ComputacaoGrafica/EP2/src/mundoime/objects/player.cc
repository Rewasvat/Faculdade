#include <mundoime/objects/player.h>
#include <btBulletDynamicsCommon.h>
#include <engine/vector3D.h>
#include <engine/engine.h>
#include <math.h>
#include <cstdlib>
#include <GL/glut.h>
#include <mundoime/mundoime.h>
#include <mundoime/objects/sun.h>

#define PI 3.14159265358979323846

using namespace engine;

namespace mundoime {
namespace objects {

Player::Player(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape) : Model(pos,direction,mesh,mass,shape) {
	body_->setAngularFactor(btVector3(0.0, 0.0, 0.0));
	body_->setDamping(0.975, 0.9);
	up_ = Vector3D(0.0, 1.0, 0.0);
	vertical_angle_ = horizontal_angle_ = 0.0;
	mouse_warped_ = false;
	forward_move_[0] = forward_move_[1] = side_move_[0] = side_move_[1] = false;
	speed_ = 2.0;
	min_speed_ = 1.0;
	max_speed_ = 6.0;
    jetpack_ = false;

	sun_ = NULL;
    spotlight_ = new Light(GL_LIGHT1, Light::SPOTLIGHT);
    spotlight_->SetAmbientColor(Color(1.0,1.0,1.0,1.0));
    spotlight_->SetDiffuseColor(Color(1.0,1.0,1.0,1.0));
    spotlight_->SetSpecularColor(Color(1.0,1.0,1.0,1.0));
    spotlight_->SetSpotlightParameters(10.0f);
    spotlight_->SetAttenuationParameters(1.2f, 0.3f, 0.1f);
	spotlight_->set_casts_shadow(false);
}

Player::~Player() {
}

void Player::Update(double dt) {
    Model::Update(dt);

	if (forward_move_[0])
		MoveForward(speed_, true);
	if (forward_move_[1])
		MoveForward(speed_, false);
	if (side_move_[0])
		MoveSideways(speed_, false);
	if (side_move_[1])
		MoveSideways(speed_, true);

    Vector3D dir (direction_.x, 0.0, direction_.z);
    dir.Normalize();
    Vector3D z_axis(0.0,0.0,1.0);
    rot_angle_ = acos( dir * z_axis );
    if ( fabs(rot_angle_) > 0.0 ) {
        rot_axis_ = dir.CrossProduct(z_axis);
        rot_axis_.Normalize();
        rot_axis_.Scale(-1.0);
    }

	forward_move_[0] = forward_move_[1] = side_move_[0] = side_move_[1] = false;

	updateSpotlight();
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
		if (vertical_angle_ > PI/2)
			vertical_angle_ = PI/2;
		if (vertical_angle_ < -PI/2)
			vertical_angle_ = -PI/2;

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
	
	if (key == 'W') {
	    speed_ += 0.2;
	    if (speed_ > max_speed_) speed_ = max_speed_;
	}
	if (key == 'S') {
	    speed_ -= 0.2;
	    if (speed_ < min_speed_) speed_ = min_speed_;
	}

    if (key == 'J')
        jetpack_ = !jetpack_;

	if (key == 'p')
		printf("position %f %f %f\n", position_.x, position_.y, position_.z);
}

void Player::MoveForward(double amount, bool forward) {
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);

    engine::Vector3D delta = direction_;
    if (!jetpack_)
        delta.y = 0.0; /*we don't want the player moving around up and down... After all he doesn't fly*/
    delta.Normalize();
    delta.Scale(amount);

	if (!forward)
        delta.Scale(-1.0);

	body_->setLinearVelocity(btVector3(delta.x, delta.y, delta.z));

	//printf("Moving forward[%d]\n", forward);
}

void Player::MoveSideways(double amount, bool right) {
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);

    engine::Vector3D delta = direction_.CrossProduct(up_);
    if (!jetpack_)
        delta.y = 0.0; /*we don't want the player moving around up and down... After all he doesn't fly*/
    delta.Normalize();
    delta.Scale(amount);

    if (!right)
        delta.Scale(-1.0);

	body_->setLinearVelocity(btVector3(delta.x, delta.y, delta.z));

	//printf("Moving sideways[%d]\n", right);
}

Vector3D Player::eye_position() {
	return position_ + Vector3D(0.0, 0.21, 0.0);
}

void Player::updateSpotlight() {
    if (sun_ == NULL) {
        MundoIME* mime = dynamic_cast<MundoIME*>(parent_);
        sun_ = mime->sun();
        spotlight_->ReparentTo(parent_);
    }

	spotlight_->set_position( position_ );
    spotlight_->set_direction( direction_ );
    if (sun_->IsDaytime()) {
        if (spotlight_->active())   spotlight_->Deactivate();
    }
    else {
        if (!spotlight_->active())  spotlight_->Activate();
    }
}

}
}
