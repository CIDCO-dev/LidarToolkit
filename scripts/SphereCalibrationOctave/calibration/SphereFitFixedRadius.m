function [center,residuals] = SphereFitFixedRadius(xyz,r)
%
% Fit a sphere to data with a known radius using the least squares approach.
%
% Fits the equation of a sphere in Cartesian coordinates to a set of xyz
% data points by solving the overdetermined system of normal equations, i.e.
% x^2 + y^2 + z^2 + a*x + b*y + c*z + d = 0
% The least squares sphere has radius R = sqrt((a^2+b^2+c^2)/4-d) and
% center coordinates (x,y,z) = (-a/2,-b/2,-c/2).
%
% Input arguments:
% x,y,z:
%    Cartesian coordinates of noisy data points
% r:
%    Known radius of the sphere
%
% Output arguments:
% center:
%    Coordinates of the least-squares fit sphere center
% residuals:
%    Residuals in the radial direction
%
% Example:
% [center,residuals] = SphereFit(xyz, r)
%
% Copyright 2016 Michael Landry

% Equation of the sphere
f = @(C) sum((sum(bsxfun(@minus,xyz,C).^2,2)-r^2).^2);

% xyz @ C xyz - C

% Search for the center
center = (fminsearch(f, [0,0,0]))';

% Calculation of the residuals
residuals = r - sqrt(sum(bsxfun(@minus,xyz,center.').^2,2));

end

