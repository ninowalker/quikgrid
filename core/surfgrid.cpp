//****************************************************************
//                        S u r f a c e   G r i d
//
//                 Copyright (c) 1993 - 2002 by W. John Coulthard
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
//*****************************************************************

#include "surfgrid.h"
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))

SurfaceGrid::SurfaceGrid( const int i, const int j )
{
	int k;
	nx=i;
	ny=j;
	zgrid = new float[(long)nx*(long)ny];
	xvect = new float[nx];
	yvect = new float[ny];
	for( k = 0; k<nx; k++ ) xvect[k] = k;
	for( k = 0; k<ny; k++ ) yvect[k] = k;
}

void  SurfaceGrid::zset( const int i, const int j, const float a )
{
	int offset = (long)j*nx + (long)i;
	zgrid[offset] = a ;
}

void  SurfaceGrid::xset( const int i, const float a )
{
	xvect[i] = a ;
}

void  SurfaceGrid::yset( const int i, const float a )
{
	yvect[i] = a ;
}


