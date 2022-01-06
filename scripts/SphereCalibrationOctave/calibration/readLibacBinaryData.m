function data = readLibacBinaryData(filename)

	nbDoublesInStruct = 22;

	fileID = fopen( filename );
	data = fread( fileID, 'float64' );
	fclose( fileID );

	nbDoublesInFile = length( data );
	nbPointsInFile = nbDoublesInFile / nbDoublesInStruct;
	
	data = ( reshape( data,  nbDoublesInStruct, nbPointsInFile ) ).';
endfunction