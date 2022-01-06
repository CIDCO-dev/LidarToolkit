function residuals = calculateResidus(sphere, r)
  xyzStack = [];
  
  for i = 1:size(sphere)(2)
    xyz = sphere{i};
    xyzStack = [xyzStack ; xyz];
  endfor
  
  [estimatedSphereCenter, residuals] = SphereFitFixedRadius(xyzStack, r);
  
  
endfunction