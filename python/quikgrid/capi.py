import atexit
from ctypes import cdll, CDLL, pydll, PyDLL, CFUNCTYPE
from ctypes import string_at, byref, c_int, c_long, c_size_t, c_char_p, c_double, c_void_p, c_float
from ctypes import Structure, pointer, cast, POINTER, addressof
import numpy

try:
    from shapely.geometry import LineString, Polygon, Point
    _HAS_SHAPELY = True
except ImportError, e:
    _HAS_SHAPELY = False

try:
    lqg = CDLL( './libquikgrid.so' )
except OSError,e:
    lqg = CDLL( '/usr/lib/libquikgrid.so' )
    
UNDEFINED_Z = -99999.0 #c_float.in_dll(lqg, "UNDEFINED_Z")
NaN = float('NaN')
            
def pycapi(func, rettype, cargs=None):
    """Convenience function for setting arguments and return types."""
    func.restype = rettype
    if cargs:
        func.argtypes = cargs
           
        


class Point3d(Structure):
    _fields_ = (('x', c_float),('y', c_float),('z', c_float))
    
    def tuple(self):
        return (self.x, self.y, self.z)
    
class _ContourSeq(Structure):
    #def __del__(self):
    #    lqg.csDestroy(self)
    pass

_ContourSeq._fields_ = (('x', c_float),('y', c_float),('op', c_int), ('next',POINTER(_ContourSeq)))


class SurfaceGrid(object):
    def __init__(self, nx, ny, sw, ne):       
        self.soul = lqg.sgNew(c_int(nx), c_int(ny), 
                              c_float(sw[0]), c_float(sw[1]),
                              c_float(ne[0]), c_float(ne[1]))
        self.nx = nx
        self.ny = ny
        self.sw = sw
        self.ne = ne
        assert(self.soul)
        
    def __del__(self):
        if lqg and self.soul:
            lqg.sgDestroy(self.soul)
            
    def __str__(self):
        return "<SurfaceGrid sw='%s' ne='%s' nx='%s' ny='%s'/>" % ( self.sw, self.ne, self.nx, self.ny)

    def __iter__(self):
        return _SurfaceGridIterator(self)

    def get(self, i, j):
        p = Point3d()
        lqg.sgValue(self.soul, i, j, byref(p))
        if p.z == UNDEFINED_Z:
            p.z = NaN
        return p
    
    def x(self, i):
        return lqg.sgX(self.soul, i)

    def y(self, j):
        return lqg.sgY(self.soul, j)
    
    @property
    def xvals(self):
        return [lqg.sgX(self.soul, i) for i in range(self.nx)]
    
    @property
    def yvals(self):
        return [lqg.sgY(self.soul, i) for i in range(self.ny)]
    
    @property
    def xsize(self):
        return (self.ne[0] - self.sw[0]) / float(self.nx)
    
    @property
    def ysize(self):
        return (self.ne[1] - self.sw[1]) / float(self.ny)
    
    def z(self, i, j):
        z = lqg.sgZ(self.soul, i, j)
        if z == UNDEFINED_Z:
            return NaN
        return z
    
    def setZ(self, i, j, z):
        if numpy.isnan(z):
            z = UNDEFINED_Z
        lqg.sgSetZ(self.soul, i, j, z)
        
    
    def expand(self, data):
        nump = len(data)
        pts = (c_float * (nump * 3))()
        for i,d in enumerate(data):
            pts[i*3] = d[0]
            pts[i*3+1] = d[1]
            pts[i*3+2] = d[2]
        lqg.sgExpand(self.soul, pointer(pts), c_int(nump))
        
    def interpolate(self, x, y):
        f = lqg.sgInterpolate(self.soul, x, y)
        if f == UNDEFINED_Z:
            return NaN
        return f
    
    # TODO synchronize
    def contour(self, contour_at, as_geometry=False, closure_tolerance=0.001):
        seq = lqg.sgContour(self.soul, contour_at)
        oseq = seq
        pseq = []
        cur = None
        while seq:
            seq = seq.contents
            if not seq.op: # is this a new contour?
                cur = []
                pseq.append(cur)
            cur.append((float(seq.x), float(seq.y)))
            seq = seq.next
        lqg.csDestroy(oseq)
        
        if not as_geometry:
            return pseq
        
        geom = []
    
        if not _HAS_SHAPELY:
            raise ImportError("shapely not installed; cannot return contours as geometry.")
        for seq in pseq:
            if len(seq) < 3:
                continue
            if Point(seq[0]).distance(Point(seq[-1])) < closure_tolerance:
                if seq[0] != seq[-1]: # to ensure closure
                    seq.append(seq[0])
                geom.append(Polygon(seq))
            else:
                geom.append(LineString(seq))
        return geom
    
    def to_matrix(self):
        a = []
        xvals = self.xvals
        yvals = self.yvals
        for i in range(self.nx):                
            b = []
            for j in range(self.ny):
                b.append((xvals[i], yvals[j], self.z(i,j)))
            a.append(b)
        return a
    
    def to_array(self):
        a = numpy.zeros(self.nx*self.ny)
        for i in range(self.nx):                
            for j in range(self.ny):
                a[i*self.nx+j] = self.z(i,j)
        a.shape = (self.nx, self.ny)
        return a
    
    def to_csv(self, filename):
        fh = open(filename, "w")
        xvals = [lqg.sgX(self.soul, i) for i in range(self.nx)]
        yvals = [lqg.sgY(self.soul, i) for i in range(self.ny)]
        for i in range(self.nx):                
            for j in range(self.ny):
                fh.write("%f,%f,%f\n" % (xvals[i], yvals[j], self.z(i,j)))
        fh.close()

class _SurfaceGridIterator:
    def __init__(self, g):
        self.g = g
        self.i = 0
        self.j = -1
        self.xvals = [lqg.sgX(g.soul, i) for i in range(g.nx)]
        self.yvals = [lqg.sgY(g.soul, i) for i in range(g.ny)]
        
    def next(self):
        if self.j >= self.g.ny - 1:
            self.j = 0
            self.i += 1
        else:
            self.j += 1
        if self.i > self.g.nx-1:
            raise StopIteration
        return (self.xvals[self.i], self.yvals[self.j], self.g.z(self.i,self.j))

""" Setup the result and arguments types for the C methods. """
#pycapi(lqg.sgNew, c_void_p, [c_int, c_int, POINTER(Point3d), c_int, c_int])
lqg.sgNew.restype = c_void_p
pycapi(lqg.sgDestroy, c_void_p, [c_void_p])
pycapi(lqg.sgContour, POINTER(_ContourSeq), [c_void_p, c_float])
pycapi(lqg.sgInterpolate, c_float, [c_void_p, c_float, c_float])
pycapi(lqg.csDestroy, c_void_p, [POINTER(_ContourSeq)])
pycapi(lqg.sgX, c_float, [c_void_p, c_int])
pycapi(lqg.sgY, c_float, [c_void_p, c_int])
pycapi(lqg.sgZ, c_float, [c_void_p, c_int, c_int])
pycapi(lqg.sgSetZ, c_void_p, [c_void_p, c_int, c_int, c_float])

    
    