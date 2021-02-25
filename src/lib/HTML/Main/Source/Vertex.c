// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Vertex.h"
#include "../Header/Macros.h"
#include "../Header/BoxTriangulation.h"

#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/Memory.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include <string.h>

// HELPER ==========================================================================================

static void Nh_HTML_getTextureTriangles(
    Nh_Triangle Triangles_p[2], Nh_HTML_ClipBox Box)
{
NH_BEGIN()

    Triangles_p[0].V1.x = Box.Position.x;
    Triangles_p[0].V1.y = Box.Position.y;
    Triangles_p[0].V1.z = Box.depth;
    Triangles_p[0].V1.u = 0.0f;
    Triangles_p[0].V1.v = 0.0f;

    Triangles_p[0].V2.x = Box.Position.x;
    Triangles_p[0].V2.y = NH_HTML_MAX_Y(Box);
    Triangles_p[0].V2.z = Box.depth;
    Triangles_p[0].V2.u = 0.0f;
    Triangles_p[0].V2.v = 1.0f;

    Triangles_p[0].V3.x = NH_HTML_MAX_X(Box);
    Triangles_p[0].V3.y = NH_HTML_MAX_Y(Box);
    Triangles_p[0].V3.z = Box.depth;
    Triangles_p[0].V3.u = 1.0f;
    Triangles_p[0].V3.v = 1.0f;

    Triangles_p[1].V1.x = Box.Position.x;
    Triangles_p[1].V1.y = Box.Position.y;
    Triangles_p[1].V1.z = Box.depth;
    Triangles_p[1].V1.u = 0.0f;
    Triangles_p[1].V1.v = 0.0f;

    Triangles_p[1].V2.x = NH_HTML_MAX_X(Box);
    Triangles_p[1].V2.y = NH_HTML_MAX_Y(Box);
    Triangles_p[1].V2.z = Box.depth;
    Triangles_p[1].V2.u = 1.0f;
    Triangles_p[1].V2.v = 1.0f;

    Triangles_p[1].V3.x = NH_HTML_MAX_X(Box);
    Triangles_p[1].V3.y = Box.Position.y;
    Triangles_p[1].V3.z = Box.depth;
    Triangles_p[1].V3.u = 1.0f;
    Triangles_p[1].V3.v = 0.0f;

NH_SILENT_END()
}

// GET =============================================================================================

int Nh_HTML_getBackgroundVertices(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, float *vertices_p, int cornerTriangleCount)
{
NH_BEGIN()

    int triangleCount = 0;
    Nh_Triangle Triangles_p[1024] = {0.0f};

    Nh_HTML_PixelBox *Box_p = &Node_p->Box;
    for (int i = 0; Box_p != NULL; ++i)
    {
        Nh_HTML_ClipBox BorderBox  = Nh_HTML_convertToClipBox(Content_p, Nh_HTML_getBorderPixelBox(Node_p, i)); 
        Nh_HTML_ClipBox ContentBox = Nh_HTML_convertToClipBox(Content_p, Nh_HTML_getContentPixelBox(Node_p, i)); 
    
        NH_PIXEL radii_p[4] = {0.0f};
        radii_p[0] = Node_p->Properties.Border.Radii.Values_p[0].topLeft;
        radii_p[1] = Node_p->Properties.Border.Radii.Values_p[0].topRight;
        radii_p[2] = Node_p->Properties.Border.Radii.Values_p[0].bottomLeft;
        radii_p[3] = Node_p->Properties.Border.Radii.Values_p[0].bottomRight;
    
        triangleCount = Nh_HTML_triangulateBox(BorderBox, ContentBox, radii_p, cornerTriangleCount, 0, Triangles_p, true, triangleCount, BorderBox.depth);
        triangleCount = Nh_HTML_triangulateBox(BorderBox, ContentBox, radii_p, cornerTriangleCount, 1, Triangles_p, true, triangleCount, BorderBox.depth);
        triangleCount = Nh_HTML_triangulateBox(BorderBox, ContentBox, radii_p, cornerTriangleCount, 2, Triangles_p, true, triangleCount, BorderBox.depth);
        triangleCount = Nh_HTML_triangulateBox(BorderBox, ContentBox, radii_p, cornerTriangleCount, 3, Triangles_p, true, triangleCount, BorderBox.depth);

        Box_p = Box_p->Next_p;
    }

    if (vertices_p != NULL) {
        Nh_scrollTriangles(Content_p, Triangles_p, triangleCount);
        Nh_trianglesToArray(Triangles_p, vertices_p, triangleCount, NH_FALSE);
    }

NH_END(triangleCount * 9)
}

NH_RESULT Nh_HTML_getBackgroundImageVertices(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, float vertices_p[30], NH_CSS_Image *Image_p, 
    float subtractFromZ)
{
NH_BEGIN()

    Nh_HTML_ClipBox Box = Nh_HTML_convertToClipBox(Content_p, Nh_HTML_getBackgroundImageBox(Content_p, Node_p, Image_p, subtractFromZ));

    Nh_Triangle Triangles_p[2] = {0.0f};
    Nh_HTML_getTextureTriangles(Triangles_p, Box);

    Nh_scrollTriangles(Content_p, Triangles_p, 2);
    Nh_trianglesToArray(Triangles_p, vertices_p, 2, NH_TRUE);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

int Nh_HTML_getBorderVertices(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, float *vertices_p, NH_CSS_BORDER type, 
    int cornerTriangleCount)
{
NH_BEGIN()

    Nh_HTML_ClipBox PaddingBox = Nh_HTML_convertToClipBox(Content_p, Nh_HTML_getPaddingPixelBox(Node_p, 0)); 
    Nh_HTML_ClipBox BorderBox  = Nh_HTML_convertToClipBox(Content_p, Nh_HTML_getBorderPixelBox(Node_p, 0));

    Nh_Triangle Triangles_p[111] = {0.0f};

    NH_PIXEL radii_p[4] = {0.0f};
    radii_p[0] = Node_p->Properties.Border.Radii.Values_p[0].topLeft;
    radii_p[1] = Node_p->Properties.Border.Radii.Values_p[0].topRight;
    radii_p[2] = Node_p->Properties.Border.Radii.Values_p[0].bottomLeft;
    radii_p[3] = Node_p->Properties.Border.Radii.Values_p[0].bottomRight;

    int triangleCount = 0;
    float offsetZ = 0.001f;

    switch (type)
    {
        case NH_CSS_BORDER_TOP :
            triangleCount = Nh_HTML_triangulateBox(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 0, Triangles_p, false, 0, BorderBox.depth - offsetZ);
            break;
        case NH_CSS_BORDER_BOTTOM :
            triangleCount = Nh_HTML_triangulateBox(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 1, Triangles_p, false, 0, BorderBox.depth - offsetZ);
            break;
        case NH_CSS_BORDER_LEFT :
            triangleCount = Nh_HTML_triangulateBox(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 2, Triangles_p, false, 0, BorderBox.depth - offsetZ);
            break;
        case NH_CSS_BORDER_RIGHT :
            triangleCount = Nh_HTML_triangulateBox(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 3, Triangles_p, false, 0, BorderBox.depth - offsetZ);
            break;
        default : NH_DIAGNOSTIC_END(NH_ERROR_PARAMETERS)
    }

    Nh_scrollTriangles(Content_p, Triangles_p, triangleCount);
    Nh_trianglesToArray(Triangles_p, vertices_p, triangleCount, NH_FALSE);

NH_END(triangleCount * 9)
}

NH_RESULT Nh_HTML_getImageVertices(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, float vertices_p[30], float subtractFromZ)
{
NH_BEGIN()

    Nh_HTML_ClipBox ContentBox = Nh_HTML_convertToClipBox(Content_p, Nh_HTML_getContentPixelBox(Node_p, 0));
    ContentBox.depth -= subtractFromZ;

    Nh_Triangle Triangles_p[2] = {0.0f};
    Nh_HTML_getTextureTriangles(Triangles_p, ContentBox);

    Nh_scrollTriangles(Content_p, Triangles_p, 2);
    Nh_trianglesToArray(Triangles_p, vertices_p, 2, NH_TRUE);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_getTextVertices(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, float **vertices_pp, uint32_t **indices_pp)
{
NH_BEGIN()

    if (!Nh_HTML_isVisibleTextNode(Node_p)) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

    Nh_Vertex *Vertices_p = Nh_allocate(sizeof(Nh_Vertex) * 4 * (Node_p->Children.last + 1));
    uint32_t *indices_p   = Nh_allocate(sizeof(uint32_t) * 6 * (Node_p->Children.last + 1));
    NH_CHECK_NULL(Vertices_p)
    NH_CHECK_NULL(indices_p)

    for (int i = 0, offset = 0, indicesIndex = 0; i <= Node_p->Children.last; ++i) 
    {
        Nh_HTML_Node *Char_p = Node_p->Children.data_pp[i];
        if (Char_p->tag != NH_HTML_TAG_CHAR) {continue;}

        Nh_Glyph Glyph = Nh_getGlyph(Char_p->Parent_p->Properties.Text.Font_p, Char_p->text_p, Char_p->Parent_p->Properties.Text.fontSize);
        Nh_HTML_ClipBox ContentBox = Nh_HTML_convertToClipBox(Content_p, Nh_HTML_getContentPixelBox(Char_p, 0));

        float depth = ContentBox.depth - (0.000001f * (i + 1));

        Vertices_p[offset].x = ContentBox.Position.x; 
        Vertices_p[offset].y = ContentBox.Position.y;
        Vertices_p[offset].z = depth;
        Vertices_p[offset].u = Glyph.u0;
        Vertices_p[offset].v = Glyph.v0;

        Vertices_p[offset + 1].x = ContentBox.Position.x; 
        Vertices_p[offset + 1].y = ContentBox.Position.y + ContentBox.Size.height; 
        Vertices_p[offset + 1].z = depth;
        Vertices_p[offset + 1].u = Glyph.u0;
        Vertices_p[offset + 1].v = Glyph.v1;

        Vertices_p[offset + 2].x = ContentBox.Position.x + ContentBox.Size.width; 
        Vertices_p[offset + 2].y = ContentBox.Position.y + ContentBox.Size.height; 
        Vertices_p[offset + 2].z = depth;
        Vertices_p[offset + 2].u = Glyph.u1;
        Vertices_p[offset + 2].v = Glyph.v1;

        Vertices_p[offset + 3].x = ContentBox.Position.x + ContentBox.Size.width; 
        Vertices_p[offset + 3].y = ContentBox.Position.y;
        Vertices_p[offset + 3].z = depth;
        Vertices_p[offset + 3].u = Glyph.u1;
        Vertices_p[offset + 3].v = Glyph.v0;

        uint32_t letterIndices_p[6] = {0, 1, 2, 0, 2, 3};
        for (int j = 0; j < 6; ++j) {indices_p[indicesIndex++] = offset + letterIndices_p[j];}
        offset += 4;
    }

    Nh_scrollVertices(Content_p, Vertices_p, 4 * (Node_p->Children.last + 1));

    float *vertices_p = Nh_allocate(sizeof(float) * 20 * (Node_p->Children.last + 1));
    NH_CHECK_MEM(vertices_p)
    Nh_verticesToArray(Vertices_p, vertices_p, 4 * (Node_p->Children.last + 1), NH_TRUE, 0);

    if (vertices_pp != NULL) {*vertices_pp = vertices_p;}
    if (indices_pp != NULL) {*indices_pp = indices_p;}

    Nh_free(Vertices_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

