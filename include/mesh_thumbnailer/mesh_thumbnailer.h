#pragma once
#include <Eigen/Eigen>

namespace mesh_thumbnailer {
	typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> ColorMatrix;
	struct RGBA {
		RGBA() {}
		RGBA(int width, int height) {
			resize(width, height);
		}
		void resize(int width, int height) {
			R.resize(width, height);
			G.resize(width, height);
			B.resize(width, height);
			A.resize(width, height);
		}
		ColorMatrix R, G, B, A;
	};

	RGBA create_thumbnail(Eigen::MatrixXd V, Eigen::MatrixXi F, int width, int height);
}