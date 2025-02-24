#pragma once

#include <unordered_map>

#include "common.hpp"

using namespace std;

namespace Shaiya {

        struct SFile {
                string          name;
                int64_t         offset;
                int32_t         length;
                int32_t         version;

                void Read(BinaryReader &reader)
                {
                        name = reader.Read<string>(reader.Read<int32_t>());
                        cout << "fileName: " << name << endl;
                        offset = reader.Read<int64_t>();
                        length = reader.Read<int32_t>();
                        version = reader.Read<int32_t>();
                }

                void Write(BinaryWriter &writer)
                {
                        writer.Write<int32_t>(name.size() + 1);
                        writer.Write<string>(name, name.size());
                        writer.Write<int64_t>(offset);
                        writer.Write<int32_t>(length);
                        writer.Write<int32_t>(version);
                }
        };

        struct SDirectory {
                string                                  name;
                int32_t                                 fileCount;
                vector<SFile>                           files;
                int32_t                                 directoryCount;
                vector<SDirectory>                      directories;

                static int32_t fileCountXorMask;

                void Read(BinaryReader &reader)
                {

                        static int begin;

                        name = reader.Read<string>(reader.Read<int32_t>());
                        cout << "dirName: " << name << " [" << begin << "] "<< endl;

                        (void)reader.Read<int32_t>();
                        begin += 1;
                        if (begin == 1) {
                                fileCount = 17;
                        } else {
                                fileCount = 99999999;//reader.Read<int32_t>();
                        }
                        cout << "fileCount: " << fileCount << endl;

                        files.reserve(fileCount);
                        for (int32_t i = 0; i < fileCount; i += 1)
                        {
                                
                                auto offset = reader.GetOffset();
                                auto str_size = reader.Read<int32_t>();
                                if (!str_size)
                                {
                                        reader.SetOffset(offset);
                                        cout << "found null string!!" << endl;
                                        break ;
                                }
                                auto str = reader.Read<string>(str_size);
                                if (str.find('.') == string::npos)
                                {
                                        reader.SetOffset(offset);
                                        cout << "found folder!!" << "[" << str << "]" << endl;
                                        break;
                                }
                                reader.SetOffset(offset);
                                files.push_back(reader.Read<SFile>());
                        }

                        fileCount = files.size();
                        directoryCount = reader.Read<int32_t>();

                        cout << "directoryCount: " << directoryCount << " for folder : " << name <<   endl;


                        directories.reserve(directoryCount);
                        for (int32_t i = 0; i < directoryCount; i += 1)
                        {
                                directories.push_back(reader.Read<SDirectory>());
                        }
                }

                void Write(BinaryWriter &writer)
                {
                        writer.Write<int32_t>(name.size() + 1);
                        writer.Write<string>(name, name.size());
                        writer.Write<int32_t>(fileCount);
                        writer.Write<vector<SFile>>(files, fileCount);
                        writer.Write<int32_t>(directoryCount);
                        writer.Write<vector<SDirectory>>(directories, directoryCount);
                }
        };

        int32_t SDirectory::fileCountXorMask = 0;

        struct Sah {
                string                                  signature;
                int32_t                                 version;
                int32_t                                 fileCount;

                SDirectory                              rootDirectory;

                string extension = "sah";


                Sah(string filePath)
                {
                        BinaryReader reader(filePath, extension);
                        this->Read(reader);
                }

                void Read(BinaryReader &reader)
                {
                    signature = reader.Read<string>(3);
                    std::cout << signature << endl;

                    version = reader.Read<int32_t>();
                    fileCount = reader.Read<int32_t>();
            
                    // Index where data starts (after header - skip padding bytes)
                    reader.Skip(40);

                    rootDirectory = reader.Read<SDirectory>();
                }

                void Write(string filePath) 
                {
                        BinaryWriter writer(filePath, extension);  
                        
                        writer.Write<string>("SAH", 3, false);
                        writer.Write<int32_t>(version);
                        writer.Write<int32_t>(fileCount);
                        writer.Skip(40);
                        writer.Write<SDirectory>(rootDirectory);
                }
        };

}