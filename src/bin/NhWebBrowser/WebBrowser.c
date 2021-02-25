/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Netzhaut/Netzhaut.h"

#include <stdio.h>
#include <unistd.h>

void logCallback(char *string_p) {printf(string_p);}

int main(int argc, char **argv_pp) 
{
#define CHECK(result) if (result) {Nh_release(NULL); return 1;}

    CHECK(Nh_init(argc, argv_pp, NULL))
    CHECK(Nh_start())

    while (1) {
        // usleep may return -1 because of signals from forked processes etc.
        if (!Nh_run()) {usleep(10000);}
        if (!Nh_keepRunning()) {break;}
    }

    CHECK(Nh_release(NULL))
}

