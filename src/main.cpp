#include <pcl/common/common_headers.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_sphere.h>
#include <pcl/point_cloud.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>

typedef struct{
	double xTRF;
	double yTRF;
	double zTRF;
	double xLGF;
	double yLGF;
	double zLGF;
	double intensity;
	double lidarTime;
	double lidarX;
	double lidarY;
	double lidarZ;
	double heading;
	double pitch;
	double roll;
	double latitude;
	double longitude;
} SpherePoint;

class SphereReader{
public:
	SphereReader(){

	}

	~SphereReader(){
		for(auto i=spherePoints.begin();i != spherePoints.end();i++){
			free(*i);
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
				SpherePoint * p = (SpherePoint*) malloc(sizeof(SpherePoint));

				if(sscanf(line.c_str(),"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&p->xTRF,&p->yTRF,&p->zTRF,&p->xLGF,&p->yLGF,&p->zLGF,&p->intensity,&p->lidarTime,&p->lidarX,&p->lidarY,&p->lidarZ,&p->heading,&p->pitch,&p->roll,&p->latitude,&p->longitude) == 16){
					spherePoints.push_back(p);
					lineCount++;
				}
				else{
					std::cerr << "Skipping line" << std::endl;
				}
			}

			if(lineCount > 0){
				std::cerr << lineCount << " lines loaded" << std::endl;
			}
			else{
				throw std::domain_error("No lines found in file " + fileName);
			}

			inputFile.close();
			std::sort (spherePoints.begin(),spherePoints.end(), &SphereReader::sortByTime);
		}
		else{
			throw std::invalid_argument("Cannot open file " + fileName);
		}
	}

	void fit(double & xCenter,double & yCenter,double & zCenter,double & radius,double & sumSquaredErrors){
		//LSQ Fit as B=Ax
		Eigen::MatrixXd	A(spherePoints.size(),4);
		Eigen::VectorXd B(spherePoints.size(),1);

		int rowCount = 0;
		for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
			A(rowCount,0)=(*i)->xLGF;
			A(rowCount,1)=(*i)->yLGF;
			A(rowCount,2)=(*i)->zLGF;
			A(rowCount,3)=1;

			B(rowCount)=(*i)->xLGF * (*i)->xLGF  + (*i)->yLGF * (*i)->yLGF   + (*i)->zLGF * (*i)->zLGF;
			rowCount++;
		}

		//LSQ QR
		Eigen::VectorXd x = A.colPivHouseholderQr().solve(B);

		//LSQ SVD
		//Eigen::MatrixXd x = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);

		xCenter = x(0)/2.0;
		yCenter = x(1)/2.0;
		zCenter = x(2)/2.0;
		radius  = sqrt(  x(3) + xCenter*xCenter + yCenter*yCenter + zCenter*zCenter );

		//Compute squared sum of errors
		sumSquaredErrors = 0;
		for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
			double error= ((*i)->xLGF - xCenter) * ((*i)->xLGF - xCenter) + ((*i)->yLGF - yCenter)*((*i)->yLGF - yCenter) + ((*i)->zLGF - zCenter)*((*i)->zLGF - zCenter) - radius * radius;
			sumSquaredErrors += error * error;
		}
	}

        void fitKnownRadius(double & xCenter,double & yCenter,double & zCenter,double radius,double & sumSquaredErrors){
                //LSQ Fit as B=Ax
                Eigen::MatrixXd A(spherePoints.size(),4);
                Eigen::VectorXd B(spherePoints.size(),1);

                int rowCount = 0;
                for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
                        A(rowCount,0)=(*i)->xLGF;
                        A(rowCount,1)=(*i)->yLGF;
                        A(rowCount,2)=(*i)->zLGF;
                        A(rowCount,3)=1;

                        B(rowCount)=(*i)->xLGF * (*i)->xLGF  + (*i)->yLGF * (*i)->yLGF   + (*i)->zLGF * (*i)->zLGF + radius * radius;
                        rowCount++;
                }

                //LSQ QR
                //Eigen::VectorXd x = A.colPivHouseholderQr().solve(B);

                //LSQ SVD
                Eigen::MatrixXd x = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);

                xCenter = x(0)/2.0;
                yCenter = x(1)/2.0;
                zCenter = x(2)/2.0;
                //radius  = sqrt(  x(3) + xCenter*xCenter + yCenter*yCenter + zCenter*zCenter );

		sumSquaredErrors = 0;
                for(auto i=spherePoints.begin();i!=spherePoints.end();i++){
                        double error= ((*i)->xLGF - xCenter) * ((*i)->xLGF - xCenter) + ((*i)->yLGF - yCenter)*((*i)->yLGF - yCenter) + ((*i)->zLGF - zCenter)*((*i)->zLGF - zCenter) - radius * radius;
                        sumSquaredErrors += error * error;
                }
        }

	/* Find sphere with RANSAC */
	bool find(){
		// TODO: maybe build point cloud outside of the method if many methods need it in the future ?
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

		// Generate pointcloud data
		cloud->width = spherePoints.size(); //Load this from whatever...
		cloud->height = 1;
		cloud->points.resize (cloud->width);

		int j = 0;
		for (auto i = spherePoints.begin();i != spherePoints.end();i++,j++){
			(*cloud)[j].x =(*i)->xLGF;
			(*cloud)[j].y =(*i)->yLGF;
			(*cloud)[j].z =(*i)->zLGF;
		}

		pcl::SampleConsensusModelSphere<pcl::PointXYZ>::Ptr sphereModel(new pcl::SampleConsensusModelSphere<pcl::PointXYZ>(cloud));

		pcl::RandomSampleConsensus<pcl::PointXYZ> ransac (sphereModel);
		ransac.setDistanceThreshold (0.01); //FIXME: is 10mm enough/too little ? Adjust accordingly
		ransac.computeModel();

		std::vector<int> inliers;

		ransac.getInliers(inliers);

		Eigen::VectorXf modelParameters;
		ransac.getModelCoefficients(modelParameters);


		std::cout << inliers.size() << " points in sphere (" << modelParameters[0]  << "," << modelParameters[1] << "," << modelParameters[2] << ") radius = " << modelParameters[3] << std::endl;

		//boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGBNormal> > cloud(scanMesh->cloud);
		/*
		pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
		pcl::SACSegmentationFromNormals<pcl::PointXYZRGBNormal, pcl::PointXYZRGBNormal> segmentation;
		segmentation.setInputCloud(cloud2);
		segmentation.setInputNormals(cloud2);
		segmentation.setModelType(pcl::SACMODEL_SPHERE);
		segmentation.setMethodType(pcl::SAC_RANSAC);
		segmentation.setDistanceThreshold(0.01);
		segmentation.setOptimizeCoefficients(true);
		segmentation.setRadiusLimits(radius - 0.005, radius + 0.005);
		segmentation.setEpsAngle(15 / (180/3.141592654));
		segmentation.setMaxIterations(1000000);

		pcl::PointIndices inlierIndices;
		segmentation.segment(inlierIndices, *coefficients);

		if (inlierIndices.indices.size() == 0){
			return false;
		}
		else{
			ROS_INFO("RANSAC found shape with [%d] points", (int)inlierIndices.indices.size());
			for (int c=0; c<coefficients->values.size(); ++c){
				ROS_INFO("Coeff %d = [%f]", (int)c+1, (float)coefficients->values[c]);

				// mark the found inliers in green
				for (int m=0; m<inlierIndices.indices.size(); ++m){
					cloud2->points[inlierIndices.indices[m]].r = 0;
					cloud2->points[inlierIndices.indices[m]].g = 255;
					cloud2->points[inlierIndices.indices[m]].b = 0;
				}
			}
		}

		*/

		return true;
	}

	std::vector<SpherePoint*> & getPoints() { return spherePoints;}

private:
	std::vector<SpherePoint*> spherePoints;
};

int main(int argc,char** argv){
	if(argc == 2){
		try{
			std::string fileName(argv[1]);

			SphereReader reader;
			reader.read(fileName);

			printf("Unknown radius fit\n");
			double x,y,z,r,sse;
			reader.find();
			reader.fit(x,y,z,r,sse);
			printf("Sphere at (%.3lf,%.3lf,%.3lf,r=%.3lf,sse=%.2lf)\n",x,y,z,r,sse);

			printf("Known radius fit\n");
			r = 0.250;
			reader.fitKnownRadius(x,y,z,r,sse);
			printf("Sphere at (%.3lf,%.3lf,%.3lf,r=%.3lf,sse=%.2lf)\n",x,y,z,r,sse);
			//TODO: use find/fit functions to tweak calibration parameters
		}
		catch(std::exception & e){
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	else{
		std::cout << "Usage: calibrate sphere-file" << std::endl;
		return 1;
	}

	return 0;
}
