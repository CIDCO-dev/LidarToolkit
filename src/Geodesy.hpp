#pragma once

#include <cmath>

#include <Eigen/Dense>

#define D2R(x) (x * (M_PI/180))
#define R2D(x) (x * (180/M_PI))

class Geodesy{
public:
	/**WGS84 ellipsoid semi-major axis*/
	static constexpr double a = 6378137.0;

	/**WGS84 ellipsoid first eccentricity squared*/
	static constexpr double e2 = 0.081819190842622 * 0.081819190842622;


	static void getPositionECEF(Eigen::Vector3d & positionECEF, double longitude, double latitude,double ellipsoidalHeight) {
		double clat = cos(latitude);
		double slat = sin(latitude);
		double clon = cos(longitude);
		double slon = sin(longitude);

		double N = a / (sqrt(1 - e2 * slat * slat));
		double xTRF = (N + ellipsoidalHeight) * clat * clon;
		double yTRF = (N + ellipsoidalHeight) * clat * slon;
		double zTRF = (N * (1 - e2) + ellipsoidalHeight) * slat;

		positionECEF << xTRF, yTRF, zTRF;
	}
};
