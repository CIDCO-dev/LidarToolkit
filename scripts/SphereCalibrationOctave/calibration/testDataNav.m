filenamePlan1 = '..\q_25\passage_1\sphere1';
filenamePlan2 = '..\q_25\passage_1\sphere2';


dataPlan1 = readLibacBinaryData(filenamePlan1);
dataPlan2 = readLibacBinaryData(filenamePlan2);

time1 = dataPlan1(:,4);
r1 = dataPlan1(:,11);

time2 = dataPlan2(:,4);
r2 = dataPlan2(:,11);


plot(time1, r1)
plot(time2, r2)

