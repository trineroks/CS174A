// anim.cpp version 5.0 -- Template code for drawing an articulated figure.  CS 174A.
#include "../CS174a template/Utilities.h"
#include "../CS174a template/Shapes.h"

std::stack<mat4> mvstack;

int g_width = 800, g_height = 800 ;
float zoom = 1 ;

int animate = 0, recording = 0, basis_to_display = -1;
double TIME = 0;

const unsigned X = 0, Y = 1, Z = 2;

vec4 eye( 0, 20, 30, 1), ref( 0, 0, 0, 1 ), up( 0, 1, 0, 0 );	// The eye point and look-at point.

mat4	orientation, model_view;
ShapeData turretData, slantData, pyramidData, cubeData, sphereData, coneData, cylData;				// Structs that hold the Vertex Array Object index and number of vertices of each shape.
GLuint	texture_cube, texture_earth, texture_leopard;
GLint   uModelView, uProjection, uView,
		uAmbient, uDiffuse, uSpecular, uLightPos, uShininess,
		uTex, uEnableTex;

void init()
{
#ifdef EMSCRIPTEN
    GLuint program = LoadShaders( "vshader.glsl", "fshader.glsl" );								// Load shaders and use the resulting shader program
    TgaImage coolImage ("challenge.tga");    
    TgaImage earthImage("earth.tga");
    TgaImage leopardImage("../my code/LeopardCamo.tga");

#else
	GLuint program = LoadShaders( "../my code/vshader.glsl", "../my code/fshader.glsl" );		// Load shaders and use the resulting shader program
    TgaImage coolImage ("../my code/challenge.tga");
    TgaImage earthImage("../my code/earth.tga");
    TgaImage LeopardCamo("../my code/LeopardCamo.tga");
#endif
    glUseProgram(program);
    
    generateTurretFront(program, &turretData);
    generateSlant(program, &slantData);
    generatePyramid(program, &pyramidData);
	generateCube(program, &cubeData);		// Generate vertex arrays for geometric shapes
    generateSphere(program, &sphereData);
    generateCone(program, &coneData);
    generateCylinder(program, &cylData);

    uModelView  = glGetUniformLocation( program, "ModelView"  );
    uProjection = glGetUniformLocation( program, "Projection" );
    uView		= glGetUniformLocation( program, "View"       );
    uAmbient	= glGetUniformLocation( program, "AmbientProduct"  );
    uDiffuse	= glGetUniformLocation( program, "DiffuseProduct"  );
    uSpecular	= glGetUniformLocation( program, "SpecularProduct" );
    uLightPos	= glGetUniformLocation( program, "LightPosition"   );
    uShininess	= glGetUniformLocation( program, "Shininess"       );
    uTex		= glGetUniformLocation( program, "Tex"             );
    uEnableTex	= glGetUniformLocation( program, "EnableTex"       );

    glUniform4f( uAmbient,    0.2,  0.2,  0.2, 1 );
    glUniform4f( uDiffuse,    0.6,  0.6,  0.6, 1 );
    glUniform4f( uSpecular,   0.2,  0.2,  0.2, 1 );
    glUniform4f( uLightPos,  15.0, 15.0, 30.0, 0 );
    glUniform1f( uShininess, 100);

    glEnable(GL_DEPTH_TEST);
    
    glGenTextures( 1, &texture_cube );
    glBindTexture( GL_TEXTURE_2D, texture_cube );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, coolImage.width, coolImage.height, 0,
                 (coolImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, coolImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    
    glGenTextures( 1, &texture_earth );
    glBindTexture( GL_TEXTURE_2D, texture_earth );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, earthImage.width, earthImage.height, 0,
                 (earthImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, earthImage.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    glGenTextures( 1, &texture_leopard );
    glBindTexture( GL_TEXTURE_2D, texture_leopard );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, LeopardCamo.width, LeopardCamo.height, 0,
                 (LeopardCamo.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, LeopardCamo.data );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    
    glUniform1i( uTex, 0);	// Set texture sampler variable to texture unit 0
	
	glEnable(GL_DEPTH_TEST);
}

struct color{ color( float r, float g, float b) : r(r), g(g), b(b) {} float r, g, b;};
std::stack<color> colors;
void set_color(float r, float g, float b)
{
	colors.push(color(r, g, b));

	float ambient  = 0.2, diffuse  = 0.6, specular = 0.2;
    glUniform4f(uAmbient,  ambient*r,  ambient*g,  ambient*b,  1 );
    glUniform4f(uDiffuse,  diffuse*r,  diffuse*g,  diffuse*b,  1 );
    glUniform4f(uSpecular, specular*r, specular*g, specular*b, 1 );
}

int mouseButton = -1, prevZoomCoord = 0 ;
vec2 anchor;
void myPassiveMotionCallBack(int x, int y) {	anchor = vec2( 2. * x / g_width - 1, -2. * y / g_height + 1 ); }

void myMouseCallBack(int button, int state, int x, int y)	// start or end mouse interaction
{
    mouseButton = button;
   
    if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
        mouseButton = -1 ;
    if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
        prevZoomCoord = -2. * y / g_height + 1;

    glutPostRedisplay() ;
}

void myMotionCallBack(int x, int y)
{
	vec2 arcball_coords( 2. * x / g_width - 1, -2. * y / g_height + 1 );
	 
    if( mouseButton == GLUT_LEFT_BUTTON )
    {
	   orientation = RotateX( -10 * (arcball_coords.y - anchor.y) ) * orientation;
	   orientation = RotateY(  10 * (arcball_coords.x - anchor.x) ) * orientation;
    }
	
	if( mouseButton == GLUT_RIGHT_BUTTON )
		zoom *= 1 + .1 * (arcball_coords.y - anchor.y);
    glutPostRedisplay() ;
}

void idleCallBack(void)
{
    if( !animate ) return;
	double prev_time = TIME;
    TIME = TM.GetElapsedTime() ;
	if( prev_time == 0 ) TM.Reset();
    glutPostRedisplay() ;
}

void drawCylinder()	//render a solid cylinder oriented along the Z axis; bases are of radius 1, placed at Z = 0, and at Z = 1.
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cylData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cylData.numVertices );
}

void drawCone()	//render a solid cone oriented along the Z axis; bases are of radius 1, placed at Z = 0, and at Z = 1.
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( coneData.vao );
    glDrawArrays( GL_TRIANGLES, 0, coneData.numVertices );
}

void drawCube()		// draw a cube with dimensions 1,1,1 centered around the origin.
{
	glBindTexture( GL_TEXTURE_2D, texture_cube );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

void drawSkyBox()
{
    glBindTexture( GL_TEXTURE_2D, texture_earth );
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

void drawSphere()	// draw a sphere with radius 1 centered around the origin.
{
	glBindTexture( GL_TEXTURE_2D, texture_earth);
    glUniform1i( uEnableTex, 1);
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

void drawPyramid()
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( pyramidData.vao );
    glDrawArrays( GL_TRIANGLES, 0, pyramidData.numVertices );
}

void drawLeopardSlant()
{
    glBindTexture( GL_TEXTURE_2D, texture_leopard );
    glUniform1i( uEnableTex, 1 );
    
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( slantData.vao );
    glDrawArrays( GL_TRIANGLES, 0, slantData.numVertices );
    
    glUniform1i( uEnableTex, 0 );
}

void drawTurretFront()
{
    glBindTexture( GL_TEXTURE_2D, texture_leopard );
    glUniform1i( uEnableTex, 1 );
    
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( turretData.vao );
    glDrawArrays( GL_TRIANGLES, 0, turretData.numVertices );
    
    glUniform1i( uEnableTex, 0 );
}

void drawLeopardCube()
{
    glBindTexture( GL_TEXTURE_2D, texture_leopard);
    glUniform1i( uEnableTex, 1 );
    
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    
    glUniform1i( uEnableTex, 0 );
}

void drawLeopardSphere()
{
    glBindTexture( GL_TEXTURE_2D, texture_leopard);
    glUniform1i( uEnableTex, 1 );
    
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
    
    glUniform1i( uEnableTex, 0 );
}

void drawLeopardCylinder()
{
    glBindTexture( GL_TEXTURE_2D, texture_leopard);
    glUniform1i( uEnableTex, 1 );
    
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cylData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cylData.numVertices );
    
    glUniform1i( uEnableTex, 0 );
}

void drawNoTexCube()
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
}

void drawNoTexSphere()
{
    glUniformMatrix4fv( uModelView, 1, GL_FALSE, transpose(model_view) );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
}

int basis_id = 0;
void drawOneAxis()
{
	mat4 origin = model_view;
	model_view *= Translate	( 0, 0, 4 );
	model_view *= Scale(.25) * Scale( 1, 1, -1 );
	drawCone();
	model_view = origin;
	model_view *= Translate	( 1,  1, .5 );
	model_view *= Scale		( .1, .1, 1 );
	drawCube();
	model_view = origin;
	model_view *= Translate	( 1, 0, .5 );
	model_view *= Scale		( .1, .1, 1 );
	drawCube();
	model_view = origin;
	model_view *= Translate	( 0,  1, .5 );
	model_view *= Scale		( .1, .1, 1 );
	drawCube();
	model_view = origin;
	model_view *= Translate	( 0,  0, 2 );
	model_view *= Scale(.1) * Scale(   1, 1, 20);
    drawCylinder();	
	model_view = origin;
}

void drawAxes(int selected)
{
	if( basis_to_display != selected ) 
		return;
	mat4 given_basis = model_view;
	model_view *= Scale		(.25);
	drawSphere();
	model_view = given_basis;
	set_color( 0, 0, 1 );
	drawOneAxis();
	model_view *= RotateX	(-90);
	model_view *= Scale		(1, -1, 1);
	set_color( 1, 1, 1);
	drawOneAxis();
	model_view = given_basis;
	model_view *= RotateY	(90);
	model_view *= Scale		(-1, 1, 1);
	set_color( 1, 0, 0 );
	drawOneAxis();
	model_view = given_basis;
	
	colors.pop();
	colors.pop();
	colors.pop();
	set_color( colors.top().r, colors.top().g, colors.top().b );
}

void drawGround(){
	mvstack.push(model_view);
    set_color( .0, .8, .0 );
    model_view *= Translate	(0, -2, 0);									drawAxes(basis_id++);
    model_view *= Scale		(600, 1, 600);									drawAxes(basis_id++);
    drawCube();
    set_color(.0, .96, .93);
    model_view *= Scale(1/600.0f, 1, 1/600.0f);
    model_view *= Translate (300, 300, 0);
    model_view *= Scale(1, 600, 600);
    drawNoTexCube();
    model_view *= Translate (-600, 0, 0);
    drawNoTexCube();
    model_view *= Translate (300, 0, 0);
    model_view *= Scale(1, 1/600.0f, 1/600.0f);
    model_view *= Translate (0, 0, 300);
    model_view *= Scale(600, 600, 1);
    drawNoTexCube();
    model_view *= Translate (0, 0, -600);
    drawNoTexCube();
	model_view = mvstack.top(); mvstack.pop();								drawAxes(basis_id++);
}


const float w_turret = 6.0f;
const float h_turret = 1.6f;
const float l_turretBody = 6.0f;
const float l_turretFront = 2.0f;
const float cannonCircle = 0.4f;
const float l_cannon = 6.0f;
const float w_turretnub = 0.6f;
const float h_turretnub = 0.5f;
const float pos_turretnub = 0.2f;
const float angle_turretnub = 20.0f;
const float w_hatch = 1.35f;
const float h_hatch = 1.1f;
const float l_hatch = 1.35f;

const float w_chassis = 7.0f;
const float h_chassis = 2.0f;
const float l_chassis = 12.0f;

const float h_chassisFront = 1.0f;
const float l_chassisFront = 1.0f;

const float w_sideSkirt = 2.0f;
const float h_sideSkirt = 1.2f;
const float l_sideSkirt = 12.0f;

const float h_sideSkirtFront = 1.2f;
const float l_sideSkirtFront = 1.2f;

const float h_wheel = 0.9f;
const float wheelCircle = 1.4f;

const float turretTurnSpeed = 0.5f;
const float chassisTurnSpeed = 0.3f;
const float tankSpeed = 0.5f;

float tank1Angle = 0;
float tank1ChassisAngle = 0;
float tankPositionX = 0;
float tankPositionZ = 0;
float explosionSize = 5;
float explosionSizeIncrease = 1.2f;

float totalXmoved = 0;
float totalYmoved = 0;
float totalZmoved = 0;

void resetXYZ()
{
    totalXmoved = 0;
    totalYmoved = 0;
    totalZmoved = 0;
}

void cTranslate(float x, float y, float z)
{
    totalXmoved += x;
    totalYmoved += y;
    totalZmoved += z;
    
    model_view *= Translate(x, y, z);
}

void cClearTranslate()
{
    model_view *= Translate(-totalXmoved, -totalYmoved, -totalZmoved);
    resetXYZ();
}

void drawTankWheel()
{
    model_view *= RotateY(90);
    model_view *= Scale(wheelCircle, wheelCircle, h_wheel);
    drawCylinder();
    model_view *= Scale(1/wheelCircle, 1/wheelCircle, 1/h_wheel);
    model_view *= RotateY(-90);
}

void drawTankChassis(char turn, char forOrBack)
{
    resetXYZ();
    //model_view *= RotateY(tank1ChassisAngle);
    
    model_view *= Translate(0, 0, tankPositionZ);
    
    switch(turn) {
        case 'l':
            tank1ChassisAngle += chassisTurnSpeed;
            break;
        case 'r':
            tank1ChassisAngle -= chassisTurnSpeed;
        default:
            break;
    }
    
    model_view *= RotateY(tank1ChassisAngle);
    
    switch(forOrBack) {
        case 'f':
            tankPositionZ += tankSpeed;
            //model_view *= Translate(0, 0, tankPositionZ);
            break;
        case 'b':
            tankPositionZ -= tankSpeed;
            //model_view *= Translate(0, 0, tankPositionZ);
        default:
            break;
    }
    //model_view *= Translate(0, 0, tankPositionZ);
    
    cTranslate(0, 0, 0);
    
    model_view *= Scale(w_chassis, h_chassis, l_chassis);
    drawLeopardCube();
    
    model_view *= Scale(1, 1/h_chassis, 1/l_chassis);
    
    cTranslate(0, h_chassis/4.0f, l_chassis/2.0f + l_chassisFront/2.0f);
    drawLeopardSlant();
    
    cTranslate(0, -h_chassis/2.0f, 0);
    drawLeopardCube();
    
    model_view *= Scale(1/w_chassis, 1, 1);
    
    cClearTranslate();
    
    cTranslate(w_chassis/2.0f, 0.7f, 0);
    model_view *= Scale(w_sideSkirt, h_sideSkirt, l_sideSkirt);
    drawLeopardCube();
    model_view *= Scale(1/w_sideSkirt, 1/h_sideSkirt, 1/l_sideSkirt);
    cClearTranslate();
    
    cTranslate(-w_chassis/2.0f, 0.7f, 0);
    model_view *= Scale(w_sideSkirt, h_sideSkirt, l_sideSkirt);
    drawLeopardCube();
    
    model_view *= Scale(1, 1, 1/l_sideSkirt);
    cTranslate(0, 0, l_sideSkirt/2.0 + l_sideSkirtFront/2.0);
    model_view *= Scale(1, 1, l_sideSkirtFront);
    drawLeopardSlant();
    
    cTranslate(w_chassis/2.0, 0, 0);
    drawLeopardSlant();
    cTranslate(-w_chassis/2.0, 0, 0);
    model_view *= Scale(1/w_sideSkirt, 1/h_sideSkirt, 1/l_sideSkirtFront);
    cClearTranslate();
    
    set_color(0.4f, 0.4f, 0.4f);
    
    model_view *= Translate(-w_chassis/2.0, -h_chassis/2.0 + wheelCircle/2.0, -l_chassis/2.0 + wheelCircle);
    drawTankWheel();
    model_view *= Translate(w_chassis/2.0, h_chassis/2.0 - wheelCircle/2.0, l_chassis/2.0 - wheelCircle);
    
    model_view *= Translate(-w_chassis/2.0, -h_chassis/2.0 + wheelCircle/2.0, (l_chassis+l_chassisFront)/2.0 - wheelCircle/1.5);
    drawTankWheel();
    model_view *= Translate(w_chassis/2.0, h_chassis/2.0 - wheelCircle/2.0, -(l_chassis+l_chassisFront)/2.0 + wheelCircle/1.5);
    
    model_view *= Translate(w_chassis/2.0, -h_chassis/2.0 + wheelCircle/2.0, -l_chassis/2.0 + wheelCircle);
    drawTankWheel();
    model_view *= Translate(-w_chassis/2.0, h_chassis/2.0 - wheelCircle/2.0, l_chassis/2.0 - wheelCircle);
    
    model_view *= Translate(w_chassis/2.0, -h_chassis/2.0 + wheelCircle/2.0, (l_chassis+l_chassisFront)/2.0 - wheelCircle/1.5);
    drawTankWheel();
    model_view *= Translate(-w_chassis/2.0, h_chassis/2.0 - wheelCircle/2.0, -(l_chassis+l_chassisFront)/2.0 + wheelCircle/1.5);
    
    model_view *= Translate(-w_chassis/2.0, -h_chassis/2.0 + wheelCircle/6.4, l_chassisFront/2.0);
    model_view *= Scale(h_wheel*2.0, h_wheel*2.0, 10.0f);
    drawNoTexCube();
    model_view *= Scale(1/(h_wheel*2.0), 1/(h_wheel*2.0), 1/10.0f);
    model_view *= Translate(w_chassis/2.0, h_chassis/2.0 - wheelCircle/6.4, -l_chassisFront/2.0);
    
    model_view *= Translate(w_chassis/2.0, -h_chassis/2.0 + wheelCircle/6.4, l_chassisFront/2.0);
    model_view *= Scale(h_wheel*2.0, h_wheel*2.0, 10.0f);
    drawNoTexCube();
    model_view *= Scale(1/(h_wheel*2.0), 1/(h_wheel*2.0), 1/10.0f);
    model_view *= Translate(-w_chassis/2.0, h_chassis/2.0 - wheelCircle/6.4, -l_chassisFront/2.0);
    
    //model_view *= Scale(1/w_sideSkirt, 1/h_sideSkirt, 1/l_sideSkirt);
    
    model_view *= RotateY(-tank1ChassisAngle);
}

void drawTankTurret(char turretTurn, char chassisTurn, char forOrBack, bool fire)
{
    mvstack.push(model_view);
    
    set_color(1, 1, 1);
    
    drawTankChassis(chassisTurn, forOrBack);
    //model_view *= RotateY(turretTurnSpeed * TIME);
    switch(turretTurn) {
        case 'l':
            tank1Angle += turretTurnSpeed;
            break;
        case 'r':
            tank1Angle -= turretTurnSpeed;
        default:
            break;
    }
    
    set_color(1, 1, 1);
    
    model_view *= RotateY(tank1Angle);
    
    model_view *= Translate(0, 2, 0);
    model_view *= Scale (w_turret, h_turret, l_turretBody);
    drawLeopardCube();
    
    model_view *= Scale (1, 1, 1/l_turretBody);
    model_view *= Translate(0, 0, 4);
    model_view *= Scale (1, 1, l_turretFront);
    drawTurretFront();
    
    set_color(0.5f, 0.5f, 0.5f);
    
    model_view *= Scale (1/w_turret, 1/h_turret, 1/l_turretFront);
    model_view *= Translate(0, 0, 4);
    model_view *= Scale (cannonCircle, cannonCircle, l_cannon);
    drawLeopardCylinder();
    
    if (fire)
    {
        set_color(.89, .44, .10);
        model_view *= Scale(1/cannonCircle, 1/cannonCircle, 1/l_cannon);
        model_view *= Translate(0, 0, l_cannon/2.0);
        explosionSize += explosionSizeIncrease;
        model_view *= Scale(explosionSize, explosionSize, explosionSize);
        drawSphere();
        model_view *= Scale(1/explosionSize, 1/explosionSize, 1/explosionSize);
        model_view *= Translate(0, 0, -l_cannon/2.0);
        model_view *= Scale(cannonCircle, cannonCircle, l_cannon);
    }
    
    set_color(1, 1, 1);
    
    model_view *= Scale(1/cannonCircle, 1/cannonCircle, 1/l_cannon);
    model_view *= Scale(w_turretnub, h_turretnub, 1);
    model_view *= RotateZ(angle_turretnub);
    model_view *= Translate(pos_turretnub, 0, 0);
    drawLeopardCylinder();
    
    model_view *= Translate(-pos_turretnub, 0, 0);
    model_view *= RotateZ(-angle_turretnub);
    model_view *= Scale(1/w_turretnub, 1/h_turretnub, 1);
    model_view *= Translate( -1.2, 0.5, -8);
    model_view *= Scale(w_hatch, h_hatch, l_hatch);
    drawLeopardSphere();
    
    
    model_view = mvstack.top(); mvstack.pop();
    
}

vec4 unRotatedPoint( 0, 0, 15, 1); //this will be defined later

int frames = 0;
double lastFrameTime = 0;
int FPS = 0;
bool resetCamera = false;
bool resetCamera1 = false;
bool resetCamera2 = false;
void display(void)
{
    FPS = (int)1.0/(TIME - lastFrameTime);
    
    printf("%d\n", FPS);
    
    lastFrameTime = TIME;
    
	basis_id = 0;
    glClearColor( .1, .1, .2, 1 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	set_color( .6, .6, .6 );
	
	model_view = LookAt( eye, ref, up );
    
    float rotationBeginTime = 1;
    float timeToRotate = 4;
    float rotationSceneTime = TIME - rotationBeginTime;
    if (rotationSceneTime > 0 && rotationSceneTime < timeToRotate)
    {
        eye = RotateY(360/timeToRotate * rotationSceneTime) * unRotatedPoint;
        if (rotationSceneTime <= 1.5)
            drawTankTurret('r', NULL, NULL, false);
        else if (rotationSceneTime > 2.0 && rotationSceneTime <= 3.5)
            drawTankTurret('l', NULL, NULL, false);
    }
    if (0 < TIME && TIME < rotationBeginTime)
    {
        unRotatedPoint = eye;
    }
    
    if (rotationSceneTime > timeToRotate)
    {
        if (!resetCamera1)
        {
            eye = RotateY(180) * unRotatedPoint;
            unRotatedPoint = eye;
            resetCamera1 = true;
        }
    }

	model_view *= orientation;
    model_view *= Scale(zoom);												drawAxes(basis_id++);
    
    drawGround();
    float BeginToTurnRight = 5;
    float tankToRotate = 6;
    float tankToRotate2 = 6.5;
    float rotateTurretTime = TIME - BeginToTurnRight;

    if (rotateTurretTime > 0 && rotateTurretTime < tankToRotate)
    {
        if (rotateTurretTime <= tankToRotate/2.0f)
        {
            eye = Translate(30, 20, 100) * unRotatedPoint;
//            eye = RotateY(120) * unRotatedPoint;
//            eye = RotateZ(180) * unRotatedPoint;
//            eye = Translate(0, 0, 50) * unRotatedPoint;
            drawTankTurret(NULL, NULL, 'f', false);
        }
        else
        {
            if (!resetCamera)
            {
                unRotatedPoint = eye;
                eye = Translate(20, 20, 100) * unRotatedPoint;
                unRotatedPoint = eye;
                eye = RotateZ(40) * unRotatedPoint;
                unRotatedPoint = eye;
                resetCamera = true;
            }
            
            eye = vec4(15, 12, 140, 1);
            
            drawTankTurret(NULL, 'l', NULL, false);
        }
        
    }
    else if (rotateTurretTime > tankToRotate && rotateTurretTime < tankToRotate2)
    {
        drawTankTurret(NULL, NULL, NULL, true);
    }
    else if (rotateTurretTime > tankToRotate2)
    {
        drawTankTurret(NULL, NULL, NULL, true);
        animate = 0;
    }
    else
        drawTankTurret(NULL, NULL, NULL, false);

    
    //printf("%f", fmod(TIME, 1.0f));
    
    //printf("%f\n", ticktimedouble);
    
//	drawMidterm();
//    model_view *= Translate( 0, -6, 0 );									drawAxes(basis_id++);	
//
//    drawShapes();
//	drawPlanets();
   
    glutSwapBuffers();
}

void myReshape(int w, int h)	// Handles window sizing and resizing.
{    
    mat4 projection = Perspective( 50, (float)w/h, 1, 1000 );
    glUniformMatrix4fv( uProjection, 1, GL_FALSE, transpose(projection) );
	glViewport(0, 0, g_width = w, g_height = h);	
}		

void instructions() {	 std::cout <<	"Press:"									<< '\n' <<
										"  r to restore the original view."			<< '\n' <<
										"  0 to restore the original state."		<< '\n' <<
										"  a to toggle the animation."				<< '\n' <<
										"  b to show the next basis's axes."		<< '\n' <<
										"  B to show the previous basis's axes."	<< '\n' <<
										"  q to quit."								<< '\n';	}

void myKey(unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':   case 27:				// 27 = esc key
            exit(0); 
		case 'b':
			std::cout << "Basis: " << ++basis_to_display << '\n';
			break;
		case 'B':
			std::cout << "Basis: " << --basis_to_display << '\n';
			break;
//        case 'a':							// toggle animation           		
//            //if(animate) std::cout << "Elapsed time " << TIME << '\n';
//            animate = 1 - animate ; 
//            break ;
		case '0':							// Add code to reset your object here.
			TIME = 0;	TM.Reset() ;											
        case 'r':
			orientation = mat4();			
            break ;
    }
    glutPostRedisplay() ;
}

int main() 
{
	char title[] = "Title";
	int argcount = 1;	 char* title_ptr = title;
	glutInit(&argcount,		 &title_ptr);
	glutInitWindowPosition (230, 70);
	glutInitWindowSize     (g_width, g_height);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(title);
	#if !defined(__APPLE__) && !defined(EMSCRIPTEN)
		glewExperimental = GL_TRUE;
		glewInit();
	#endif
    std::cout << "GL version " << glGetString(GL_VERSION) << '\n';
    animate = 1;
	instructions();
	init();

	glutDisplayFunc(display);
    glutIdleFunc(idleCallBack) ;
    glutReshapeFunc (myReshape);
    glutKeyboardFunc( myKey );
    //glutMouseFunc(myMouseCallBack) ;
    glutMotionFunc(myMotionCallBack) ;
    glutPassiveMotionFunc(myPassiveMotionCallBack) ;

	glutMainLoop();
}