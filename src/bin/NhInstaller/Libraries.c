// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Libraries.h"
#include "Util.h"
#include "Objects.h"
#include "Message.h"
#include "Unicode.h"
#include "Main.h"
#include "Web.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_CUSTOM_CHECK

#include "../../lib/Netzhaut/Netzhaut.h"
#include "../../lib/NhLoader/Common/About.h"
#include "../../lib/NhCore/Common/About.h"
#include "../../lib/NhTTY/Common/About.h"
#include "../../lib/NhHTML/Common/About.h"
#include "../../lib/NhDOM/Common/About.h"
#include "../../lib/NhWeb/Common/About.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>

// CREATE ==========================================================================================

static NH_INSTALLER_RESULT Nh_Installer_createLibrary(
    NH_BYTE *wrkDir_p, NH_BYTE *objPath_p, NH_BYTE *extra_p, NH_BYTE *libName_p, int major, int minor, int patch)
{
NH_INSTALLER_BEGIN()

#include NH_INSTALLER_DEFAULT_CHECK

    chdir(objPath_p);

#ifdef __unix__

    int index = 0;
    NH_BYTE objects_p[16384] = {'\0'};

    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(objPath_p)) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) {
            for (int i = 0; ent->d_name[i] != '\0' && ent->d_name[0] != '.'; ++i) {
                objects_p[index++] = ent->d_name[i];
            }
            objects_p[index++] = ' ';
        }
        closedir(dir);
    } 
    else {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_CANT_OPEN_DIR)}

    NH_BYTE libPath_p[256] = {'\0'};
    sprintf(libPath_p, "../../lib%s.so.%d.%d.%d", libName_p, major, minor, patch);
    NH_BYTE symPath1_p[256] = {'\0'};
    sprintf(symPath1_p, "../../lib%s.so.%d", libName_p, major);
    NH_BYTE symPath2_p[256] = {'\0'};
    sprintf(symPath2_p, "../../lib%s.so", libName_p);
  
    NH_INSTALLER_CHECK(Nh_Installer_createSharedLibraryUsingGCC(objects_p, libPath_p, extra_p))

    memset(libPath_p, 0, 256); 
    sprintf(libPath_p, "lib%s.so.%d.%d.%d", libName_p, major, minor, patch);
    Nh_Installer_createSymLink(libPath_p, symPath1_p, NH_FALSE);
    Nh_Installer_createSymLink(libPath_p, symPath2_p, NH_FALSE);

#endif 

    chdir(wrkDir_p);

#include NH_INSTALLER_CUSTOM_CHECK

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// INSTALL_ALL =========================================================================================

static NH_INSTALLER_RESULT Nh_Installer_installLibrary(
    NH_BYTE *libName_p, int major, int minor, int patch)
{
NH_INSTALLER_BEGIN()

    NH_BYTE wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    NH_BYTE projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    NH_BYTE libPath_p[512] = {'\0'};
    if (!strcmp(libName_p, "NhExternal")) {sprintf(libPath_p, "lib/lib%s.a", libName_p);}
    else {sprintf(libPath_p, "lib/lib%s.so.%d.%d.%d", libName_p, major, minor, patch);}

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy(libPath_p, "/usr/local/lib", NH_FALSE, NH_TRUE, NH_TRUE))

    chdir("/usr/local/lib");

    if (strcmp(libName_p, "NhExternal"))
    {
        NH_BYTE cpyPath_p[256] = {'\0'};
        sprintf(cpyPath_p, "lib%s.so.%d.%d.%d", libName_p, major, minor, patch);
        NH_BYTE symPath1_p[256] = {'\0'};
        sprintf(symPath1_p, "lib%s.so.%d", libName_p, major);
        NH_BYTE symPath2_p[256] = {'\0'};
        sprintf(symPath2_p, "lib%s.so", libName_p);

        Nh_Installer_createSymLink(cpyPath_p, symPath1_p, NH_TRUE);
        Nh_Installer_createSymLink(cpyPath_p, symPath2_p, NH_TRUE);
    }

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NETZHAUT ==================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNetzhaut(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};
    sprintf(extra_p, "-ldl", projDir_p);

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/NETZHAUT");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "Netzhaut"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "Netzhaut", NETZHAUT_MAJOR_VERSION, NETZHAUT_MINOR_VERSION, NETZHAUT_PATCH_VERSION
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "Netzhaut", NETZHAUT_MAJOR_VERSION, NETZHAUT_MINOR_VERSION, NETZHAUT_PATCH_VERSION
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_LOADER =================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhLoader(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};
    sprintf(extra_p, "-ldl");

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/LOADER");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhLoader"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhLoader", NH_LOADER_MAJOR_VERSION, NH_LOADER_MINOR_VERSION, NH_LOADER_PATCH_VERSION
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhLoader", NH_LOADER_MAJOR_VERSION, NH_LOADER_MINOR_VERSION, NH_LOADER_PATCH_VERSION
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_CORE ===================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhCore(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};
    sprintf(extra_p, "-lm -ldl -lfreetype -lNhExternal");

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/CORE");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_createUnicodeLookup(projDir_p))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhCore"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhCore", NH_MAJOR_VERSION, NH_MINOR_VERSION, NH_PATCH_VERSION
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhCore", NH_MAJOR_VERSION, NH_MINOR_VERSION, NH_PATCH_VERSION
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_IO =====================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhIO(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};
    sprintf(extra_p, "-lX11 -lX11-xcb -lXcursor -lxkbcommon -lxkbcommon-x11");

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/IO");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhIO"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhIO", 0, 0, 0
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhIO", 0, 0, 0 
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_TTY ====================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhTTY(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};
    sprintf(extra_p, "-lutil");

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/TTY");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhTTY"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhTTY", NH_TTY_MAJOR_VERSION, NH_TTY_MINOR_VERSION, NH_TTY_PATCH_VERSION
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhTTY", NH_TTY_MAJOR_VERSION, NH_TTY_MINOR_VERSION, NH_TTY_PATCH_VERSION
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_NETWORK ================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhNetwork(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};
    sprintf(extra_p, "-lssl");

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/NETWORK");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhNetwork"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhNetwork", 0, 0, 0
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhNetwork", 0, 0, 0 
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_ECMASCRIPT =============================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhECMAScript(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/ECMASCRIPT");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhECMAScript"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhECMAScript", 0, 0, 0
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhECMAScript", 0, 0, 0 
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_HTML ===================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhHTML(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/HTML");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhHTML"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhHTML", NH_HTML_MAJOR_VERSION, NH_HTML_MINOR_VERSION, NH_HTML_PATCH_VERSION
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhHTML", NH_HTML_MAJOR_VERSION, NH_HTML_MINOR_VERSION, NH_HTML_PATCH_VERSION
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_HTML ===================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhDOM(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/DOM");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhDOM"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhDOM", NH_DOM_MAJOR_VERSION, NH_DOM_MINOR_VERSION, NH_DOM_PATCH_VERSION
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhDOM", NH_DOM_MAJOR_VERSION, NH_DOM_MINOR_VERSION, NH_DOM_PATCH_VERSION
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_WEB =================================================================================

static NH_INSTALLER_RESULT Nh_Installer_buildNhWeb(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    NH_BYTE extra_p[1024] = {'\0'};

    NH_BYTE objPath_p[2048] = {'\0'};
    sprintf(objPath_p, "%s%s", projDir_p, "/lib/OBJECTS/WEB");

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_processWebSpecs())
    chdir(wrkDir_p);

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECTS, Nh_Installer_createObjectsDir(wrkDir_p, projDir_p, "NhWeb"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_LIBRARY, Nh_Installer_createLibrary(
        wrkDir_p, objPath_p, extra_p, "NhWeb", NH_WEB_MAJOR_VERSION, NH_WEB_MINOR_VERSION, NH_WEB_PATCH_VERSION
    ))

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhWeb", NH_WEB_MAJOR_VERSION, NH_WEB_MINOR_VERSION, NH_WEB_PATCH_VERSION
        ))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD NH_EXTERNAL ===============================================================================

static NH_INSTALLER_RESULT Nh_Installer_createStaticLibraryUsingAr(
    const NH_BYTE *objects_p, const NH_BYTE *out_p)
{
NH_INSTALLER_BEGIN()

    NH_BYTE command_p[1024] = {'\0'};
    sprintf(command_p, "ar rcs %s %s", out_p, objects_p);
    
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_AR_EXECUTION_FAILED)}

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_buildXXD(
    NH_BYTE *projDir_p)
{
NH_INSTALLER_BEGIN()

    NH_BYTE path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", projDir_p, "/external/xxd");
    chdir(path_p);

    int status = system("gcc -o ../../bin/external/xxd xxd.c");
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_GCC_EXECUTION_FAILED)}

    chdir(projDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_buildNhExternal(
    NH_BYTE *wrkDir_p, NH_BYTE *projDir_p, NH_BOOL install)
{
NH_INSTALLER_BEGIN()

    if (!Nh_Installer_canRunCommand("gcc")) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_GCC_NOT_FOUND)}
    if (!Nh_Installer_canRunCommand("ar")) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_AR_NOT_FOUND)}

    chdir(projDir_p);

#ifdef __unix__

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS/EXTERNAL"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_XXD_FAILED, Nh_Installer_buildXXD(projDir_p))

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/freetype-gl/utf8-utils.c", "lib/OBJECTS/EXTERNAL/utf8-utils.o", NULL))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/freetype-gl/vector.c", "lib/OBJECTS/EXTERNAL/vector.o", NULL))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/freetype-gl/texture-atlas.c", "lib/OBJECTS/EXTERNAL/texture-atlas.o", NULL))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/freetype-gl/texture-font.c", "lib/OBJECTS/EXTERNAL/texture-font.o", "-I/usr/include/freetype2"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/freetype-gl/distance-field.c", "lib/OBJECTS/EXTERNAL/distance-field.o", NULL))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/freetype-gl/edtaa3func.c", "lib/OBJECTS/EXTERNAL/edtaa3func.o", NULL))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/freetype-gl/platform.c", "lib/OBJECTS/EXTERNAL/platform.o", NULL))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/c_hashmap/hashmap.c", "lib/OBJECTS/EXTERNAL/hashmap.o", NULL))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Installer_createPICObjectFileUsingGCC("external/DOWNLOADS/volk-master/volk.c", "lib/OBJECTS/EXTERNAL/volk.o", "-DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface -DVOLK_VULKAN_H_PATH=\\\"../Vulkan-Headers-master/include/vulkan/vulkan.h\\\""))

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_STATIC_LIB, Nh_Installer_createStaticLibraryUsingAr(
        "                                         \
            lib/OBJECTS/EXTERNAL/utf8-utils.o     \
            lib/OBJECTS/EXTERNAL/vector.o         \
            lib/OBJECTS/EXTERNAL/texture-font.o   \
            lib/OBJECTS/EXTERNAL/texture-atlas.o  \
            lib/OBJECTS/EXTERNAL/distance-field.o \
            lib/OBJECTS/EXTERNAL/edtaa3func.o     \
            lib/OBJECTS/EXTERNAL/platform.o       \
            lib/OBJECTS/EXTERNAL/hashmap.o        \
            lib/OBJECTS/EXTERNAL/volk.o           \
        ",
        "lib/libNhExternal.a"
    ))

#elif defined(WIN_32)

#endif

    if (install) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BAD_STATE, Nh_Installer_installLibrary(
            "NhExternal", 0, 0, 0 
        ))
    }

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// PREPARE BUILD ===================================================================================

static NH_INSTALLER_RESULT Nh_Installer_copyAPIHeader()
{
NH_INSTALLER_BEGIN()

    NH_BYTE wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    NH_BYTE projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/NhCore/Common/API.h", "src/lib/Netzhaut/NhCore/Common", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/NhCore/Common/Result.h", "src/lib/Netzhaut/NhCore/Common", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/NhTTY/Common/API.h", "src/lib/Netzhaut/NhTTY/Common", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/NhTTY/Common/Result.h", "src/lib/Netzhaut/NhTTY/Common", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/NhLoader/Common/API.h", "src/lib/Netzhaut/NhLoader/Common", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/NhLoader/Common/Result.h", "src/lib/Netzhaut/NhLoader/Common", NH_FALSE, NH_FALSE, NH_FALSE))

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/Netzhaut/Netzhaut.h", "include/Netzhaut", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/Netzhaut/NetzhautTTY.h", "include/Netzhaut", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/Netzhaut/NhCore", "include/Netzhaut", NH_TRUE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/Netzhaut/NhTTY", "include/Netzhaut", NH_TRUE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("src/lib/Netzhaut/NhLoader", "include/Netzhaut", NH_TRUE, NH_FALSE, NH_FALSE))

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_copyExternalHeader()
{
NH_INSTALLER_BEGIN()

    NH_BYTE wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    NH_BYTE projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    NH_BYTE external_p[1024] = {'\0'};
    sprintf(external_p, "%s/src/lib/NhGraphics/Vulkan/Header/External", projDir_p);

    chdir(projDir_p);

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_EXTERNAL_DIRECTORY, Nh_Installer_createDir(external_p))

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("external/DOWNLOADS/volk-master/volk.h", "src/lib/NhGraphics/Vulkan/Header/External", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("external/DOWNLOADS/Vulkan-Headers-master/include/vulkan/*", "src/lib/NhGraphics/Vulkan/Header/External", NH_FALSE, NH_FALSE, NH_FALSE))

    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

static NH_INSTALLER_RESULT Nh_Installer_createIncludeFileUsingXXD(
    const NH_BYTE *projDir_p, const NH_BYTE *in_p, const NH_BYTE *out_p, const NH_BYTE *name_p)
{
NH_INSTALLER_BEGIN()

    Nh_Installer_messagef("CREATE FILE %s/%s", projDir_p, out_p);

    NH_BYTE command_p[1024] = {'\0'};
    sprintf(command_p, "./xxd -i %s/%s %s/%s %s", projDir_p, in_p, projDir_p, out_p, name_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_XXD_EXECUTION_FAILED)}

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

NH_INSTALLER_RESULT Nh_Installer_prepareLibraryBuild()
{
NH_INSTALLER_BEGIN()

    Nh_Installer_operationf("PREPARE LIBRARY BUILD");

    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_EXTERNAL_HEADER_FAILED, Nh_Installer_copyExternalHeader())
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_PUBLIC_LIBRARY_HEADER_FAILED, Nh_Installer_copyAPIHeader())

    NH_BYTE wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    NH_BYTE projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("lib/OBJECTS"))
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Installer_createDir("bin/external"))
    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// BUILD ===========================================================================================

NH_INSTALLER_RESULT Nh_Installer_buildLibrary(
    NH_BYTE *name_p, NH_BOOL install) 
{
NH_INSTALLER_BEGIN()

    Nh_Installer_operationf("BUILD %s LIBRARY", name_p);

    NH_BYTE wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    NH_BYTE projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    if (!strcmp(name_p, "Netzhaut")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNetzhaut(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhLoader")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhLoader(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhCore")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhCore(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhExternal")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhExternal(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhIO")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhIO(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhTTY")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhTTY(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhHTML")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhHTML(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhDOM")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhDOM(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhNetwork")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhNetwork(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhECMAScript")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhECMAScript(wrkDir_p, projDir_p, install))
    }
    else if (!strcmp(name_p, "NhWeb")) {
        NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_BUILD_LIBRARY_FAILED, Nh_Installer_buildNhWeb(wrkDir_p, projDir_p, install))
    }

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

// INSTALL_ALL INCLUDES ================================================================================

NH_INSTALLER_RESULT Nh_Installer_installIncludes()
{
NH_INSTALLER_BEGIN()

    NH_BYTE wrkDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK_NULL(NH_INSTALLER_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))
    NH_BYTE projDir_p[2048] = {'\0'};
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_GET_PROJECT_DIRECTORY, Nh_Installer_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);
    NH_INSTALLER_CHECK(NH_INSTALLER_ERROR_COPY_FAILED, Nh_Installer_copy("include/Netzhaut", "/usr/local/include", NH_TRUE, NH_TRUE, NH_TRUE))
    chdir(wrkDir_p);

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

