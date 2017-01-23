#ifndef __Models_h_
#define __Models_h_

#include <GL/glew.h>

#define PI 3.14159265358979323846
#define PI_DIV_180 0.017453292519943296
#define deg PI_DIV_180

class Models
{
private:
    GLint lon;
    GLint lat;
    GLfloat *dat;
    GLfloat *normals;

    void CrossProd(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat *cx, GLfloat *cy, GLfloat *cz);
    void resetArraySize(GLint ilon, GLint ilat);
    int pos(int i, int j, int k);

    GLint vPosition = 0;
    GLint vColor = 1;

    GLboolean reload = GL_FALSE;
    GLboolean drawFaces = GL_TRUE;
    GLboolean drawBorder = GL_TRUE;

    GLfloat red = 1;
    GLfloat green = 1;
    GLfloat blue = 1;
    GLfloat bred = 0;
    GLfloat bgreen = 0;
    GLfloat bblue = 0;

    GLuint vboptr;
    GLuint bufptr;
    GLuint eboptr;
    GLuint vboptr2;
    GLuint bufptr2;
    GLuint eboptr2;

public:
    Models();
    ~Models();

    void load(GLint v, GLint c);
    void reloadData();
    void draw();

    void setDrawBorder(GLboolean d);
    void setDrawFaces(GLboolean d);
    void setColorCube(GLboolean d);

    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setBorderColor(GLfloat r, GLfloat g, GLfloat b);

    void createNormals();
    void reverseNormals();
    void createSphereOBJ(GLfloat r, GLint ilon, GLint ilat);
    void createPartialSphereOBJ(GLfloat r, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsy, GLfloat endpsy);
    void createTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat);
    void createPartialTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsy, GLfloat endpsy);
    void createTrefoilOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint lon, GLint lat);
    void createUmbilicTorusOBJ(GLint ilon, GLint ilat);
    void createBraidedTorusOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint ilon, GLint ilat);
    void createCylinderOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat);
    void createLatheOBJ(GLfloat Xlist[], GLfloat Ylist[], int size, GLint ilon);
    void createExpHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat);
    void createQuadHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat);
    void createHelicalTorusOBJ(GLfloat irad, GLfloat orad, GLfloat strFact, GLfloat begang, GLfloat endang, GLint ilon, GLint ilat);
    void createMobiusOBJ(GLfloat begv, GLfloat endv, GLint ilon, GLint ilat);
    void createTessellatedWallOBJ(GLfloat width, GLfloat height, GLint ilon, GLint ilat);

    void perturbVeritces(float a);
    void perturbNormalsRandom(float a);
};

#endif
