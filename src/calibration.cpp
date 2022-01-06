#include "GeneticFitter.hpp"

#include <list>

#include <filesystem>
#include <cstdlib>
#include <time.h>





int main(int argc,char** argv){
	if(argc != 0){
		try{
			srand (time(NULL));
			
			//FIXME: Read initial guess parameters from CLI
			//x -0.145596 -y 0.037658 -z 2.379063 -h 0.3992 -p -1.8681 -r 0.2159
			double x=-0.145596;
			double y=0.037658;
			double z=2.379063;
			double h=0.3992;
			double p=-1.8681;
			double r=0.2159;
			
			int c;			
			
			while ((c = getopt(argc, argv, "x:y:z:h:p:r:")) != -1) {
				int this_option_optind = optind ? optind : 1;

				switch (c) {
					case 'x':
						if(sscanf(optarg,"%lf",&x) != 1){
							std::cerr << "Invalid x offset parameter: " << optarg << std::endl;
						}
						break;
					case 'y':
						if(sscanf(optarg,"%lf",&y) != 1){
							std::cerr << "Invalid y offset parameter: " << optarg << std::endl;
						}
						break;
					case 'z':
						if(sscanf(optarg,"%lf",&z) != 1){
							std::cerr << "Invalid z offset parameter: " << optarg << std::endl;
						}
						break;
					case 'h':
						if(sscanf(optarg,"%lf",&h) != 1){
							std::cerr << "Invalid heading boresight parameter: " << optarg << std::endl;
						}
						break;
					case 'p':
						if(sscanf(optarg,"%lf",&p) != 1){
							std::cerr << "Invalid pitch boresight parameter: " << optarg << std::endl;
						}
						break;
					case 'r':
						if(sscanf(optarg,"%lf",&r) != 1){
							std::cerr << "Invalid roll boresight parameter: " << optarg << std::endl;
						}
						break;					
				}
			}			

			//Read spheres
			std::list<SphereReader*> spheres;

			std::string path(argv[1]);

			for (const auto & entry : std::filesystem::directory_iterator(path)){
				std::string filePath(entry.path());

				std::cerr << "[+] Loading " << filePath  << " ... ";
				
				SphereReader * reader = new SphereReader();

				reader->read(filePath);
				
				std::cerr << reader->getPoints().size() << " points loaded" << std::endl;
				
				spheres.push_back(reader);
			}


			std::cerr << "[*] " << spheres.size() << " sphere files loaded" << std::endl;

			//TODO: get guess from CLI
			CalibrationConfiguration * initialGuess = new CalibrationConfiguration(x,y,z,h,p,r);

			GeneticFitter fitter(initialGuess);

			fitter.fit(spheres);
		}
		catch(std::exception & e){
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	else{
		std::cout << "Usage: calibrate sphere-files-directory" << std::endl;
		return 1;
	}

	return 0;
}
