/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_CSS_UTILITY_MACROS_H
#define NH_CSS_UTILITY_MACROS_H

#define NH_CSS_SHOW_TOP_BORDER(Node_p)                                          \
(                                                                               \
       Node_p->Properties.Border.Width.top > 0.0f                      \
    && Node_p->Properties.Border.Style.top != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_SHOW_BOTTOM_BORDER(Node_p)                                          \
(                                                                                  \
       Node_p->Properties.Border.Width.bottom > 0.0f                      \
    && Node_p->Properties.Border.Style.bottom != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_SHOW_RIGHT_BORDER(Node_p)                                          \
(                                                                                 \
       Node_p->Properties.Border.Width.right > 0.0f                      \
    && Node_p->Properties.Border.Style.right != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_SHOW_LEFT_BORDER(Node_p)                                          \
(                                                                                \
       Node_p->Properties.Border.Width.left > 0.0f                      \
    && Node_p->Properties.Border.Style.left != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_IS_SHEET(path)         \
(                                     \
    strlen(path) >= 4                 \
    &&  path[strlen(path) - 1] == 's' \
    &&  path[strlen(path) - 2] == 's' \
    &&  path[strlen(path) - 3] == 'c' \
    &&  path[strlen(path) - 4] == '.' \
)

#endif 
