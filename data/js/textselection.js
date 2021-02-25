/**
 * Netzhaut - Web Browser Engine 
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// text selection

{
    var TextSelection = new Range();

    document.addEventListener("mousedown", setTextSelectBegin(), {text: true});
    document.addEventListener("mousemove", setTextSelectEnd(), {text: true});

    TextSelection.setStart(event.target);

    function setTextSelectBegin(event) {
        TextSelection.setStart(event.target);
        TextSelection.setEnd(event.target);
        updateTextSelection();
    }

    function setTextSelectEnd(event) {
        TextSelection.setEnd(event.target);
        updateTextSelection();
    }

    function updateTextSelection() {
        window.getSelection().removeAllRanges();
        window.getSelection().addRange(TextSelection);
    }
}
