function dio32 = getdio32(flavour, lun)
	fn = sprintf("%s.%d.stats.DI32", flavour, lun);
	fp = fopen(fn,"r");
	raw = fread(fp,Inf,"uint8");
	ll = length(raw);
	dio32 = transpose(reshape(raw,4,ll/4));
endfunction
