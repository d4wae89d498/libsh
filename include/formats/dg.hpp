#pragma once

#include <cmath>

#include "common.hpp"

namespace Shaiya {
       
    struct DgCollisionMeshVertex
    {
        Vector3 coordinates;
    
        void Read(BinaryReader &reader)
        {
                coordinates = reader.Read<Vector3>();
        }
    };
    
    struct DgMeshVertex
    {
        Vector3 coordinates, normal;
        uint16_t boneId;
        Vector2 textureUV, lightmapUV;

        void Read(BinaryReader &reader)
        {

                coordinates = reader.Read<Vector3>();
                normal = reader.Read<Vector3>();
                boneId = reader.Read<uint16_t>();
                textureUV = reader.Read<Vector2>();
                lightmapUV = reader.Read<Vector2>();
        }
    };
    
    enum struct DgMeshCollisionType
    {
        TRANSPARENT = 0,
        COLLISION = 1
    };


    struct DgCollisionMesh 
    {
        vector<DgCollisionMeshVertex> vertices;

        vector<MeshFace> faces;

        void Read(BinaryReader &reader)
        {
            vertices = reader.Read<vector<DgCollisionMeshVertex>>(reader.Read<int32_t>());
            faces = reader.Read<vector<MeshFace>>(reader.Read<int32_t>());
        }
    };
    

    struct DgMesh
    {
        AlphaBlendingMode alphaBlendingMode;
    
        vector<DgMeshVertex> vertices;
    
        vector<MeshFace> faces;
    
        void Read(BinaryReader &reader)
        {
            alphaBlendingMode = (AlphaBlendingMode)reader.Read<int32_t>();
            vertices = reader.Read<vector<DgMeshVertex>>(reader.Read<int32_t>());
            faces = reader.Read<vector<MeshFace>>(reader.Read<int32_t>());
        }
    };
    

    struct DgObject;

    struct DgNode
    {
        int nodeChildCount = 8;
    
        Vector3 center;
    
        BoundingBox viewBox;
    
        BoundingBox collisionBox;
    
        vector<DgObject> objects;
    
        DgMeshCollisionType collisionType;
    
        DgCollisionMesh collisionMesh;
    
        vector<DgNode> childNodes;
    
        void Read(BinaryReader &reader)
        {
            center = reader.Read<Vector3>();
            viewBox = reader.Read<BoundingBox>();
            collisionBox = reader.Read<BoundingBox>();
    
            objects = reader.Read<vector<DgObject>>(reader.Read<int32_t>());
    
            collisionType = (DgMeshCollisionType)reader.Read<int32_t>();
    
            if (collisionType == DgMeshCollisionType::COLLISION)
            {
                // Read extra node info
                collisionMesh = reader.Read<DgCollisionMesh>();
            }
    
            for (int32_t i = 0; i < nodeChildCount; i++)
            {
                int32_t value = reader.Read<int32_t>();
    
                if (value > 0)
                {
                    childNodes.push_back(reader.Read<DgNode>());
                }
            }
        }
    };
    
    struct DgObject
    {
        int32_t textureIndex;
    
        vector<DgMesh> meshes;
    
        void Read(BinaryReader &reader)
        {
            textureIndex = reader.Read<int32_t>();
            std::cout<<"BFORE"<<endl;
            meshes = reader.Read<vector<DgMesh>>(reader.Read<int32_t>());
            std::cout<<"END"<<endl;
        }
    };



    struct Dg
    {
        BoundingBox boundingBox ;
        vector<String256> textureNames;
        int32_t unknownInt32;
        DgNode rootNode;
        string extension = "dg";

        Dg()
        {}

        Dg(string filePath)
        {
            BinaryReader reader(filePath, extension);
            reader.Read<Dg>();
        }

        void Read(BinaryReader &reader)
        {
            boundingBox = reader.Read<BoundingBox>();
            textureNames = reader.Read<vector<String256>>(reader.Read<int32_t>());
            unknownInt32 = reader.Read<int32_t>();

            auto value = reader.Read<int32_t>();
            std::cout << "VALUE=" << value;
            if (value > 0)
            {
                rootNode = reader.Read<DgNode>();
            }
        }

    };
}