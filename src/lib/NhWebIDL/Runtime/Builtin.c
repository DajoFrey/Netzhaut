// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 *
 * This file was generated by NhInstaller
 */

// INCLUDE =========================================================================================

#include "Builtin.h"

#include "../Specifications/DOM/TreeWalker.idl.h"
#include "../Specifications/DOM/Text.idl.h"
#include "../Specifications/DOM/StaticRange.idl.h"
#include "../Specifications/DOM/Slottable.idl.h"
#include "../Specifications/DOM/ShadowRoot.idl.h"
#include "../Specifications/DOM/Range.idl.h"
#include "../Specifications/DOM/ProcessingInstruction.idl.h"
#include "../Specifications/DOM/ParentNode.idl.h"
#include "../Specifications/DOM/NonElementParentNode.idl.h"
#include "../Specifications/DOM/NonDocumentTypeChildNode.idl.h"
#include "../Specifications/DOM/NodeList.idl.h"
#include "../Specifications/DOM/NodeIterator.idl.h"
#include "../Specifications/DOM/NodeFilter.idl.h"
#include "../Specifications/DOM/Node.idl.h"
#include "../Specifications/DOM/NamedNodeMap.idl.h"
#include "../Specifications/DOM/MutationRecord.idl.h"
#include "../Specifications/DOM/MutationObserver.idl.h"
#include "../Specifications/DOM/HTMLCollection.idl.h"
#include "../Specifications/DOM/EventTarget.idl.h"
#include "../Specifications/DOM/Event.idl.h"
#include "../Specifications/DOM/Element.idl.h"
#include "../Specifications/DOM/DocumentType.idl.h"
#include "../Specifications/DOM/DocumentOrShadowRoot.idl.h"
#include "../Specifications/DOM/DocumentFragment.idl.h"
#include "../Specifications/DOM/Document.idl.h"
#include "../Specifications/DOM/DOMTokenList.idl.h"
#include "../Specifications/DOM/DOMImplementation.idl.h"
#include "../Specifications/DOM/Comment.idl.h"
#include "../Specifications/DOM/ChildNode.idl.h"
#include "../Specifications/DOM/CharacterData.idl.h"
#include "../Specifications/DOM/CDataSection.idl.h"
#include "../Specifications/DOM/Attr.idl.h"
#include "../Specifications/DOM/AbstractRange.idl.h"
#include "../Specifications/DOM/AbortSignal.idl.h"
#include "../Specifications/DOM/AbortController.idl.h"
#include "../Specifications/HTML/AbstractWorker.idl.h"
#include "../Specifications/HTML/AudioAndVideoTrackList.idl.h"
#include "../Specifications/HTML/CanvasRenderingContext2D.idl.h"
#include "../Specifications/HTML/CustomElementRegistry.idl.h"
#include "../Specifications/HTML/DOMStringMap.idl.h"
#include "../Specifications/HTML/DedicatedWorkerGlobalScope.idl.h"
#include "../Specifications/HTML/Document.idl.h"
#include "../Specifications/HTML/DocumentOrShadowRoot.idl.h"
#include "../Specifications/HTML/ElementInternals.idl.h"
#include "../Specifications/HTML/FormDataEvent.idl.h"
#include "../Specifications/HTML/HTMLAnchorElement.idl.h"
#include "../Specifications/HTML/HTMLAreaElement.idl.h"
#include "../Specifications/HTML/HTMLAudioElement.idl.h"
#include "../Specifications/HTML/HTMLBRElement.idl.h"
#include "../Specifications/HTML/HTMLBaseElement.idl.h"
#include "../Specifications/HTML/HTMLBodyElement.idl.h"
#include "../Specifications/HTML/HTMLButtonElement.idl.h"
#include "../Specifications/HTML/HTMLCanvasElement.idl.h"
#include "../Specifications/HTML/HTMLDListElement.idl.h"
#include "../Specifications/HTML/HTMLDataElement.idl.h"
#include "../Specifications/HTML/HTMLDataListElement.idl.h"
#include "../Specifications/HTML/HTMLDetailsElement.idl.h"
#include "../Specifications/HTML/HTMLDialogElement.idl.h"
#include "../Specifications/HTML/HTMLDivElement.idl.h"
#include "../Specifications/HTML/HTMLElement.idl.h"
#include "../Specifications/HTML/HTMLEmbedElement.idl.h"
#include "../Specifications/HTML/HTMLFieldSetElement.idl.h"
#include "../Specifications/HTML/HTMLFormElement.idl.h"
#include "../Specifications/HTML/HTMLHRElement.idl.h"
#include "../Specifications/HTML/HTMLHeadElement.idl.h"
#include "../Specifications/HTML/HTMLHeadingElement.idl.h"
#include "../Specifications/HTML/HTMLHtmlElement.idl.h"
#include "../Specifications/HTML/HTMLHyperlinkElementUtils.idl.h"
#include "../Specifications/HTML/HTMLIFrameElement.idl.h"
#include "../Specifications/HTML/HTMLImageElement.idl.h"
#include "../Specifications/HTML/HTMLInputElement.idl.h"
#include "../Specifications/HTML/HTMLLIElement.idl.h"
#include "../Specifications/HTML/HTMLLabelElement.idl.h"
#include "../Specifications/HTML/HTMLLegendElement.idl.h"
#include "../Specifications/HTML/HTMLLinkElement.idl.h"
#include "../Specifications/HTML/HTMLMapElement.idl.h"
#include "../Specifications/HTML/HTMLMediaElement.idl.h"
#include "../Specifications/HTML/HTMLMediaError.idl.h"
#include "../Specifications/HTML/HTMLMenuElement.idl.h"
#include "../Specifications/HTML/HTMLMetaElement.idl.h"
#include "../Specifications/HTML/HTMLMeterElement.idl.h"
#include "../Specifications/HTML/HTMLModElement.idl.h"
#include "../Specifications/HTML/HTMLOListElement.idl.h"
#include "../Specifications/HTML/HTMLObjectElement.idl.h"
#include "../Specifications/HTML/HTMLOptGroupElement.idl.h"
#include "../Specifications/HTML/HTMLOptionElement.idl.h"
#include "../Specifications/HTML/HTMLOrSVGElement.idl.h"
#include "../Specifications/HTML/HTMLOutputElement.idl.h"
#include "../Specifications/HTML/HTMLParagraphElement.idl.h"
#include "../Specifications/HTML/HTMLParamElement.idl.h"
#include "../Specifications/HTML/HTMLPictureElement.idl.h"
#include "../Specifications/HTML/HTMLPreElement.idl.h"
#include "../Specifications/HTML/HTMLProgressElement.idl.h"
#include "../Specifications/HTML/HTMLQuoteElement.idl.h"
#include "../Specifications/HTML/HTMLScriptElement.idl.h"
#include "../Specifications/HTML/HTMLSelectElement.idl.h"
#include "../Specifications/HTML/HTMLSlotElement.idl.h"
#include "../Specifications/HTML/HTMLSourceElement.idl.h"
#include "../Specifications/HTML/HTMLSpanElement.idl.h"
#include "../Specifications/HTML/HTMLStyleElement.idl.h"
#include "../Specifications/HTML/HTMLTableCellElement.idl.h"
#include "../Specifications/HTML/HTMLTableColElement.idl.h"
#include "../Specifications/HTML/HTMLTableElement.idl.h"
#include "../Specifications/HTML/HTMLTableRowElement.idl.h"
#include "../Specifications/HTML/HTMLTableSectionElement.idl.h"

// FRAGMENT NAMES ==================================================================================

NH_BYTE *NH_WEBIDL_FRAGMENT_NAMES_PP[] = {
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
    "HTML_AbstractWorker",
    "HTML_AudioAndVideoTrackList",
    "HTML_CanvasRenderingContext2D",
    "HTML_CustomElementRegistry",
    "HTML_DOMStringMap",
    "HTML_DedicatedWorkerGlobalScope",
    "HTML_Document",
    "HTML_DocumentOrShadowRoot",
    "HTML_ElementInternals",
    "HTML_FormDataEvent",
    "HTML_HTMLAnchorElement",
    "HTML_HTMLAreaElement",
    "HTML_HTMLAudioElement",
    "HTML_HTMLBRElement",
    "HTML_HTMLBaseElement",
    "HTML_HTMLBodyElement",
    "HTML_HTMLButtonElement",
    "HTML_HTMLCanvasElement",
    "HTML_HTMLDListElement",
    "HTML_HTMLDataElement",
    "HTML_HTMLDataListElement",
    "HTML_HTMLDetailsElement",
    "HTML_HTMLDialogElement",
    "HTML_HTMLDivElement",
    "HTML_HTMLElement",
    "HTML_HTMLEmbedElement",
    "HTML_HTMLFieldSetElement",
    "HTML_HTMLFormElement",
    "HTML_HTMLHRElement",
    "HTML_HTMLHeadElement",
    "HTML_HTMLHeadingElement",
    "HTML_HTMLHtmlElement",
    "HTML_HTMLHyperlinkElementUtils",
    "HTML_HTMLIFrameElement",
    "HTML_HTMLImageElement",
    "HTML_HTMLInputElement",
    "HTML_HTMLLIElement",
    "HTML_HTMLLabelElement",
    "HTML_HTMLLegendElement",
    "HTML_HTMLLinkElement",
    "HTML_HTMLMapElement",
    "HTML_HTMLMediaElement",
    "HTML_HTMLMediaError",
    "HTML_HTMLMenuElement",
    "HTML_HTMLMetaElement",
    "HTML_HTMLMeterElement",
    "HTML_HTMLModElement",
    "HTML_HTMLOListElement",
    "HTML_HTMLObjectElement",
    "HTML_HTMLOptGroupElement",
    "HTML_HTMLOptionElement",
    "HTML_HTMLOrSVGElement",
    "HTML_HTMLOutputElement",
    "HTML_HTMLParagraphElement",
    "HTML_HTMLParamElement",
    "HTML_HTMLPictureElement",
    "HTML_HTMLPreElement",
    "HTML_HTMLProgressElement",
    "HTML_HTMLQuoteElement",
    "HTML_HTMLScriptElement",
    "HTML_HTMLSelectElement",
    "HTML_HTMLSlotElement",
    "HTML_HTMLSourceElement",
    "HTML_HTMLSpanElement",
    "HTML_HTMLStyleElement",
    "HTML_HTMLTableCellElement",
    "HTML_HTMLTableColElement",
    "HTML_HTMLTableElement",
    "HTML_HTMLTableRowElement",
    "HTML_HTMLTableSectionElement",
};

size_t NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP) / sizeof(NH_WEBIDL_FRAGMENT_NAMES_PP[0]);

// FRAGMENTS =======================================================================================

unsigned NH_BYTE *NH_WEBIDL_FRAGMENTS_PP[] = {
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
    Nh_WebIDL_HTML_AbstractWorker_p,
    Nh_WebIDL_HTML_AudioAndVideoTrackList_p,
    Nh_WebIDL_HTML_CanvasRenderingContext2D_p,
    Nh_WebIDL_HTML_CustomElementRegistry_p,
    Nh_WebIDL_HTML_DOMStringMap_p,
    Nh_WebIDL_HTML_DedicatedWorkerGlobalScope_p,
    Nh_WebIDL_HTML_Document_p,
    Nh_WebIDL_HTML_DocumentOrShadowRoot_p,
    Nh_WebIDL_HTML_ElementInternals_p,
    Nh_WebIDL_HTML_FormDataEvent_p,
    Nh_WebIDL_HTML_HTMLAnchorElement_p,
    Nh_WebIDL_HTML_HTMLAreaElement_p,
    Nh_WebIDL_HTML_HTMLAudioElement_p,
    Nh_WebIDL_HTML_HTMLBRElement_p,
    Nh_WebIDL_HTML_HTMLBaseElement_p,
    Nh_WebIDL_HTML_HTMLBodyElement_p,
    Nh_WebIDL_HTML_HTMLButtonElement_p,
    Nh_WebIDL_HTML_HTMLCanvasElement_p,
    Nh_WebIDL_HTML_HTMLDListElement_p,
    Nh_WebIDL_HTML_HTMLDataElement_p,
    Nh_WebIDL_HTML_HTMLDataListElement_p,
    Nh_WebIDL_HTML_HTMLDetailsElement_p,
    Nh_WebIDL_HTML_HTMLDialogElement_p,
    Nh_WebIDL_HTML_HTMLDivElement_p,
    Nh_WebIDL_HTML_HTMLElement_p,
    Nh_WebIDL_HTML_HTMLEmbedElement_p,
    Nh_WebIDL_HTML_HTMLFieldSetElement_p,
    Nh_WebIDL_HTML_HTMLFormElement_p,
    Nh_WebIDL_HTML_HTMLHRElement_p,
    Nh_WebIDL_HTML_HTMLHeadElement_p,
    Nh_WebIDL_HTML_HTMLHeadingElement_p,
    Nh_WebIDL_HTML_HTMLHtmlElement_p,
    Nh_WebIDL_HTML_HTMLHyperlinkElementUtils_p,
    Nh_WebIDL_HTML_HTMLIFrameElement_p,
    Nh_WebIDL_HTML_HTMLImageElement_p,
    Nh_WebIDL_HTML_HTMLInputElement_p,
    Nh_WebIDL_HTML_HTMLLIElement_p,
    Nh_WebIDL_HTML_HTMLLabelElement_p,
    Nh_WebIDL_HTML_HTMLLegendElement_p,
    Nh_WebIDL_HTML_HTMLLinkElement_p,
    Nh_WebIDL_HTML_HTMLMapElement_p,
    Nh_WebIDL_HTML_HTMLMediaElement_p,
    Nh_WebIDL_HTML_HTMLMediaError_p,
    Nh_WebIDL_HTML_HTMLMenuElement_p,
    Nh_WebIDL_HTML_HTMLMetaElement_p,
    Nh_WebIDL_HTML_HTMLMeterElement_p,
    Nh_WebIDL_HTML_HTMLModElement_p,
    Nh_WebIDL_HTML_HTMLOListElement_p,
    Nh_WebIDL_HTML_HTMLObjectElement_p,
    Nh_WebIDL_HTML_HTMLOptGroupElement_p,
    Nh_WebIDL_HTML_HTMLOptionElement_p,
    Nh_WebIDL_HTML_HTMLOrSVGElement_p,
    Nh_WebIDL_HTML_HTMLOutputElement_p,
    Nh_WebIDL_HTML_HTMLParagraphElement_p,
    Nh_WebIDL_HTML_HTMLParamElement_p,
    Nh_WebIDL_HTML_HTMLPictureElement_p,
    Nh_WebIDL_HTML_HTMLPreElement_p,
    Nh_WebIDL_HTML_HTMLProgressElement_p,
    Nh_WebIDL_HTML_HTMLQuoteElement_p,
    Nh_WebIDL_HTML_HTMLScriptElement_p,
    Nh_WebIDL_HTML_HTMLSelectElement_p,
    Nh_WebIDL_HTML_HTMLSlotElement_p,
    Nh_WebIDL_HTML_HTMLSourceElement_p,
    Nh_WebIDL_HTML_HTMLSpanElement_p,
    Nh_WebIDL_HTML_HTMLStyleElement_p,
    Nh_WebIDL_HTML_HTMLTableCellElement_p,
    Nh_WebIDL_HTML_HTMLTableColElement_p,
    Nh_WebIDL_HTML_HTMLTableElement_p,
    Nh_WebIDL_HTML_HTMLTableRowElement_p,
    Nh_WebIDL_HTML_HTMLTableSectionElement_p,
};

size_t NH_WEBIDL_FRAGMENTS_PP_COUNT = sizeof(NH_WEBIDL_FRAGMENTS_PP) / sizeof(NH_WEBIDL_FRAGMENTS_PP[0]);

// FRAGMENT LENGTHS =================================================================================

const unsigned int NH_WEBIDL_FRAGMENT_LENGTHS_P[] = {
    Nh_WebIDL_DOM_TreeWalker_p_len,
    Nh_WebIDL_DOM_Text_p_len,
    Nh_WebIDL_DOM_StaticRange_p_len,
    Nh_WebIDL_DOM_Slottable_p_len,
    Nh_WebIDL_DOM_ShadowRoot_p_len,
    Nh_WebIDL_DOM_Range_p_len,
    Nh_WebIDL_DOM_ProcessingInstruction_p_len,
    Nh_WebIDL_DOM_ParentNode_p_len,
    Nh_WebIDL_DOM_NonElementParentNode_p_len,
    Nh_WebIDL_DOM_NonDocumentTypeChildNode_p_len,
    Nh_WebIDL_DOM_NodeList_p_len,
    Nh_WebIDL_DOM_NodeIterator_p_len,
    Nh_WebIDL_DOM_NodeFilter_p_len,
    Nh_WebIDL_DOM_Node_p_len,
    Nh_WebIDL_DOM_NamedNodeMap_p_len,
    Nh_WebIDL_DOM_MutationRecord_p_len,
    Nh_WebIDL_DOM_MutationObserver_p_len,
    Nh_WebIDL_DOM_HTMLCollection_p_len,
    Nh_WebIDL_DOM_EventTarget_p_len,
    Nh_WebIDL_DOM_Event_p_len,
    Nh_WebIDL_DOM_Element_p_len,
    Nh_WebIDL_DOM_DocumentType_p_len,
    Nh_WebIDL_DOM_DocumentOrShadowRoot_p_len,
    Nh_WebIDL_DOM_DocumentFragment_p_len,
    Nh_WebIDL_DOM_Document_p_len,
    Nh_WebIDL_DOM_DOMTokenList_p_len,
    Nh_WebIDL_DOM_DOMImplementation_p_len,
    Nh_WebIDL_DOM_Comment_p_len,
    Nh_WebIDL_DOM_ChildNode_p_len,
    Nh_WebIDL_DOM_CharacterData_p_len,
    Nh_WebIDL_DOM_CDataSection_p_len,
    Nh_WebIDL_DOM_Attr_p_len,
    Nh_WebIDL_DOM_AbstractRange_p_len,
    Nh_WebIDL_DOM_AbortSignal_p_len,
    Nh_WebIDL_DOM_AbortController_p_len,
    Nh_WebIDL_HTML_AbstractWorker_p_len,
    Nh_WebIDL_HTML_AudioAndVideoTrackList_p_len,
    Nh_WebIDL_HTML_CanvasRenderingContext2D_p_len,
    Nh_WebIDL_HTML_CustomElementRegistry_p_len,
    Nh_WebIDL_HTML_DOMStringMap_p_len,
    Nh_WebIDL_HTML_DedicatedWorkerGlobalScope_p_len,
    Nh_WebIDL_HTML_Document_p_len,
    Nh_WebIDL_HTML_DocumentOrShadowRoot_p_len,
    Nh_WebIDL_HTML_ElementInternals_p_len,
    Nh_WebIDL_HTML_FormDataEvent_p_len,
    Nh_WebIDL_HTML_HTMLAnchorElement_p_len,
    Nh_WebIDL_HTML_HTMLAreaElement_p_len,
    Nh_WebIDL_HTML_HTMLAudioElement_p_len,
    Nh_WebIDL_HTML_HTMLBRElement_p_len,
    Nh_WebIDL_HTML_HTMLBaseElement_p_len,
    Nh_WebIDL_HTML_HTMLBodyElement_p_len,
    Nh_WebIDL_HTML_HTMLButtonElement_p_len,
    Nh_WebIDL_HTML_HTMLCanvasElement_p_len,
    Nh_WebIDL_HTML_HTMLDListElement_p_len,
    Nh_WebIDL_HTML_HTMLDataElement_p_len,
    Nh_WebIDL_HTML_HTMLDataListElement_p_len,
    Nh_WebIDL_HTML_HTMLDetailsElement_p_len,
    Nh_WebIDL_HTML_HTMLDialogElement_p_len,
    Nh_WebIDL_HTML_HTMLDivElement_p_len,
    Nh_WebIDL_HTML_HTMLElement_p_len,
    Nh_WebIDL_HTML_HTMLEmbedElement_p_len,
    Nh_WebIDL_HTML_HTMLFieldSetElement_p_len,
    Nh_WebIDL_HTML_HTMLFormElement_p_len,
    Nh_WebIDL_HTML_HTMLHRElement_p_len,
    Nh_WebIDL_HTML_HTMLHeadElement_p_len,
    Nh_WebIDL_HTML_HTMLHeadingElement_p_len,
    Nh_WebIDL_HTML_HTMLHtmlElement_p_len,
    Nh_WebIDL_HTML_HTMLHyperlinkElementUtils_p_len,
    Nh_WebIDL_HTML_HTMLIFrameElement_p_len,
    Nh_WebIDL_HTML_HTMLImageElement_p_len,
    Nh_WebIDL_HTML_HTMLInputElement_p_len,
    Nh_WebIDL_HTML_HTMLLIElement_p_len,
    Nh_WebIDL_HTML_HTMLLabelElement_p_len,
    Nh_WebIDL_HTML_HTMLLegendElement_p_len,
    Nh_WebIDL_HTML_HTMLLinkElement_p_len,
    Nh_WebIDL_HTML_HTMLMapElement_p_len,
    Nh_WebIDL_HTML_HTMLMediaElement_p_len,
    Nh_WebIDL_HTML_HTMLMediaError_p_len,
    Nh_WebIDL_HTML_HTMLMenuElement_p_len,
    Nh_WebIDL_HTML_HTMLMetaElement_p_len,
    Nh_WebIDL_HTML_HTMLMeterElement_p_len,
    Nh_WebIDL_HTML_HTMLModElement_p_len,
    Nh_WebIDL_HTML_HTMLOListElement_p_len,
    Nh_WebIDL_HTML_HTMLObjectElement_p_len,
    Nh_WebIDL_HTML_HTMLOptGroupElement_p_len,
    Nh_WebIDL_HTML_HTMLOptionElement_p_len,
    Nh_WebIDL_HTML_HTMLOrSVGElement_p_len,
    Nh_WebIDL_HTML_HTMLOutputElement_p_len,
    Nh_WebIDL_HTML_HTMLParagraphElement_p_len,
    Nh_WebIDL_HTML_HTMLParamElement_p_len,
    Nh_WebIDL_HTML_HTMLPictureElement_p_len,
    Nh_WebIDL_HTML_HTMLPreElement_p_len,
    Nh_WebIDL_HTML_HTMLProgressElement_p_len,
    Nh_WebIDL_HTML_HTMLQuoteElement_p_len,
    Nh_WebIDL_HTML_HTMLScriptElement_p_len,
    Nh_WebIDL_HTML_HTMLSelectElement_p_len,
    Nh_WebIDL_HTML_HTMLSlotElement_p_len,
    Nh_WebIDL_HTML_HTMLSourceElement_p_len,
    Nh_WebIDL_HTML_HTMLSpanElement_p_len,
    Nh_WebIDL_HTML_HTMLStyleElement_p_len,
    Nh_WebIDL_HTML_HTMLTableCellElement_p_len,
    Nh_WebIDL_HTML_HTMLTableColElement_p_len,
    Nh_WebIDL_HTML_HTMLTableElement_p_len,
    Nh_WebIDL_HTML_HTMLTableRowElement_p_len,
    Nh_WebIDL_HTML_HTMLTableSectionElement_p_len,
};
