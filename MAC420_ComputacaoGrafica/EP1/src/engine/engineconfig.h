#ifndef ENGINECONFIG_H_
#define ENGINECONFIG_H_

#include <string>

namespace engine {

class EngineConfig {
public:
	EngineConfig();
	~EngineConfig();

	std::string& window_name() { return window_name_; }
	void set_window_name(std::string& name) { window_name_ = name; }
	void set_window_name(const char* name) { window_name_ = std::string(name); }

	void set_window_size(int width, int height) { width_ = width; height_ = height; }
	int width() { return width_; }
	int height() { return height_; }

	bool do_multisampling() { return do_multisampling_; }
	void set_do_multisampling(bool do_ms) { do_multisampling_ = do_ms; }

	unsigned int display_mode() const;

private:
	std::string window_name_;
	bool do_multisampling_;
	int width_, height_;
};

}
#endif
