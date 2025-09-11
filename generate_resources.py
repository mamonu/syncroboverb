#!/usr/bin/env python3
"""
Resource Generator for SyncRoboVerb
Converts image files to binary C++ data for embedding in the plugin

Usage:
    python3 generate_resources.py

This script automatically finds all image files in data/content/ and generates:
- src/res.hpp: Header file with resource declarations
- src/res.cpp: Implementation file with binary data

Image formats supported: .jpg, .jpeg, .png, .gif, .bmp

Example:
    # Add a new background image
    cp new_background.jpg data/content/syncroboverb_bg.jpg
    python3 generate_resources.py
    
    # The resource will be available as:
    # res::syncroboverb_bg_jpg and res::syncroboverb_bg_jpgSize
"""

import os
import sys
from pathlib import Path

def file_to_cpp_name(filename):
    """Convert filename to C++ variable name"""
    # Remove extension and convert to valid C++ identifier
    name = Path(filename).stem
    name = name.replace('-', '_').replace(' ', '_').replace('.', '_')
    return name.lower()

def generate_cpp_data(file_path):
    """Convert binary file to C++ char array data"""
    with open(file_path, 'rb') as f:
        data = f.read()
    
    # Generate hex data string
    hex_data = []
    for i, byte in enumerate(data):
        if i % 12 == 0:  # New line every 12 bytes
            hex_data.append('\n  ')
        hex_data.append(f'0x{byte:02x},')
    
    return ''.join(hex_data)[:-1]  # Remove last comma

def generate_resources():
    """Generate res.cpp and res.hpp files from images in data/content/"""
    
    content_dir = Path('data/content')
    if not content_dir.exists():
        print(f"Error: {content_dir} directory not found")
        return False
    
    # Find all image files
    image_extensions = {'.jpg', '.jpeg', '.png', '.gif', '.bmp'}
    image_files = []
    
    for ext in image_extensions:
        image_files.extend(content_dir.glob(f'*{ext}'))
    
    image_files.sort()  # Consistent ordering
    
    if not image_files:
        print(f"No image files found in {content_dir}")
        return False
    
    print(f"Found {len(image_files)} image files:")
    for f in image_files:
        print(f"  - {f}")
    
    # Generate header file content
    header_content = """// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#pragma once

namespace res {
"""
    
    # Generate cpp file content
    cpp_content = """// Copyright (C) 2015-2025  Kushview, LLC <info@kushview.net>
// SPDX-License-Identifier: GPL3-or-later

#include "res.hpp"
#include <cstring>

namespace res {

"""
    
    resource_names = []
    original_filenames = []
    
    # Process each image file
    for img_file in image_files:
        var_name = file_to_cpp_name(img_file.name)
        file_size = img_file.stat().st_size
        
        # Add to header
        header_content += f"extern const unsigned char {var_name}_{img_file.suffix[1:]}[];\n"
        header_content += f"const int {var_name}_{img_file.suffix[1:]}Size = {file_size};\n\n"
        
        # Add to cpp
        cpp_data = generate_cpp_data(img_file)
        cpp_content += f"const unsigned char {var_name}_{img_file.suffix[1:]}[] = {{{cpp_data}\n}};\n\n"
        
        resource_names.append(f"{var_name}_{img_file.suffix[1:]}")
        original_filenames.append(img_file.name)
    
    # Add resource list arrays to header
    header_content += f"// Number of elements in the namedResourceList and originalFileNames arrays.\n"
    header_content += f"const int namedResourceListSize = {len(resource_names)};\n\n"
    header_content += "// Points to the start of a list of resource names.\n"
    header_content += "extern const char* namedResourceList[];\n\n"
    header_content += "// Points to the start of a list of resource filenames.\n"
    header_content += "extern const char* originalFilenames[];\n\n"
    header_content += "// If you provide the name of one of the binary resource variables above, this function will\n"
    header_content += "// return the corresponding data and its size (or a null pointer if the name isn't found).\n"
    header_content += "const unsigned char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);\n\n"
    header_content += "// If you provide the name of one of the binary resource variables above, this function will\n"
    header_content += "// return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).\n"
    header_content += "const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);\n"
    header_content += "} // namespace res\n"
    
    # Add resource list arrays to cpp
    cpp_content += "const char* namedResourceList[] = {\n"
    for name in resource_names:
        cpp_content += f'    "{name}",\n'
    cpp_content += "};\n\n"
    
    cpp_content += "const char* originalFilenames[] = {\n"
    for filename in original_filenames:
        cpp_content += f'    "{filename}",\n'
    cpp_content += "};\n\n"
    
    # Add lookup functions
    cpp_content += """const unsigned char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) {
    for (int i = 0; i < namedResourceListSize; ++i) {
        if (strcmp (resourceNameUTF8, namedResourceList[i]) == 0) {
"""
    
    for i, name in enumerate(resource_names):
        cpp_content += f"            if (i == {i}) {{ dataSizeInBytes = {name}Size; return {name}; }}\n"
    
    cpp_content += """        }
    }
    dataSizeInBytes = 0;
    return nullptr;
}

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8) {
    for (int i = 0; i < namedResourceListSize; ++i) {
        if (strcmp (resourceNameUTF8, namedResourceList[i]) == 0) {
            return originalFilenames[i];
        }
    }
    return nullptr;
}

} // namespace res
"""
    
    # Write the files
    with open('src/res.hpp', 'w') as f:
        f.write(header_content)
    
    with open('src/res.cpp', 'w') as f:
        f.write(cpp_content)
    
    print(f"\nGenerated resources:")
    print(f"  - src/res.hpp ({len(header_content)} chars)")
    print(f"  - src/res.cpp ({len(cpp_content)} chars)")
    print(f"  - {len(resource_names)} resources embedded")
    
    return True

if __name__ == "__main__":
    if generate_resources():
        print("✅ Resource generation completed successfully!")
        sys.exit(0)
    else:
        print("❌ Resource generation failed!")
        sys.exit(1)