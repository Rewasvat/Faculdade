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
	static LoaderUtil* reference() { return reference_ ? reference_ : reference_ = new LoaderUtil; }

	~LoaderUtil();

	void Load(int argc, char* argv[]);

	engine::EngineConfig* GetEngineConfig() { return config_; }
	VectorField* GetVectorField() { return field_; }

	void PrintUsage();

private:
	static LoaderUtil* reference_;

	std::string input_filename_;
	engine::EngineConfig* config_;
	VectorField* field_;

	LoaderUtil();

	bool ParseOptions(int argc, char* argv[]);
	void LoadInputFile(const char* name);
};

}
#endif