//******************************************************
//                  S c a t  D a t a
//                       
//                          Copyright (c) 2002 by John Coulthard
//
//    This source code is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//******************************************************
// Basic Scattered Data class to demo Xpand routine.
#include <stdlib.h>
#include "scatdata.h"

ScatData::ScatData(int MaxSize)
{
	MaxPoints = MaxSize;
	size = 0;
	X = new float[MaxPoints];
	Y = new float[MaxPoints];
	Z = new float[MaxPoints];
}

ScatData::~ScatData()
{ delete [] X;
delete [] Y;
delete [] Z;
}

void ScatData::SetNext( float x, float y, float z )
{
	if( size >= MaxPoints ) exit(EXIT_FAILURE);

	X[size] = x;
	Y[size] = y;
	Z[size] = z;

	if( size == 0 )
	{ xmax = xmin = x;
	ymax = ymin = y;
	zmax = zmin = z;
	}
	else
	{ if( x > xmax ) xmax = x;
	if( x < xmin ) xmin = x;
	if( y > ymax ) ymax = y;
	if( y < ymin ) ymin = y;
	if( z > zmax ) zmax = z;
	if( z < zmin ) zmin = z;
	}

	++size;
}


