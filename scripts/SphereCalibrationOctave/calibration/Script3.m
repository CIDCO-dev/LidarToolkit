


filenamePlan1 = '..\spherePlan\spherePlan_1';
filenamePlan2 = '..\spherePlan\spherePlan_2';
filenamePlan3 = '..\spherePlan\spherePlan_3';
filenamePlan4 = '..\spherePlan\spherePlan_4';
filenamePlan5 = '..\spherePlan\spherePlan_5';
filenamePlan6 = '..\spherePlan\spherePlan_6';

dataPlan1 = readLibacBinaryData(filenamePlan1);
dataPlan2 = readLibacBinaryData(filenamePlan2);
dataPlan3 = readLibacBinaryData(filenamePlan3);
dataPlan4 = readLibacBinaryData(filenamePlan4);
dataPlan5 = readLibacBinaryData(filenamePlan5);
dataPlan6 = readLibacBinaryData(filenamePlan6);

cPlan = {filenamePlan1, dataPlan1};
cPlan(2,:) = {filenamePlan2, dataPlan2};
cPlan(3,:) = {filenamePlan3, dataPlan3};
cPlan(4,:) = {filenamePlan4, dataPlan4};
cPlan(5,:) = {filenamePlan5, dataPlan5};
cPlan(6,:) = {filenamePlan6, dataPlan6};

##cTest = {filenamePlan1, dataPlan1(1,:)};
##cTest2 = {filenamePlan1, dataPlan1(:,:)};
##
####a_X,-0.1438
####a_Y,0.0225
####a_Z,2.374
##
##
##platParams = [-0.1438, 0.0225, 2.374, 0, 0, 0];
##cXYZ = georef(cTest2, platParams);
##
##dataXYZinit = cPlan{1,2}(:,1:3);
##dataXYZfin = cXYZ(1);
##
##dataXYZinit - dataXYZinit




filenameDrapeau1 = '..\sphereDrapeau\sphere_Drapeau_1';
filenameDrapeau2 = '..\sphereDrapeau\sphere_Drapeau_2';
filenameDrapeau3 = '..\sphereDrapeau\sphere_Drapeau_3';
filenameDrapeau4 = '..\sphereDrapeau\sphere_Drapeau_4';
filenameDrapeau5 = '..\sphereDrapeau\sphere_Drapeau_5';
filenameDrapeau6 = '..\sphereDrapeau\sphere_Drapeau_6';

dataDrapeau1 = readLibacBinaryData(filenameDrapeau1);
dataDrapeau2 = readLibacBinaryData(filenameDrapeau2);
dataDrapeau3 = readLibacBinaryData(filenameDrapeau3);
dataDrapeau4 = readLibacBinaryData(filenameDrapeau4);
dataDrapeau5 = readLibacBinaryData(filenameDrapeau5);
dataDrapeau6 = readLibacBinaryData(filenameDrapeau6);

cDrapeau = {filenameDrapeau1, dataDrapeau1};
cDrapeau(2,:) = {filenameDrapeau2, dataDrapeau2};
cDrapeau(3,:) = {filenameDrapeau3, dataDrapeau3};
cDrapeau(4,:) = {filenameDrapeau4, dataDrapeau4};
cDrapeau(5,:) = {filenameDrapeau5, dataDrapeau5};
cDrapeau(6,:) = {filenameDrapeau6, dataDrapeau6};

##georefDataSphere1 = georef(cPlan, platParams);

initialPlatParams = [-0.1438, 0.0225, 2.374];
r = 0.19;
[leverArmEstimation] = twoSphereCalib(initialPlatParams, cPlan, cDrapeau, r)
initialPlatParams



