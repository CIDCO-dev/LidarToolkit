function [leverArmEstimation, boresightEstimation] = twoSphereCalib(initialPlatParams, dataSphere1, dataSphere2, dataSphere3, dataSphere4, dataSphere5, r)
  initialPlatParams
  
  function cost = f(platParams)
    platParams
    georefDataSphere1 = georef(dataSphere1, platParams);
    nodesSphere1 = buildDistanceNetwork(georefDataSphere1, r);
    diffsSphere1 = distanceNetworkRec(nodesSphere1);
    c1 = sum(sqrt(sum(diffsSphere1.^2, 2)));
    
    georefDataSphere2 = georef(dataSphere2, platParams);
    nodesSphere2 = buildDistanceNetwork(georefDataSphere2, r);
    diffsSphere2 = distanceNetworkRec(nodesSphere2);
    c2 = sum(sqrt(sum(diffsSphere2.^2, 2)));
    
    georefDataSphere3 = georef(dataSphere3, platParams);
    nodesSphere3 = buildDistanceNetwork(georefDataSphere3, r);
    diffsSphere3 = distanceNetworkRec(nodesSphere3);
    c3 = sum(sqrt(sum(diffsSphere3.^2, 2)));
    
    georefDataSphere4 = georef(dataSphere4, platParams);
    nodesSphere4 = buildDistanceNetwork(georefDataSphere4, r);
    diffsSphere4 = distanceNetworkRec(nodesSphere4);
    c4 = sum(sqrt(sum(diffsSphere4.^2, 2)));
    
    georefDataSphere5 = georef(dataSphere5, platParams);
    nodesSphere5 = buildDistanceNetwork(georefDataSphere5, r);
    diffsSphere5 = distanceNetworkRec(nodesSphere5);
    c5 = sum(sqrt(sum(diffsSphere5.^2, 2)));
    
    residu1 = calculateResidus(georefDataSphere1, r);
    residu2 = calculateResidus(georefDataSphere2, r);
    residu3 = calculateResidus(georefDataSphere3, r);
    residu4 = calculateResidus(georefDataSphere4, r);
    residu5 = calculateResidus(georefDataSphere5, r);
    
    cost = c1 + c2 + c3 + c4 + c5 + 10*sum(residu1.^2) + 10*sum(residu2.^2) + 10*sum(residu3.^2) + 10*sum(residu4.^2) + 10*sum(residu5.^2)
  endfunction
  
  
  platParamsEstimated = (fminsearch(@f, initialPlatParams))';
  
  
  leverArmEstimation = platParamsEstimated(1:3);
  boresightEstimation = platParamsEstimated(4:6);
    
endfunction
