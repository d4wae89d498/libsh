#pragma once
#include "common.hpp"

namespace Shaiya
{
        struct Vector2 
        {
                float x, y;

                double Length() const 
                {
                        return std::sqrt(x * x + y * y);
                }

                Vector2 operator+(const Vector2& vec2) const 
                {
                        return Vector2(x + vec2.x, y + vec2.y);
                }

                Vector2 operator-(const Vector2& vec2) const 
                {
                        return Vector2(x - vec2.x, y - vec2.y);
                }

                void Read(BinaryReader &reader)
                {
                        x = reader.Read<float>();
                        y = reader.Read<float>();
                }
        };
}