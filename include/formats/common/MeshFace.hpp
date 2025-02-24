#pragma once
#include "common.hpp"

namespace Shaiya
{
        struct MeshFace
        {
                uint16_t vertexIndex1;
                uint16_t vertexIndex2;
                uint16_t vertexIndex3;

                void Read(BinaryReader &reader)
                {
                        vertexIndex1 = reader.Read<uint16_t>();
                        vertexIndex2 = reader.Read<uint16_t>();
                        vertexIndex3 = reader.Read<uint16_t>();
                }

                void Write(BinaryWriter &writer)
                {
                        writer.Write(vertexIndex1);
                        writer.Write(vertexIndex1);
                        writer.Write(vertexIndex1);
                }
        };
}
