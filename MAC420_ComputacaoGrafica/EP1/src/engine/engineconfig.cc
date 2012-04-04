#include "engineconfig.h"
#include <GL/glut.h>

namespace engine {

EngineConfig::EngineConfig() {
	window_name_ = "MAC420 Engine Window";
	do_multisampling_ = true;
	width_ = 800;
	height_ = 600;
}

EngineConfig::~EngineConfig() {
}

unsigned int EngineConfig::display_mode() const {
	unsigned int mode = GLUT_DOUBLE | GLUT_RGB;
	if (do_multisampling_)
		mode |= GLUT_MULTISAMPLE;
	return mode;
}

}