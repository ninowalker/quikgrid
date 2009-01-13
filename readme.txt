             Xpand and Contour documentation - Version 5

May, 2002
Revised: February, 2003   
Revised: May, 2003  
Revised: March, 2007   
Revised: August, 2007                                   John Coulthard 
                                               www.perspectiveedge.com
                                                   or www.galiander.ca

                Copyright (C) 2007 by John Coulthard 
           
This source code is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version. This license is included in the file
lesser.txt . For more information see http://www.gnu.org . 

The files in this .zip file contain the C++ source code for the grid
generator (called xpand) and contouring routine (called contour) in the
QuikGrid program. Also included is a small demo program that
demonstrates the use of the functions by generating a small grid and
contouring a contour line. 

I have compiled this code using Borland C++ version 5.0 and under UNIX
(Linux). 

The documentation that follows gives a description of the different
modules. 

Version 2 changes (February, 2003): 

There have been several changes to allow this to compile under UNIX. The
include file <mem.h> has been replaced by <memory.h>. All calls to the
"assert" function have been commented out. The include to <windows.h>
has been removed (it was not required). All file names have been
converted to lower case. In function CONTOUR the variables XTracePoint,
YTracePoint, XTraceSubPoint and YTraceSubPoint should have been declared
"int", not "float". That was a true bug that has been in there since day
1 near as I can tell. Didn't seem to affect the results but would have
slowed things down a mite. 

Version 3 changes (May, 2003) 

Only this readme file has been changed. I have included a description of
the GRIDXTYPE CLASS (used by XPAND - the grid generation code), a more
detailed description of the Algorithm used by XPAND, and a description
of the algorithm artifacts. 

Version 4 changes (March, 2007)

Corrects a bug in the grid generation module that manifests itself
typically as differences in the placement of unevaluated grid
intersections around the edges of the generated grid or in areas where
there are no data points. (That is an oversimplified description of the
results of a programming bug - an array index overflow in areas near the
edge of the grid or the edge of areas where there are no data points and
grid intersections are left undefined). My thanks go to Matt Gessner, a
software engineer from Ohio for identifying and correcting this bug.

Version 5 changes (August, 2007) 

Delete the text "Permission to copy for non commercial purposes is
granted provided the source is acknowledged." at the beginning of this
file. This text was left in as a mistake after the decision to adopt the
GNU Lesser General Public License. There is no intention to limit or
qualify the Lesser General Public License in any fashion at all. 

DEMO (Demonstration program) 

Source: DEMO.CPP, DEMO.H 

The demo program defines a small set of 10 data points, generates a grid
from the data points and contours a single contour line. Output consists
of a print of the grid and the trace of the contour line segments. The
output may be compared to the output from QuikGrid by loading the points
in the file TESTDATA.XYZ into QuikGrid and generating a grid using the
default grid generation parameters. 

The demo program should be compiled under MS Windows as a 32 bit
"console" or "command line" application. You can also compile and run it
as a 16 bit DOS application but you may receive many warning messages
due to the 2 byte size of integers. It will run but there are size
limitations under 16 bit DOS: 16,000 data points and approximately a
125x125 grid. 

This source has also been compiled and tested under Linux. A makefile is
included. 

                     XPAND (Grid generation)

Source: XPAND.CPP, XPAND.H,       // The main body of code. 
        SCATDATA.CPP, SCATDATA.H, // class to store data points. 
        SURFGRID.CPP, SURFGRID.H, // class to store grid. 
        GRIDXTYP.CPP, GRIDXTYP.H. // class used internally by XPAND.  

void Xpand( SurfaceGrid &Zgrid, ScatData &RandomData)

The data for the grid to be generated is handled by the class
"SurfaceGrid". A basic functional class is included in the files
SURFGRID.H and SURFGRID.CPP. The scattered data points are supplied by
the class "ScatData". A basic functional class is included in the files
SCATDATA.H and SCATDATA.CPP. 

The following entry points allow you to customize the grid generation
parameters. See the QuikGrid help file for an explanation of the
parameters. Any call to one of these functions without an argument, for
example XpandScanRatio(), will return the current value of the
parameter. 

int XpandScanRatio( const int NewRatio )
int XpandDensityRatio(const int NewRatio )
int XpandEdgeFactor(const int NewFactor )
float XpandUndefinedZ( const float z )
long XpandSample( const long i )


Undefined grid intersections are flagged, by default,  as -99999. It is
generally necessary to make sure that all your data points are positive.
For negative data QuikGrid normalizes the data so it is always positive
and then corrects the values for display purposes. Contour makes use of
this convention as well and does not contour negative areas of the grid.



                   CONTOUR (Contour line generation) 

Source: CONTOUR.CPP, CONTOUR.H, 
        SURFGRID.CPP, SURFGRID.H, // class to store grid. 

Will contour a grid for a single contour value. Call it repeatedly to
contour more than one contour value. The function void DoLineTo(float x,
float y, int draw), which you must supply, will be called repeatedly as
the contour line is traced. Contour lines which intercept the edge of
the grid are contoured first, then interior closed contour lines are
traced. 

CONTOUR assigns the average of the four corners of a grid to the centre
of the grid and then contours the 4 triangles. There is only one way a
contour line can traverse a triangle. 

void Contour( SurfaceGrid &Zgrid, float ContourValue )



                          SCATDATA CLASS 

The ScatData class defines a simple 3 by N array to store scattered data
points. For example to define an array called Points which is big enough
to hold 100 scattered data points use "ScatData Points(100)". "SetNext"
is provided to fill the array. Thus successive calls to
Points.SetNext(x,y,z) will load up the array. The class provides
functions to query the size and maximums and minimums of the data as
well as to fetch the x, y and z values. 


                        SURFACEGRID CLASS 

The SurfaceGrid class defines a simple 2 dimensional array. Functions
are provided to set the x and y coordinates for the array and the z
value. Other functions allow you to retrieve the same, query the maximum
and minimum z values, size of the array and so forth. 


                         GRIDXTYPE CLASS 

This class is used only by XPAND. It is used to store, sort and search a
table which contains all the scattered data points and the grid
intersection they are closest to. 

                         XPAND ALGORITHM

XPAND is basically a Nearest Neighbour algorithm with eight sectors. The
algorithm lends itself to an efficient implementation, which is why
XPAND is fast. 

Xpand is organized so that each grid intersection can be evaluated in a
separate function call. This feature was included to allow a grid to be
evaluated in the background under Windows 3.1. Under Windows 3.1
timesharing only worked if each program voluntarily relinquished control
back to the OS after a small time period. With Xpand this was
accomplished by only computing one Grid Intersection at a time. The
functions involved include XpandInit - to initialize everything and
XpandPoint, which evaluated one grid intersection only. It is very
unlikely that these functions have any external use any more. 

Routine Xpand calls XpandInit to set everything up and then calls
XpandPoint for each grid intersection until the grid is evaluated. 

Initialization is handled by the functions XpandInit and LocateInit.
XpandInit initalizes a lot of local variables and then calls LocateInit. 
LocateInit makes use of the Class GridXtype which sets up an array
called Locator, which contains all the scattered data points and
associates with each data point the grid intersection it is closest to.
The array is then sorted by grid intersection. It then creates a lookup
table that makes it very fast to find all the data points that are close
to a given grid intersection. 

The procedure is then identical for the calculation of each grid
intersection. 

First the closest data points in each octant are determined. This is
done by looking at all the data points close to the grid intersection to
be evaluated and then scanning the data points for nearby grid
intersections by shelling out from the intersection being evaluated. The
process will eventually stop due to: 

1. An edge of the grid is reached. 
2. The distance from the grid intersection being evaluated becomes 
   too large. 
3. The distance becomes far enough that any new data points can not 
   contribute appreciably to the weighted value of the intersection. 

Function PutInOctant keeps track of the closest data point in each
octant. 

Once the scanning process is complete the weighted average of the
selected points are used to determine the value for the grid
intersection. The grid intersection may be left unevaluated because
there are no points nearby or because the points are all on one side of
the grid intersection. 

Speed considerations: 
---------------------

Speed of access to the scattered data points is the most critical part
of the program. Storing the data points as single dimensioned contiguous
memory arrays for X, Y and Z seems to produce the best results for me.
My tests showed that storing them as a structure of x,y,z was slower and
storing them as any sort of chained blocks of memory (which was
necessary for data sets larger than 16000 points under Windows 3.1) was
much slower. All this was done back in the Windows 3.1 days. Perhaps the
compilers are smarter optomizers now? It is easy to experiment with this
if you are interested in doing so. 

The rest of the time critical code pretty well cascades back from there
to the table lookup for data points close to a given grid intersection
(ScanOneGrid) and the shelling out process (SelectPoints). 

The sorting of the array of data points and associated grid
intersections is another time eater. Xpand uses the standard Unix math
function Qsort for this purpose. I don't know what has been going on in
the world of sorting these days but back then the Quick Sort algorithm
was about as good as you could get and it is reasonable to suppose that
the one provided in your library would be written in assembler and
highly optomized (but maybe not?). 

Much of this code dates back to 1993 when I was using a 25Mhz cpu with
no floating point processor. I suppose you might say it was fine tuned
for that type of platform. 

If you can make it faster let me know - I'd be curious to know what you did. 

Artifacts
---------

Xpand suffers from the same artifacts that all grid generation schemes
share. For example the contour lines may not "honour" the scattered data
points (i.e. the contour line may go on the wrong side of the data point
- this is because it is the generated grid being contoured, not the
original data points). 

The algorithm works best with scattered data points that are more or
less evenly distributed. The most common artifact is typically a
"ripple" in the generated grid that goes at 45 or 90 degree angles. This
may become pronounced if your scattered data points tend to be oriented
in rows or columns and is generated as a column or row of data points
moves from one octant to another as grid intersections are evaluated. 

The algorithm is interpolative, not extrapolative. If you ask it to
extrapolate to the edges when there are no points "out there" the
results may be quite strange (play with the Edge Sensitivity and
Distance cutoff to experiment with this - different data sets will
behave in different ways). 

If the data points are clustered, leaving large areas of the grid with
no data points nearby, the grid generation times will suffer. By default
these grid intersections may be left unevaluated but you may over-ride
this by used the Distance cutoff parameter (and increasing the grid
generation time). If the scattered data points are very sparse in
comparison to the grid (a dense grid), grid generation times will
suffer. Basically anything that causes Xpand to shell out a long way to
find data points will cause the grid generation time to suffer. 


