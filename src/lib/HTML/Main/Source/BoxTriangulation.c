// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/BoxTriangulation.h"
#include "../Header/Macros.h"

#include "../../../Netzhaut/Math.h"

#include NH_CSS_UTILS
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

// DECLARE ==========================================================================================

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3

// DEFINE ==========================================================================================

typedef struct Nh_HTML_CornerData {
    float radii_p[4];
    struct {
        int leftCount;
        int rightCount;
        int bottomCount;
        int topCount;
        bool topLeft;
        bool topRight;
        bool bottomLeft;
        bool bottomRight;
    } Rounded;
    Nh_HTML_ClipBox TopLeft;
    Nh_HTML_ClipBox TopRight;
    Nh_HTML_ClipBox BottomLeft;
    Nh_HTML_ClipBox BottomRight;
} Nh_HTML_CornerData;

// DECLARE =========================================================================================

static int Nh_HTML_getTopTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z
);
static int Nh_HTML_getBottomTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z
);
static int Nh_HTML_getLeftTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z
);
static int Nh_HTML_getRightTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z
);

static int Nh_HTML_getTopLeftCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_HTML_CornerData, int cornerTriangleCount, int triangleCount, int side, float z
);
static int Nh_HTML_getTopRightCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_HTML_CornerData Corners, int cornerTriangleCount, int triangleCount, int side, float z
);
static int Nh_HTML_getBottomLeftCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_HTML_CornerData Corners, int cornerTriangleCount, int triangleCount, int side, float z
);
static int Nh_HTML_getBottomRightCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_HTML_CornerData Corners, int cornerTriangleCount, int triangleCount, int side, float z
);

static Nh_HTML_CornerData Nh_HTML_getCorners(
    Nh_HTML_ClipBox EnclosingBox, float Radii_p[4]
);

// GET =============================================================================================

int Nh_HTML_triangulateBox(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, NH_PIXEL radii_p[4], int cornerTriangleCount, 
    int side, Nh_Triangle *Triangles_p, bool enclosedPoint, int offset, float z)
{
NH_BEGIN()

    Nh_HTML_ClipBox EnclosedPoint;

    EnclosedPoint.Position.x = EnclosedBox.Position.x + EnclosedBox.Size.width / 2.0f;
    EnclosedPoint.Size.width = 0.0f;

    EnclosedPoint.Position.y = EnclosedBox.Position.y + EnclosedBox.Size.height / 2.0f;
    EnclosedPoint.Size.height = 0.0f;

    float clipRadii_p[4] = {0.0f};

    // TODO
    clipRadii_p[0] = 0.0f;
    clipRadii_p[1] = 0.0f;
    clipRadii_p[2] = 0.0f;
    clipRadii_p[3] = 0.0f;

    int triangleCount = offset;
    Nh_HTML_CornerData Corners = Nh_HTML_getCorners(EnclosingBox, clipRadii_p);

    switch (side)
    {
        case TOP :
            triangleCount = Nh_HTML_getTopTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
        case BOTTOM :
            triangleCount = Nh_HTML_getBottomTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
        case LEFT :
            triangleCount = Nh_HTML_getLeftTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
        case RIGHT :
            triangleCount = Nh_HTML_getRightTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
    }

NH_END(triangleCount)
}

static int Nh_HTML_getTopTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].V1.x = Corners.TopLeft.Position.x + Corners.TopLeft.Size.width;
    Triangles_p[triangleCount].V1.y = Corners.TopLeft.Position.y;
    Triangles_p[triangleCount].V1.z = z;

    Triangles_p[triangleCount].V2.x = Corners.TopRight.Position.x;
    Triangles_p[triangleCount].V2.y = Corners.TopRight.Position.y;
    Triangles_p[triangleCount].V2.z = z;

    Triangles_p[triangleCount].V3.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x + EnclosedBox.Size.width : EnclosedPoint_p->Position.x + EnclosedPoint_p->Size.width;
    Triangles_p[triangleCount].V3.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y : EnclosedPoint_p->Position.y;
    Triangles_p[triangleCount].V3.z = z;

    triangleCount++;

    if (EnclosedPoint_p == NULL) 
    {
        Triangles_p[triangleCount].V1.x = Corners.TopLeft.Position.x + Corners.TopLeft.Size.width;
        Triangles_p[triangleCount].V1.y = Corners.TopLeft.Position.y;
        Triangles_p[triangleCount].V1.z = z;
    
        Triangles_p[triangleCount].V2.x = EnclosedBox.Position.x;
        Triangles_p[triangleCount].V2.y = EnclosedBox.Position.y;
        Triangles_p[triangleCount].V2.z = z;
    
        Triangles_p[triangleCount].V3.x = EnclosedBox.Position.x + EnclosedBox.Size.width;
        Triangles_p[triangleCount].V3.y = EnclosedBox.Position.y;
        Triangles_p[triangleCount].V3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.topLeft)
    {
        triangleCount = Nh_HTML_getTopLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, TOP, z
        );
    }

    if (Corners.Rounded.topRight)
    {
        triangleCount = Nh_HTML_getTopRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, TOP, z
        );
    }

NH_END(triangleCount)
}

static int Nh_HTML_getBottomTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].V1.x = Corners.BottomLeft.Position.x + Corners.BottomLeft.Size.width;
    Triangles_p[triangleCount].V1.y = Corners.BottomLeft.Position.y + Corners.BottomLeft.Size.height;
    Triangles_p[triangleCount].V1.z = z;

    Triangles_p[triangleCount].V2.x = Corners.BottomRight.Position.x;
    Triangles_p[triangleCount].V2.y = Corners.BottomRight.Position.y + Corners.BottomRight.Size.height;
    Triangles_p[triangleCount].V2.z = z;

    Triangles_p[triangleCount].V3.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x + EnclosedBox.Size.width  : EnclosedPoint_p->Position.x + EnclosedPoint_p->Size.width;
    Triangles_p[triangleCount].V3.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y + EnclosedBox.Size.height : EnclosedPoint_p->Position.y + EnclosedPoint_p->Size.height;
    Triangles_p[triangleCount].V3.z = z; 

    triangleCount++;

    if (EnclosedPoint_p == NULL)
    {
        Triangles_p[triangleCount].V1.x = Corners.BottomLeft.Position.x + Corners.BottomLeft.Size.width;
        Triangles_p[triangleCount].V1.y = Corners.BottomLeft.Position.y + Corners.BottomLeft.Size.height;
        Triangles_p[triangleCount].V1.z = z;
 
        Triangles_p[triangleCount].V2.x = EnclosedBox.Position.x;
        Triangles_p[triangleCount].V2.y = EnclosedBox.Position.y + EnclosedBox.Size.height;
        Triangles_p[triangleCount].V2.z = z;

        Triangles_p[triangleCount].V3.x = EnclosedBox.Position.x + EnclosedBox.Size.width;
        Triangles_p[triangleCount].V3.y = EnclosedBox.Position.y + EnclosedBox.Size.height;
        Triangles_p[triangleCount].V3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.bottomLeft)
    {
        triangleCount = Nh_HTML_getBottomLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, BOTTOM, z
        );
    }

    if (Corners.Rounded.bottomRight)
    {
        triangleCount = Nh_HTML_getBottomRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, BOTTOM, z
        );
    }

NH_END(triangleCount)
}

static int Nh_HTML_getLeftTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].V1.x = Corners.TopLeft.Position.x;
    Triangles_p[triangleCount].V1.y = Corners.TopLeft.Position.y + Corners.TopLeft.Size.height;
    Triangles_p[triangleCount].V1.z = z;

    Triangles_p[triangleCount].V2.x = Corners.BottomLeft.Position.x;
    Triangles_p[triangleCount].V2.y = Corners.BottomLeft.Position.y;
    Triangles_p[triangleCount].V2.z = z;

    Triangles_p[triangleCount].V3.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x : EnclosedPoint_p->Position.x;
    Triangles_p[triangleCount].V3.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y + EnclosedBox.Size.height : EnclosedPoint_p->Position.y + EnclosedPoint_p->Size.height;
    Triangles_p[triangleCount].V3.z = z;

    triangleCount++;

    if (EnclosedPoint_p == NULL)
    {                         
        Triangles_p[triangleCount].V1.x = Corners.TopLeft.Position.x;
        Triangles_p[triangleCount].V1.y = Corners.TopLeft.Position.y + Corners.TopLeft.Size.height;
        Triangles_p[triangleCount].V1.z = z;

        Triangles_p[triangleCount].V2.x = EnclosedBox.Position.x;
        Triangles_p[triangleCount].V2.y = EnclosedBox.Position.y;
        Triangles_p[triangleCount].V2.z = z;

        Triangles_p[triangleCount].V3.x = EnclosedBox.Position.x;
        Triangles_p[triangleCount].V3.y = EnclosedBox.Position.y + EnclosedBox.Size.height;
        Triangles_p[triangleCount].V3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.topLeft)
    {
        triangleCount = Nh_HTML_getTopLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, LEFT, z
        );
    }

    if (Corners.Rounded.bottomLeft)
    {
        triangleCount = Nh_HTML_getBottomLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, LEFT, z
        );
    }

NH_END(triangleCount)
}

static int Nh_HTML_getRightTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_HTML_CornerData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].V1.x = Corners.TopRight.Position.x + Corners.TopRight.Size.width;
    Triangles_p[triangleCount].V1.y = Corners.TopRight.Position.y + Corners.TopRight.Size.height;
    Triangles_p[triangleCount].V1.z = z;

    Triangles_p[triangleCount].V2.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x + EnclosedBox.Size.width  : EnclosedPoint_p->Position.x + EnclosedPoint_p->Size.width;
    Triangles_p[triangleCount].V2.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y + EnclosedBox.Size.height : EnclosedPoint_p->Position.y + EnclosedPoint_p->Size.height;
    Triangles_p[triangleCount].V2.z = z;

    Triangles_p[triangleCount].V3.x = Corners.BottomRight.Position.x + Corners.BottomRight.Size.width;
    Triangles_p[triangleCount].V3.y = Corners.BottomRight.Position.y;
    Triangles_p[triangleCount].V3.z = z;

    triangleCount++;

    if (EnclosedPoint_p == NULL)
    {                       
        Triangles_p[triangleCount].V1.x = Corners.TopRight.Position.x + Corners.TopRight.Size.width;
        Triangles_p[triangleCount].V1.y = Corners.TopRight.Position.y + Corners.TopRight.Size.height;
        Triangles_p[triangleCount].V1.z = z;

        Triangles_p[triangleCount].V2.x = EnclosedBox.Position.x + EnclosedBox.Size.width;
        Triangles_p[triangleCount].V2.y = EnclosedBox.Position.y + EnclosedBox.Size.height;
        Triangles_p[triangleCount].V2.z = z;

        Triangles_p[triangleCount].V3.x = EnclosedBox.Position.x + EnclosedBox.Size.width;
        Triangles_p[triangleCount].V3.y = EnclosedBox.Position.y;
        Triangles_p[triangleCount].V3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.topRight)
    {
        triangleCount = Nh_HTML_getTopRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, RIGHT, z
        );
    }

    if (Corners.Rounded.bottomRight)
    {
        triangleCount = Nh_HTML_getBottomRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, RIGHT, z
        );
    }

NH_END(triangleCount)
}

static int Nh_HTML_getTopLeftCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p,
    Nh_HTML_CornerData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxTopLeft = {EnclosingBox.Position.x, EnclosingBox.Position.y};
    Nh_Vector2D EnclosedBoxTopLeft  = {EnclosedBox.Position.x, EnclosedBox.Position.y};
    Nh_Vector2D EnclosedBoxTopRight = {EnclosedBox.Position.x + EnclosedBox.Size.width, EnclosedBox.Position.y};
       
    float totalRadians, divRadians, radians;

    if (side == TOP)
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxTopLeft, EnclosedBoxTopLeft, EnclosedBoxTopRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(360) - totalRadians;
    }
    else if (side == LEFT) 
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxTopLeft, EnclosedBoxTopLeft, EnclosedBoxTopRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(270);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.TopLeft.Position.x + Corners.TopLeft.Size.width, Corners.TopLeft.Position.y + Corners.TopLeft.Size.height};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_LEFT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_LEFT], radians); 

        Triangles_p[index].V1.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x : EnclosedPoint_p->Position.x;
        Triangles_p[index].V1.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y : EnclosedPoint_p->Position.y;
        Triangles_p[index].V1.z = z;

        Triangles_p[index].V2.x = OldPoint.x;
        Triangles_p[index].V2.y = OldPoint.y;
        Triangles_p[index].V2.z = z;

        Triangles_p[index].V3.x = Point.x;
        Triangles_p[index].V3.y = Point.y;
        Triangles_p[index].V3.z = z;

        triangleCount++;
        OldPoint = Point;
    }
 
NH_END(triangleCount)
}

static int Nh_HTML_getTopRightCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p,
    Nh_HTML_CornerData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxTopRight = {EnclosingBox.Position.x + EnclosingBox.Size.width, EnclosingBox.Position.y};
    Nh_Vector2D EnclosedBoxTopRight  = {EnclosedBox.Position.x + EnclosedBox.Size.width, EnclosedBox.Position.y};
    Nh_Vector2D EnclosedBoxTopLeft   = {EnclosedBox.Position.x, EnclosedBox.Position.y};

    float totalRadians, divRadians, radians;

    if (side == TOP)
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxTopRight, EnclosedBoxTopRight, EnclosedBoxTopLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = 0.0f;
    }
    else if (side == RIGHT)
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxTopRight, EnclosedBoxTopRight, EnclosedBoxTopLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_innerAngle2D(EnclosingBoxTopRight, EnclosedBoxTopRight, EnclosedBoxTopLeft);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.TopRight.Position.x, Corners.TopRight.Position.y + Corners.TopRight.Size.height};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_RIGHT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_RIGHT], radians); 

        Triangles_p[index].V1.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x + EnclosedBox.Size.width : EnclosedPoint_p->Position.x + EnclosedPoint_p->Size.width;
        Triangles_p[index].V1.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y : EnclosedPoint_p->Position.y;
        Triangles_p[index].V1.z = z;

        Triangles_p[index].V2.x = OldPoint.x;
        Triangles_p[index].V2.y = OldPoint.y;
        Triangles_p[index].V2.z = z;

        Triangles_p[index].V3.x = Point.x;
        Triangles_p[index].V3.y = Point.y;
        Triangles_p[index].V3.z = z;

        triangleCount++;
        OldPoint = Point;
    }

NH_END(triangleCount)
}

static int Nh_HTML_getBottomLeftCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_HTML_CornerData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxBottomLeft = {EnclosingBox.Position.x, EnclosingBox.Position.y + EnclosingBox.Size.height};
    Nh_Vector2D EnclosedBoxBottomLeft  = {EnclosedBox.Position.x, EnclosedBox.Position.y + EnclosedBox.Size.height};
    Nh_Vector2D EnclosedBoxBottomRight = {EnclosedBox.Position.x + EnclosedBox.Size.width, EnclosedBox.Position.y + EnclosedBox.Size.height};

    float totalRadians, divRadians, radians;

    if (side == BOTTOM) 
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxBottomLeft, EnclosedBoxBottomLeft, EnclosedBoxBottomRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(180);
    }
    else if (side == LEFT)
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxBottomLeft, EnclosedBoxBottomLeft, EnclosedBoxBottomRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(180) + Nh_innerAngle2D(EnclosingBoxBottomLeft, EnclosedBoxBottomLeft, EnclosedBoxBottomRight);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.BottomLeft.Position.x + Corners.BottomLeft.Size.width, Corners.BottomLeft.Position.y};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_LEFT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_LEFT], radians); 

        Triangles_p[index].V1.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x : EnclosedPoint_p->Position.x;
        Triangles_p[index].V1.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y + EnclosedBox.Size.height : EnclosedPoint_p->Position.y + EnclosedPoint_p->Size.height;
        Triangles_p[index].V1.z = z;

        Triangles_p[index].V2.x = OldPoint.x;
        Triangles_p[index].V2.y = OldPoint.y;
        Triangles_p[index].V2.z = z;

        Triangles_p[index].V3.x = Point.x;
        Triangles_p[index].V3.y = Point.y;
        Triangles_p[index].V3.z = z;

        triangleCount++;
        OldPoint = Point;
    } 

NH_END(triangleCount)
}

static int Nh_HTML_getBottomRightCornerTriangles(
    Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, Nh_HTML_ClipBox *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_HTML_CornerData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxBottomRight = {EnclosingBox.Position.x + EnclosingBox.Size.width, EnclosingBox.Position.y + EnclosingBox.Size.height};
    Nh_Vector2D EnclosedBoxBottomRight  = {EnclosedBox.Position.x + EnclosedBox.Size.width, EnclosedBox.Position.y + EnclosedBox.Size.height};
    Nh_Vector2D EnclosedBoxBottomLeft   = {EnclosedBox.Position.x, EnclosedBox.Position.y + EnclosedBox.Size.height};

    float totalRadians, divRadians, radians;

    if (side == BOTTOM)
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxBottomRight, EnclosedBoxBottomRight, EnclosedBoxBottomLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(180) - totalRadians;
    }
    else if (side == RIGHT)
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxBottomRight, EnclosedBoxBottomRight, EnclosedBoxBottomLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(90);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.BottomRight.Position.x, Corners.BottomRight.Position.y};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_RIGHT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_RIGHT], radians); 

        Triangles_p[index].V1.x = EnclosedPoint_p == NULL ? EnclosedBox.Position.x + EnclosedBox.Size.width  : EnclosedPoint_p->Position.x + EnclosedPoint_p->Size.width;
        Triangles_p[index].V1.y = EnclosedPoint_p == NULL ? EnclosedBox.Position.y + EnclosedBox.Size.height : EnclosedPoint_p->Position.y + EnclosedPoint_p->Size.height;
        Triangles_p[index].V1.z = z;

        Triangles_p[index].V2.x = OldPoint.x;
        Triangles_p[index].V2.y = OldPoint.y;
        Triangles_p[index].V2.z = z;

        Triangles_p[index].V3.x = Point.x;
        Triangles_p[index].V3.y = Point.y;
        Triangles_p[index].V3.z = z;

        triangleCount++;
        OldPoint = Point;
    }

NH_END(triangleCount)
}

static Nh_HTML_CornerData Nh_HTML_getCorners(
    Nh_HTML_ClipBox EnclosingBox, float radii_p[4])
{
NH_BEGIN()

    Nh_HTML_CornerData Corners = {0};

    memcpy(Corners.radii_p, radii_p, sizeof(float) * 4);

    Corners.Rounded.leftCount = 0;
    Corners.Rounded.rightCount = 0;
    Corners.Rounded.bottomCount = 0; 
    Corners.Rounded.topCount = 0;

    Corners.Rounded.topLeft     = radii_p[TOP_LEFT] > 0.000001f;
    Corners.Rounded.topRight    = radii_p[TOP_RIGHT] > 0.000001f;
    Corners.Rounded.bottomLeft  = radii_p[BOTTOM_LEFT] > 0.00001f;
    Corners.Rounded.bottomRight = radii_p[BOTTOM_RIGHT] > 0.00001f;

    if (Corners.Rounded.topLeft)     {Corners.Rounded.topCount++; Corners.Rounded.leftCount++;}
    if (Corners.Rounded.topRight)    {Corners.Rounded.topCount++; Corners.Rounded.rightCount++;}
    if (Corners.Rounded.bottomLeft)  {Corners.Rounded.bottomCount++; Corners.Rounded.leftCount++;}
    if (Corners.Rounded.bottomRight) {Corners.Rounded.bottomCount++; Corners.Rounded.rightCount++;}

    Corners.TopLeft.Position.x  = EnclosingBox.Position.x; 
    Corners.TopLeft.Position.y  = EnclosingBox.Position.y;
    Corners.TopLeft.Size.width  = radii_p[TOP_LEFT]; 
    Corners.TopLeft.Size.height = radii_p[TOP_LEFT]; 

    Corners.TopRight.Position.x  = (EnclosingBox.Position.x + EnclosingBox.Size.width) - radii_p[TOP_RIGHT]; 
    Corners.TopRight.Position.y  = EnclosingBox.Position.y; 
    Corners.TopRight.Size.width  = radii_p[TOP_RIGHT]; 
    Corners.TopRight.Size.height = radii_p[TOP_RIGHT]; 

    Corners.BottomLeft.Position.x  = EnclosingBox.Position.x;
    Corners.BottomLeft.Position.y  = (EnclosingBox.Position.y + EnclosingBox.Size.height) - radii_p[BOTTOM_LEFT]; 
    Corners.BottomLeft.Size.width  = radii_p[BOTTOM_LEFT]; 
    Corners.BottomLeft.Size.height = radii_p[BOTTOM_LEFT]; 

    Corners.BottomRight.Position.x  = (EnclosingBox.Position.x + EnclosingBox.Size.width) - radii_p[BOTTOM_RIGHT]; 
    Corners.BottomRight.Position.y  = (EnclosingBox.Position.y + EnclosingBox.Size.height) - radii_p[BOTTOM_RIGHT]; 
    Corners.BottomRight.Size.width  = radii_p[BOTTOM_RIGHT];
    Corners.BottomRight.Size.height = radii_p[BOTTOM_RIGHT]; 

NH_END(Corners)
}

