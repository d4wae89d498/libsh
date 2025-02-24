#pragma once
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <memory>
#include <cstring>
#include <iostream>
#include <cxxabi.h>
#include <math.h>

using namespace std;

template <typename T>
std::string type_to_string()
{
    int status;
    char* demangledName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    
    if (status == 0) {
        std::string result(demangledName);
        free(demangledName);  // Don't forget to free the allocated memory
        return result;
    } else {
        return "Error demangling type";
    }
}

namespace Shaiya 
{
    struct BinaryReader
    {
        vector<unsigned char>       buffer;
        unsigned long long          offset;

        BinaryReader() : offset(0) 
        {}

        BinaryReader(const string& filePath, const string &extension)
        {
            size_t pos = filePath.find_last_of('.');
            if (pos == std::string::npos || filePath.substr(pos + 1) != extension) {
                throw std::invalid_argument("File extension must be '" + extension +"'");
            }

            ifstream file(filePath, ios::binary | ios::ate);

            if (!file.is_open())
                    throw runtime_error("Failed to open file " + filePath);
                
            streamsize size = file.tellg();
            file.seekg(0, ios::beg);
            buffer.resize(size);
            if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
                    throw runtime_error("Failed to read file contents");
            }
            offset = 0;
            file.close();
        }

        void SetOffset(unsigned long long newOffset)
        {
            offset = newOffset;
        }

        unsigned long long GetOffset() const
        {
            return offset;
        }

        void ResetOffset()
        {
            offset = 0;
        }

        void Skip(size_t count)
        {
            if (offset + count > buffer.size())
                throw out_of_range("Buffer out of range");
            offset += count;
        }    

        vector<unsigned char> ReadAllBytes() const
        {
            return vector<unsigned char>(buffer.begin() + offset, buffer.end());
        }    
        
        void SetBuffer(vector<unsigned char>& _buffer)
        {
            buffer = _buffer;
            ResetOffset();
        }

        void SetBuffer(vector<unsigned char>&& _buffer)
        {
            buffer = std::move(_buffer);
            ResetOffset();
        }    

        const vector<unsigned char>& GetBuffer() const 
        {
            return buffer;
        }

        void Clear()
        {
            buffer.clear();
            offset = 0;
        }

        template <typename T>
        T Read()
        {
            if constexpr (std::is_same_v<T, string> || std::is_same_v<T, std::basic_string<char>>) 
            {
                T output;
                char c;
                while ((c = this->Read<char>()))
                    output += c;
                cout << "---- [" << output << "]" << endl;
                return output;
            }
            else if constexpr (std::is_class_v<T>)
            {
                T result;
                result.Read(*this);
                return result;
            } 
            else if constexpr (std::is_fundamental_v<T>)
            {
                if (offset + sizeof(T) > buffer.size())
                    throw std::out_of_range("Buffer out of range");
                T result;
                std::memcpy(&result, buffer.data() + offset, sizeof(T));
                offset += sizeof(T);
                return result;
            } 
            else if constexpr (std::is_same_v<T, byte>) 
            {
                return byte(this->Read<unsigned char>());
            }
            else 
            {
                static_assert(sizeof(T) == 0, "Unsupported type");
            }
        }

        template <typename T>
        struct is_vector : std::false_type 
        {};

        template <typename T, typename A>
        struct is_vector<std::vector<T, A>> : std::true_type 
        {};

        template <typename T>
        T Read(int count) 
        {
            if constexpr (is_vector<T>::value) 
            {
                // Handle reading a vector
                T result;
                result.reserve(count);
                for (int i = 0; i < count; ++i) 
                {
                    result.push_back(this->Read<typename T::value_type>()); // Recursively read each element
                }
                return result;
            } 
            else if constexpr (std::is_same_v<T, std::string>) 
            {
                // Handle reading a string
                if (count < 0)
                    throw std::runtime_error("ReadString with negative integer");
                if (offset + count > buffer.size())
                    throw std::runtime_error("Buffer out of range (provided length greater than buffer size)");

                T output("");

                while (count > 0)
                {
                    char c = this->Read<char>();
                    count -= 1;
                    if (!c)
                        break;
                    output += c;
                }

                while (count > 0)
                {
                    count -= 1;
                    this->Read<char>();
                }
                return output;
              // std::string result(buffer.begin() + offset, buffer.begin() + offset + count);
              // offset += count;
              // return result;
            } 
            else
            {
                static_assert(std::is_arithmetic_v<T> || is_vector<T>::value || std::is_same_v<T, std::string>,
                    "Unsupported type: Only vectors, and strings are supported.");
            }
        };
    };

    struct BinaryWriter
    {

        std::ofstream file;

        BinaryWriter() 
        {}

        BinaryWriter(const std::string& filePath, const std::string& expectedExtension) : file(filePath, std::ios::binary | std::ios::out)
        {
            if (!file) {
                throw std::runtime_error("Error: Could not open file for writing.");
            }

            size_t pos = filePath.find_last_of('.');
            if (pos == std::string::npos || filePath.substr(pos + 1) != expectedExtension) {
                file.close(); // Close the file before throwing an exception
                throw std::invalid_argument("File extension must be '" + expectedExtension + "'");
            }
        }

        void SetOffset(unsigned long long newOffset)
        {
            if (!file.is_open()) {
                throw std::runtime_error("Error: File is not open.");
            }
            file.seekp(newOffset);
        }

        unsigned long long GetOffset()
        {
            if (!file.is_open()) {
                throw std::runtime_error("Error: File is not open.");
            }
            return static_cast<unsigned long long>(file.tellp());
        }

        void ResetOffset()
        {
            if (!file.is_open())
            {
                throw std::runtime_error("Error: File is not open.");
            }
            file.seekp(0);
        }

        void Skip(size_t count)
        {
            if (!file.is_open()) {
                throw std::runtime_error("Error: File is not open.");
            }
            file.seekp(file.tellp() + static_cast<std::streamoff>(count));
        }

        template <typename T>
        void Write(T input)
        {
            if constexpr (!std::is_same_v<T, char> && !std::is_same_v<T, unsigned char>) 
                cout << "Write " << type_to_string<T>() << endl;
            if constexpr (std::is_same_v<T, string> || std::is_same_v<T, std::basic_string<char>>) 
            {
                cout << "WRITE STRING" << endl;
                size_t input_size = input.size();
                for (size_t i = 0; i < input.size(); i += 1)
                {
                    this->Write<char>(input[i]);
                }
                this->Write<char>(0);
            }
            else if constexpr (std::is_class_v<T>)
            {
                input.Write(*this);
            } 
            else if constexpr (std::is_fundamental_v<T>)
            {
                file.write(reinterpret_cast<char*>(&input), sizeof(T));
            } 
            else if constexpr (std::is_same_v<T, byte>) 
            {
                return this->Write<unsigned char>((unsigned char)input);
            }
            else 
            {
                static_assert(sizeof(T) == 0, "Unsupported type");
            }
        }

        template <typename T>
        struct is_vector : std::false_type 
        {};

        template <typename T, typename A>
        struct is_vector<std::vector<T, A>> : std::true_type 
        {};

        template <typename T>
        void Write(T value, int count, bool null_term = true) 
        {
            if constexpr (is_vector<T>::value) 
            {
                for (int i = 0; i < count; ++i) 
                {
                    this->Write(value[i]);
                }
            } 
            else if constexpr (std::is_same_v<T, std::string>) 
            {
                for (int i = 0; i < count; ++i) 
                {
                    this->Write<char>(value[i]);
                }
                if (null_term)
                    this->Write<char>(0);
            } 
            else
            {
                static_assert(std::is_arithmetic_v<T> || is_vector<T>::value || std::is_same_v<T, std::string>,
                    "Unsupported type: Only vectors, and strings are supported.");
            }
        };
    };

};

#include "formats/common/Vector2.hpp"
#include "formats/common/Vector3.hpp"
#include "formats/common/String256.hpp"
#include "formats/common/MeshFace.hpp"
#include "formats/common/BoundingBox.hpp"
#include "formats/common/AlphaBlendingMode.hpp"
#include "formats/common/Color.hpp"

