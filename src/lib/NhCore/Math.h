#ifndef NH_MATH_H
#define NH_MATH_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Content.h"
#include "Common/Result.h"
#include "Common/API.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Vector2D {
        double x, y;
    } Nh_Vector2D;

    typedef struct Nh_Vector3D {
        double x, y, z;
    } Nh_Vector3D;

    typedef struct Nh_UV {
        double u, v;
    } Nh_UV;

    typedef struct Nh_Vertex {
        double x, y, z;
        double u, v;
    } Nh_Vertex;

    typedef struct Nh_PixelVertex {
        NH_PIXEL x, y;
        double z;
        double u, v;
    } Nh_PixelVertex;

    typedef struct Nh_Triangle {
        Nh_Vertex V1, V2, V3; 
    } Nh_Triangle;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    Nh_Vector2D Nh_subtract2D(
        Nh_Vector2D V1, Nh_Vector2D V2
    );

    double Nh_norm2D(
        Nh_Vector2D V
    );

    double Nh_dotProduct2D(
        Nh_Vector2D V1, Nh_Vector2D V2
    );

    float Nh_innerAngle2D(
        Nh_Vector2D PIntersect, Nh_Vector2D P1, Nh_Vector2D P2
    );

    float Nh_radiansToDegrees(
        float radians
    );

    float Nh_degreesToRadians(
        float degrees
    ); 

    Nh_Vector2D Nh_lineLineIntersection(
        Nh_Vector2D A, Nh_Vector2D B, Nh_Vector2D C, Nh_Vector2D D
    ); 

    NH_BOOL Nh_pointInBox(
        Nh_Vector2D TopLeft, Nh_Vector2D BottomRight, Nh_Vector2D Point
    );

    Nh_Vector2D Nh_getPointOnCircle(
        Nh_Vector2D Center, float radius, float radians
    ); 

    Nh_Vector2D Nh_getInversedPointOnCircle(
        Nh_Vector2D Center, float radius, float radians
    ); 

    int Nh_verticesToArray(
        Nh_Vertex *Vertices_p, float *values_p, int count, NH_BOOL uv, int offset
    );
    
    int Nh_trianglesToArray(
        Nh_Triangle *Triangles_p, float *values_p, int count, NH_BOOL uv
    );
    
    void Nh_scrollVertices(
        Nh_Content *Content_p, Nh_Vertex *Vertices_p, int count
    );
    
    void Nh_scrollTriangles(
        Nh_Content *Content_p, Nh_Triangle *Triangles_p, int triangleCount
    );

/** @} */

#endif
