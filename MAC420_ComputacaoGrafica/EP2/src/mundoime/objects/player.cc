#include <mundoime/objects/player.h>
#include <btBulletDynamicsCommon.h>
#include <engine/vector3D.h>
#include <cstdlib>

namespace mundoime {
namespace objects {

Player::Player(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape) : Model(pos,direction,mesh,mass,shape) {

}

Player::~Player() {
}

void Player::Update(double dt) {
    Model::Update(dt);
}

void Player::Render() {
    Model::Render();
}

void Player::MouseHandler(int btn, int state, int x, int y) {
}

void Player::MouseMotionHandler(int btn, int dx, int dy) {

}

void Player::KeyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		MoveForward(0.1, true);
		break;
	case 's':
		MoveForward(0.1, false);
		break;
	case 'a':
		MoveSideways(0.1, false);
		break;
	case 'd':
		MoveSideways(0.1, true);
		break;
	}
}

void Player::MoveForward(double amount, bool forward) {
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);

    engine::Vector3D delta = direction_;
    delta.Normalize();
    delta.Scale(amount);
    if (!forward)
        delta.Scale(-1.0);

    t.setOrigin( t.getOrigin() + btVector3(delta.x, delta.y, delta.z) );
    body_->getMotionState()->setWorldTransform(t);
}

void Player::MoveSideways(double amount, bool right) {
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);

    engine::Vector3D up (0.0, 1.0, 0.0);
    engine::Vector3D delta = direction_.CrossProduct(up);
    delta.Normalize();
    delta.Scale(amount);
    if (!right)
        delta.Scale(-1.0);

    t.setOrigin( t.getOrigin() + btVector3(delta.x, delta.y, delta.z) );
    body_->getMotionState()->setWorldTransform(t);
}

}
}
