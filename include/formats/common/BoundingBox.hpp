#pragma once
#include "common.hpp"

namespace Shaiya
{
        struct BoundingBox
        {
                Vector3 lowerLimit, upperLimit;

                double Radius() const 
                {
                        float x = upperLimit.x - lowerLimit.x;
                        float y = upperLimit.y - lowerLimit.y;
                        float z = upperLimit.z - lowerLimit.z;
                        return std::sqrt(x * x + y * y + z * z) / 2.0f;
                }

                void Read(BinaryReader &reader)
                {
                        lowerLimit = reader.Read<Vector3>();
                        upperLimit = reader.Read<Vector3>();
                }
        };
}
