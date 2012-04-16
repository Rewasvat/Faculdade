#ifndef LOADERUTIL_H_
#define LOADERUTIL_H_

#include <string>

namespace engine {
	class EngineConfig;
}

namespace simulation {

class VectorField;

class LoaderUtil {
public:
	LoaderUtil(int argc, char* argv[]);
	~LoaderUtil();

	engine::EngineConfig* GetEngineConfig() { return config_; }
	VectorField* GetVectorField() { return field_; }

	void PrintUsage();

private:
	std::string input_filename_;
	engine::EngineConfig* config_;
	VectorField* field_;

	bool ParseOptions(int argc, char* argv[]);
	void LoadInputFile(const char* name);
};

}
#endif