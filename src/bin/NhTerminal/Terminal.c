/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../lib/Netzhaut/Netzhaut.h"
#include "../../lib/Netzhaut/NetzhautTTY.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv_pp) 
{
    if (Nh_init() != NH_SUCCESS) {return 1;}

    Nh_TTY_Terminal *Terminal_p = Nh_TTY_openTerminal();

    if (Terminal_p == NULL) {
        system("gnome-terminal -- NhTerminal");
    }
    else {

        if (Nh_TTY_addDefaultProgram(Terminal_p, "editor") != NH_TTY_SUCCESS) {
            return 1;
        }
        if (Nh_TTY_addDefaultProgram(Terminal_p, "logger") != NH_TTY_SUCCESS) {
            return 1;
        }

        while (1) {
            if (!Nh_run()) {usleep(10000);}
            if (!Nh_keepRunning()) {break;}
        }

        Nh_release();
    }
}

