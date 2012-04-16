#include "loaderutil.h"

#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <engine/engineconfig.h>
#include <simulation/vectorfield.h>
#include <engine/vector3D.h>

namespace simulation {

LoaderUtil::LoaderUtil(int argc, char* argv[]) : config_(NULL), field_(NULL) {
	bool has_inputfile = ParseOptions(argc, argv);
	if (has_inputfile)
		LoadInputFile(input_filename_.c_str());
}

LoaderUtil::~LoaderUtil() {
}

bool checkArgOption(const char* arg_name, char* full_arg, int* arg_size) {
	*arg_size = strlen(arg_name);
	return strncmp(full_arg, arg_name, *arg_size) == 0 && strlen(full_arg) > *arg_size;
}

#define CHECK_ARG(code, argName) \
	if ( !recognized && checkArgOption(argName, argv[i], &arg_size)) { \
		code( atoi(&argv[i][arg_size+1]) ); \
		recognized = true; \
	} \

bool LoaderUtil::ParseOptions(int argc, char* argv[]) {
	int i, arg_size;
	bool got_input = false;

	config_ = new engine::EngineConfig();

	for (i=1; i<argc; i++) {
		bool recognized = false;

		CHECK_ARG(config_->set_width, "-width")
		CHECK_ARG(config_->set_height, "-height")
		
		if (!recognized) {
			FILE* input_file = fopen(argv[i], "r");
			if (input_file != NULL)	{
				input_filename_ = std::string(argv[i]);
				fclose(input_file);
				got_input = true;
			}
		}
	}
	return got_input;
}

void LoaderUtil::LoadInputFile(const char* name) {
	std::ifstream ifs ( name, std::ifstream::in );

	double stepX, stepY, stepZ;
	int nX, nY, nZ;
	ifs >> nX >> nY >> nZ;
	ifs >> stepX >> stepY >> stepZ;

	field_ = new VectorField(stepX, stepY, stepZ, nX, nY, nZ);

	double x, y, z;
	int i, j, k;
	for (i=0; i < nX; i++)
		for (j=0; j < nY; j++)
			for (k=0; k < nZ; k++) {
				ifs >> x >> y >> z;
				field_->SetVectorAtPos(i, j, k, engine::Vector3D(x,y,z));
			}

	field_->Normalize();

	ifs.close();
}

void LoaderUtil::PrintUsage() {
	printf("MAC420 EP1  Usage:\n");
	printf("\t./ep1 <input_file> [OPTION1 | OPTION2 | ...]\n\n");
	printf("Where OPTIONi can be the following:\n");
	printf("\t-width=INTEGER  :  sets the window width\n");
	printf("\t-height=INTEGER  :  sets the window height\n");
}

}