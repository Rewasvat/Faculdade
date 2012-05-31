#include "loaderutil.h"

#include <string.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <engine/engineconfig.h>
#include <simulation/vectorfield.h>
#include <engine/vector3D.h>

namespace simulation {

LoaderUtil* LoaderUtil::reference_ = NULL;

LoaderUtil::LoaderUtil() : 
	config_(NULL), field_(NULL), vector_color_mode_(0), sphere_resolution_(12), cylinder_resolution_(10), sphere_radius_factor_(1.0)
{
}

LoaderUtil::~LoaderUtil() {
	/* No need to delete the EngineConfig... It is the single instance in the program and the Engine will remove it. */
	delete field_;
}

void LoaderUtil::Load(int argc, char* argv[]) {
	bool has_inputfile = ParseOptions(argc, argv);
	if (has_inputfile)
		LoadInputFile(input_filename_.c_str());
}

bool checkArgOption(const char* arg_name, char* full_arg, int* arg_size) {
	*arg_size = strlen(arg_name);
	return strncmp(full_arg, arg_name, *arg_size) == 0 && (int)strlen(full_arg) > *arg_size;
}

#define CHECK_ARG(code, argName, conversionFunction) \
	if ( !recognized && checkArgOption(argName, argv[i], &arg_size)) { \
		code( conversionFunction(&argv[i][arg_size+1]) ); \
		recognized = true; \
	} \

#define CHECK_INT_ARG(code, argName)		CHECK_ARG(code, argName, atoi)
#define CHECK_DOUBLE_ARG(code, argName)		CHECK_ARG(code, argName, atof)

bool LoaderUtil::ParseOptions(int argc, char* argv[]) {
	int i, arg_size;
	bool got_input = false;

	config_ = new engine::EngineConfig();

	for (i=1; i<argc; i++) {
		bool recognized = false;

		CHECK_INT_ARG(config_->set_width, "-width")
		CHECK_INT_ARG(config_->set_height, "-height")
		CHECK_INT_ARG(vector_color_mode_ =, "-vcm")
		CHECK_INT_ARG(sphere_resolution_ =, "-sphereres")
		CHECK_INT_ARG(cylinder_resolution_ =, "-cylinderres")
		CHECK_DOUBLE_ARG(sphere_radius_factor_ =, "-srf")

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
                engine::Vector3D v(x,y,z);
				field_->SetVectorAtPos(i, j, k, v);
			}

	field_->Normalize();

	ifs.close();
}

void LoaderUtil::PrintUsage() {
	printf("MAC420 EP1  Usage:\n");
	printf("\t./ep1 <input_file> [OPTION1 | OPTION2 | ...]\n\n");
	printf("Where OPTIONi can be the following:\n");
	printf("\t-width=INTEGER  :  sets the window width in pixels [default = 1024]\n");
	printf("\t-height=INTEGER  :  sets the window height in pixels [default = 768]\n");
	printf("\t-vcm=INTEGER  :  sets the vector color mode [default = 0]\n");
	printf("\t\t%d = vector colors are based on their magnetude ( weaker = light blue / stronger = red).\n", VCM_MAGNETUDE);
	printf("\t\t%d = vector colors are based on their direction (vector components are color components).\n", VCM_DIRECTION);
	printf("\t\t%d = vector colors are randomly generated.\n", VCM_RANDOM);
	printf("\t-sphereres=INTEGER  :  sets the sphere resolution [default = 12]\n");
	printf("\t-cylinderres=INTEGER  :  sets the cylinder resolution [default = 10]\n");
	printf("\t-srf=DOUBLE  :  sets the factor by which default sphere radius will be multiplied [default = 1.0]\n");
}

}
