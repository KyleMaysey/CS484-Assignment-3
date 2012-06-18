/* Your Name Here
 * somebody at something dot TLD
 * CS 484
 * October 2008
 *
 * $Id: PlyModel.h 2517 2010-09-28 20:40:57Z mshafae $
 *
 * Reads a PLY format model file and returns an indexed triangle face list.
 *
 */

#ifndef _PLYMODEL_H_
#define _PLYMODEL_H_

#include "FaceList.h"

FaceList* readPlyModel( const char* filename );

#endif