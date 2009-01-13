#include <stdlib.h>
#include <iostream>
#include <math.h>


#include "surfgrid.h"
#include "scatdata.h"
#include "xpand.h"
#include "contour.h" 

extern "C" {

struct Point3d { 
	float x, y, z;
};

struct ContourSeq {
	float x, y;
	int op;
	ContourSeq* next;
};

ContourSeq* globalseq; 
ContourSeq* globalseqcurr; 

SurfaceGrid* 
sgNew( int nx, int ny, float minx, float miny, float maxx, float maxy) {
	SurfaceGrid* sg = new SurfaceGrid(nx, ny);
	
	float delta_x = (maxx - minx) / float(nx);
	float delta_y = (maxy - miny) / float(ny);
	
	for( int i = 0; i <nx; i++ ) sg->xset( i, minx+(delta_x*float(i)) );
	for( int i = 0; i <ny; i++ ) sg->yset( i, miny+(delta_y*float(i)));
	
	
	return sg;
}

void sgExpand( SurfaceGrid* sg, float* vals, int num_points ) {
	ScatData points(num_points);
	
	for( int i=0; i<num_points; i++ ) {
		points.SetNext( vals[i*3], vals[i*3+1], vals[i*3+2] );
	}
	Xpand( *sg, points ); // Generate the grid.
}

void
sgDestroy( SurfaceGrid* sg ) {
	delete sg;
}

float 
sgZ( SurfaceGrid* sg, int i, int j ) {
	return sg->z(i, j);
}

float 
sgY( SurfaceGrid* sg, int i ) {
	return sg->y(i);
}

float 
sgX( SurfaceGrid* sg, int i ) {
	return sg->x(i);
}

void 
sgValue( SurfaceGrid* sg, int i, int j, Point3d* p ) {
	p->x = sg->x(i);
	p->y = sg->y(j);
	p->z = sg->z(i,j);
}

void 
sgSetZ( SurfaceGrid* sg, int i, int j, float z) {
	sg->zset(i,j,z);
}


float
sgInterpolate( SurfaceGrid* sg, float x, float y) {
	int xsize = sg->xsize(), ysize = sg->ysize();
	double maxx = sg->x(xsize-1), maxy = sg->y(ysize-1);
	double minx = sg->x(0), miny = sg->y(0);
	
	// what data point in t is closest to (x,y)?
	double local_x = (x - minx) / (maxx - minx);
	double local_y = (y - miny) / (maxy - miny);

	int index_x = (int) floor(local_x * (xsize-1));
	if (index_x < 0 || index_x >= xsize)
		return UNDEFINED_Z;

	int index_y = (int) floor(local_y * (ysize-1));
	if (index_y < 0 || index_y >= ysize)
		return UNDEFINED_Z;

	double findex_x = local_x * (xsize-1);
	double findex_y = local_y * (ysize-1);

	double fData;
	float fDataBL, fDataTL, fDataTR, fDataBR;
	if (index_x == xsize-1)
	{
		if (index_y == ysize-1)
		{
			// far corner, no interpolation
			fData = sg->z(index_x, index_y);
		}
		else
		{
			// right edge - interpolate north-south
			fDataBL = sg->z(index_x, index_y);
			fDataTL = sg->z(index_x, index_y+1);
			if (fDataBL == UNDEFINED_Z || fDataTL == UNDEFINED_Z)
				return UNDEFINED_Z;
			double diff_y = findex_y - index_y;
			fData = fDataBL + (fDataTL - fDataBL) * diff_y;
		}
	}
	else if (index_y == ysize-1)
	{
		// top edge - interpolate east-west
		fDataBL = sg->z(index_x, index_y);
		fDataBR = sg->z(index_x+1, index_y);
		if (fDataBL == UNDEFINED_Z || fDataBR == UNDEFINED_Z)
			return UNDEFINED_Z;
		double diff_x = findex_x - index_x;
		fData = fDataBL + (fDataBR - fDataBL) * diff_x;
	}
	else // do bilinear filtering
	{
		double diff_x = findex_x - index_x;
		double diff_y = findex_y - index_y;
		// catch numerical roundoff, diff must be [0..1]
		if (diff_x < 0)
			diff_x = 0;
		if (diff_y < 0)
			diff_y = 0;
		fDataBL = sg->z(index_x, index_y);
		fDataBR = sg->z(index_x+1, index_y);
		fDataTL = sg->z(index_x, index_y+1);
		fDataTR = sg->z(index_x+1, index_y+1);
		if ((fDataBL != UNDEFINED_Z) &&
				(fDataBR != UNDEFINED_Z) &&
				(fDataTL != UNDEFINED_Z) &&
				(fDataTR != UNDEFINED_Z))
		{
			fData = fDataBL + (fDataBR-fDataBL)*diff_x +
				(fDataTL-fDataBL)*diff_y +
				(fDataTR-fDataTL-fDataBR+fDataBL)*diff_x*diff_y;
		}
		else
			fData = UNDEFINED_Z;
	}
	return (float) fData;

}

ContourSeq*
sgContour( SurfaceGrid* sg, float contour_at ) {
	Contour(*sg, contour_at);
	
	ContourSeq* ret = globalseq;
	globalseq = 0;
	globalseqcurr = 0;

	return ret;
}

void
csDestroy( ContourSeq* cs) {
	ContourSeq* next;
	while (cs) {
		next = cs->next;
		delete cs;
		cs = next;
	}
}
};

void DoLineTo( float x, float y, int drawtype ) {
	if (!globalseq) {
		globalseq = new ContourSeq();
		globalseqcurr = globalseq;
	} else {
		globalseqcurr->next = new ContourSeq();
		globalseqcurr = globalseqcurr->next;
	}
	globalseqcurr->x = x;
	globalseqcurr->y = y;
	globalseqcurr->op = drawtype;
}

int main() {
	return 1;	
}
