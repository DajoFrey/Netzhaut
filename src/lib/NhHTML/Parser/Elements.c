// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Elements.h"

#include "../Common/IndexMap.h"
#include "../Common/Macro.h"
#include NH_HTML_DEFAULT_CHECK
#include NH_HTML_FLOW

#include "../../NhWebIDL/Runtime/Runtime.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TAG NAMES =======================================================================================

const NH_BYTE *NH_HTML_TAG_NAMES_PP[] =
{
    "a",
    "abbr",
    "address",
    "area",
    "article",
    "aside",
    "audio",
    "b",
    "base",
    "bdi",
    "bdo",
    "blockquote",
    "body",
    "br",
    "button",
    "canvas",
    "caption",
    "cite",
    "code",
    "col",
    "colgroup",
    "data",
    "datalist",
    "dd",
    "del",
    "details",
    "dfn",
    "dialog",
    "div",
    "dl",
    "dt",
    "em",
    "embed",
    "fieldset",
    "figcaption",
    "figure",
    "footer",
    "form",
    "h1",
    "h2",
    "h3",
    "h4",
    "h5",
    "h6",
    "head",
    "header",
    "hgroup",
    "hr",
    "html",
    "i",
    "iframe",
    "img",
    "input",
    "ins",
    "kbd",
    "label",
    "legend",
    "li",
    "link",
    "main",
    "map",
    "mark",
    "math",
    "menu",
    "meta",
    "meter",
    "nav",
    "noscript",
    "object",
    "ol",
    "optgroup",
    "option",
    "output",
    "p",
    "param",
    "picture",
    "pre",
    "progress",
    "q",
    "rp",
    "rt",
    "ruby",
    "s",
    "samp",
    "script",
    "section",
    "select",
    "slot",
    "small",
    "source",
    "span",
    "strong",
    "style",
    "sub",
    "summary",
    "sup",
    "svg",
    "table",
    "tbody",
    "td",
    "template",
    "textarea",
    "tfoot",
    "th",
    "thead",
    "time",
    "title",
    "tr",
    "track",
    "u",
    "ul",
    "var",
    "video",
    "wbr",
};

unsigned long NH_HTML_TAG_NAMES_PP_COUNT = sizeof(NH_HTML_TAG_NAMES_PP) / sizeof(NH_HTML_TAG_NAMES_PP[0]);

// GET CORRESPONDING CLASS =========================================================================

// https://html.spec.whatwg.org/multipage/dom.html#elements-in-the-dom
Nh_WebIDL_Interface *Nh_HTML_getElementInterface(
    NH_BYTE *tagname_p)
{
NH_HTML_BEGIN()

    void *value_p = Nh_getFromHashMap(&NH_HTML_INDEXMAP.Tags, tagname_p);

    if (!value_p) 
    {
        if (!strcmp(tagname_p, "applet")
        ||  !strcmp(tagname_p, "bgsound")
        ||  !strcmp(tagname_p, "blink")
        ||  !strcmp(tagname_p, "isindex")
        ||  !strcmp(tagname_p, "keygen")
        ||  !strcmp(tagname_p, "multicol")
        ||  !strcmp(tagname_p, "nextid")
        ||  !strcmp(tagname_p, "spacer")) 
        { 
            NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLUnknownElement"))
        }

        if (!strcmp(tagname_p, "acronym")
        ||  !strcmp(tagname_p, "basefont")
        ||  !strcmp(tagname_p, "big")
        ||  !strcmp(tagname_p, "center")
        ||  !strcmp(tagname_p, "nobr")
        ||  !strcmp(tagname_p, "noembed")
        ||  !strcmp(tagname_p, "noframes")
        ||  !strcmp(tagname_p, "plaintext")
        ||  !strcmp(tagname_p, "rb")
        ||  !strcmp(tagname_p, "rtc")
        ||  !strcmp(tagname_p, "strike")
        ||  !strcmp(tagname_p, "tt")) 
        { 
            NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLElement"))
        }

        if (!strcmp(tagname_p, "listing")
        ||  !strcmp(tagname_p, "xmp")) 
        {
            NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLPreElement"))
        }

        NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLUnknownElement"))
    }

    unsigned int index = *((unsigned int*)value_p);

    switch (index)
    {
        case NH_HTML_TAG_A          : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLAnchorElement"))
        case NH_HTML_TAG_AREA       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLAreaElement"))
        case NH_HTML_TAG_AUDIO      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLAudioElement"))
        case NH_HTML_TAG_BASE       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLBaseElement"))
        case NH_HTML_TAG_BLOCKQUOTE : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLQuoteElement"))
        case NH_HTML_TAG_BODY       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLBodyElement"))
        case NH_HTML_TAG_BR         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLBRElement"))
        case NH_HTML_TAG_BUTTON     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLButtonElement"))
        case NH_HTML_TAG_CANVAS     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLCanvasElement"))
        case NH_HTML_TAG_CAPTION    : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTableCaptionElement"))
        case NH_HTML_TAG_COL        :
        case NH_HTML_TAG_COLGROUP   : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTableColElement"))
        case NH_HTML_TAG_DATA       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLDataElement"))
        case NH_HTML_TAG_DATALIST   : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLDataListElement"))
        case NH_HTML_TAG_DEL        : 
        case NH_HTML_TAG_INS        : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLModElement"))
        case NH_HTML_TAG_DETAILS    : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLDetailsElement"))
        case NH_HTML_TAG_DIALOG     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLDialogElement"))
        case NH_HTML_TAG_DIV        : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLDivElement"))
        case NH_HTML_TAG_DL         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLDListElement"))
        case NH_HTML_TAG_EMBED      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLEmbedElement"))
        case NH_HTML_TAG_FIELDSET   : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLFieldSetElement"))
        case NH_HTML_TAG_FORM       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLFormElement"))
        case NH_HTML_TAG_H1         :
        case NH_HTML_TAG_H2         :
        case NH_HTML_TAG_H3         :
        case NH_HTML_TAG_H4         :
        case NH_HTML_TAG_H5         :
        case NH_HTML_TAG_H6         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLHeadingElement"))
        case NH_HTML_TAG_HEAD       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLHeadElement"))
        case NH_HTML_TAG_HR         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLHRElement"))
        case NH_HTML_TAG_HTML       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLHtmlElement"))
        case NH_HTML_TAG_IFRAME     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLIFrameElement"))
        case NH_HTML_TAG_IMG        : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLImageElement"))
        case NH_HTML_TAG_INPUT      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLInputElement"))
        case NH_HTML_TAG_LABEL      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLLabelElement"))
        case NH_HTML_TAG_LEGEND     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLLegendElement"))
        case NH_HTML_TAG_LI         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLLIElement"))
        case NH_HTML_TAG_LINK       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLLinkElement"))
        case NH_HTML_TAG_MAP        : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLMapElement"))
        case NH_HTML_TAG_MATH       : NH_HTML_END(Nh_WebIDL_getInterface("DOM", "Element"))
        case NH_HTML_TAG_MENU       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLMenuElement"))
        case NH_HTML_TAG_META       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLMetaElement"))
        case NH_HTML_TAG_METER      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLMeterElement"))
        case NH_HTML_TAG_OBJECT     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLObjectElement"))
        case NH_HTML_TAG_OL         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLOListElement"))
        case NH_HTML_TAG_OPTGROUP   : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLOptGroupElement"))
        case NH_HTML_TAG_OPTION     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLOptionElement"))
        case NH_HTML_TAG_OUTPUT     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLOutputElement"))
        case NH_HTML_TAG_P          : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLParagraphElement"))
        case NH_HTML_TAG_PARAM      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLParamElement"))
        case NH_HTML_TAG_PICTURE    : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLPictureElement"))
        case NH_HTML_TAG_PRE        : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLPreElement"))
        case NH_HTML_TAG_PROGRESS   : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLProgressElement"))
        case NH_HTML_TAG_Q          : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLQuoteElement"))
        case NH_HTML_TAG_SCRIPT     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLScriptElement"))
        case NH_HTML_TAG_SELECT     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLSelectElement"))
        case NH_HTML_TAG_SLOT       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLSlotElement"))
        case NH_HTML_TAG_SOURCE     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLSourceElement"))
        case NH_HTML_TAG_SPAN       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLSpanElement"))
        case NH_HTML_TAG_STYLE      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLStyleElement"))
        case NH_HTML_TAG_SVG        : NH_HTML_END(Nh_WebIDL_getInterface("SVG", "SVGSVGElement"))
        case NH_HTML_TAG_TABLE      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTableElement"))
        case NH_HTML_TAG_TD         :
        case NH_HTML_TAG_TH         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTableCellElement"))
        case NH_HTML_TAG_TEMPLATE   : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTemplateElement"))
        case NH_HTML_TAG_TEXTAREA   : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTextAreaElement"))
        case NH_HTML_TAG_TBODY      :
        case NH_HTML_TAG_TFOOT      :
        case NH_HTML_TAG_THEAD      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTableSectionElement"))
        case NH_HTML_TAG_TIME       : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTimeElement"))
        case NH_HTML_TAG_TITLE      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTitleElement"))
        case NH_HTML_TAG_TR         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTableRowElement"))
        case NH_HTML_TAG_TRACK      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLTrackElement"))
        case NH_HTML_TAG_UL         : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLUListElement"))
        case NH_HTML_TAG_VIDEO      : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLVideoElement"))
        default                     : NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLElement"))
    }

NH_HTML_END(Nh_WebIDL_getInterface("HTML", "HTMLUnknownElement"))
}

