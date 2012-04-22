#ifndef LOADERUTIL_H_
#define LOADERUTIL_H_

#include <string>

namespace engine {
	class EngineConfig;
}

namespace simulation {

#define VCM_MAGNETUDE	0
#define VCM_DIRECTION	1
#define VCM_RANDOM		2

class VectorField;

class LoaderUtil {
public:
	static LoaderUtil* reference() { return reference_ ? reference_ : reference_ = new LoaderUtil; }

	~LoaderUtil();

	void Load(int argc, char* argv[]);

	engine::EngineConfig* GetEngineConfig() { return config_; }
	VectorField* GetVectorField() { return field_; }
	int GetVectorColorMode() { return vector_color_mode_; }
	int GetSphereResolution() { return sphere_resolution_; }
	int GetCylinderResolution() { return cylinder_resolution_; }
	double GetSphereRadiusFactor() { return sphere_radius_factor_; }

	void PrintUsage();

private:
	static LoaderUtil* reference_;

	std::string input_filename_;
	engine::EngineConfig* config_;
	VectorField* field_;
	int vector_color_mode_;
	int sphere_resolution_;
	int cylinder_resolution_;
	double sphere_radius_factor_;

	LoaderUtil();

	bool ParseOptions(int argc, char* argv[]);
	void LoadInputFile(const char* name);
};

}
#endif