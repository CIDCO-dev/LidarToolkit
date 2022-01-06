#pragma once

class CalibrationConfiguration{

public:
	CalibrationConfiguration(){
	
	}
	
	CalibrationConfiguration(double xOffset,double yOffset,double zOffset,double headingBoresight,double pitchBoresight,double rollBoresight){
		offset << xOffset,yOffset,zOffset;
		boresight << headingBoresight,pitchBoresight,rollBoresight;
		recomputeBoresightMatrix();
	}	

	//TODO: encapsulate with accessors
	Eigen::Vector3d offset;
	double totalDistance;
	
	inline Eigen::Vector3d & getBoresight() { return boresight;}
	inline void setBoresight(Eigen::Vector3d & b) { boresight = b; }
	
	inline Eigen::Matrix3d & getBoresightMatrix(){ return boresightMatrix;}
	
	void recomputeBoresightMatrix(){
		//recompute boresight matrix
               double ch = cos(boresight[0]);
               double sh = sin(boresight[0]);
               double cp = cos(boresight[1]);
               double sp = sin(boresight[1]);
               double cr = cos(boresight[2]);
               double sr = sin(boresight[2]);

               boresightMatrix <<      ch*cp , ch*sp*sr - sh*cr  , ch*sp*cr + sh*sr,
                	 		sh*cp , sh*sp*sr + ch*cr  , sh*sp*cr - ch*sr,
                	 		-sp   , cp*sr             , cp*cr;		
	}
private:
	Eigen::Vector3d boresight; // (heading,pitch,roll)
	Eigen::Matrix3d boresightMatrix;
};
