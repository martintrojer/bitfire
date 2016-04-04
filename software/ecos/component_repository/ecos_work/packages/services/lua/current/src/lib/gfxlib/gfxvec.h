// Bitfire Graphics Library, 3D Vector Graphics
// mtrojer@arrownordic.com
// $WCREV$
//! The Graphics Library 3D vector part.
//! \page GFXVEC gfxvec.h
//! \ingroup GFXLIBINC

#include "gfxlib.h"
#include "myfixed.h"

#ifndef __GFXVEC_H
#define __GFXVEC_H

#define XOFFSET 0
#define YOFFSET 0

//----- constants
#define MAXDEGREES		1024
#define MAXVERTICES		62
#define MAXPOLYGONS		72
#define MAXPOLYVERT		4

// structure of one vertex
typedef struct
{
  fixed ox,oy,oz;			// vertex original coordinates
  fixed wx,wy,wz;			// vertex working  coordinates
  fixed Nox,Noy,Noz;			// vertex normal original coordinates
  fixed Nwx,Nwy,Nwz;			// vertex normal working coordinates
  INT_16 sx,sy,sz;			// vertex screen coordinates
  INT_16 color;		                // color of vertex normal
} VertexTYPE;
		  
// structure of one polygon
typedef struct
{
  fixed Nox,Noy,Noz;			// polygon normals (only used to get vertex normals)
  INT_16 NumOfVertices;			// number of vertices that make up the polygon
  INT_16 Vertex[MAXPOLYVERT];		// vertex indices in counter clockwise order
  INT_16 zcenter;			// for sorting
} PolygonTYPE;

// object structure
typedef struct
{
  fixed Ox,Oy,Oz;			// coordinates of object's origin
  VertexTYPE Vertex[MAXVERTICES];       // all vertices in object
  PolygonTYPE Polygon[MAXPOLYGONS];     // all polygons in object
  INT_16 Ax,Ay,Az;			// object's rotation angles
  INT_16 NumOfVertices;			// number of vertices in object
  INT_16 NumOfPolygons;			// number of polygons in object
} ObjectTYPE;

//! \defgroup gfxvec gfxvec

#ifdef __cplusplus

extern "C" static void LoadObject(const INT_16 *);
extern "C" static void RotatePoints(INT_16 distance);
extern "C" static void DrawObject(gl_col *c);
extern "C" static void MakePolygonList(void);
extern "C" static void SortPolygonList(void);
extern "C" static int ComparePolygons(const void *a, const void *b);

extern "C" void InitGVectors(const INT_16 *data, BOOL_8 setuptables);
extern "C" void UpdateGVectors(INT_16 distance, gl_col *c);	

#else

static void LoadObject(const INT_16 *);
static void RotatePoints(INT_16 distance);
static void DrawObject(gl_col *c);
static void MakePolygonList(void);
static void SortPolygonList(void);
static int ComparePolygons(const void *a, const void *b);

//! Initialize a 3D object and the rendering engine.
//! @param *data pointer to vertex and polygon data.
//! @param setuptables whether of not to reset the position and angels of the object
//!\ingroup gfxvec
void InitGVectors(const INT_16 *data, BOOL_8 setuptables);

//! Render next screen of 3D object.
//! Angels are automatically incremented.
//! @param distance drawing distance (zoom level)
//! @param *c color of the lines
//!\ingroup gfxvec
void UpdateGVectors(INT_16 distance, gl_col *c);		

#endif

#endif
