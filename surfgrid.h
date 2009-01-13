//******************************************************************
//              S u r f a c e    G r i d
//         Copyright (c) 2002 by John Coulthard
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
//    USA or visit their web site at www.gnu.org .
//  
//*******************************************************************

class SurfaceGrid
	// Define a simple class to handle the definition of a rectangular grid
	// representing a surface.
{
private:
	float *zgrid,	// z[i][j]
		*xvect,		// x[i]
		*yvect;		// y[j]

	long  nx, ny;     // size of the array.

public:
	SurfaceGrid(const int i, const int j);

	void  zset( const int i, const int j, const float a );
	void  xset( const int i, const float a );
	void  yset( const int i, const float a );

	float x( const int i ) { return xvect[i];}
	float y( const int i ) { return yvect[i];}
	float z( const int i, const int j ) { return  zgrid[j*nx+i]; }

	int   xsize()          { return nx; }
	int   ysize()          { return ny; }

	~SurfaceGrid() { delete[] zgrid; delete[] xvect; delete[] yvect;}
};

