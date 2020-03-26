#include <igl/png/writePNG.h>
#include <igl/readOBJ.h>
#include <igl/readOFF.h>

#include <mesh_thumbnailer/mesh_thumbnailer.h>

#include <string>

bool endsWith(const std::string &str, const std::string &suffix) {
	return str.rfind(suffix) == (str.size() - suffix.size());
}

int main(int argc, char **argv) {
	if(argc != 2) return 1;
	std::string filename(argv[1]);

	std::string filename_lowercase(filename);
	std::transform(filename_lowercase.begin(), filename_lowercase.end(), filename_lowercase.begin(), [](unsigned char c) { return std::tolower(c); });
	std::string output_filename = filename.substr(0, filename.size() - 3) + "png";

	Eigen::MatrixXd V;
	Eigen::MatrixXi F;

	if(endsWith(filename_lowercase, ".obj")) {
		igl::readOBJ(filename, V, F);
	} else if(endsWith(filename_lowercase, ".off")) {
		igl::readOFF(filename, V, F);
	} else {
		return 1;
	}

	mesh_thumbnailer::RGBA thumbnail = mesh_thumbnailer::create_thumbnail(V, F, 1024, 1024);
	igl::png::writePNG(thumbnail.R, thumbnail.G, thumbnail.B, thumbnail.A, output_filename);

	return 0;
}
