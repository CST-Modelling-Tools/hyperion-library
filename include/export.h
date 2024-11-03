#ifndef EXPORT_H
#define EXPORT_H

// Define import/export macros based on platform
#if !defined(HYPERIONLIBRARY_API)
    #if defined(_WIN32) || defined(_WIN64) // Windows builds
        #if defined(HYPERIONLIBRARY_DLL_EXPORT)
            #define HYPERIONLIBRARY_API __declspec(dllexport)
        #else
            #define HYPERIONLIBRARY_API __declspec(dllimport)
        #endif
    #else // Non-Windows builds (e.g., Linux)
        #define HYPERIONLIBRARY_API
    #endif
#endif

#endif // EXPORT_H