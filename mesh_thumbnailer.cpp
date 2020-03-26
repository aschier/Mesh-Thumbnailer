#include <mesh_thumbnailer/mesh_thumbnailer.h>

#include <igl/opengl/glfw/Viewer.h>
#include <igl/png/writePNG.h>
#include <igl/readOBJ.h>
#include <igl/readOFF.h>

using namespace mesh_thumbnailer;

struct View {
	Eigen::MatrixX3d V;
	int mesh_id;
	RGBA rgba;
};

RGBA mesh_thumbnailer::create_thumbnail(Eigen::MatrixXd V, Eigen::MatrixXi F, int width, int height) {
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
		views[j].rgba.resize(width / 2, height / 2);
		viewer.data(views[j].mesh_id).set_mesh(views[j].V, F);
		viewer.data(views[j].mesh_id).show_lines = false;
	}

	RGBA result(width, height);

	viewer.callback_post_draw = [&](igl::opengl::glfw::Viewer &viewer) {
		for(short j = 0; j < 4; j++) {
			short row = j / 2;
			short col = j % 2;
			viewer.core().draw_buffer(viewer.data(views[j].mesh_id), false,
			views[j].rgba.R, views[j].rgba.G, views[j].rgba.B, views[j].rgba.A);
			int view_width = width / 2;
			int view_height = height / 2;
			result.R.block(view_width * row, view_height * col, view_width, view_height) = views[j].rgba.R;
			result.G.block(view_width * row, view_height * col, view_width, view_height) = views[j].rgba.G;
			result.B.block(view_width * row, view_height * col, view_width, view_height) = views[j].rgba.B;
			result.A.block(view_width * row, view_height * col, view_width, view_height) = views[j].rgba.A;
		}
		return true;
	};

	viewer.core().camera_zoom *= 1.4;

	glfwInit();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	viewer.launch_init(false, false, "", 1024, 1024);
	viewer.launch_rendering(false);

	return result;
}
