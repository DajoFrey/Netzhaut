// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 *
 * This file was generated by NhWebIDLGenerator
 */

// INCLUDE =========================================================================================

#include "FragmentData.h"

#include "../DOM/TreeWalker.idl.h"
#include "../DOM/Text.idl.h"
#include "../DOM/StaticRange.idl.h"
#include "../DOM/Slottable.idl.h"
#include "../DOM/ShadowRoot.idl.h"
#include "../DOM/Range.idl.h"
#include "../DOM/ProcessingInstruction.idl.h"
#include "../DOM/ParentNode.idl.h"
#include "../DOM/NonElementParentNode.idl.h"
#include "../DOM/NonDocumentTypeChildNode.idl.h"
#include "../DOM/NodeList.idl.h"
#include "../DOM/NodeIterator.idl.h"
#include "../DOM/NodeFilter.idl.h"
#include "../DOM/Node.idl.h"
#include "../DOM/NamedNodeMap.idl.h"
#include "../DOM/MutationRecord.idl.h"
#include "../DOM/MutationObserver.idl.h"
#include "../DOM/HTMLCollection.idl.h"
#include "../DOM/EventTarget.idl.h"
#include "../DOM/Event.idl.h"
#include "../DOM/Element.idl.h"
#include "../DOM/DocumentType.idl.h"
#include "../DOM/DocumentOrShadowRoot.idl.h"
#include "../DOM/DocumentFragment.idl.h"
#include "../DOM/Document.idl.h"
#include "../DOM/DOMTokenList.idl.h"
#include "../DOM/DOMImplementation.idl.h"
#include "../DOM/Comment.idl.h"
#include "../DOM/ChildNode.idl.h"
#include "../DOM/CharacterData.idl.h"
#include "../DOM/CDataSection.idl.h"
#include "../DOM/Attr.idl.h"
#include "../DOM/AbstractRange.idl.h"
#include "../DOM/AbortSignal.idl.h"
#include "../DOM/AbortController.idl.h"

// FRAGMENT NAMES ==================================================================================

const char *NH_WEBIDL_FRAGMENT_NAMES_PP[] = {
    "DOM_TreeWalker",
    "DOM_Text",
    "DOM_StaticRange",
    "DOM_Slottable",
    "DOM_ShadowRoot",
    "DOM_Range",
    "DOM_ProcessingInstruction",
    "DOM_ParentNode",
    "DOM_NonElementParentNode",
    "DOM_NonDocumentTypeChildNode",
    "DOM_NodeList",
    "DOM_NodeIterator",
    "DOM_NodeFilter",
    "DOM_Node",
    "DOM_NamedNodeMap",
    "DOM_MutationRecord",
    "DOM_MutationObserver",
    "DOM_HTMLCollection",
    "DOM_EventTarget",
    "DOM_Event",
    "DOM_Element",
    "DOM_DocumentType",
    "DOM_DocumentOrShadowRoot",
    "DOM_DocumentFragment",
    "DOM_Document",
    "DOM_DOMTokenList",
    "DOM_DOMImplementation",
    "DOM_Comment",
    "DOM_ChildNode",
    "DOM_CharacterData",
    "DOM_CDataSection",
    "DOM_Attr",
    "DOM_AbstractRange",
    "DOM_AbortSignal",
    "DOM_AbortController",
};

size_t NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP) / sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP[0]);

// FRAGMENTS =======================================================================================

unsigned char *NH_WEBIDL_FRAGMENTS_PP[] = {
    Nh_WebIDL_DOM_TreeWalker_p,
    Nh_WebIDL_DOM_Text_p,
    Nh_WebIDL_DOM_StaticRange_p,
    Nh_WebIDL_DOM_Slottable_p,
    Nh_WebIDL_DOM_ShadowRoot_p,
    Nh_WebIDL_DOM_Range_p,
    Nh_WebIDL_DOM_ProcessingInstruction_p,
    Nh_WebIDL_DOM_ParentNode_p,
    Nh_WebIDL_DOM_NonElementParentNode_p,
    Nh_WebIDL_DOM_NonDocumentTypeChildNode_p,
    Nh_WebIDL_DOM_NodeList_p,
    Nh_WebIDL_DOM_NodeIterator_p,
    Nh_WebIDL_DOM_NodeFilter_p,
    Nh_WebIDL_DOM_Node_p,
    Nh_WebIDL_DOM_NamedNodeMap_p,
    Nh_WebIDL_DOM_MutationRecord_p,
    Nh_WebIDL_DOM_MutationObserver_p,
    Nh_WebIDL_DOM_HTMLCollection_p,
    Nh_WebIDL_DOM_EventTarget_p,
    Nh_WebIDL_DOM_Event_p,
    Nh_WebIDL_DOM_Element_p,
    Nh_WebIDL_DOM_DocumentType_p,
    Nh_WebIDL_DOM_DocumentOrShadowRoot_p,
    Nh_WebIDL_DOM_DocumentFragment_p,
    Nh_WebIDL_DOM_Document_p,
    Nh_WebIDL_DOM_DOMTokenList_p,
    Nh_WebIDL_DOM_DOMImplementation_p,
    Nh_WebIDL_DOM_Comment_p,
    Nh_WebIDL_DOM_ChildNode_p,
    Nh_WebIDL_DOM_CharacterData_p,
    Nh_WebIDL_DOM_CDataSection_p,
    Nh_WebIDL_DOM_Attr_p,
    Nh_WebIDL_DOM_AbstractRange_p,
    Nh_WebIDL_DOM_AbortSignal_p,
    Nh_WebIDL_DOM_AbortController_p,
};

size_t NH_WEBIDL_FRAGMENTS_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENTS_PP) / sizeof(NH_WEBIDL_FRAGMENTS_PP[0]);

// FRAGMENT FUNCTION NAMES =========================================================================

const char *NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP[] = {
//    "DOM_TreeWalker_parentNode",
//    "DOM_TreeWalker_firstChild",
//    "DOM_TreeWalker_lastChild",
//    "DOM_TreeWalker_previousSibling",
//    "DOM_TreeWalker_nextSibling",
//    "DOM_TreeWalker_previousNode",
//    "DOM_TreeWalker_nextNode",
//    "DOM_Text_constructor",
//    "DOM_Text_splitText",
//    "DOM_StaticRange_constructor",
//    "DOM_Range_constructor",
//    "DOM_Range_setStart",
//    "DOM_Range_setEnd",
//    "DOM_Range_setStartBefore",
//    "DOM_Range_setStartAfter",
//    "DOM_Range_setEndBefore",
//    "DOM_Range_setEndAfter",
//    "DOM_Range_collapse",
//    "DOM_Range_selectNode",
//    "DOM_Range_selectNodeContents",
//    "DOM_Range_compareBoundaryPoints",
//    "DOM_Range_deleteContents",
//    "DOM_Range_extractContents",
//    "DOM_Range_cloneContents",
//    "DOM_Range_insertNode",
//    "DOM_Range_surroundContents",
//    "DOM_Range_cloneRange",
//    "DOM_Range_detach",
//    "DOM_Range_isPointInRange",
//    "DOM_Range_comparePoint",
//    "DOM_Range_intersectsNode",
//    "DOM_Range_onslotchange",
//    "DOM_NodeList_item",
//    "DOM_NodeIterator_nextNode",
//    "DOM_NodeIterator_previousNode",
//    "DOM_NodeIterator_detach",
//    "DOM_Node_getRootNode",
//    "DOM_Node_hasChildNodes",
//    "DOM_Node_normalize",
//    "DOM_Node_cloneNode",
//    "DOM_Node_isEqualNode",
//    "DOM_Node_isSameNode",
//    "DOM_Node_compareDocumentPosition",
//    "DOM_Node_contains",
//    "DOM_Node_lookupPrefix",
//    "DOM_Node_lookupNamespaceURI",
//    "DOM_Node_isDefaultNamespace",
//    "DOM_Node_insertBefore",
//    "DOM_Node_appendChild",
//    "DOM_Node_replaceChild",
//    "DOM_Node_removeChild",
//    "DOM_NamedNodeMap_item",
//    "DOM_NamedNodeMap_getNamedItem",
//    "DOM_NamedNodeMap_getNamedItemNS",
//    "DOM_NamedNodeMap_setNamedItem",
//    "DOM_NamedNodeMap_setNamedItemNS",
//    "DOM_NamedNodeMap_removeNamedItem",
//    "DOM_NamedNodeMap_removeNamedItemNS",
//    "DOM_MutationObserver_constructor",
//    "DOM_MutationObserver_observe",
//    "DOM_MutationObserver_disconnect",
//    "DOM_MutationObserver_takeRecords",
//    "DOM_HTMLCollection_item",
//    "DOM_HTMLCollection_namedItem",
//    "DOM_EventTarget_constructor",
//    "DOM_EventTarget_addEventListener",
//    "DOM_EventTarget_removeEventListener",
//    "DOM_EventTarget_dispatchEvent",
//    "DOM_Event_constructor",
//    "DOM_Event_composedPath",
//    "DOM_Event_stopPropagation",
//    "DOM_Event_stopImmediatePropagation",
//    "DOM_Event_preventDefault",
//    "DOM_Event_initEvent",
//    "DOM_Element_hasAttributes",
//    "DOM_Element_getAttributeNames",
//    "DOM_Element_getAttribute",
//    "DOM_Element_getAttributeNS",
//    "DOM_Element_setAttribute",
//    "DOM_Element_setAttributeNS",
//    "DOM_Element_removeAttribute",
//    "DOM_Element_removeAttributeNS",
//    "DOM_Element_toggleAttribute",
//    "DOM_Element_hasAttribute",
//    "DOM_Element_hasAttributeNS",
//    "DOM_Element_getAttributeNode",
//    "DOM_Element_getAttributeNodeNS",
//    "DOM_Element_setAttributeNode",
//    "DOM_Element_setAttributeNodeNS",
//    "DOM_Element_removeAttributeNode",
//    "DOM_Element_attachShadow",
//    "DOM_Element_closest",
//    "DOM_Element_matches",
//    "DOM_Element_webkitMatchesSelector",
//    "DOM_Element_getElementsByTagName",
//    "DOM_Element_getElementsByTagNameNS",
//    "DOM_Element_getElementsByClassName",
//    "DOM_Element_insertAdjacentElement",
//    "DOM_Element_insertAdjacentText",
//    "DOM_DocumentFragment_constructor",
//    "DOM_Document_constructor",
//    "DOM_Document_getElementsByTagName",
//    "DOM_Document_getElementsByTagNameNS",
//    "DOM_Document_getElementsByClassName",
//    "DOM_Document_createElement",
//    "DOM_Document_createElementNS",
//    "DOM_Document_createDocumentFragment",
//    "DOM_Document_createTextNode",
//    "DOM_Document_createCDATASection",
//    "DOM_Document_createComment",
//    "DOM_Document_createProcessingInstruction",
//    "DOM_Document_importNode",
//    "DOM_Document_adoptNode",
//    "DOM_Document_createAttribute",
//    "DOM_Document_createAttributeNS",
//    "DOM_Document_createEvent",
//    "DOM_Document_createRange",
//    "DOM_Document_createNodeIterator",
//    "DOM_Document_createTreeWalker",
//    "DOM_DOMTokenList_item",
//    "DOM_DOMTokenList_contains",
//    "DOM_DOMTokenList_add",
//    "DOM_DOMTokenList_remove",
//    "DOM_DOMTokenList_toggle",
//    "DOM_DOMTokenList_replace",
//    "DOM_DOMTokenList_supports",
//    "DOM_DOMImplementation_createDocumentType",
//    "DOM_DOMImplementation_createDocument",
//    "DOM_DOMImplementation_createHTMLDocument",
//    "DOM_DOMImplementation_hasFeature",
//    "DOM_Comment_constructor",
//    "DOM_CharacterData_substringData",
//    "DOM_CharacterData_appendData",
//    "DOM_CharacterData_insertData",
//    "DOM_CharacterData_deleteData",
//    "DOM_CharacterData_replaceData",
//    "DOM_AbortController_constructor",
//    "DOM_AbortController_abort",
};

size_t NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP) / sizeof(NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP[0]);

// FRAGMENT FUNCTIONS ==============================================================================

Nh_WebIDL_Result (*NH_WEBIDL_FRAGMENT_FUNCTIONS_PP[]) (Nh_ECMAScript_Object *This_p, Nh_List Arguments) = {
//    Nh_WebIDL_DOM_TreeWalker_parentNode,
//    Nh_WebIDL_DOM_TreeWalker_firstChild,
//    Nh_WebIDL_DOM_TreeWalker_lastChild,
//    Nh_WebIDL_DOM_TreeWalker_previousSibling,
//    Nh_WebIDL_DOM_TreeWalker_nextSibling,
//    Nh_WebIDL_DOM_TreeWalker_previousNode,
//    Nh_WebIDL_DOM_TreeWalker_nextNode,
//    Nh_WebIDL_DOM_Text_constructor,
//    Nh_WebIDL_DOM_Text_splitText,
//    Nh_WebIDL_DOM_StaticRange_constructor,
//    Nh_WebIDL_DOM_Range_constructor,
//    Nh_WebIDL_DOM_Range_setStart,
//    Nh_WebIDL_DOM_Range_setEnd,
//    Nh_WebIDL_DOM_Range_setStartBefore,
//    Nh_WebIDL_DOM_Range_setStartAfter,
//    Nh_WebIDL_DOM_Range_setEndBefore,
//    Nh_WebIDL_DOM_Range_setEndAfter,
//    Nh_WebIDL_DOM_Range_collapse,
//    Nh_WebIDL_DOM_Range_selectNode,
//    Nh_WebIDL_DOM_Range_selectNodeContents,
//    Nh_WebIDL_DOM_Range_compareBoundaryPoints,
//    Nh_WebIDL_DOM_Range_deleteContents,
//    Nh_WebIDL_DOM_Range_extractContents,
//    Nh_WebIDL_DOM_Range_cloneContents,
//    Nh_WebIDL_DOM_Range_insertNode,
//    Nh_WebIDL_DOM_Range_surroundContents,
//    Nh_WebIDL_DOM_Range_cloneRange,
//    Nh_WebIDL_DOM_Range_detach,
//    Nh_WebIDL_DOM_Range_isPointInRange,
//    Nh_WebIDL_DOM_Range_comparePoint,
//    Nh_WebIDL_DOM_Range_intersectsNode,
//    Nh_WebIDL_DOM_Range_onslotchange,
//    Nh_WebIDL_DOM_NodeList_item,
//    Nh_WebIDL_DOM_NodeIterator_nextNode,
//    Nh_WebIDL_DOM_NodeIterator_previousNode,
//    Nh_WebIDL_DOM_NodeIterator_detach,
//    Nh_WebIDL_DOM_Node_getRootNode,
//    Nh_WebIDL_DOM_Node_hasChildNodes,
//    Nh_WebIDL_DOM_Node_normalize,
//    Nh_WebIDL_DOM_Node_cloneNode,
//    Nh_WebIDL_DOM_Node_isEqualNode,
//    Nh_WebIDL_DOM_Node_isSameNode,
//    Nh_WebIDL_DOM_Node_compareDocumentPosition,
//    Nh_WebIDL_DOM_Node_contains,
//    Nh_WebIDL_DOM_Node_lookupPrefix,
//    Nh_WebIDL_DOM_Node_lookupNamespaceURI,
//    Nh_WebIDL_DOM_Node_isDefaultNamespace,
//    Nh_WebIDL_DOM_Node_insertBefore,
//    Nh_WebIDL_DOM_Node_appendChild,
//    Nh_WebIDL_DOM_Node_replaceChild,
//    Nh_WebIDL_DOM_Node_removeChild,
//    Nh_WebIDL_DOM_NamedNodeMap_item,
//    Nh_WebIDL_DOM_NamedNodeMap_getNamedItem,
//    Nh_WebIDL_DOM_NamedNodeMap_getNamedItemNS,
//    Nh_WebIDL_DOM_NamedNodeMap_setNamedItem,
//    Nh_WebIDL_DOM_NamedNodeMap_setNamedItemNS,
//    Nh_WebIDL_DOM_NamedNodeMap_removeNamedItem,
//    Nh_WebIDL_DOM_NamedNodeMap_removeNamedItemNS,
//    Nh_WebIDL_DOM_MutationObserver_constructor,
//    Nh_WebIDL_DOM_MutationObserver_observe,
//    Nh_WebIDL_DOM_MutationObserver_disconnect,
//    Nh_WebIDL_DOM_MutationObserver_takeRecords,
//    Nh_WebIDL_DOM_HTMLCollection_item,
//    Nh_WebIDL_DOM_HTMLCollection_namedItem,
//    Nh_WebIDL_DOM_EventTarget_constructor,
//    Nh_WebIDL_DOM_EventTarget_addEventListener,
//    Nh_WebIDL_DOM_EventTarget_removeEventListener,
//    Nh_WebIDL_DOM_EventTarget_dispatchEvent,
//    Nh_WebIDL_DOM_Event_constructor,
//    Nh_WebIDL_DOM_Event_composedPath,
//    Nh_WebIDL_DOM_Event_stopPropagation,
//    Nh_WebIDL_DOM_Event_stopImmediatePropagation,
//    Nh_WebIDL_DOM_Event_preventDefault,
//    Nh_WebIDL_DOM_Event_initEvent,
//    Nh_WebIDL_DOM_Element_hasAttributes,
//    Nh_WebIDL_DOM_Element_getAttributeNames,
//    Nh_WebIDL_DOM_Element_getAttribute,
//    Nh_WebIDL_DOM_Element_getAttributeNS,
//    Nh_WebIDL_DOM_Element_setAttribute,
//    Nh_WebIDL_DOM_Element_setAttributeNS,
//    Nh_WebIDL_DOM_Element_removeAttribute,
//    Nh_WebIDL_DOM_Element_removeAttributeNS,
//    Nh_WebIDL_DOM_Element_toggleAttribute,
//    Nh_WebIDL_DOM_Element_hasAttribute,
//    Nh_WebIDL_DOM_Element_hasAttributeNS,
//    Nh_WebIDL_DOM_Element_getAttributeNode,
//    Nh_WebIDL_DOM_Element_getAttributeNodeNS,
//    Nh_WebIDL_DOM_Element_setAttributeNode,
//    Nh_WebIDL_DOM_Element_setAttributeNodeNS,
//    Nh_WebIDL_DOM_Element_removeAttributeNode,
//    Nh_WebIDL_DOM_Element_attachShadow,
//    Nh_WebIDL_DOM_Element_closest,
//    Nh_WebIDL_DOM_Element_matches,
//    Nh_WebIDL_DOM_Element_webkitMatchesSelector,
//    Nh_WebIDL_DOM_Element_getElementsByTagName,
//    Nh_WebIDL_DOM_Element_getElementsByTagNameNS,
//    Nh_WebIDL_DOM_Element_getElementsByClassName,
//    Nh_WebIDL_DOM_Element_insertAdjacentElement,
//    Nh_WebIDL_DOM_Element_insertAdjacentText,
//    Nh_WebIDL_DOM_DocumentFragment_constructor,
//    Nh_WebIDL_DOM_Document_constructor,
//    Nh_WebIDL_DOM_Document_getElementsByTagName,
//    Nh_WebIDL_DOM_Document_getElementsByTagNameNS,
//    Nh_WebIDL_DOM_Document_getElementsByClassName,
//    Nh_WebIDL_DOM_Document_createElement,
//    Nh_WebIDL_DOM_Document_createElementNS,
//    Nh_WebIDL_DOM_Document_createDocumentFragment,
//    Nh_WebIDL_DOM_Document_createTextNode,
//    Nh_WebIDL_DOM_Document_createCDATASection,
//    Nh_WebIDL_DOM_Document_createComment,
//    Nh_WebIDL_DOM_Document_createProcessingInstruction,
//    Nh_WebIDL_DOM_Document_importNode,
//    Nh_WebIDL_DOM_Document_adoptNode,
//    Nh_WebIDL_DOM_Document_createAttribute,
//    Nh_WebIDL_DOM_Document_createAttributeNS,
//    Nh_WebIDL_DOM_Document_createEvent,
//    Nh_WebIDL_DOM_Document_createRange,
//    Nh_WebIDL_DOM_Document_createNodeIterator,
//    Nh_WebIDL_DOM_Document_createTreeWalker,
//    Nh_WebIDL_DOM_DOMTokenList_item,
//    Nh_WebIDL_DOM_DOMTokenList_contains,
//    Nh_WebIDL_DOM_DOMTokenList_add,
//    Nh_WebIDL_DOM_DOMTokenList_remove,
//    Nh_WebIDL_DOM_DOMTokenList_toggle,
//    Nh_WebIDL_DOM_DOMTokenList_replace,
//    Nh_WebIDL_DOM_DOMTokenList_supports,
//    Nh_WebIDL_DOM_DOMImplementation_createDocumentType,
//    Nh_WebIDL_DOM_DOMImplementation_createDocument,
//    Nh_WebIDL_DOM_DOMImplementation_createHTMLDocument,
//    Nh_WebIDL_DOM_DOMImplementation_hasFeature,
//    Nh_WebIDL_DOM_Comment_constructor,
//    Nh_WebIDL_DOM_CharacterData_substringData,
//    Nh_WebIDL_DOM_CharacterData_appendData,
//    Nh_WebIDL_DOM_CharacterData_insertData,
//    Nh_WebIDL_DOM_CharacterData_deleteData,
//    Nh_WebIDL_DOM_CharacterData_replaceData,
//    Nh_WebIDL_DOM_AbortController_constructor,
//    Nh_WebIDL_DOM_AbortController_abort,
};

size_t NH_WEBIDL_FRAGMENT_FUNCTIONS_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENT_FUNCTIONS_PP) / sizeof(NH_WEBIDL_FRAGMENT_FUNCTIONS_PP[0]);
