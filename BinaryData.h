/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   background_png;
    const int            background_pngSize = 17338;

    extern const char*   redRocks_png;
    const int            redRocks_pngSize = 234351;

    extern const char*   RecitalHall_jpg;
    const int            RecitalHall_jpgSize = 1519990;

    extern const char*   PracticeRoom_png;
    const int            PracticeRoom_pngSize = 517254;

    extern const char*   Carnigie_jpg;
    const int            Carnigie_jpgSize = 7670632;

    extern const char*   testImage_png;
    const int            testImage_pngSize = 722247;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
