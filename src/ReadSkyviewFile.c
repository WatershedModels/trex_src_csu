/*---------------------------------------------------------------------
C-  Function:	ReadSkyviewFile.c
C-
C-  Purpose/    Reads the skyview (open fraction of cell area with an
C-  Methods:    unobstructed view of the sky) (0-1) for each cell in
C-              the overland plane.
C-
C-
C-  Inputs:     None
C-
C-  Outputs:    skyview[][]
C-
C-  Controls:   None
C-
C-  Calls:      None
C-
C-  Called by:  ReadDataGroupB
C-
C-  Created:    Mark Velleux
C-              HydroQual, Inc.
C-              1200 MacArthur Boulevard
C-              Mahwah NJ  07430
C-
C-  Date:       24-JAN-2007
C-                         
C-  Revised:	Mark Velleux
C-              HydroQual, Inc.
C-              1200 MacArthur Boulevard
C-              Mahwah, NJ  07430
C-
C-  Date:		31-JUL-2009
C-
C-  Revisions:	Added code so that errors caused by bad file names
C-              and paths are are reported to the simulation error
C-              file in addition to the screen.
C-
C-	Revised:
C-
C-	Date:
C-
C-	Revisions:
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

void ReadSkyviewFile()
{
	//Local Variable Declarations
	int
		gridrows,	//number of rows in grid
		gridcols;	//number of columns in grid

	float
		cellsize;	//length of grid cell (m) (this means dx must equal dy)

	//write message to screen
	printf("\n\n********************************\n");
	printf(	   "*                              *\n");
	printf(	   "*   Reading DEM Skyview File   *\n");
	printf(	   "*                              *\n");
	printf(	   "********************************\n\n\n");

	//Set the skyview file pointer to null
	skyviewfile_fp = NULL;

	//Open the skyview file for reading
	skyviewfile_fp=fopen(skyviewfile,"r");

	//if the skyview file pointer is null, abort execution
	if(skyviewfile_fp==NULL)
	{
		//Write message to file
		fprintf(echofile_fp, "Error! Can't open Skyview File : %s \n", skyviewfile);

		//Write message to screen
		printf("Error! Can't open Skyview File : %s \n", skyviewfile);
		exit(EXIT_FAILURE);	//abort
	}

	//Write label for skyview file to file
	fprintf(echofile_fp, "\n\n\n  DEM Grid Cell Skyview (in Degrees)  \n");
	fprintf(echofile_fp,       "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	//Record 1
	fgets(header, MAXHEADERSIZE, skyviewfile_fp);	//read header

	//Echo header to file
	fprintf(echofile_fp, "\n%s\n", header);

	//Record 2
	fscanf(skyviewfile_fp, "%s %d %s %d %s %f %s %f %s %f %s %d",	//read
		varname,		//dummy
		&gridcols,		//number of columns in grid
		varname,		//dummy
		&gridrows,		//number of rows in grid
		varname,		//dummy
		&xllcorner,		//x location of grid lower left corner (m) (GIS projection)
		varname,		//dummy
		&yllcorner,		//y location projection of grid lower left corner (m) (GIS projection)
		varname,		//dummy
		&cellsize,		//length of grid cell (m) (this means dx must equal dy)
		varname,		//dummy
		&nodatavalue);	//no data value (null value)

	//If number of grid rows, grid columns, or cell size do not equal global values, abort...
	if(gridrows != nrows || gridcols != ncols || cellsize != dx)
	{
		//Write error message to file
		fprintf(echofile_fp, "\n\n\nDEM Skyview File Error:\n");
		fprintf(echofile_fp,       "  nrows = %5d   grid rows = %5d\n", nrows, gridrows);
		fprintf(echofile_fp,       "  ncols = %5d   grid cols = %5d\n", ncols, gridcols);
		fprintf(echofile_fp,       "  dx = %12.4f   dy = %12.4f   cell size = %12.4f\n", dx, dy, cellsize);

		//Write error message to screen
		printf("DEM Skyview File Error:\n");
		printf("  nrows = %5d   grid rows = %5d\n", nrows, gridrows);
		printf("  ncols = %5d   grid cols = %5d\n", ncols, gridcols);
		printf("  dx = %12.4f   dy = %12.4f   cell size = %12.4f\n", dx, dy, cellsize);

		exit(EXIT_FAILURE);					//abort
	}	//end check of grid dimensions

	//Echo skyview characteristics to file
	fprintf(echofile_fp, "\nDEM Grid Cell Skyview Characteristics:\n");
	fprintf(echofile_fp, "   Grid Rows = %5d\n", gridrows);
	fprintf(echofile_fp, "   Grid Columns = %5d\n", gridcols);
	fprintf(echofile_fp, "   Cell size = %10.2f (m)\n", cellsize);
	fprintf(echofile_fp, "   No Data Value = %6d\n", nodatavalue);

	//Allocate memory for skyview
	skyview = (float **)malloc((gridrows+1) * sizeof(float *));		//allocate memory

	//Loop over number of rows
	for(i=1; i<=gridrows; i++)
	{
		//Allocate remaining memory for skyview
		skyview[i] = (float *)malloc((gridcols+1) * sizeof(float));		//allocate memory

		//Loop over number of columns
		for(j=1; j<=gridcols; j++)
		{
			//Record 3
			fscanf(skyviewfile_fp, "%f", &skyview[i][j]);	//read skyview value

			//Echo skyview in overland cell to file
			fprintf(echofile_fp, "  %10.4f", skyview[i][j]);

		}	//end loop over grid columns

		//Start a new line for the next row of data in the echo file 
		fprintf(echofile_fp, "\n");

	}	//end loop over grid rows

	//Close the skyview file
	fclose(skyviewfile_fp);

//End of function: Return to ReadDataGroupB
}
