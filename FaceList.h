/* Your Name Here
 * somebody at something dot TLD
 * CS 484
 * October 2008
 *
 * $Id: FaceList.h 2404 2010-09-10 23:16:42Z mshafae $
 *
 * Indexed triangle face list data structure. Uses a 2D array of
 * floats for storage.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

#ifndef _FACELIST_H_
#define _FACELIST_H_

#ifndef MIN
#define MIN( x, y ) ((x) <= (y) ? (x) :  (y))
#endif
#ifndef MAX
#define MAX( x, y ) ((x) >= (y) ? (x) :  (y))
#endif
#ifndef ABS
#define ABS( x )    ((x) >= 0.0 ? (x) : -(x))
#endif


class FaceList{

private:
  // Model's file name
  std::string _filename;
  unsigned int _vertexCount;
  unsigned int _faceCount;

public:
  /*
   * Adjust the types of the following pointers
   * according to the name of your Vector class.
   */
  // Array of vertices (decimal number type)
  float **vertices;
  // Array of faces (indices into the array of vertices)
  int **faces;
  // Array of face normals (decimal number type)
  float **faceNormals;
  // Array of vertex normals (decimal number type)
  float **vertexNormals;

  FaceList( const char *filename, unsigned int vertexCount, unsigned int  faceCount ) : _filename( filename ), _vertexCount( vertexCount ), _faceCount( faceCount )
  {
    unsigned int i;
    if( !(vertices = (float**)calloc( _vertexCount, sizeof(float*))) ){
      std::cerr << "Didn't allocated memory for vertex array." << std::endl;
    }
    for( i = 0; i < _vertexCount; i++ ){
      if( !(vertices[i] = (float*)calloc( 4, sizeof(float) )) ){
        std::cerr << "Couldn't allocate memory for the vertices at location " << i << std::endl;
      }
    }
    
    if( !(faces = (int**)calloc( _faceCount, sizeof(int*))) ){
      std::cerr << "Didn't allocated memory for face array." << std::endl;
    }
    for( i = 0; i < _faceCount; i++ ){
      if( !(faces[i] = (int*)calloc( 3, sizeof(int) )) ){
        std::cerr << "Couldn't allocate memory for the vertices at location " << i << std::endl;
      }
    }
    
    if( !(faceNormals = (float**)calloc( _faceCount, sizeof(float*))) ){
      std::cerr << "Didn't allocated memory for vertex array." << std::endl;
    }
    for( i = 0; i < _faceCount; i++ ){
      if( !(faceNormals[i] = (float*)calloc( 3, sizeof(float) )) ){
        std::cerr << "Couldn't allocate memory for the vertices at location " << i << std::endl;
      }
    }

    if( !(vertexNormals = (float**)calloc( _vertexCount, sizeof(float*))) ){
      std::cerr << "Didn't allocated memory for vertex array." << std::endl;
    }
    for( i = 0; i < _vertexCount; i++ ){
      if( !(vertexNormals[i] = (float*)calloc( 3, sizeof(float) )) ){
        std::cerr << "Couldn't allocate memory for the vertices at location " << i << std::endl;
      }
    }
    
  };
  
  ~FaceList( ){
   	for( unsigned int i = 0; i < _vertexCount; i++ ){
   		free( vertices[i] );
      free( vertexNormals[i] );
   	}
    for( unsigned int i = 0; i < _faceCount; i++ ){
      free( faces[i] );
      free( faceNormals[i] );
    }
   	free( vertices );
    free( faces );
    free( faceNormals );
    free( vertexNormals );
  };

  unsigned int vertexCount( ){
    return _vertexCount;
  }

  unsigned int faceCount( ){
    return _faceCount;
  }
  
  std::string filename( ){
    return _filename;
  }
  
  void computeFaceNormals( )
  {
	  // do something...like calculate the surface normal for a vertex and normalize it

	  for( unsigned int x = 0; x < _faceCount; x++)
	  {

		  float vec1x = vertices[faces[x][1]][0] - vertices[faces[x][0]][0];
		  float vec1y = vertices[faces[x][1]][1] - vertices[faces[x][0]][1];
		  float vec1z = vertices[faces[x][1]][2] - vertices[faces[x][0]][2];

		  float vec2x = vertices[faces[x][2]][0] - vertices[faces[x][0]][0];
		  float vec2y = vertices[faces[x][2]][1] - vertices[faces[x][0]][1];
		  float vec2z = vertices[faces[x][2]][2] - vertices[faces[x][0]][2];

		  faceNormals[x][0] = (vec1y * vec2z) - (vec2y * vec1z);
		  faceNormals[x][1] = (vec1z * vec2x) - (vec2z * vec1x);
		  faceNormals[x][2] = (vec1x * vec2y) - (vec2x * vec1y);

		  float nMag = sqrt(	(faceNormals[x][0] * faceNormals[x][0]) + (faceNormals[x][1] * faceNormals[x][1]) 
			  + (faceNormals[x][2] * faceNormals[x][2]));

		  faceNormals[x][0] /= nMag;
		  faceNormals[x][1] /= nMag;
		  faceNormals[x][2] /= nMag;

	  }


  }

  void computeVertexNormals( )
  {
	  // do something...
	  

	  for (int x = 0; x < _vertexCount; x++)
		  for (int i = 0; i < 3; i++)
			  vertexNormals[x][i] = 0.0;

	  for (int x = 0; x < _faceCount; x++)
	  {
		  for (int i = 0; i < 3; i++)
		  {
			  vertexNormals[faces[x][i]][0] += faceNormals[x][0];
			  vertexNormals[faces[x][i]][1] += faceNormals[x][1];
			  vertexNormals[faces[x][i]][2] += faceNormals[x][2];
		  }
	  }

	  for (int x = 0; x < _vertexCount; x++)
	  {
		  float vertMag = sqrt((vertexNormals[x][0] * vertexNormals[x][0]) + (vertexNormals[x][1] * vertexNormals[x][1]) + (vertexNormals[x][2] * vertexNormals[x][2]));

		  vertexNormals[x][0] /= vertMag;
		  vertexNormals[x][1] /= vertMag;
		  vertexNormals[x][2] /= vertMag;
	  }
	  
  }
};

#endif