function dcm = getDCM(rph)
  r = rph(1);
  p = rph(2);
  h = rph(3);
  
  sr = sin(r);
  cr = cos(r);
  
  sp = sin(p);
  cp = cos(p);
  
  sh = sin(h);
  ch = cos(h);
  
  dcm = [ cp*ch,  cr*sh+sr*sp*ch, -sr*sh+cr*sp*ch;...
          -cp*sh, ch*cr-sr*sp*sh, -sr*ch-cr*sp*sh;...
          -sp,    sr*cp,          cr*cp];
endfunction