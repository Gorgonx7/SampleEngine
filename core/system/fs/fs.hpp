//
//  fs.hpp
//  SampleEngine
//
//  Created by James Gordon on 24/02/2024.
//

#pragma once
#include <vector>
#include <iostream>
#include <fstream>
class fs
{
public:
    /// @brief
    /// @param filename
    /// @return
    static std::vector<char> readFile(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
};