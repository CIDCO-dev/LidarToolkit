filenames1 = '..\q_29\sphere1';
filenames2 = '..\q_29\sphere2';
filenames3 = '..\q_29\sphere3';

datas1 = readLibacBinaryData(filenames1);
datas2 = readLibacBinaryData(filenames2);
datas3 = readLibacBinaryData(filenames3);

writeZF(datas1, '..\q_29\sphere1.txt');
writeZF(datas2, '..\q_29\sphere2.txt');
writeZF(datas3, '..\q_29\sphere3.txt');


