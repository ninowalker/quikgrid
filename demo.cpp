//***********************************************************************
//          X p a n d                      C o n t o u r
//                D e m o n s t r a t i o n
//
//                                Copyright (c) John Coulthard, 2002
//***********************************************************************
#include <stdlib.h>
#include <iostream.h>
#include "scatdata.h"
#include "surfgrid.h"
#include "xpand.h"
#include "contour.h" 

void main()
{
 ScatData Points(100);              // Define up to 100 scattered data points.

 static const int nx = 10, ny = 10; // Grid dimensions.

 SurfaceGrid Result(nx,ny);         // Define the grid to be generated.

 struct Point3d { float x, y, z;} ; // The scattered data points....
 static Point3d TestData[] = {
  {11,11,11},
  {14,11.5,12},
  {11.2,15.4,12},
  {13,13,17},
  {15.1,14.7,13},
  {18,18,19},
  {20,20,13}, 
  {14.6,19,14.2},
  {18.5,16.2,12} };

  static int i, j;

  // Load up our input array with scattered data points.
  int NumberOfPoints = sizeof(TestData)/sizeof(TestData[0]);
  for( int i=0; i<NumberOfPoints; i++ )
    Points.SetNext( TestData[i].x, TestData[i].y, TestData[i].z) ;

  // Initialize the grid x and y coordinates
  for( i = 0; i <nx; i++ ) Result.xset( i, 11+i );
  for( i = 0; i <ny; i++ ) Result.yset( i, 11+i );

  Xpand( Result, Points);  // Generate the grid.

  cout.setf( ios::fixed ); // Makes the printed output pretty. 
  cout.precision(2);

  cout << " The Scattered Data Points are: " << endl;
  for( i = 0; i < NumberOfPoints; i++ )
    cout << TestData[i].x << " " <<
            TestData[i].y << " " <<
            TestData[i].z << endl;  

  cout << endl << "   Y              The generated grid is: " << endl;
  for( j = ny-1; j >= 0; j-- )
   {
    cout << Result.y(j) << "   " ;
    for( i = 0; i < nx; i++ )
      if( Result.z(i,j) < 0.0 ) cout << "      ";  // Blank for negative undefined intersections.
      else                       cout << Result.z(i,j) << " " ;
    cout << endl ;
   }

  // Output the X coordinates.
  cout << endl << " X->    ";
  for( i = 0; i< nx; i++ )
     cout<< Result.x(i) << " " ;
  cout << endl;

  cout << endl << " The contour line for 14.0 follows " << endl;

  Contour( Result , 14.0 ) ;

  cout << "The end." << endl; 
}
//***********************************************************************
//           D o   L i n e    T o
//
// Called by Contour to trace contour lines.
//************************************************************************
void DoLineTo( float x, float y, int draw )
{
  cout << x << " " << y ;
  if( draw ) cout << " draw " << endl;
  else       cout << "    move " << endl; 
}