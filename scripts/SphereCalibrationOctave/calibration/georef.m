function xyzCells = georef(data, platParams)
  
  leverArm = platParams(1:3)';
  
  boresight = platParams(4:6);
  boresightMatrix = getDCM(boresight);
  
  for i = 1:size(data)(1)
    xyz_i = [];
    
    for j = 1:size(data{i,2})(1)
      dataj = data{i,2}(j,:);
      xn = dataj(1:3)';
      xs = dataj(5:7)';
      rph = dataj(11:13);
      c_bI_n = getDCM(rph);
      pn = dataj(17:19)';
      newXn = pn + c_bI_n*(boresightMatrix*xs + leverArm);
      xyz_i = [xyz_i; newXn'];
    endfor
    
    xyzCells(i) = {xyz_i};
  endfor
endfunction