#include <igl/opengl/glfw/Viewer.h>
#include <igl/png/writePNG.h>
#include <igl/readOBJ.h>
#include <igl/readOFF.h>

#include <string>

bool endsWith(const std::string &str, const std::string &suffix) {
	return str.rfind(suffix) == (str.size() - suffix.size());
}

struct View {
	Eigen::MatrixX3d V;
	int mesh_id;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> R;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> G;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> B;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> A;
};

int main(int argc, char **argv) {
	if(argc != 2) return 1;
	std::string filename(argv[1]);

	Eigen::MatrixXd V;
	Eigen::MatrixXi F;

	std::string filename_lowercase(filename);
	std::transform(filename_lowercase.begin(), filename_lowercase.end(), filename_lowercase.begin(), [](unsigned char c) { return std::tolower(c); });

	if(endsWith(filename_lowercase, ".obj")) {
		igl::readOBJ(filename, V, F);
	} else if(endsWith(filename_lowercase, ".off")) {
		igl::readOFF(filename, V, F);
	} else {
		return 1;
	}
	std::string output_filename = filename.substr(0, filename.size() - 3) + "png";

	igl::opengl::glfw::Viewer viewer;
	viewer.core().is_animating = true;

	std::array<View, 4> views;

	Eigen::RowVector3d center = V.colwise().mean();
	V.rowwise() -= center;

	views[0].V = V;
	views[0].mesh_id = viewer.data().id;

	Eigen::Matrix3d rotation_x;
	rotation_x << 1, 0, 0,
	    0, 0, -1,
	    0, 1, 0;
	Eigen::Matrix3d rotation_y;
	rotation_y << 0, 0, 1,
	    0, 1, 0,
	    -1, 0, 0;
	Eigen::Matrix3d rotation_z;
	rotation_z << 0, -1, 0,
	    1, 0, 0,
	    0, 0, 1;

	views[1].mesh_id = viewer.append_mesh();
	views[1].V = V * rotation_x;
	views[2].mesh_id = viewer.append_mesh();
	views[2].V = V * rotation_y;
	views[3].mesh_id = viewer.append_mesh();
	views[3].V = V * rotation_z;

	for(short j = 0; j < 4; j++) {
		views[j].R.resize(512, 512);
		views[j].G.resize(512, 512);
		views[j].B.resize(512, 512);
		views[j].A.resize(512, 512);
		viewer.data(views[j].mesh_id).set_mesh(views[j].V, F);
		viewer.data(views[j].mesh_id).show_lines = false;
	}

	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> R(1024, 1024);
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> G(1024, 1024);
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> B(1024, 1024);
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> A(1024, 1024);

	viewer.callback_post_draw = [&](igl::opengl::glfw::Viewer &viewer) {
		for(short j = 0; j < 4; j++) {
			short row = j / 2;
			short col = j % 2;
			viewer.core().draw_buffer(viewer.data(views[j].mesh_id), false, views[j].R, views[j].G, views[j].B, views[j].A);
			R.block(512 * row, 512 * col, 512, 512) = views[j].R;
			G.block(512 * row, 512 * col, 512, 512) = views[j].G;
			B.block(512 * row, 512 * col, 512, 512) = views[j].B;
			A.block(512 * row, 512 * col, 512, 512) = views[j].A;
		}
		igl::png::writePNG(R, G, B, A, output_filename);
		return true;
	};

	viewer.core().camera_zoom *= 1.4;

	glfwInit();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	viewer.launch_init(false, false, "", 1024, 1024);
	viewer.launch_rendering(false);
}
