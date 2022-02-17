/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   WT_noise_off_png;
    const int            WT_noise_off_pngSize = 3631;

    extern const char*   WT_noise_on_png;
    const int            WT_noise_on_pngSize = 3883;

    extern const char*   WT_roundsaw_off_png;
    const int            WT_roundsaw_off_pngSize = 3005;

    extern const char*   WT_roundsaw_on_png;
    const int            WT_roundsaw_on_pngSize = 3237;

    extern const char*   WT_sawtooth_off_png;
    const int            WT_sawtooth_off_pngSize = 3229;

    extern const char*   WT_sawtooth_on_png;
    const int            WT_sawtooth_on_pngSize = 3425;

    extern const char*   WT_sinus_off_png;
    const int            WT_sinus_off_pngSize = 3901;

    extern const char*   WT_sinus_on_png;
    const int            WT_sinus_on_pngSize = 4286;

    extern const char*   WT_square_off_png;
    const int            WT_square_off_pngSize = 2522;

    extern const char*   WT_square_on_png;
    const int            WT_square_on_pngSize = 2622;

    extern const char*   WT_triangle_off_png;
    const int            WT_triangle_off_pngSize = 3720;

    extern const char*   WT_triangle_on_png;
    const int            WT_triangle_on_pngSize = 4129;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 12;

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
