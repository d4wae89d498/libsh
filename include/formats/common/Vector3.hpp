#pragma once
#include "common.hpp"

namespace Shaiya
{
        struct Vector3
        {
                float x, y, z;

                double Length() const 
                {
                        return std::sqrt(x * x + y * y + z * z);
                }

                Vector3 operator+(const Vector3& vec2) const 
                {
                        return Vector3(x + vec2.x, y + vec2.y, z + vec2.z);
                }

                Vector3 operator-(const Vector3& vec2) const 
                {
                        return Vector3(x - vec2.x, y - vec2.y, z - vec2.z);
                }

                void Read(BinaryReader &reader)
                {
                        x = reader.Read<float>();
                        y = reader.Read<float>();
                        z = reader.Read<float>();
                }
        };
}
