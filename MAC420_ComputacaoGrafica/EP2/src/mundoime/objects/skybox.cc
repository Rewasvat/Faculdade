#include <mundoime/objects/skybox.h>
#include <engine/texture/texture.h>
#include <GL/glut.h>
#include <cstdlib>
#include <string>

#define PI 3.14159265358979323846

namespace mundoime {
namespace objects {

Skybox::Skybox(const char*  base_tex_name, double size) : size_(size/2.0) {
	std::string btn (base_tex_name);
	top_tex_ = engine::texture::LoadTexture( (btn + "_top.tga").c_str() , false);
	bottom_tex_ = engine::texture::LoadTexture( (btn + "_bottom.tga").c_str() , false);
	right_tex_ = engine::texture::LoadTexture( (btn + "_right.tga").c_str() , false);
	left_tex_ = engine::texture::LoadTexture( (btn + "_left.tga").c_str() , false);
	forward_tex_ = engine::texture::LoadTexture( (btn + "_forward.tga").c_str() , false);
	back_tex_ = engine::texture::LoadTexture( (btn + "_back.tga").c_str() , false);
}

Skybox::~Skybox() {

}

void Skybox::Update(double dt) {

}

void Skybox::Render() {
	// Reset and transform the matrix.
 
    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
 
    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);
	glTranslated(0.0, size_*0.55, 0.0);
 
    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, left_tex_);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3d(  size_, -size_, -size_ );
        glTexCoord2f(0, 1); glVertex3d( -size_, -size_, -size_ );
        glTexCoord2f(0, 0); glVertex3d( -size_,  size_, -size_ );
        glTexCoord2f(1, 0); glVertex3d(  size_,  size_, -size_ );
    glEnd();
 
    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, forward_tex_);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3d(  size_, -size_,  size_ );
        glTexCoord2f(0, 1); glVertex3d(  size_, -size_, -size_ );
        glTexCoord2f(0, 0); glVertex3d(  size_,  size_, -size_ );
        glTexCoord2f(1, 0); glVertex3d(  size_,  size_,  size_ );
    glEnd();
 
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, right_tex_);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3d( -size_, -size_,  size_ );
        glTexCoord2f(0, 1); glVertex3d(  size_, -size_,  size_ );
        glTexCoord2f(0, 0); glVertex3d(  size_,  size_,  size_ );
        glTexCoord2f(1, 0); glVertex3d( -size_,  size_,  size_ );
 
    glEnd();
 
    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, back_tex_);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3d( -size_, -size_, -size_ );
        glTexCoord2f(0, 1); glVertex3d( -size_, -size_,  size_ );
        glTexCoord2f(0, 0); glVertex3d( -size_,  size_,  size_ );
        glTexCoord2f(1, 0); glVertex3d( -size_,  size_, -size_ );
    glEnd();
 
    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, top_tex_);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3d( -size_,  size_, -size_ );
        glTexCoord2f(0, 0); glVertex3d( -size_,  size_,  size_ );
        glTexCoord2f(1, 0); glVertex3d(  size_,  size_,  size_ );
        glTexCoord2f(1, 1); glVertex3d(  size_,  size_, -size_ );
    glEnd();
 
    // Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, bottom_tex_);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3d( -size_, -size_, -size_ );
        glTexCoord2f(0, 1); glVertex3d( -size_, -size_,  size_ );
        glTexCoord2f(1, 1); glVertex3d(  size_, -size_,  size_ );
        glTexCoord2f(1, 0); glVertex3d(  size_, -size_, -size_ );
    glEnd();
 
    // Restore enable bits and matrix
	glPopAttrib();
}

}
}

