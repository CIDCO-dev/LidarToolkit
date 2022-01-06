 
params = [leverArmEstimation, boresightEstimation];
georefDataSphere1 = georef(cs1, params);
georefDataSphere2 = georef(cs2, params);
georefDataSphere3 = georef(cs3, params);
georefDataSphere4 = georef(cs4, params);
georefDataSphere5 = georef(cs5, params);
georefDataSphere6 = georef(cs6, params);

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


xyzs41 = georefDataSphere4{1};
xyzs42 = georefDataSphere4{2};

figure
scatter3(xyzs41(:,1), xyzs41(:,2), xyzs41(:,3));
hold
scatter3(xyzs42(:,1), xyzs42(:,2), xyzs42(:,3));


xyzs51 = georefDataSphere5{1};
xyzs52 = georefDataSphere5{2};

figure
scatter3(xyzs51(:,1), xyzs51(:,2), xyzs51(:,3));
hold
scatter3(xyzs52(:,1), xyzs52(:,2), xyzs52(:,3));


xyzs61 = georefDataSphere6{1};
xyzs62 = georefDataSphere6{2};

figure
scatter3(xyzs61(:,1), xyzs61(:,2), xyzs61(:,3));
hold
scatter3(xyzs62(:,1), xyzs62(:,2), xyzs62(:,3));