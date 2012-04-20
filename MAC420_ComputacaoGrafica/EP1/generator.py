#!/usr/bin/python

import sys
import os
import math

#####################
class vector:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

    def length(self):
        return math.sqrt(self.x**2 + self.y**2 + self.z**2)

    def __add__(self, rhs):
        return vector(self.x + rhs.x, self.y + rhs.y, self.z + rhs.z)

    def __sub__(self, rhs):
        return vector(self.x - rhs.x, self.y - rhs.y, self.z - rhs.z)

    def __mul__(self, scale):
        return vector(self.x*scale, self.y*scale, self.z*scale)
        
    def normalize(self):
        l = self.length()
        if l == 0:  return
        self.x = self.x / l
        self.y = self.y / l
        self.z = self.z / l

    def Scale(self, s):
        self.x *= s
        self.y *= s
        self.z *= s
        
    def InnerProduct(self, b):
        return (self.x * b.x) + (self.y * b.y) + (self.z * b.z)
        
    def CrossProduct(self, b):
        cpx = (self.y * b.z) - (self.z * b.y)
        cpy = (self.z * b.x) - (self.x * b.z)
        cpz = (self.x * b.y) - (self.y * b.x)
        return vector(cpx, cpy, cpz)
        
    def Rotate(self, angle, d):
        #yes, since I dont have matrixes here and I dont wanna bother getting them, lets do it by hand
        #  [ A B C 0 ]  [ X ]
        #  [ D E F 0 ]  [ Y ]    = result
        #  [ G H I 0 ]  [ Z ]   
        #  [ 0 0 0 1 ]  [ 0 ]
        one_cos = 1 - math.cos(angle)
        A = math.cos(angle) + (d.x**2)*one_cos
        B = d.x*d.y*one_cos - d.z*math.sin(angle) 
        C = d.x*d.z*one_cos + d.y*math.sin(angle)
        D = d.y*d.x*one_cos + d.z*math.sin(angle)
        E = math.cos(angle) + (d.y**2)*one_cos
        F = d.y*d.z*one_cos - d.x*math.sin(angle)
        G = d.z*d.x*one_cos - d.y*math.sin(angle)
        H = d.z*d.y*one_cos + d.x*math.sin(angle)
        I = math.cos(angle) + (d.z**2)*one_cos
        
        newx = A*self.x + B*self.y + C*self.z
        newy = D*self.x + E*self.y + F*self.z
        newz = G*self.x + H*self.y + I*self.z
        return vector(newx, newy, newz)
        
    def __repr__(self):
        return "<%s, %s, %s>" % (self.x, self.y, self.z)
    def __str__(self):
        return self.__repr__()

##################
        
class field:
    def __init__(self, nx, ny, nz, dx, dy, dz):
        self.nx = int(nx)
        self.ny = int(ny)
        self.nz = int(nz)
        self.dx = float(dx)
        self.dy = float(dy)
        self.dz = float(dz)
        
        self.cube = [ [ [None for k in range(self.nz)] for j in range(self.ny) ] for i in range(self.nx) ]
        
    def createVectorForPos(self, i, j, k):
        mx = self.nx / 2.0
        my = self.ny / 2.0
        mz = self.nz / 2.0

        p = vector(i, j, k)
        mp = vector(mx, j, mz)
        center = vector(mx, my, mz)
        step_size = self.ny / 4.0
        
        den =  j - (self.ny/2.0)
        de = abs(den)
        #print "p =", p

        v = mp - p   # A - B == vector B->A
        xz_dir = vector(v.x, v.y, v.z)
        xz_dir.normalize() #xz_dir is the direction in the XZ plane perpendicular to the central Y axis, pointing in the direction of P

        r = v.length()
        #print "raio =", r

        def getTangent():
            cc = vector(0,0,0)
            cc.y = den
            cc.normalize()
            cc = cc * step_size
            cc = center + cc #cc a point in the central Y axis, in the XZ plane containing the center points of the circles in this elevation
            
            y_pp_dir = xz_dir * step_size #y_pp_dir is a vector perpendicular to the central Y axis)
            
            cc = cc + y_pp_dir  #now cc is the center point of the circle in this elevation and plane
            
            
            y_pp_dir = vector(y_pp_dir.z, y_pp_dir.y, -y_pp_dir.x)
            
            ccp = p - cc
            cr = ccp.length()
            if cr > step_size and r <= step_size:
                ccp.normalize()
                ccp.Scale(-1.0)
                return ccp
            
            ccp = ccp.Rotate(math.pi/2.0, y_pp_dir)
            ccp.normalize()
            ccp.Scale( cr/step_size )
            return ccp
        #######

        if r >= 2*step_size:
            v = mp - p
            v.normalize()
        elif de > step_size:
            nv = getTangent()
            if den > 0:
                nv.Scale(-1.0)
            v = nv
        else:
            v.x = 0.0
            v.z = 0.0

            if r <= step_size:
                if r == 0.0:
                    v.y = -1.0 #-(self.nx / 4.0)
                else:
                    v.y = -1.0 #-(self.nx / (4.0 * r**2))
            else:
                v.y = 1.0 #self.nx / (4*r - self.nx)
        
        return v
        
    def generate(self):
        for i in range(self.nx):
            for j in range(self.ny):
                for k in range(self.nz):
                    self.cube[i][j][k] = self.createVectorForPos(i, j, k)
        
    def save(self, filename):
        f = open(filename, "w")
        f.write("%s %s %s\n" % (self.nx, self.ny, self.nz) )
        f.write("%s %s %s\n" % (self.dx, self.dy, self.dz) )
        for i in range(self.nx):
            for j in range(self.ny):
                for k in range(self.nz):
                    v = self.cube[i][j][k]
                    f.write("%s %s %s\n" % (v.x, v.y, v.z) )
        f.close()
      
def invoke(filename):
    print "===== INVOKING EP1 FOR CHECKING ====="
    os.system("./bin/ep1 ./campos/%s " % (filename)  )

def run():
    #print "Name of the file to save the generated field?"
    savename = sys.argv[1] #raw_input()
    #print "Enter, in order and in succesion, the NX, NY and NZ values of the field"
    nx = sys.argv[2] #raw_input()
    ny = sys.argv[3] #raw_input()
    nz = sys.argv[4] #raw_input()
    #print "Enter, in order and in succesion, the DX, DY and DZ values of the field"
    dx = sys.argv[5] #raw_input()
    dy = sys.argv[6] #raw_input()
    dz = sys.argv[7] #raw_input()
    print "Generating field..."
    campo = field(nx,ny,nz,dx,dy,dz)
    campo.generate()
    campo.save("./campos/"+savename)
    
    if len(sys.argv) >= 9 and sys.argv[8] == "display":
        invoke(savename)

if __name__ == "__main__":
    if len(sys.argv) < 8:
        print "generator.py should be called as:"
        print "\t./generator.py <FILENAME> <NX> <NY> <NZ> <DX> <DY> <DZ> [display]"
    else:
        run()
