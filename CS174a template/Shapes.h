#pragma once
#include "Utilities.h"

struct ShapeData	{	GLuint vao;
						int numVertices;	};

void generateTurretFront(GLuint program, ShapeData* turretData);
void generateSlant(GLuint program, ShapeData* slantData);
void generatePyramid(GLuint program, ShapeData* pyramidData);
void generateCube(GLuint program, ShapeData* cubeData);
void generateSphere(GLuint program, ShapeData* sphereData);
void generateCone(GLuint program, ShapeData* coneData);
void generateCylinder(GLuint program, ShapeData* cylData);

//----------------------------------------------------------------------------
// Turret //Custom Shape

const int numTurretVertices = 84;

point4 turretPoints [numTurretVertices];
point3 turretNormals[numTurretVertices];
point2 turretUV     [numTurretVertices];

point4 verticesTurret[8] = {
    point4( -0.5, -0.5,  -0.5, 1 ),
    point4( 0.5,  -0.5,  -0.5, 1 ),
    point4( -0.5,  0.5,  -0.5, 1 ),
    point4(  0.5, 0.5,  -0.5, 1 ),
    point4( -0.2, -0.5, 0.5, 1 ),
    point4( -0.2,  0.5, 0.5, 1 ),
    point4(  0.2,  -0.5, 0.5, 1 ),
    point4(  0.2, 0.5, 0.5, 1 )
};

int TurIndex = 0;
void quadTurret( int a, int b, int c, int d, const point3& normal )
{
    turretPoints[TurIndex] = verticesTurret[a]; turretNormals[TurIndex] = normal;
    turretUV[TurIndex] = point2(0, 1); TurIndex++;
    
    turretPoints[TurIndex] = verticesTurret[b]; turretNormals[TurIndex] = normal;
    turretUV[TurIndex] = point2(0, 0); TurIndex++;
    
    turretPoints[TurIndex] = verticesTurret[c]; turretNormals[TurIndex] = normal;
    turretUV[TurIndex] = point2(1, 0); TurIndex++;
    
    turretPoints[TurIndex] = verticesTurret[a]; turretNormals[TurIndex] = normal;
    turretUV[TurIndex] = point2(0, 1); TurIndex++;
    
    turretPoints[TurIndex] = verticesTurret[d]; turretNormals[TurIndex] = normal;
    turretUV[TurIndex] = point2(1, 0); TurIndex++;
    
    turretPoints[TurIndex] = verticesTurret[c]; turretNormals[TurIndex] = normal;
    turretUV[TurIndex] = point2(0, 1); TurIndex++;
}

void colorTurret()
{
    quadTurret(1, 0, 2, 3, point3(0, 0, -1));
    quadTurret(1, 6, 4, 0, point3(0, -1, 0));
    quadTurret(3, 7, 5, 2, point3(0, 1, 0));
    quadTurret(6, 4, 5, 7, point3(0, 0, 1));
    quadTurret(1, 6, 7, 3, point3(.98, 0, .196));
    quadTurret(0, 4, 5, 2, point3(-.98, 0, .196));
}

void generateTurretFront(GLuint program, ShapeData* turretData)
{
    colorTurret();
    turretData->numVertices = numTurretVertices;
    
    glGenVertexArrays( 1, &turretData->vao );
    glBindVertexArray( turretData->vao );
    
    setVertexAttrib(program,
                    (float*)turretPoints,  sizeof(turretPoints),
                    (float*)turretNormals, sizeof(turretNormals),
                    (float*)turretUV,      sizeof(turretUV));
}


//----------------------------------------------------------------------------
// Slant //Custom Shape

const int numSlantVertices = 24;

point4 slantPoints [numSlantVertices];
point3 slantNormals[numSlantVertices];
point2 slantUV     [numSlantVertices];

point4 verticesSlant[6] = {
    point4(-0.5, -0.5, -0.5, 1),
    point4(0.5, -0.5, -0.5, 1),
    point4(-0.5, -0.5, 0.5, 1),
    point4(0.5, -0.5, 0.5, 1),
    point4(-0.5, 0.5, -0.5, 1),
    point4(0.5, 0.5, -0.5, 1)
};

int SlIndex = 0;
void quadSlant( int a, int b, int c, int d, const point3& normal )
{
    slantPoints[SlIndex] = verticesSlant[a]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(0, 1); SlIndex++;
    
    slantPoints[SlIndex] = verticesSlant[b]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(0, 0); SlIndex++;
    
    slantPoints[SlIndex] = verticesSlant[c]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(1, 0); SlIndex++;
    
    slantPoints[SlIndex] = verticesSlant[a]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(0, 1); SlIndex++;
    
    slantPoints[SlIndex] = verticesSlant[d]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(1, 0); SlIndex++;
    
    slantPoints[SlIndex] = verticesSlant[c]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(1, 1); SlIndex++;
}

void slantTriangles( int a, int b, int c, const point3& normal )
{
    slantPoints[SlIndex] = verticesSlant[a]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(0, 1); SlIndex++;
    
    slantPoints[SlIndex] = verticesSlant[b]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(0, 0); SlIndex++;
    
    slantPoints[SlIndex] = verticesSlant[c]; slantNormals[SlIndex] = normal;
    slantUV[SlIndex] = point2(1, 0); SlIndex++;
}

void colorSlant()
{
    quadSlant(3, 2, 0, 1, point3(0, -1, 0));
    quadSlant(1, 0, 4, 5, point3(0, 0, -1));
    quadSlant(3, 2, 4, 5, point3(0.707, 0.707, 0));
    slantTriangles(3, 1, 5, point3(1, 0, 0));
    slantTriangles(2, 0, 4, point3(-1, 0, 0));
}

void generateSlant(GLuint program, ShapeData* slantData)
{
    colorSlant();
    slantData->numVertices = numSlantVertices;
    
    glGenVertexArrays( 1, &slantData->vao );
    glBindVertexArray( slantData->vao );
    
    setVertexAttrib(program,
                    (float*)slantPoints,  sizeof(slantPoints),
                    (float*)slantNormals, sizeof(slantNormals),
                    (float*)slantUV,      sizeof(slantUV));
}

//----------------------------------------------------------------------------
// Pyramid //Custom Shape

const int numPyramidVertices = 18;

point4 pyramidPoints [numPyramidVertices];
point3 pyramidNormals[numPyramidVertices];
point2 pyramidUV     [numPyramidVertices];

point4 verticesPyramid[5] = {
    point4(-0.5, -0.5, 0.5, 1),
    point4(0.5, -0.5, 0.5, 1),
    point4(-0.5, -0.5, -0.5, 1),
    point4(0.5, -0.5, -0.5, 1),
    point4(0, 0.5, 0, 1)
};

int PyIndex = 0;
void quadPyramid( int a, int b, int c, int d, const point3& normal )
{
    pyramidPoints[PyIndex] = verticesPyramid[a]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(0, 1); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[b]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(0, 0); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[c]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(1, 0); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[a]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(0, 1); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[d]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(1, 0); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[c]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(1, 1); PyIndex++;
}

void triangles( const point3& normal, const point3& normal1, const point3& normal2, const point3& normal3)
{
    pyramidPoints[PyIndex] = verticesPyramid[0]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(0, 1); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[1]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(0, 0); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[4]; pyramidNormals[PyIndex] = normal;
    pyramidUV[PyIndex] = point2(1, 0); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[1]; pyramidNormals[PyIndex] = normal1;
    pyramidUV[PyIndex] = point2(0, 1); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[3]; pyramidNormals[PyIndex] = normal1;
    pyramidUV[PyIndex] = point2(0, 0); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[4]; pyramidNormals[PyIndex] = normal1;
    pyramidUV[PyIndex] = point2(1, 0); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[3]; pyramidNormals[PyIndex] = normal2;
    pyramidUV[PyIndex] = point2(0, 1); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[2]; pyramidNormals[PyIndex] = normal2;
    pyramidUV[PyIndex] = point2(0, 0); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[4]; pyramidNormals[PyIndex] = normal2;
    pyramidUV[PyIndex] = point2(1, 0); PyIndex++;
    
    pyramidPoints[PyIndex] = verticesPyramid[2]; pyramidNormals[PyIndex] = normal3;
    pyramidUV[PyIndex] = point2(0, 1); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[0]; pyramidNormals[PyIndex] = normal3;
    pyramidUV[PyIndex] = point2(0, 0); PyIndex++;
    pyramidPoints[PyIndex] = verticesPyramid[4]; pyramidNormals[PyIndex] = normal3;
    pyramidUV[PyIndex] = point2(1, 0); PyIndex++;
}

void colorPyramid()
{
    quadPyramid(1, 0, 2, 3, point3(0, -1, 0));
    triangles( point3(0, 0.707, 0.707),
               point3(0.707, 0.707, 0),
               point3(0, 0.707, -0.707),
               point3(-0.707, 0.707, 0));
}

void generatePyramid(GLuint program, ShapeData* pyramidData)
{
    colorPyramid();
    pyramidData->numVertices = numPyramidVertices;
    
    glGenVertexArrays( 1, &pyramidData->vao );
    glBindVertexArray( pyramidData->vao );
    
    setVertexAttrib(program,
                    (float*)pyramidPoints,  sizeof(pyramidPoints),
                    (float*)pyramidNormals, sizeof(pyramidNormals),
                    (float*)pyramidUV,      sizeof(pyramidUV));
}

//----------------------------------------------------------------------------
// Cube

const int numCubeVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 cubePoints [numCubeVertices];
point3 cubeNormals[numCubeVertices];
point2 cubeUV     [numCubeVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1 ),
    point4( -0.5,  0.5,  0.5, 1 ),
    point4(  0.5,  0.5,  0.5, 1 ),
    point4(  0.5, -0.5,  0.5, 1 ),
    point4( -0.5, -0.5, -0.5, 1 ),
    point4( -0.5,  0.5, -0.5, 1 ),
    point4(  0.5,  0.5, -0.5, 1 ),
    point4(  0.5, -0.5, -0.5, 1 )
};

// quad generates two triangles for each face and assigns normals and texture coordinates
//    to the vertices
int Index = 0;
void quad( int a, int b, int c, int d, const point3& normal )
{
    cubePoints[Index] = vertices[a]; cubeNormals[Index] = normal; 
    cubeUV[Index] = point2(0, 1); Index++;
    
    cubePoints[Index] = vertices[b]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0, 0); Index++;
    
    cubePoints[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1, 0); Index++;
    
    cubePoints[Index] = vertices[a]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0, 1); Index++;
    
    cubePoints[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1, 0); Index++;
    
    cubePoints[Index] = vertices[d]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1, 1); Index++;
}

// generate 12 triangles: 36 vertices, 36 normals, 36 texture coordinates
void colorcube()
{
    quad( 1, 0, 3, 2, point3( 0,  0,  1) );
    quad( 2, 3, 7, 6, point3( 1,  0,  0) );
    quad( 3, 0, 4, 7, point3( 0, -1,  0) );
    quad( 6, 5, 1, 2, point3( 0,  1,  0) );
    quad( 4, 5, 6, 7, point3( 0,  0, -1) );
    quad( 5, 4, 0, 1, point3(-1,  0,  0) );
}

// initialization
void generateCube(GLuint program, ShapeData* cubeData)
{
    colorcube();
    cubeData->numVertices = numCubeVertices;

    // Create a vertex array object
    glGenVertexArrays( 1, &cubeData->vao );
    glBindVertexArray( cubeData->vao );

    // Set vertex attributes
    setVertexAttrib(program, 
        (float*)cubePoints,  sizeof(cubePoints), 
        (float*)cubeNormals, sizeof(cubeNormals),
        (float*)cubeUV,      sizeof(cubeUV));
}


//----------------------------------------------------------------------------
// Sphere approximation by recursive subdivision of a tetrahedron

const int N = 5;  // number of subdivisions
const int numSphereVertices = 16*256*3;  // number of resulting points

point4 spherePoints [numSphereVertices];
point3 sphereNormals[numSphereVertices];
point2 sphereUVs[numSphereVertices];

// four equally spaced points on the unit circle

point4 v[4] = {
    vec4( 0, 0, 1, 1 ), 
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
};

static int k = 0;

// move a point to unit circle

point4 unit(const point4 &p)
{
    point4 c;
    double d=0;
    for(int i=0; i<3; i++) d+=p[i]*p[i];
    d=sqrt(d);
    if(d > 0) for(int i=0; i<3; i++) c[i] = p[i]/d;
    c[3] = 1;
    return c;
}

void triangle(point4 a, point4 b, point4 c)
{
    spherePoints[k] = a;
    k++;
    spherePoints[k] = b;
    k++;
    spherePoints[k] = c;
    k++;
}

void divide_triangle(point4 a, point4 b, point4 c, int n)
{
    point4 v1, v2, v3;
    if(n>0)
    {
        v1 = unit(a + b);
        v2 = unit(a + c);
        v3 = unit(b + c);   
        divide_triangle(a , v2, v1, n-1);
        divide_triangle(c , v3, v2, n-1);
        divide_triangle(b , v1, v3, n-1);
        divide_triangle(v1, v2, v3, n-1);
    }
    else triangle(a, b, c);
}

void tetrahedron(int n)
{
    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n);
    divide_triangle(v[0], v[3], v[1], n);
    divide_triangle(v[0], v[3], v[2], n);
}

// initialization

void generateSphere(GLuint program, ShapeData* sphereData)
{
    tetrahedron(N);

    sphereData->numVertices = numSphereVertices;

    // Normals
    for (int i = 0; i < numSphereVertices; i++)
    {
        sphereNormals[i] = point3(spherePoints[i].x, spherePoints[i].y, spherePoints[i].z);
    }
    
    // TexCoords
    double u, v;
    for (int i = 0; i < numSphereVertices; i++)
    {
        v = 0.5 - asin(spherePoints[i].y)/PI ; //0~1
        u = 0.5*(atan2(spherePoints[i].z,spherePoints[i].x)/PI + 1); //0~1
        sphereUVs[i] = point2(u,v);
    }

    // Create a vertex array object
    glGenVertexArrays( 1, &sphereData->vao );
    glBindVertexArray( sphereData->vao );

    // Set vertex attributes
    setVertexAttrib(program,
        (float*)spherePoints,  sizeof(spherePoints),
        (float*)sphereNormals, sizeof(sphereNormals),
        (float*)sphereUVs, sizeof(sphereUVs));
}

//----------------------------------------------------------------------------
// Cone

const int numConeDivisions = 32;
const int numConeVertices = numConeDivisions * 6;

point4 conePoints [numConeVertices];
point3 coneNormals[numConeVertices];

point2 circlePoints[numConeDivisions];

void makeCircle(point2* dest, int numDivisions)
{
    for (int i = 0; i < numDivisions; i++)
    {
        float a = i * 2 * PI / numDivisions;
        dest[i] = point2(cosf(a), sinf(a));
    }
}

void makeConeWall(point4* destp, point3* destn, int numDivisions, float z1, float z2, int& Index, int dir)
{
    for (int i = 0; i < numDivisions; i++)
    {
        point3 p1(circlePoints[i].x, circlePoints[i].y, z1);
        point3 p2(0, 0, z2);
        point3 p3(circlePoints[(i+1)%numDivisions].x, circlePoints[(i+1)%numDivisions].y, z1);
        if (dir == -1)
        {
            point3 temp = p1;
            p1 = p3;
            p3 = temp;
        }
        point3 n = cross(p1-p2, p3-p2);
        destp[Index] = p1; destn[Index] = n; Index++;
        destp[Index] = p2; destn[Index] = n; Index++;
        destp[Index] = p3; destn[Index] = n; Index++;
    }    
}

void generateCone(GLuint program, ShapeData* coneData)
{
    makeCircle(circlePoints, numConeDivisions);
    int Index = 0;
    makeConeWall(conePoints, coneNormals, numConeDivisions, 1, 1, Index, 1);
    makeConeWall(conePoints, coneNormals, numConeDivisions, 1, -1, Index, -1);
    
    coneData->numVertices = numConeVertices;

    // Create a vertex array object
    glGenVertexArrays( 1, &coneData->vao );
    glBindVertexArray( coneData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)conePoints,  sizeof(conePoints),
                    (float*)coneNormals, sizeof(coneNormals),
                    0, 0 );
}

//----------------------------------------------------------------------------
// Cylinder

const int numCylDivisions = 32;
const int numCylVertices = numCylDivisions * 12;

point4 cylPoints [numCylVertices];
point3 cylNormals[numCylVertices];

void generateCylinder(GLuint program, ShapeData* cylData)
{
    makeCircle(circlePoints, numCylDivisions);
    int Index = 0;
    makeConeWall(cylPoints, cylNormals, numCylDivisions, 1, 1, Index, 1);
    makeConeWall(cylPoints, cylNormals, numCylDivisions, -1, -1, Index, -1);
    
    for (int i = 0; i < numCylDivisions; i++)
    {
        int i2 = (i+1)%numCylDivisions;
        point3 p1(circlePoints[i2].x, circlePoints[i2].y, -1);
        point3 p2(circlePoints[i2].x, circlePoints[i2].y, 1);
        point3 p3(circlePoints[i].x,  circlePoints[i].y,  1);
        //point3 n = cross(p3-p2, p1-p2);
        cylPoints[Index] = p1; cylNormals[Index] = point3(p1.x, p1.y, 0); Index++;
        cylPoints[Index] = p2; cylNormals[Index] = point3(p2.x, p2.y, 0); Index++;
        cylPoints[Index] = p3; cylNormals[Index] = point3(p3.x, p3.y, 0); Index++;
        p1 = point3(circlePoints[i2].x, circlePoints[i2].y, -1);
        p2 = point3(circlePoints[i].x,  circlePoints[i].y,  1);
        p3 = point3(circlePoints[i].x,  circlePoints[i].y,  -1);
        //n = cross(p3-p2, p1-p2);
        cylPoints[Index] = p1; cylNormals[Index] = point3(p1.x, p1.y, 0); Index++;
        cylPoints[Index] = p2; cylNormals[Index] = point3(p2.x, p2.y, 0); Index++;
        cylPoints[Index] = p3; cylNormals[Index] = point3(p3.x, p3.y, 0); Index++;
    }
    
    cylData->numVertices = numCylVertices;
    
    // Create a vertex array object
    glGenVertexArrays( 1, &cylData->vao );
    glBindVertexArray( cylData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)cylPoints,  sizeof(cylPoints),
                    (float*)cylNormals, sizeof(cylNormals),
                    0, 0 );
}