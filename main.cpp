/*
 * Your contact info here
 *
 * $Id: main.cpp 2473 2010-09-23 22:38:49Z mshafae $
 *
 * Modified version of the Redbook movelight demo program. Added
 * functionality to load vertex and fragment shader programs.
 * As written, will support only one shader program that is composed
 * of one vertex shader and one fragment shader.
 */
/*
 * Based on the OpenGL Redbook example:
 *  movelight.c
 *  This program demonstrates when to issue lighting and
 *  transformation commands to render a model with a light
 *  which is moved by a modeling transformation (rotate or
 *  translate).  The light position is reset after the modeling
 *  transformation is called.  The eye position does not change.
 *
 *  A sphere is drawn using a grey material characteristic.
 *  A single light source illuminates the object.
 *
 *  Interaction:  pressing the left mouse button alters
 *  the modeling transformation (x rotation) by 30 degrees.
 *  The scene is then redrawn with the light in a new position.
 */
 
 /*
  * Copyright (c) 1993-1997, Silicon Graphics, Inc.
  * ALL RIGHTS RESERVED 
  * Permission to use, copy, modify, and distribute this software for 
  * any purpose and without fee is hereby granted, provided that the above
  * copyright notice appear in all copies and that both the copyright notice
  * and this permission notice appear in supporting documentation, and that 
  * the name of Silicon Graphics, Inc. not be used in advertising
  * or publicity pertaining to distribution of the software without specific,
  * written prior permission. 
  *
  * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
  * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
  * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
  * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
  * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
  * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
  * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
  * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
  * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
  * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
  * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
  * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
  * 
  * US Government Users Restricted Rights 
  * Use, duplication, or disclosure by the Government is subject to
  * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
  * (c)(1)(ii) of the Rights in Technical Data and Computer Software
  * clause at DFARS 252.227-7013 and/or in similar or successor
  * clauses in the FAR or the DOD or NASA FAR Supplement.
  * Unpublished-- rights reserved under the copyright laws of the
  * United States.  Contractor/manufacturer is Silicon Graphics,
  * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
  *
  * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
  */
 

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>

/* Thanks to Apple's Sample Code... */
#include "SurfaceGeometry.h"

#include "SurfaceGeometryFaceList.h"

#include "getopt.h"

#include "GLSLShader.h"

//Adding from previous assignment
#include "PlyModel.h"
#include "FaceList.h"

#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <gl/GL.h>
#include <GL/glew.h>
#include <gl/glut.h>
#endif

const unsigned int gNumModels = 5;
SurfaceGeometryFaceList *gModel[5];
unsigned int gCurrentModel;
bool gIsVerbose;
GLSLProgram *gProgram;
double lSpin = 0;
int gMaterialMode;
bool gShaderEnabled;
bool mLight;
int gMouseButtonState;
int gMouseButton;
int gMouseModifiers;
float lTranslate[3];
float mTranslate[3];

using namespace std;

void shaderInit( const char *vsFile, const char *fsFile ){

  VertexShader vertexShader( vsFile );
  FragmentShader fragmentShader( fsFile );

  gProgram = new GLSLProgram( );
  
  if( !(gProgram->attach( vertexShader )) ){
    fprintf( stderr, "Couldn't attach the vertex shader to the program\n" );
  }
  if( !(gProgram->attach( fragmentShader )) ){
    fprintf( stderr, "Couldn't attach the fragment shader to the program\n" );
  }
  
  if( !(gProgram->link( )) ){
    fprintf( stderr, "Couldn't link the shader.\n" );
  }

  if( !(gProgram->activate( )) ){
    fprintf( stderr, "Unable to activate the shader.\n" );
  }

  gProgram->isHardwareAccelerated( );

}

/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init( void ){
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   mLight = true;
   gShaderEnabled = true;
   gMaterialMode = 2;
   gCurrentModel = 0;
   // start from 1, skip the cube
   for( unsigned int i = 1; i <= gNumModels; i++ ){
     //gModel[i-1] = BuildGeometry( i, 16, 32, 12 );
     gModel[i-1] = BuildGeometry( i, 8, 16, 6 );
	 //gModel[i] = BuildGeometry( i + 1, 8, 16, 6 );
   }
   material(gMaterialMode);

   lTranslate[0] = 0.0;
   lTranslate[1] = 0.0;
   lTranslate[2] = 0.0;

   mTranslate[0] = 0.0;
   mTranslate[1] = 0.0;
   mTranslate[2] = 0.0;
}


void drawSurface( SurfaceGeometryFaceList *f ){
  int i = 0;
  int j = 0;
  int index;
  for( i = 0; i < (f->maxI); i++ ){
    glBegin( GL_TRIANGLE_STRIP );
    for( j = 0; j <= (f->maxJ); j++ ){
      index = (i % (f->maxI)) * (f->maxJ) + (j % (f->maxJ));
      glColor3fv( f->colors[index] );
      glNormal3fv( f->v_normals[index] );
      glVertex3fv( f->vertices[index] );
      
      index = ((i + 1) % f->maxI) * f->maxJ + (j % f->maxJ);
      glColor3fv( f->colors[index] );
      glNormal3fv( f->v_normals[index] );
      glVertex3fv( f->vertices[index] );
    }
    glEnd( );
  }
}

/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void display(void){
   GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   gluLookAt (0.0, 0.0, -15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPushMatrix ();
   glRotated( lSpin, 1.0, 0.0, 0.0 );
   glTranslated(-lTranslate[0], lTranslate[1], lTranslate[2]);
   glLightfv (GL_LIGHT0, GL_POSITION, position);

   glTranslated (0.0, 0.0, 1.5);
   glDisable (GL_LIGHTING);
   glColor3f (0.0, 1.0, 1.0);
   glutWireCube(0.1);
   glEnable (GL_LIGHTING);
   glPopMatrix ();

   glPushMatrix();
   glTranslated(-mTranslate[0], mTranslate[1], mTranslate[2]);
   
   //glColor3f(0.0, 1.0, 0.0);
   glutSolidSphere(2.0, 32, 60 );
   //glutSolidTorus (0.5, 1.0, 32, 60);
   //glutSolidTorus (0.5, 1.0, 2, 6);
   //drawSurface( gModel[gCurrentModel] );
   glPopMatrix();

   glPopMatrix ();
   glFlush ();
   glutSwapBuffers();

   GLenum ErrorCode;
   const GLubyte* ErrorString;

   while((ErrorCode = glGetError()) != GL_NO_ERROR)
   {
	   ErrorString = gluErrorString(ErrorCode);
	   fprintf(stderr, "OpenGL Error: %s\n", ErrorString);
   }

}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	gMouseButtonState = state;
	gMouseButton = button;
	gMouseModifiers = glutGetModifiers( );
	glutPostRedisplay( );
}

void mouseMotion( int x, int y ){
  static int sLastX = x, sLastY = y;
  // scale it down a bit
  float tx = (x - sLastX) / 32.f;
  float ty = (y - sLastY) / 32.f;
  sLastX = x;
  sLastY = y;
  
  if( gMouseButtonState == GLUT_DOWN ){
    if( gMouseModifiers == GLUT_ACTIVE_SHIFT ){
      // translate in XZ plane
		if (mLight)
		{
			lTranslate[0] += tx;
			lTranslate[2] += ty;
		}
		else
		{
			mTranslate[0] += tx;
			mTranslate[2] += ty;
		}
    }else if( gMouseModifiers == GLUT_ACTIVE_CTRL ){
      // dolly the camera
      //gCamera.position[2] += ty;
		if (mLight)
			lTranslate[2] += ty;
		else
			mTranslate[2] += ty;
    }else{
      // No modifier key pressed in the mouse function
      // translate in XY plane
		if(mLight)
		{
			lTranslate[0] += tx;
			lTranslate[1] -= ty;
		}
		else
		{
			mTranslate[0] += tx;
			mTranslate[1] -= ty;
		}
    }
  }
  glutPostRedisplay( );
}

void keyboard(unsigned char key, int x, int y){
   switch (key) {
		case 32:
			mLight = !mLight;
		break;
		case 27:
         exit(0);
         break;
       case 'c':
       case 'C':
         gCurrentModel += 1;
         gCurrentModel = gCurrentModel % gNumModels;
       break;
       case 'G':
       case 'g':
       gShaderEnabled = !gShaderEnabled;
        if( gShaderEnabled ){
          fprintf( stderr, "Shader enabled\n" );
          gProgram->activate( );
        }else{
          fprintf( stderr, "Shader disabled\n" );
          gProgram->deactivate( );
        }
       break;
       case 'M':
       case 'm':
         ++gMaterialMode;
         gMaterialMode = gMaterialMode % 3;
         material(gMaterialMode);
       break;

   }
   glutPostRedisplay();
}

void special( int key, int px, int py )
{
	// If you need to save what key was last pressed
	// uncomment the line below
	//static int sLastKey = key;
	switch( key )
	{
		case GLUT_KEY_UP:
		//rotate around X
			lSpin += 30.0;
			lSpin = fmod(lSpin, 360.0);
		break;
		case GLUT_KEY_DOWN:
		//rotate around X
			lSpin -= 30.0;
			lSpin = fmod(lSpin, 360.0);
      break;
    }
	glutPostRedisplay( );
}

void usage( string msg = "" ){
  cerr << msg.c_str( );
  cerr << "usage: shader [-hv] -x vertexshadersrc.vs -f fragmentshadersrc.fs" << endl;
}

int main(int argc, char** argv){
  int ch;
  char* in = NULL;
  string vertexShaderSrc, fragmentShaderSrc;
  
  static struct option longopts[] = {
    { "fragmentshader", required_argument, NULL, 'f' },
    { "vertexshader", required_argument, NULL, 'x' },
    { "combo", required_argument, NULL, 'c' },
    { "verbose", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 }
  };
  
  gIsVerbose = false;
  
  if( argc < 3 ){
    usage( string( "You must specify the correct number of parameters.\n" ) );
    exit(1);
  }
  
   glutInit(&argc, argv);
   while( (ch = getopt_long(argc, argv, "i:c:x:f:vh", longopts, NULL)) != -1 ){
     switch( ch ){
		case 'i':
        /* input matrix file */
        in = strdup( optarg );
		break;
       case 'c':
         /* basename for vertex and fragment shader file */
         vertexShaderSrc = string(optarg) + string(".vs");
         fragmentShaderSrc = string(optarg) + string(".fs");
         break;
       case 'x':
         /* input vertex shader file */
        vertexShaderSrc = string( optarg );
       break;
       case 'f':
         /* input fragment shader file */
        fragmentShaderSrc = string( optarg );
       break;
       case 'v':
         /* turn on verbose logging */
         gIsVerbose = true;
       break;
       case 'h':
         usage( );
         return(0);
       break;
       default:
         /* do nothing */
         fprintf( stderr, "Ignoring unknown option: %c\n", ch );
       break;
     } 
   }
   
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 800); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("GLSL Shader");
#ifdef __APPLE__
   if( supportsOpenGLVersion( 2, 0 ) ){
     fprintf( stderr, "Congrats! OpenGL Shading Language is supported.\n" );
   }else{
     fprintf( stderr, "OpenGL Shading Language not supported. Sorry.\n" );
     exit(1);
   }
   if( gProgram->isHardwareAccelerated( ) ){
     fprintf( stderr, "Oh and it's hardware accelerated!\n" );
   }
#else
   GLenum err = glewInit();
   if( GLEW_OK != err ){
     /* Problem: glewInit failed, something is seriously wrong. */
     fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
     exit(1);
   }
   if( GLEW_VERSION_2_0 ){
     fprintf( stderr, "Congrats! OpenGL Shading Language is supported.\n" );
   }else{
     fprintf( stderr, "OpenGL Shading Language not supported. Sorry.\n" );
     exit(1);
   }
#endif
   init( );

   shaderInit( vertexShaderSrc.c_str( ), fragmentShaderSrc.c_str( ) );

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc( mouseMotion );
   glutSpecialFunc( special );
   glutKeyboardFunc(keyboard);
   glutMainLoop( );
   return( 0 );
}
