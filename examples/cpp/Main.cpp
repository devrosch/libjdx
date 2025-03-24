/* MIT License
 *
 * Copyright (c) 2025 Robert Schiwon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "api/Node.hpp"
#include "jdx/api/JdxScanner.hpp"

#include <iostream>

using namespace libjdx::api;
using namespace libjdx::jdx::api;

void printNode(const std::string& path, const Node& node)
{
    std::cout << "Node path: \"" << path << "\"\n";

    std::cout << "Name: \"" << node.name << "\"\n";

    std::cout << "Parameters: [";
    for (size_t i = 0; i < node.parameters.size(); i++)
    {
        if (i != 0)
        {
            std::cout << ", ";
        }
        const auto parameter = node.parameters[i];
        std::cout << "{\"" << parameter.key << "\": \"" << parameter.value
                  << "\"}";
    }
    std::cout << "]\n";

    std::cout << "Data: [";
    for (size_t i = 0; i < node.data.size(); i++)
    {
        if (i != 0)
        {
            std::cout << ", ";
        }
        const auto point = node.data[i];
        std::cout << "{" << point.x << ", " << point.y << "}";
    }
    std::cout << "]\n";

    std::cout << "Metadata: [";
    bool firstMeta = true;
    for (auto const& [key, val] : node.metadata)
    {
        if (!firstMeta)
        {
            std::cout << ", ";
        }
        else
        {
            firstMeta = false;
        }
        std::cout << "{\"" << key << "\", \"" << val << "\"}";
    }
    std::cout << "]\n";

    std::cout << "Table: {" << "columnNames: [";
    for (size_t i = 0; i < node.table.columnNames.size(); i++)
    {
        if (i != 0)
        {
            std::cout << ", ";
        }
        const auto columnName = node.table.columnNames[i];
        std::cout << "{\"" << columnName.first << "\", \"" << columnName.second
                  << "\"}";
    }
    std::cout << "], rows: [";
    bool firstRow = true;
    for (auto&& row : node.table.rows)
    {
        if (!firstRow)
        {
            std::cout << ", ";
        }
        else
        {
            firstRow = false;
        }
        std::cout << "{";
        bool firstCol = true;
        for (auto const& [col, val] : row)
        {
            if (!firstCol)
            {
                std::cout << ", ";
            }
            else
            {
                firstCol = false;
            }
            std::cout << "{\"" << col << "\", \"" << val << "\"}";
        }
        std::cout << "}";
    }
    std::cout << "]}\n";

    std::cout << "Child node names: [";
    for (size_t i = 0; i < node.childNodeNames.size(); i++)
    {
        if (i != 0)
        {
            std::cout << ", ";
        }
        const auto childNodeName = node.childNodeNames[i];
        std::cout << "\"" << childNodeName << "\"";
    }
    std::cout << "]\n";
}

void printNodes(Converter& reader, const std::string& nodePath)
{
    auto node = reader.read(nodePath);
    printNode(nodePath, node);

    for (size_t i = 0; i < node.childNodeNames.size(); i++)
    {
        auto subNodePath = nodePath;
        if (nodePath == "/")
        {
            subNodePath += std::to_string(i);
        }
        else
        {
            subNodePath += "/" + std::to_string(i);
        }

        std::cout << '\n';
        printNodes(reader, subNodePath);
    }
}

void read(const char* path)
{
    JdxScanner scanner{};

    const bool isRecognized = scanner.isRecognized(path);
    std::cout << "isRecognized(" << path << "): " << isRecognized << "\n\n";
    if (!isRecognized)
    {
        return;
    }

    const auto reader = scanner.getConverter(path);
    printNodes(*reader, "/");
    std::cout << '\n';
}

int main()
{
    std::cout << "C++ example app started\n";

    // The paths below assume that the app is run from a "build" subdirectory.

    const char* simpleFilePath = "../../resources/SimpleFile.jdx";
    read(simpleFilePath);

    const char* compoundFilePath = "../../resources/CompoundFile.jdx";
    read(compoundFilePath);
}
