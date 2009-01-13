//**************************************************************
//                   S c a t   D a t a  . h
//
//        Copyright (c) 2002 by John Coulthard
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
//**************************************************************
//   Basic Scattered Data class to demo Xpand routine.
class ScatData
{
protected:
	float *X, *Y, *Z;
	long size, MaxPoints;
	float xmax, xmin, ymax, ymin, zmax, zmin;

public:
	ScatData(int MaxSize);
	void SetNext( float x, float y, float z);

	void Reset() { size = 0; return;} 
	long  Size() { return size; }
	float xMax() { return xmax; }
	float xMin() { return xmin; }
	float yMax() { return ymax; }
	float yMin() { return ymin; }
	float zMax() { return zmax; }
	float zMin() { return zmin; }
	float x( const long i) { return X[i]; }
	float y( const long i) { return Y[i]; }
	float z( const long i) { return Z[i]; }

	~ScatData();
};

