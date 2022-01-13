function nodes = buildDistanceNetwork(cells, knownRadius)
  nodes = [];
  
  for i = 1:size(cells)(2)
    xyz = cells{i};
    meanXYZ = mean(xyz);
    s1 = xyz - meanXYZ;
    [estimatedSphereCenter, residuals] = SphereFitFixedRadius(s1, knownRadius);
    nodes = [nodes ; estimatedSphereCenter' + meanXYZ];
  endfor
  
endfunction