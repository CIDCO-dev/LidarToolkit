function diffs = distanceNetworkRec(nodes)
  if (size(nodes)(1) > 2)
    recursive_diffs = bsxfun(@minus, nodes(2:end,:), nodes(1,:));
    diffs = [recursive_diffs ; distanceNetworkRec(nodes(2:end,:))];
  elseif (size(nodes)(1) == 2)
    diffs = bsxfun(@minus, nodes(2,:), nodes(1,:));
  else
    diffs = [];
  endif
endfunction