dio320 = getdio32("LOG/Clyde",0);
dio321 = getdio32("LOG/Clyde",1);

printf("Show BOX ID\n")
id=[dio320(:,1)/16,dio321(:,1)/16,dio320(:,3)/16,dio321(:,3)/16];

id(1:10,:)


printf("Show ramp patterns\n")
ramps=[dio320(:,2),dio320(:,4),dio321(:,2),dio321(:,4)]

ramps(1:10,:)



