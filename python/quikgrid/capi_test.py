from capi import SurfaceGrid
import time
import numpy
import filecmp, os
from shapely.geometry import Polygon, LineString

def test_basic():
    """ testing the basic functionality. """
    # define the scatter data
    testdata = [
      (11,11,11),
      (14,11.5,12),
      (11.2,15.4,12),
      (13,13,17),
      (15.1,14.7,13),
      (18,18,19),
      (20,20,13), 
      (14.6,19,14.2),
      (18.5,16.2,12)]
    
    # create the grid: xsize, ysize, SW corner, NE corner, scatter data
    t0 = time.time()
    sg = SurfaceGrid(100, 100, (11,11), (21,21))
    assert sg.xsize == 0.1, sg.xsize
    assert sg.ysize == 0.1, sg.ysize
    
    sg.expand(testdata)
    t1 = time.time()
    print "grid creation took: %f"%(t1-t0)

    # print the grid points to a file
    fp = open("grid.txt", "w")
    t0 = time.time()
    m = sg.to_matrix()
    t1 = time.time()
    print "to matrix took: %f"%(t1-t0)
    for a in m:
        for b in a:
            fp.write("%f,%f,%f\n" % b)
    fp.close()
    sg.to_csv("grid2.txt")
    assert filecmp.cmp("grid.txt","grid2.txt", False)
    os.unlink("grid2.txt")
    # print the contours to a file
    fp = open("contours.txt", "w")
    contours = sg.contour(14.0)
    print "Found n contours: ",len(contours)
    assert len(contours) >= 8
    for c in contours:
        fp.write("NAME=Contour\n")
        for p in c:
            fp.write("%.2f, %.2f\n" % p)
    fp.close()
    print "Interpolate: ",round(sg.interpolate(15.5, 11.9), 2), " "
    assert round(sg.interpolate(15.5, 11.9), 2) == round(13.12, 2)
    
    sg.setZ(1,1,1.0)
    print sg.get(1,1).z
    assert sg.get(1,1).z == 1

    sg.setZ(1,1,numpy.NaN)
    print sg.get(1,1).z
    assert numpy.isnan(sg.get(1,1).z)

    # test iterate
    pts = [pt for pt in sg]
    print len(pts)
    assert len(pts) == sg.nx * sg.ny
    
    # test contour/shapely
    contours = sg.contour(14.0, as_geometry=True, closure_tolerance=0.1)
    print contours
    fp = open("contours_geom.txt", "w")
    print "Found n contours: ",len(contours)
    assert len(contours) >= 8
    for c in contours:
        fp.write("NAME=%s\n" % c.geom_type)
        if isinstance(c, Polygon):
            fp.write("CLOSED=Yes\n")
            for p in c.exterior.coords:
                fp.write("%.2f, %.2f\n" % p)
        else: # linestring
            for p in c.coords:
                fp.write("%.2f, %.2f\n" % p)
    fp.close()
    
    #assert False

if __name__ == '__main__':
    test_basic() 
