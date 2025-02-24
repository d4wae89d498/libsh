#pragma once
#include "common.hpp"

namespace Shaiya
{
        struct Color
        {
                float red, green, blue;

                void Read(BinaryReader &reader)
                {
                        red = reader.Read<float>();
                        green = reader.Read<float>();
                        blue = reader.Read<float>();
                }

                void Write(BinaryWriter &writer)
                {
                        writer.Write(red);
                        writer.Write(green);
                        writer.Write(blue);
                }
        };
}
