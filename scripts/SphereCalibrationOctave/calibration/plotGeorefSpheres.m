 
params = [leverArmEstimation, boresightEstimation];
georefDataSphere1 = georef(cs1, params);
georefDataSphere2 = georef(cs2, params);
georefDataSphere3 = georef(cs3, params);

xyzs11 = georefDataSphere1{1};
xyzs12 = georefDataSphere1{2};

figure
scatter3(xyzs11(:,1), xyzs11(:,2), xyzs11(:,3));
hold
scatter3(xyzs12(:,1), xyzs12(:,2), xyzs12(:,3));


xyzs21 = georefDataSphere2{1};
xyzs22 = georefDataSphere2{2};

figure
scatter3(xyzs21(:,1), xyzs21(:,2), xyzs21(:,3));
hold
scatter3(xyzs22(:,1), xyzs22(:,2), xyzs22(:,3));


xyzs31 = georefDataSphere3{1};
xyzs32 = georefDataSphere3{2};

figure
scatter3(xyzs31(:,1), xyzs31(:,2), xyzs31(:,3));
hold
scatter3(xyzs32(:,1), xyzs32(:,2), xyzs32(:,3));