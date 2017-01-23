#include <math.h>
#include <GL/glew.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Models.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

Models::Models()
{
    lon = 0;
    lat = 0;
    dat = NULL;
    normals = NULL;
    srand(static_cast<unsigned int>(clock()));
}

Models::~Models()
{
    if (dat != NULL) delete[] dat;
    if (normals != NULL) delete[] normals;
}

void Models::resetArraySize(GLint ilon, GLint ilat)
{
    if (dat != NULL) delete[] dat;
    if (normals != NULL) delete[] normals;

    lon = ilon;
    lat = ilat;
    dat = new GLfloat[3*(lon+1)*(lat+1)];
    normals = new GLfloat[3*(lon+1)*(lat+1)];
}

int Models::pos(int i, int j, int k)
{
    return 3*((lon+1)*j+i)+k;
}

void Models::CrossProd(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat *cx, GLfloat *cy, GLfloat *cz)
{
    *cx = y1*z2-y2*z1;
    *cy = -x1*z2+z1*x2;
    *cz = x1*y2-x2*y1;
}

void Models::createNormals()
{
    GLfloat v1x, v1y, v1z, v2x, v2y, v2z;
    GLfloat cx, cy, cz;
    GLfloat scx, scy, scz;
    int v1i, v1j, v2i, v2j;
    GLfloat mod;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            scx = 0;
            scy = 0;
            scz = 0;
            for (int k = 0; k < 8; k++)
            {
                switch (k)
                {
                case 0:
                    v1i = i+1;
                    v1j = j;
                    v2i = i+1;
                    v2j = j+1;
                    break;
                case 1:
                    v1i = i+1;
                    v1j = j+1;
                    v2i = i;
                    v2j = j+1;
                    break;
                case 2:
                    v1i = i;
                    v1j = j+1;
                    v2i = i-1;
                    v2j = j+1;
                    break;
                case 3:
                    v1i = i-1;
                    v1j = j+1;
                    v2i = i-1;
                    v2j = j;
                    break;
                case 4:
                    v1i = i-1;
                    v1j = j;
                    v2i = i-1;
                    v2j = j-1;
                    break;
                case 5:
                    v1i = i-1;
                    v1j = j-1;
                    v2i = i;
                    v2j = j-1;
                    break;
                case 6:
                    v1i = i;
                    v1j = j-1;
                    v2i = i+1;
                    v2j = j-1;
                    break;
                case 7:
                    v1i = i+1;
                    v1j = j-1;
                    v2i = i+1;
                    v2j = j;
                    break;
                }

                if ((v1i >= 0) && (v1i <= lon) && (v1j >= 0) && (v1j <= lat) && (v2i >= 0) && (v2i <= lon) && (v2j >= 0) && (v2j <= lat))
                {
                    v1x = dat[pos(v1i,v1j,0)] - dat[pos(i,j,0)];
                    v1y = dat[pos(v1i,v1j,1)] - dat[pos(i,j,1)];
                    v1z = dat[pos(v1i,v1j,2)] - dat[pos(i,j,2)];
                    v2x = dat[pos(v2i,v2j,0)] - dat[pos(i,j,0)];
                    v2y = dat[pos(v2i,v2j,1)] - dat[pos(i,j,1)];
                    v2z = dat[pos(v2i,v2j,2)] - dat[pos(i,j,2)];

                    CrossProd(v1x, v1y, v1z, v2x, v2y, v2z, &cx, &cy, &cz);
                    mod = sqrt(cx*cx + cy*cy + cz*cz);
                    if (mod > 0.000001)
                    {
                        scx += cx;
                        scy += cy;
                        scz += cz;
                    }
                }
            }
            mod = sqrt(scx*scx + scy*scy + scz*scz);
            if (mod > 0.000001)
            {
                normals[pos(i,j,0)] = scx/mod;
                normals[pos(i,j,1)] = scy/mod;
                normals[pos(i,j,2)] = scz/mod;
            }
            else
            {
                normals[pos(i,j,0)] = scx;
                normals[pos(i,j,1)] = scy;
                normals[pos(i,j,2)] = scz;
            }
        }
}

void Models::reverseNormals()
{
    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
            for (int k = 0; k < 3; k++)
                normals[pos(i,j,k)] = -normals[pos(i,j,k)];
}

void Models::setDrawBorder(GLboolean d)
{
    drawBorder = d;
}

void Models::setDrawFaces(GLboolean d)
{
    drawFaces = d;
}

void Models::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
    reloadData();
}

void Models::setBorderColor(GLfloat r, GLfloat g, GLfloat b)
{
    bred = r;
    bgreen = g;
    bblue = b;
    reloadData();
}

void Models::reloadData()
{
    reload = GL_TRUE;
    load(vPosition, vColor);
}

void Models::load(GLint v, GLint c)
{
    vPosition = v;
    vColor = c;

    GLuint *indices;
    GLuint *border_indices;
    GLfloat *points;
    GLfloat *colors;
    GLfloat *border_colors;

    indices = new GLuint[6*(lon)*(lat)];
    border_indices = new GLuint[4*(lon)*(lat)+2*(lon+lat)];
    points = new GLfloat[4*(lon+1)*(lat+1)];
    colors = new GLfloat[3*(lon+1)*(lat+1)];
    border_colors = new GLfloat[3*(lon+1)*(lat+1)];

    for (int i = 0; i < (lon+1)*(lat+1); i++)
    {
        colors[3*i] = red;
        colors[3*i+1] = green;
        colors[3*i+2] = blue;
        border_colors[3*i] = bred;
        border_colors[3*i+1] = bgreen;
        border_colors[3*i+2] = bblue;
    }

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            points[4*i*(lat+1)+4*j] = dat[pos(i,j,0)];
            points[4*i*(lat+1)+4*j+1] = dat[pos(i,j,1)];
            points[4*i*(lat+1)+4*j+2] = dat[pos(i,j,2)];
            points[4*i*(lat+1)+4*j+3] = 1;
        }

    int indexarraypos = 0;
    for (int i = 0; i < lon; i++)
        for (int j = 0; j < lat; j++)
        {
            indices[indexarraypos++] = i*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j+1;
            indices[indexarraypos++] = i*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j+1;
            indices[indexarraypos++] = i*(lat+1)+j+1;;
        }

    indexarraypos = 0;
    for (int i = 0; i < lon; i++)
        for (int j = 0; j < lat; j++)
        {
            border_indices[indexarraypos++] = i*(lat+1)+j;
            border_indices[indexarraypos++] = (i+1)*(lat+1)+j;
            border_indices[indexarraypos++] = i*(lat+1)+j;
            border_indices[indexarraypos++] = i*(lat+1)+j+1;
        }

    for (int i = 0; i < lon; i++)
    {
        border_indices[indexarraypos++] = i*(lat+1)+lat;
        border_indices[indexarraypos++] = (i+1)*(lat+1)+lat;
    }

    for (int j = 0; j < lat; j++)
    {
        border_indices[indexarraypos++] = lon*(lat+1)+j;
        border_indices[indexarraypos++] = lon*(lat+1)+j+1;
    }

    if (!reload)
        glGenVertexArrays(1, &vboptr);

    glBindVertexArray(vboptr);

    if (!reload)
        glGenBuffers(1, &eboptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*lon*lat*sizeof(GLuint), indices, GL_DYNAMIC_DRAW);

    if (!reload)
        glGenBuffers(1, &bufptr);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat) + 3*(lon+1)*(lat+1)*sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*(lon+1)*(lat+1)*sizeof(GLfloat), points);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat), 3*(lon+1)*(lat+1)*sizeof(GLfloat), colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);

    if (!reload)
        glGenVertexArrays(1, &vboptr2);

    glBindVertexArray(vboptr2);

    if (!reload)
        glGenBuffers(1, &eboptr2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (4*(lon)*(lat)+2*(lon+lat))*sizeof(GLuint), border_indices, GL_DYNAMIC_DRAW);

    if (!reload)
        glGenBuffers(1, &bufptr2);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr2);
    glBufferData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat) + 3*(lon+1)*(lat+1)*sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*(lon+1)*(lat+1)*sizeof(GLfloat), points);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat), 3*(lon+1)*(lat+1)*sizeof(GLfloat), border_colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);

    if (indices) delete indices;
    if (border_indices) delete border_indices;
    if (points) delete points;
    if (colors) delete colors;
    if (border_colors) delete border_colors;

    reload = GL_FALSE;
}

void Models::draw()
{
    if (drawFaces)
    {
        glBindVertexArray(vboptr);
        glDrawElements(GL_TRIANGLES, 6*lon*lat, GL_UNSIGNED_INT, NULL);
    }

    if (drawBorder)
    {
        glBindVertexArray(vboptr2);
        glLineWidth(2);
        glDrawElements(GL_LINES, 4*(lon)*(lat)+2*(lon+lat), GL_UNSIGNED_INT, NULL);
        glLineWidth(1);
    }
}

void Models::createSphereOBJ(GLfloat r, GLint ilon, GLint ilat)
{
    createPartialSphereOBJ(r, ilon, ilat, 0, 2*PI, -PI/2, PI/2);
}

void Models::createPartialSphereOBJ(GLfloat r, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsy, GLfloat endpsy)
{
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = begintheta + (endtheta-begintheta)*((float)i/lon);
            GLfloat Psy = beginpsy + (endpsy-beginpsy)*((float)j/lat);

            dat[pos(i,j,0)] = r*cos(Psy)*cos(Theta);
            dat[pos(i,j,1)] = r*cos(Psy)*sin(Theta);
            dat[pos(i,j,2)] = r*sin(Psy);
            normals[pos(i,j,0)] = dat[pos(i,j,0)]/r;
            normals[pos(i,j,1)] = dat[pos(i,j,1)]/r;
            normals[pos(i,j,2)] = dat[pos(i,j,2)]/r;
        }
}

void Models::createTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat)
{
    createPartialTorusOBJ(irad, orad, ilon, ilat, -PI, PI, -PI, PI);
}

void Models::createPartialTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsy, GLfloat endpsy)
{
    GLfloat r = (orad - irad)/2;
    GLfloat rax = r+irad;
    resetArraySize(ilon, ilat);
    GLdouble cx, cy, cz;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = begintheta + (endtheta-begintheta)*((float)i/lon);
            GLfloat Psy = endpsy - (beginpsy + (endpsy-beginpsy)*((float)j/lat));

            dat[pos(i,j,0)] = (rax-r*cos(Psy))*cos(Theta);
            dat[pos(i,j,1)] = (rax-r*cos(Psy))*sin(Theta);
            dat[pos(i,j,2)] = r*sin(Psy);

            cx = -r*cos(Psy)*cos(Theta);
            cy = -r*cos(Psy)*sin(Theta);
            cz = r*sin(Psy);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

void Models::createTrefoilOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = 4*PI-4*PI*((float)i/lon);
            GLfloat Psy = 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = rmin*cos(Psy)*cos(Theta)+rmaj*cos(Theta)*(1+amp*cos(numtwists*Theta));
            dat[pos(i,j,1)] = elongfact*rmin*sin(Psy)+amp*sin(numtwists*Theta);
            dat[pos(i,j,2)] = rmin*cos(Psy)*sin(Theta)+rmaj*sin(Theta)*(1+amp*cos(numtwists*Theta));

            x2 = -rmin*cos(Psy)*sin(Theta)-rmaj*sin(Theta)*(1+amp*cos(numtwists*Theta))-rmaj*cos(Theta)*(numtwists*amp*sin(numtwists*Theta));
            y2 = numtwists*amp*cos(numtwists*Theta);
            z2 = rmin*cos(Psy)*cos(Theta)+rmaj*cos(Theta)*(1+amp*cos(numtwists*Theta))-rmaj*sin(Theta)*(numtwists*amp*sin(numtwists*Theta));
            x1 = -rmin*sin(Psy)*cos(Theta);
            y1 = elongfact*rmin*cos(Psy);
            z1 = -rmin*sin(Psy)*sin(Theta);

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

void Models::createUmbilicTorusOBJ(GLint ilon, GLint ilat)
{
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = 2*PI - 2*PI*((float)i/lon);
            GLfloat Psy = 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = sin(Theta)*(7+cos(Theta/3-2*Psy)+2*cos(Theta/3+Psy));
            dat[pos(i,j,1)] = cos(Theta)*(7+cos(Theta/3-2*Psy)+2*cos(Theta/3+Psy));
            dat[pos(i,j,2)] = sin(Theta/3-2*Psy)+2*sin(Theta/3+Psy);
        }
    createNormals();  //  Since the edges are unnatural
}

void Models::createBraidedTorusOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = 8*PI - 8*PI*((float)i/lon);
            GLfloat Psy = 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = rmin*cos(Psy)*cos(Theta)+rmaj*cos(Theta)*(1+amp*cos(numtwists*Theta));
            dat[pos(i,j,1)] = elongfact*(rmin*sin(Psy)+amp*sin(numtwists*Theta));
            dat[pos(i,j,2)] = rmin*cos(Psy)*sin(Theta)+rmaj*sin(Theta)*(1+amp*cos(numtwists*Theta));

            x1 = -rmin*sin(Psy)*cos(Theta);
            y1 = elongfact*rmin*cos(Psy);
            z1 = -rmin*sin(Psy)*sin(Theta);

            x2 = -rmin*cos(Psy)*sin(Theta)-rmaj*sin(Theta)*(1+amp*cos(numtwists*Theta))-rmaj*cos(Theta)*(numtwists*amp*sin(numtwists*Theta));
            y2 = elongfact*(numtwists*amp*cos(numtwists*Theta));
            z2 = rmin*cos(Psy)*cos(Theta)+rmaj*cos(Theta)*(1+amp*cos(numtwists*Theta))-rmaj*sin(Theta)*(numtwists*amp*sin(numtwists*Theta));

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

void Models::createCylinderOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat)
{
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
    {
        GLfloat Theta = 2*PI - 2*PI*((float)i/lon);
        for (int j = 0; j <= lat; j++)
        {
            GLfloat x = len*((float)j/lat);
            GLfloat Rad = (r2-r1)/len*x + r1;

            dat[pos(i,j,0)] = Rad*sin(Theta);
            dat[pos(i,j,1)] = Rad*cos(Theta);
            dat[pos(i,j,2)] = x;
        }
    }
}


void Models::createLatheOBJ(GLfloat Xlist[], GLfloat Ylist[], int size, GLint ilon)
{
    resetArraySize(ilon, size-1);

    for (int i = 0; i <= lon; i++)
    {
        GLfloat Theta = 2*PI - 2*PI*((float)i/lon);
        for (int j = 0; j <= lat; j++)
        {
            GLfloat x = Xlist[j];
            GLfloat Rad = Ylist[j];

            dat[pos(i,j,0)] = Rad*sin(Theta);
            dat[pos(i,j,1)] = Rad*cos(Theta);
            dat[pos(i,j,2)] = x;
        }
    }
}


void Models::createExpHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = 2*PI - 2*PI*((float)i/lon);
            GLfloat x = len*((float)j/lat);
            GLfloat Rad = r1*exp(log(r2/r1)/len*x);

            dat[pos(i,j,0)] = Rad*sin(Theta);
            dat[pos(i,j,1)] = Rad*cos(Theta);
            dat[pos(i,j,2)] = x;

            x1 = log(r2/r1)/len*r1*exp(log(r2/r1)/len*x)*sin(Theta);
            y1 = log(r2/r1)/len*r1*exp(log(r2/r1)/len*x)*cos(Theta);
            z1 = 1;

            x2 = Rad*cos(Theta);
            y2 = -Rad*sin(Theta);
            z2 = 0;

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

void Models::createQuadHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = 2*PI - 2*PI*((float)i/lon);
            GLfloat x = len*((float)j/lat);
            GLfloat Rad = (r2-r1)/(len*len)*(x*x) + r1;

            dat[pos(i,j,0)] = Rad*sin(Theta);
            dat[pos(i,j,1)] = Rad*cos(Theta);
            dat[pos(i,j,2)] = x;

            x1 = 2.0*(r2-r1)/(len*len)*x*sin(Theta);
            y1 = 2.0*(r2-r1)/(len*len)*x*cos(Theta);
            z1 = 1;

            x2 = Rad*cos(Theta);
            y2 = -Rad*sin(Theta);
            z2 = 0;

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}


void Models::createHelicalTorusOBJ(GLfloat irad, GLfloat orad, GLfloat strFact, GLfloat begang, GLfloat endang, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);
    GLfloat r = (orad - irad)/2;
    GLfloat rax = r+irad;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = endang - (begang + (endang - begang)*((float)i/lon));
            GLfloat Psy = -PI + 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = (rax-r*cos(Psy))*cos(Theta);
            dat[pos(i,j,1)] = (rax-r*cos(Psy))*sin(Theta);
            dat[pos(i,j,2)] = strFact*Theta + r*sin(Psy);

            x1 = r*sin(Psy)*cos(Theta);
            y1 = r*sin(Psy)*sin(Theta);
            z1 = r*cos(Psy);

            x2 = -sin(Theta);
            y2 = cos(Theta);
            z2 = strFact;

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

void Models::createMobiusOBJ(GLfloat begv, GLfloat endv, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat Theta = 2*PI*((float)i/lon);
            GLfloat v = begv + (endv-begv)*((float)j/lat);

            dat[pos(i,j,0)] = cos(Theta)+v*cos(Theta/2)*cos(Theta);
            dat[pos(i,j,1)] = sin(Theta)+v*cos(Theta/2)*sin(Theta);
            dat[pos(i,j,2)] = v*sin(Theta/2);

            x2 = cos(Theta/2)*cos(Theta);
            y2 = cos(Theta/2)*sin(Theta);
            z2 = sin(Theta/2);

            x1 = -sin(Theta)+v*(-cos(Theta/2)*sin(Theta)-0.5*sin(Theta/2)*cos(Theta));
            y1 = cos(Theta)+v*(cos(Theta/2)*cos(Theta)-0.5*sin(Theta/2)*sin(Theta));
            z1 = 0.5*v*cos(Theta/2);

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

void Models::createTessellatedWallOBJ(GLfloat width, GLfloat height, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            dat[pos(i,j,0)] = -width/2.0+(float)i/(float)lon*width;
            dat[pos(i,j,1)] = -height/2.0+(float)j/(float)lat*height;
            dat[pos(i,j,2)] = 0;

            normals[pos(i,j,0)] = 0;
            normals[pos(i,j,1)] = 0;
            normals[pos(i,j,2)] = 1;
        }
}

void Models::perturbVeritces(float a)
{
    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
            for (int k = 0; k < 3; k++)
                dat[pos(i,j,k)] += (double(rand())/double(RAND_MAX)-0.5)*a;

    createNormals();
}

void Models::perturbNormalsRandom(float a)
{
    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
            for (int k = 0; k < 3; k++)
                normals[pos(i,j,k)] += (double(rand())/double(RAND_MAX)-0.5)*a;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat mod = sqrt(normals[pos(i,j,0)]*normals[pos(i,j,0)] +
                               normals[pos(i,j,1)]*normals[pos(i,j,1)] +
                               normals[pos(i,j,2)]*normals[pos(i,j,2)]);

            if (mod < 0.00001) mod = 0.00001;
            normals[pos(i,j,0)] /= mod;
            normals[pos(i,j,1)] /= mod;
            normals[pos(i,j,2)] /= mod;
        }
}
