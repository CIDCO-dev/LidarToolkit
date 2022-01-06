##filenameDome1p1 = '..\dome1_p1';
##filenameDome1p2 = '..\dome1_p2';
##
##dataDome1 = readLibacBinaryData(filenameDome1p1);
##dataDome2 = readLibacBinaryData(filenameDome1p2);
##
##writeZF(dataDome1, '..\dome1_p1.txt');
##writeZF(dataDome2, '..\dome1_p2.txt');

filenameSphere1Passage1 = '..\q_104_105_106\passage_1\sphere1';
s1p1 = readLibacBinaryData(filenameSphere1Passage1);

filenameSphere1Passage2 = '..\q_104_105_106\passage_2\sphere1';
s1p2 = readLibacBinaryData(filenameSphere1Passage2);

writeZF(s1p1, '..\q_104_105_106\passage_1\sphere1.txt');
writeZF(s1p2, '..\q_104_105_106\passage_2\sphere1.txt');



filenameSphere2Passage1 = '..\q_104_105_106\passage_1\sphere2';
s2p1 = readLibacBinaryData(filenameSphere2Passage1);

filenameSphere2Passage2 = '..\q_104_105_106\passage_2\sphere2';
s2p2 = readLibacBinaryData(filenameSphere2Passage2);

writeZF(s2p1, '..\q_104_105_106\passage_1\sphere2.txt');
writeZF(s2p2, '..\q_104_105_106\passage_2\sphere2.txt');



filenameSphere3Passage1 = '..\q_104_105_106\passage_1\sphere3';
s3p1 = readLibacBinaryData(filenameSphere3Passage1);

filenameSphere3Passage2 = '..\q_104_105_106\passage_2\sphere3';
s3p2 = readLibacBinaryData(filenameSphere3Passage2);

writeZF(s3p1, '..\q_104_105_106\passage_1\sphere3.txt');
writeZF(s3p2, '..\q_104_105_106\passage_2\sphere3.txt');