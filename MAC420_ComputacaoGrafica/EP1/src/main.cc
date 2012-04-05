#include <cstdlib>
#include <cstdio>
#include <engine/engine.h>
#include <GL/glut.h>

/* triângulo inicial */
GLfloat v[3][2]={{-0.5, 0},{0.5, 0},{0, 0.8660}};

/* número de subdivisões */
int n; 
                      
/* desenha um triângulo */
void triangulo(GLfloat *v1, GLfloat *v2, GLfloat *v3)
{
    /* desenha um triângulo */
    glVertex2fv(v1); 
    glVertex2fv(v2);  
    glVertex2fv(v3);
}

/* subdivide triângulo */
void divide_triangulo(GLfloat *v1, GLfloat *v2, GLfloat *v3, int n)
{
    int j;
    GLfloat v12[2], v23[2], v13[2];
    if (n > 0)
    {
        for(j=0; j<2; j++) v12[j]=(v1[j]+v2[j])/2;
        for(j=0; j<2; j++) v23[j]=(v2[j]+v3[j])/2;
        for(j=0; j<2; j++) v13[j]=(v1[j]+v3[j])/2;
        divide_triangulo(v1, v12, v13, n-1);
        divide_triangulo(v12, v2, v23, n-1);
        divide_triangulo(v13, v23, v3, n-1);
    }
    /* ao final da recursão, desenha triângulo */
    else(triangulo(v1, v2, v3));
}

void display()
{   
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    divide_triangulo(v[0], v[1], v[2], n);
    glEnd();        
    //glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
    int x = 0, y = 0;
    if (w > h) {
        x = (w - h) / 2;
        w = h;
    } else if (w < h) {
        y = (h - w) / 2; 
        h = w;
    }

	glViewport((GLint)x, (GLint)y, (GLint)w, (GLint)h); 
}

void mouse(int btn, int state, int x, int y)
{
    if (btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
        n++;
    if (btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
        n > 0 ? n-- : n;
   	  
    glutPostRedisplay();
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,0.0);
}

int main(int argc, char* argv[]) {
    n=0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("MAC420 EP1 - Simulacao de Particulas em um Campo de Forcas");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse);

    init();

    glutMainLoop();

	engine::Engine* e = engine::Engine::reference();

	delete e;

    return 0;
}
