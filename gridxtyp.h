//**************************************************************
//                   G r i d X T y p e. h
//         Copyright (c) 1993 - 1999 by John Coulthard
//
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
//**************************************************************
// Sept. 21/98: Converted to Win32 - long variables.
// Apr. 8/99: Convert to use lookup table.
//**************************************************************

class GridXType 
{
protected:
	enum { Shift = 8192 } ;  // Increased from 4096 for larger grids.

	struct LocateStructure { long intersection;
	long DataLocation; } ;
	LocateStructure *FindPoints;
	long Size,             // Number of data points.
		nx, ny,           // Size of grid being generated.
		*Lookup,          // Lookup grid (nx by ny in size)
		LookupSize, 
		PreviousSearch,   // Remembers result from previous search.
		np;               // Actual number of points used in search.

public:
	GridXType( const long iSize, const long nx, const long ny);
	void New( const long iSize, const long nx, const long ny );
	void Sort();
	long Search( const int i, const int j, const int n );

	long x( long i) { return FindPoints[i].intersection/Shift; }
	long y( long i) { return FindPoints[i].intersection%Shift; }
	long location( const long i)  { return FindPoints[i].DataLocation; }
	void setnext( long i, int ix, int iy )
	{ FindPoints[np].DataLocation = i;
	FindPoints[np].intersection = (long)ix*(long)Shift+(long)iy;
	np++; }


	~GridXType() { delete[] FindPoints; delete Lookup; }
};
