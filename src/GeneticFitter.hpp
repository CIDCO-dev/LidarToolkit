#pragma once

#include <random>

#include "Geodesy.hpp"
#include "CalibrationConfiguration.hpp"
#include "SphereFitting.hpp"
#include <list>

#define POPULATION_SIZE 100

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

//decimate X% of the total population
#define DECIMATION_PERCENTAGE 75



class GeneticFitter{
public:
	GeneticFitter(CalibrationConfiguration * initialGuess){
		population.push_back(initialGuess);
		repopulate();
	}

	void repopulate(){
		while(population.size() < POPULATION_SIZE){
			CalibrationConfiguration * newConfiguration = new CalibrationConfiguration();

			if(population.size() >= 2){
				//pick 2 an crossover
				CalibrationConfiguration * a = *select_randomly(population.begin(),population.end());
				CalibrationConfiguration * b = NULL;

				do{
					b = *select_randomly(population.begin(),population.end());
				}while(a==b);

				crossover(*a,*b,*newConfiguration);
			}
			else{
				//pick 1 and copy its content
				CalibrationConfiguration * a = *select_randomly(population.begin(),population.end());
				newConfiguration->offset    = a->offset;
				newConfiguration->setBoresight(a->getBoresight());
				newConfiguration->recomputeBoresightMatrix();
				newConfiguration->totalDistance = a->totalDistance;
			}

			//mutate and put into population
			mutate(*newConfiguration);
			population.push_back(newConfiguration);
		}
	}

	void crossover(CalibrationConfiguration & a,CalibrationConfiguration & b,CalibrationConfiguration & out){
		out.offset[0]  = (rand() % 100 > 50) ? a.offset[0]  : b.offset[0];
		out.offset[1]  = (rand() % 100 > 50) ? a.offset[1]  : b.offset[1];
		out.offset[2]  = (rand() % 100 > 50) ? a.offset[2]  : b.offset[2];
		
		out.getBoresight()[0]  = (rand() % 100 > 50) ? a.getBoresight()[0]  : b.getBoresight()[0];
		out.getBoresight()[1]  = (rand() % 100 > 50) ? a.getBoresight()[1]  : b.getBoresight()[1];
		out.getBoresight()[2]  = (rand() % 100 > 50) ? a.getBoresight()[2]  : b.getBoresight()[2];
		
		out.recomputeBoresightMatrix();
	}

	void mutate(CalibrationConfiguration & config){
		double boresightChanged = false;
		double probability = 50;

		double deltaOffset = (double)(rand() % 1000) / (double) 1000.0;
		
		config.offset[0]  = (rand() % 100 > probability) ? config.offset[0]  + ((rand() % 100 > 50)?1:-1) * deltaOffset : config.offset[0];
		config.offset[1]  = (rand() % 100 > probability) ? config.offset[1]  + ((rand() % 100 > 50)?1:-1) * deltaOffset : config.offset[1];
		config.offset[2]  = (rand() % 100 > probability) ? config.offset[2]  + ((rand() % 100 > 50)?1:-1) * deltaOffset : config.offset[2];
		
		double deltaBoresight = (double)(rand() % 10000) / (double) 10000.0;
		
		if(rand() % 100 > probability){
			config.getBoresight()[0] = config.getBoresight()[0] + ((rand() % 100 > 50)?1:-1) * deltaBoresight;
			boresightChanged = true;
		}
		
		if(rand() % 100 > probability){
			config.getBoresight()[1] = config.getBoresight()[1] + ((rand() % 100 > 50)?1:-1) * deltaBoresight;
			boresightChanged = true;
		}

		if(rand() % 100 > probability){
			config.getBoresight()[2] = config.getBoresight()[2] + ((rand() % 100 > 50)?1:-1) * deltaBoresight;
			boresightChanged = true;
		}
		
		if(boresightChanged){
			config.recomputeBoresightMatrix();
		}
	}

	//Sort in ascending order of fitness and kill off the weaker ones
	void decimate(){
		//ascending sort by distance
		population.sort(&GeneticFitter::sortByTotalDistance);

		fprintf(stderr,"[%d] X: %0.3f Y=%0.3f Z=%0.3f dH=%0.3f dP=%0.3f dR=%0.3f (Loss: %.2f) \n",generationCount,population.front()->offset[0],population.front()->offset[1],population.front()->offset[2],R2D(population.front()->getBoresight()[0]),R2D(population.front()->getBoresight()[1]),R2D(population.front()->getBoresight()[2]),population.front()->totalDistance);
		
		int decimationCount =0;

		//exterminate!
		while(decimationCount < DECIMATION_PERCENTAGE * population.size()){
			CalibrationConfiguration * dead = population.back();
			population.pop_back();

			delete dead;

			decimationCount++;
		}
	}

	static bool sortByTotalDistance(CalibrationConfiguration * a,CalibrationConfiguration * b){
		return a->totalDistance < b->totalDistance;
	}
	
	void initLocalFrame(std::list<SphereReader*> & spheres){
		//compute centroid
		Eigen::Vector3d centroidGeodesic(0,0,0);
		originNedEcef << 0,0,0;
		
		int pointCount = 0;
		
		for(auto i = spheres.begin() ; i!=spheres.end() ; i++){
			for(auto j = (*i)->getPoints().begin(); j!= (*i)->getPoints().end(); j++ ){
				centroidGeodesic[0] += (*j)->latitude;
				centroidGeodesic[1] += (*j)->longitude;
				centroidGeodesic[2] += (*j)->ellipsoidalHeight;
				pointCount++;
			}
		}
		
		centroidGeodesic /= (double) pointCount; 
		
		Geodesy::getPositionECEF(originNedEcef,centroidGeodesic[0],centroidGeodesic[1],centroidGeodesic[2]);
	
		//Compute ECEF to NED transform corresponding to the centroid
		double clat = cos(centroidGeodesic[0]);
		double slat = sin(centroidGeodesic[0]);
		double clon = cos(centroidGeodesic[1]);
		double slon = sin(centroidGeodesic[1]);
		
		//FIXME: this is actually a NED to ECEF transform matrix, thuis the need to transpose after...
		ecef2ned << 	-clon*slat,-slon,-clat*clon,
				-slat*slon,clon,-clat*slon,
    				clat,0,-slat;
    				
		ecef2ned.transposeInPlace();
	}

	void fit(std::list<SphereReader*> & spheres){
	
		initLocalFrame(spheres);
		generationCount = 0;
		
	        do{
			//For every configuration, re-georeference
			for(auto i = population.begin();i!=population.end();i++){
	        		//Evaluate fitness of config
				(*i)->totalDistance =  computeTotalDistance(**i,spheres);
			}

			decimate();
			repopulate();

			generationCount++;
                }while( generationCount < 10000); //TODO threshold distance 
	}

	double computeTotalDistance(CalibrationConfiguration & config, std::list<SphereReader*> & spheres){
		double distance=0.0;

		//georeference and fit every sphere
		for(auto i = spheres.begin() ; i!=spheres.end() ; i++){
			(*i)->georeference(ecef2ned,originNedEcef,config.getBoresightMatrix(),config.offset);
			(*i)->fit();
		}

		//add the distance between each distinct pair to the total distance heuristic
		for(auto i= spheres.begin();i!=spheres.end();i++){
			for(auto j =std::next(i); j != spheres.end() ; j++){
				distance += SphereReader::distance(**i,**j);
			}
		}


		return distance;
	}

private:
	Eigen::Matrix3d ecef2ned;
	Eigen::Vector3d originNedEcef;
	
	std::list<CalibrationConfiguration*> population;
	unsigned int generationCount =0;
	double lastLoss = 0;
	int    staleGenerations = 0;
};
