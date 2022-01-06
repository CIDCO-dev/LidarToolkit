#pragma once

#include <pcl/common/common_headers.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_sphere.h>
#include <pcl/point_cloud.h>

#include "Geodesy.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>

class SpherePoint{

public:

	//TODO encapsulate public attributes
	Eigen::Vector3d georeferencedPosition;


        double intensity;
        double lidarTime;

	Eigen::Vector3d lidarPing;

	//angles in radians
	//HPR angles
        double heading;
        double pitch;
        double roll;	

	//coordinates in radians
	double latitude;
        double longitude;
        double ellipsoidalHeight;



	Eigen::Vector3d positionEcef;
	
	void recompute(){
		ch = cos(heading);
		sh = sin(heading);
		cp = cos(pitch);
		sp = sin(pitch);
		cr = cos(roll);
		sr = sin(roll);
		
		Geodesy::getPositionECEF(positionEcef,longitude,latitude,ellipsoidalHeight);
	}

	
	inline double getHeading() { return heading;}
	inline double getPitch() { return pitch; }
	inline double getRoll()  { return roll; }

	inline double getCh() { return ch;}
	inline double getSh() { return sh;}

	inline double getCp() { return cp;}
	inline double getSp() { return sp;}

	inline double getCr() { return cr;}
	inline double getSr() { return sr;}

private:

	

	//trig for HPR angles
	double ch;
	double sh;
	double cp;
	double sp;
	double cr;
	double sr;

};

class SphereReader{
public:
        SphereReader(){

        }

        ~SphereReader(){
                for(auto i=spherePoints.begin();i != spherePoints.end();i++){
                        delete *i;
                }
        }

        static bool sortByTime(SpherePoint * p1,SpherePoint * p2){
                return p1->lidarTime < p2->lidarTime;
        }

        /* Read points from file and sort them by time */
        void read(std::string & fileName){
                std::ifstream inputFile;

                inputFile.open(fileName);

                if(inputFile.is_open()){
                        std::string line;
                        int lineCount = 0;

                        while(std::getline(inputFile,line)){
                                SpherePoint * p = new SpherePoint();

				/*double xTRF,yTRF,zTRF;
				double xLGF,yLGF,zLGF;
				double intensity;
				double lidarTime,lidarX,lidarY,lidarZ,heading,pitch,roll,latitude,longitude;
				*/

				if(sscanf(line.c_str(),"%*f %*f %*f %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&p->georeferencedPosition[0],&p->georeferencedPosition[1],&p->georeferencedPosition[2],&p->intensity,&p->lidarTime,&p->lidarPing[0],&p->lidarPing[1],&p->lidarPing[2],&p->heading,&p->pitch,&p->roll,&p->latitude,&p->longitude,&p->ellipsoidalHeight) == 14){
					//convert angles to radians
					p->heading = D2R(p->heading);
					p->pitch   = D2R(p->pitch);
					p->roll    = D2R(p->roll);
					p->latitude = D2R(p->latitude);
					p->longitude = D2R(p->longitude);
				
					p->recompute();
					spherePoints.push_back(p);
					lineCount++;
                                }
                                else{
#ifdef DEBUG
                                        std::cerr << "Skipping line" << std::endl;
#endif
                                }
                        }

                        if(lineCount > 0){
#ifdef DEBUG
                                std::cerr << lineCount << " lines loaded" << std::endl;
#endif
                        }
                        else{
                                throw std::domain_error("No lines found in file " + fileName);

                        }

			//std::cerr << "Loaded " << fileName << " (" << spherePoints.size() << " points loaded)" << std::endl;

                        inputFile.close();
                        std::sort (spherePoints.begin(),spherePoints.end(), &SphereReader::sortByTime);
                }
                else{
                        throw std::invalid_argument("Cannot open file " + fileName);
                }
        }

        void fit(){
                //LSQ Fit as B=Ax
                Eigen::MatrixXd A(spherePoints.size(),4);
                Eigen::VectorXd B(spherePoints.size(),1);

                int rowCount = 0;
                for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
                        A(rowCount,0)=(*i)->georeferencedPosition[0];
                        A(rowCount,1)=(*i)->georeferencedPosition[1];
                        A(rowCount,2)=(*i)->georeferencedPosition[2];
                        A(rowCount,3)=1;

                        B(rowCount)=(*i)->georeferencedPosition[0] * (*i)->georeferencedPosition[0]  + (*i)->georeferencedPosition[1] * (*i)->georeferencedPosition[1]   + (*i)->georeferencedPosition[2] * (*i)->georeferencedPosition[2];
                        rowCount++;
                }

                //LSQ QR
                Eigen::VectorXd x = A.colPivHouseholderQr().solve(B);

                //LSQ SVD
                //Eigen::MatrixXd x = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);

		center << x(0)/2.0 , x(1)/2.0, x(2)/2.0;

                radius  = sqrt(  x(3) + center[0]*center[0] + center[1]*center[1] + center[2]*center[2] );

		/*
                //Compute squared sum of errors
                sumSquaredErrors = 0;
                for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
			double error= ((*i)->xTRF - xCenter) * ((*i)->xTRF - xCenter) + ((*i)->yTRF - yCenter)*((*i)->yTRF - yCenter) + ((*i)->zTRF - zCenter)*((*i)->zTRF - zCenter) - radius * radius;
                        sumSquaredErrors += error * error;
                }
                */
        }

/*
        void fitKnownRadius(double & xCenter,double & yCenter,double & zCenter,double radius,double & sumSquaredErrors){
                //LSQ Fit as B=Ax
                Eigen::MatrixXd A(spherePoints.size(),4);
                Eigen::VectorXd B(spherePoints.size(),1);

                int rowCount = 0;
                for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
                        A(rowCount,0)=(*i)->xTRF;
                        A(rowCount,1)=(*i)->yTRF;
                        A(rowCount,2)=(*i)->zTRF;
                        A(rowCount,3)=1;

                        B(rowCount)=(*i)->xTRF * (*i)->xTRF  + (*i)->yTRF * (*i)->yTRF   + (*i)->zTRF * (*i)->zTRF + radius * radius;
                        rowCount++;
                }

                //LSQ QR
                //Eigen::VectorXd x = A.colPivHouseholderQr().solve(B);

                //LSQ SVD
                Eigen::MatrixXd x = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);

                xCenter = x(0)/2.0;
                yCenter = x(1)/2.0;
                zCenter = x(2)/2.0;

                sumSquaredErrors = 0;
                for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
			double error= ((*i)->xTRF - xCenter) * ((*i)->xTRF - xCenter) + ((*i)->yTRF - yCenter)*((*i)->yTRF - yCenter) + ((*i)->zTRF - zCenter)*((*i)->zTRF - zCenter) - radius * radius;
                        sumSquaredErrors += error * error;
                }
        }
*/
        /* Find sphere with RANSAC */
        bool find(double x,double y, double z,double radius){
                // TODO: maybe build point cloud outside of the method if many methods need it in the future ?
                pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

                // Generate pointcloud data
                cloud->width = spherePoints.size(); //Load this from whatever...
                cloud->height = 1;
                cloud->points.resize (cloud->width);

                int j = 0;
                for (auto i = spherePoints.begin();i != spherePoints.end();i++,j++){
                        (*cloud)[j].x =(*i)->georeferencedPosition[0];
                        (*cloud)[j].y =(*i)->georeferencedPosition[1];
                        (*cloud)[j].z =(*i)->georeferencedPosition[2];
                }

                pcl::SampleConsensusModelSphere<pcl::PointXYZ>::Ptr sphereModel(new pcl::SampleConsensusModelSphere<pcl::PointXYZ>(cloud));

                pcl::RandomSampleConsensus<pcl::PointXYZ> ransac (sphereModel);
                ransac.setDistanceThreshold (0.01); //FIXME: is 10mm enough/too little ? Adjust accordingly
                ransac.computeModel();

		//TODO: maybe we'd like to use the point count to see how good of a fit we have...
                //std::vector<int> inliers;
                //ransac.getInliers(inliers);

                Eigen::VectorXf modelParameters;
                ransac.getModelCoefficients(modelParameters);

		x=modelParameters[0];
		y=modelParameters[1];
		z=modelParameters[2];
		radius=modelParameters[3];

                return true;
        }

        void georeference(Eigen::Matrix3d & ecef2ned,Eigen::Vector3d & originNedEcef,Eigen::Matrix3d & boresight,Eigen::Vector3d & offset){

		for(auto i : spherePoints){
			//georeference point
			Eigen::Matrix3d imu2ned;

			imu2ned << 	i->getCh()*i->getCp(),   i->getCh()*i->getSp()*i->getSr()-i->getCr()*i->getSh(), i->getCh()*i->getCr()*i->getSp()+i->getSr()*i->getSh(),
    					i->getCp()*i->getSh(),   i->getCh()*i->getCr()+i->getSp()*i->getSr()*i->getSh(), i->getSh()*i->getCr()*i->getSp()-i->getCh()*i->getSr(),
    					-i->getSp(),          	i->getCp()*i->getSr(),        			     i->getCr()*i->getCp();

			//Compute total NED vector
			i->georeferencedPosition = ecef2ned * ( i->positionEcef-originNedEcef ) + imu2ned * (offset + boresight * i->lidarPing);

			//TRF Eigen::Vector3d georeferencedPosition = (*i)->positionEcef + nav2ecef * imu2nav  * (boresight * lidarX + offset);
		}

        }

        static double distance(SphereReader & s1,SphereReader & s2){
        	return (s1.getCenter()-s2.getCenter()).norm();
        }

        inline std::vector<SpherePoint*> & getPoints() { return spherePoints;}
	inline double getRadius(){ return radius;}

	inline Eigen::Vector3d & getCenter(){ return center;}

private:
        std::vector<SpherePoint*> spherePoints;
        
        //fit variables

	Eigen::Vector3d center;
        double radius;
};


