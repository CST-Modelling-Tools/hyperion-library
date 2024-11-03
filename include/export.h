#ifndef EXPORT_H
#define EXPORT_H

#pragma once

#if ! defined(DLL_API)
#   if defined(_WIN32) || defined(_WIN64) // for windows builds
#       if defined(HYPERIONLIBRARY_DLL_EXPORT)
#           define DLL_API __declspec(dllexport)
#       else
#           define DLL_API __declspec(dllimport)
#       endif
#   else               // for linux builds
#       define DLL_API
#   endif
#endif

#endif // EXPORT_H