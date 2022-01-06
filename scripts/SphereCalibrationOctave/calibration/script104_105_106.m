#quai 104 105 106
filenameSphere1Passage1 = '..\q_104_105_106\passage_1\sphere1';
s1p1 = readLibacBinaryData(filenameSphere1Passage1);

filenameSphere1Passage2 = '..\q_104_105_106\passage_2\sphere1';
s1p2 = readLibacBinaryData(filenameSphere1Passage2);

cs1 = {filenameSphere1Passage1, s1p1};
cs1(2,:) = {filenameSphere1Passage2, s1p2};

filenameSphere2Passage1 = '..\q_104_105_106\passage_1\sphere2';
s2p1 = readLibacBinaryData(filenameSphere2Passage1);

filenameSphere2Passage2 = '..\q_104_105_106\passage_2\sphere2';
s2p2 = readLibacBinaryData(filenameSphere2Passage2);

cs2 = {filenameSphere2Passage1, s2p1};
cs2(2,:) = {filenameSphere2Passage2, s2p2};

filenameSphere3Passage1 = '..\q_104_105_106\passage_1\sphere3';
s3p1 = readLibacBinaryData(filenameSphere3Passage1);

filenameSphere3Passage2 = '..\q_104_105_106\passage_2\sphere3';
s3p2 = readLibacBinaryData(filenameSphere3Passage2);

cs3 = {filenameSphere3Passage1, s3p1};
cs3(2,:) = {filenameSphere3Passage2, s3p2};


filenameSphere4Passage1 = '..\q_25\passage_1\sphere1';
s4p1 = readLibacBinaryData(filenameSphere4Passage1);

filenameSphere4Passage2 = '..\q_25\passage_2\sphere1';
s4p2 = readLibacBinaryData(filenameSphere4Passage2);

cs4 = {filenameSphere4Passage1, s4p1};
cs4(2,:) = {filenameSphere4Passage2, s4p2};

#quai 25
filenameSphere5Passage1 = '..\q_25\passage_1\sphere2';
s5p1 = readLibacBinaryData(filenameSphere5Passage1);

filenameSphere5Passage2 = '..\q_25\passage_2\sphere2';
s5p2 = readLibacBinaryData(filenameSphere5Passage2);

cs5 = {filenameSphere5Passage1, s5p1};
cs5(2,:) = {filenameSphere5Passage2, s5p2};

#quai 26
filenameSphere6Passage1 = '..\q_26\passage_1\sphere1';
s6p1 = readLibacBinaryData(filenameSphere6Passage1);

filenameSphere6Passage2 = '..\q_26\passage_2\sphere1';
s6p2 = readLibacBinaryData(filenameSphere6Passage2);

cs6 = {filenameSphere6Passage1, s6p1};
cs6(2,:) = {filenameSphere6Passage2, s6p2};

initialPlatParams = [-0.185596, 0.037658, 2.419063, deg2rad(-0.0759), deg2rad(-2.2592), deg2rad(0.3156)];
r = 0.1;
#[leverArmEstimation, boresightEstimation] = twoSphereCalib(initialPlatParams, cs1, cs3, r)
[leverArmEstimation, boresightEstimation] = threeSphereCalib(initialPlatParams, cs1, cs2, cs3, r)

#[leverArmEstimation, boresightEstimation] = sixSphereCalib(initialPlatParams, cs1, cs2, cs3, cs4, cs5, cs6, r)

#plot6Spheres
#plot6GeorefSpheres