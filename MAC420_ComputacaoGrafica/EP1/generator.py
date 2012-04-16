#!/usr/bin/python

import sys
import os

class vector:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)
        
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
        
        x = -(i - my)
        y = (j - mx)
        z = 0
        return vector(x,y,z)
        
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
