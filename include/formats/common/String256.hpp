#pragma once
#include "common.hpp"

namespace Shaiya
{
        struct String256
        {
                string value;

                void Read(BinaryReader &reader)
                {
                        value = reader.Read<string>(256);
                }

                void Write(BinaryWriter &writer)
                {
                        writer.Write<string>(value, 256);
                }
        };
}