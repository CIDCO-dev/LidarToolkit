#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/SphereFitting.hpp"

TEST_CASE( "Read sphere file", "read()" ) {
	SphereReader sphere;
	std::string filename("../test/data/sphere1.txt");
	sphere.read(filename);

	REQUIRE(sphere.getPoints().size()==135);
}

TEST_CASE( "Fit LSQ sphere of unknown radius", "fit()" ) {
        SphereReader sphere;
        std::string filename("../test/data/sphere1.txt");
        sphere.read(filename);

        REQUIRE(sphere.getPoints().size()==135);

	sphere.fit();

	REQUIRE(sphere.getCenter()[0] == Approx(1551212.569));
	REQUIRE(sphere.getCenter()[1] == Approx(-3941554.217));
	REQUIRE(sphere.getCenter()[2] == Approx(4752477.434));
	REQUIRE(sphere.getRadius() == Approx(0.2420614591));
}


TEST_CASE( "Fit LSQ sphere of known radius", "fitKnownRadius()" ) {
	REQUIRE(2==1);

/*        SphereReader reader;
        std::string filename("../test/data/sphere1.txt");
        reader.read(filename);

        REQUIRE(reader.getPoints().size()==135);

        double x,y,z,radius,sse;

	radius = 0.25;
        reader.fitKnownRadius(x,y,z,radius,sse);

        REQUIRE(x == Approx(1551212.569));
        REQUIRE(y == Approx(-3941554.217));
        REQUIRE(z == Approx(4752477.434));
        REQUIRE(radius == Approx(0.2420614591));
        REQUIRE(sse == Approx(0.006924533));
*/
}


TEST_CASE( "Find sphere with RANSAC", "find()" ) {
    REQUIRE( 2 == 1 );
}
