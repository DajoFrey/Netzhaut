/**
 * Netzhaut - Web Browser Engine 
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// misc

document.addEventListener("mouseenter", activateHover);
document.addEventListener("mouseleave", deactivateHover);

function activateHover(event) {
    event.target.activatePseudoClass("hover");
    let p = event.target.parentElement;
    while (p) {
        p.target.activatePseudoClass("hover");
        p = p.parentElement;
    }
}
function deactivateHover(event) {
    event.target.deactivatePseudoClass("hover");
    let p = event.target.parentElement;
    while (p) {
        p.target.deactivatePseudoClass("hover");
        p = p.parentElement;
    }
}

document.addEventListener("mousedown", activateActive);
document.addEventListener("mouseup", deactivateActive);

function activateActive(event) {
    document.deactivatePseudoClass("focus");
    event.target.activatePseudoClass("active");
    event.target.activatePseudoClass("focus");
    let p = event.target.parentElement;
    while (p) {
        p.target.activatePseudoClass("active");
        p.target.activatePseudoClass("focus");
        p = p.parentElement;
    }
}
function deactivateActive(event) {
    document.deactivatePseudoClass("active");
}
