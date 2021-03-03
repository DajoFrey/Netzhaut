// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Node.h"
#include "../Header/Macros.h"

#include "../../Vulkan/Header/Node.h"
#include "../../Vulkan/Header/Text.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include "../../../Netzhaut/Memory.h"
#include "../../../HTML/Main/Header/Vertex.h"
#include "../../../HTML/Main/Header/Node.h"

// CREATE ==========================================================================================

/**
 * Creates resources for text rendering based on \p Node_p.
 *
 * @param  Tab_p  The tab which contains the document holding \p Node_p.
 * @param  Node_p Expects pointer to a text-node. 
 * @return        @ref NH_SUCCESS when the text-rendering resources were created, various error codes if not.
 */
static NH_RESULT Nh_Gfx_createText(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float *vertices_p = NULL;
    uint32_t *indices_p = NULL;
    NH_CHECK(Nh_HTML_getTextVertices(Content_p, Node_p, &vertices_p, &indices_p))

    NH_CHECK_NULL(Nh_Gfx_initTexture(Content_p, &Node_p->Properties.Text.Font_p->URI, NULL))

    switch (Content_p->Tab_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createText(Content_p, Node_p, true, vertices_p, indices_p))
    }

    Nh_free(vertices_p);
    Nh_free(indices_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_Gfx_createNode(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Nh_HTML_isVisibleTextNode(Node_p)) {NH_CHECK(Nh_Gfx_createText(Content_p, Node_p))}

    switch (Content_p->Tab_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createNode(Content_p, Node_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

NH_RESULT Nh_Gfx_updateHTMLDocumentNodes(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    for (int i = 0; i <= Document_p->Tree.Render.last; ++i) 
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.Render.data_pp[i];

        for (int j = 0; j < Node_p->Properties.Candidates.count; ++j) 
        {
            switch (Document_p->Content_p->Tab_p->GPU.API)
            {
                case NH_API_VULKAN :
                {
                    NH_CHECK(Nh_Vk_updateNodeProperty(
                        Document_p->Content_p->Tab_p->GPU.Pointer, Node_p, 
                        Nh_getFromLinkedList(&Node_p->Properties.Candidates, j)
                    )) 
                    break;
                }
            }
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_Gfx_destroyNode(
    Nh_GPU *GPU_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    switch (GPU_p->API)
    {
        case NH_API_VULKAN : Nh_Vk_destroyNode(GPU_p->Pointer, Node_p);
    }

NH_SILENT_END()
}

