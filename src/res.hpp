// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#pragma once

namespace res {
extern const char* syncroboverb_bg_jpg;
const int syncroboverb_bg_jpgSize = 167072;

extern const char* red_knob_png;
const int red_knob_pngSize = 546683;

extern const char* toggle_switch_png;
const int toggle_switch_pngSize = 20873;

extern const char* sphere_scope_png;
const int sphere_scope_pngSize = 2034111;

// Number of elements in the namedResourceList and originalFileNames arrays.
const int namedResourceListSize = 4;

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
} // namespace res
