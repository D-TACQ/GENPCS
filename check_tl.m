fp=fopen("LOG/Clyde.0.stats.TL","r")
tl=fread(fp,Inf,"uint32");
fclose(fp);
ll=length(tl);
plot(tl(2:ll)-tl(1:ll-1))
set(gca,"yscale","lin")
input("Hit return for histogram")
hist(tl(2:ll)-tl(1:ll-1))
set(gca,"yscale","log")

