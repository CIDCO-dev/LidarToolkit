function writeZF(data, filename)
  x_lidar = data(:,5);
  y_lidar = data(:,6);
  z_lidar = data(:,7);
  time = data(:,4);
  intensity = repmat(0,size(time)(1),1);
  
  lidarData = [x_lidar, y_lidar, z_lidar, intensity, time];
  
  dlmwrite (filename, lidarData, "delimiter", " ", "precision", 15);
endfunction