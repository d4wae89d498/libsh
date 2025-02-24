#pragma once

#include <cmath>
#include <vector>

#include "common.hpp"

namespace Shaiya
{
        enum struct WldFaction
        {
            AllianceOfLight,
            UnionOfFury,
            Any
        };

        enum struct WldType 
        {
                Field,
                Dungeon
        };

        struct WldTerrainLayer
        {
            String256 textureFileName ;
            float layerTileSize ;
            String256 walkSoundFileName;
        
            void Read(BinaryReader &reader)
            {
                textureFileName = reader.Read<String256>();
                layerTileSize = reader.Read<float>();
                walkSoundFileName = reader.Read<String256>();
            }
        };

        struct WldObjectInstance
        {
            int assetIndex;
            Vector3 position;
            Vector3 rotationForward;
            Vector3 rotationUp;
        
            void Read(BinaryReader & reader)
            {
                assetIndex = reader.Read<int32_t>();
                position = reader.Read<Vector3>();
                rotationForward = reader.Read<Vector3>();
                rotationUp = reader.Read<Vector3>();
            }
        };

        struct WldManiCoordinate
        {
            int32_t buildingAssetId;
            int32_t maniAssetIndex;
            Vector3 position;
            Vector3 rotationForward;
            Vector3 rotationUp;
        
            void Read(BinaryReader &reader)
            {
                buildingAssetId = reader.Read<int32_t>();
                maniAssetIndex = reader.Read<int32_t>();
                position = reader.Read<Vector3>();
                rotationForward = reader.Read<Vector3>();
                rotationUp = reader.Read<Vector3>();
            }
        };

        struct WldEffectInstance
        {
            Vector3 position;
            Vector3 rotationForward;
            Vector3 rotationUp;
            int32_t effectId;
        
            void Read(BinaryReader &reader)
            {
                position = reader.Read<Vector3>();
                rotationForward = reader.Read<Vector3>();
                rotationUp = reader.Read<Vector3>();
                effectId = reader.Read<int32_t>();
            }
        };

        struct WldMusicZone
        {
            BoundingBox boundingBox;
            float radius;
            int musicAssetId;
            int unknown;
        
            void Read(BinaryReader & reader)
            {
                boundingBox = reader.Read<BoundingBox>();
                radius = reader.Read<float>();
                musicAssetId = reader.Read<int32_t>();
                unknown = reader.Read<int32_t>();
            }
        };

        struct WldZone
        {
            BoundingBox boundingBox;
            vector<int> identifiers;
        
            void Read(BinaryReader & reader)
            {
                boundingBox = reader.Read<BoundingBox>();
                auto identifierCount = reader.Read<int32_t>();
                for (auto i = 0; i < identifierCount; i++)
                {
                    auto identifier = reader.Read<int32_t>();
                    identifiers.push_back(identifier);
                }
            }
        };

        struct WldSoundEffect
        {
            int SoundEffectAssetId;
            Vector3 Center;
            float Radius;

            void Read(BinaryReader & reader)
            {
                SoundEffectAssetId = reader.Read<int32_t>();
                Center = reader.Read<Vector3>();
                Radius = reader.Read<float>();
            }
        };

        struct WldMonsterRestrictedZone
        {
             BoundingBox boundingBox;
             float radius;
        
             void Read(BinaryReader &reader)
             {
                boundingBox = reader.Read<BoundingBox>();
                radius = reader.Read<float>();
             }
        };

        struct WldPortal
        {
                BoundingBox boundingBox;
                float radius;
                String256 text1;
                String256 text2;
                byte mapId;
                WldFaction faction;
                byte unknown;
                Vector3 destinationPosition;

                void Read(BinaryReader & reader)
                {
                        boundingBox = reader.Read<BoundingBox>();
                        radius = reader.Read<float>();
                        text1 = reader.Read<String256>();
                        text2 = reader.Read<String256>();
                        mapId = reader.Read<byte>();
                        faction = (WldFaction)reader.Read<int16_t>();
                        unknown = reader.Read<byte>();
                        destinationPosition = reader.Read<Vector3>();
                }
        };

        struct WldSpawn
        {
            int unknown1;
            BoundingBox boundingBox;
            float radius;
            WldFaction faction;
            int unknown3;
        
            void Read(BinaryReader & reader)
            {
                unknown1 = reader.Read<int32_t>();
                boundingBox = reader.Read<BoundingBox>();
                radius = reader.Read<float>();
                faction = (WldFaction)reader.Read<int32_t>();
                unknown3 = reader.Read<int32_t>();
            }
        };


        enum struct WldNamedAreaMode
        {
                WorldIndexTxt = 0,
                BmpFile = 2
        };

        struct WldNamedArea
        {
            BoundingBox boundingBox;
            float radius;
            String256 text1;
            String256 text2;
            WldNamedAreaMode mode;
            int unknown;
        
            void Read(BinaryReader &reader)
            {
                boundingBox = reader.Read<BoundingBox>();
                radius = reader.Read<float>();
                text1 = reader.Read<String256>();
                text2 = reader.Read<String256>();
                mode = (WldNamedAreaMode)reader.Read<int32_t>();
                unknown = reader.Read<int32_t>();
            }
        };

        struct WldNpc
        {
            int type;
            int typeId;
            Vector3 position;
            float orientation;
            vector<Vector3> patrolPositions;
        
            void Read(BinaryReader &reader)
            {
                type = reader.Read<int32_t>();
                typeId = reader.Read<int32_t>();
                position = reader.Read<Vector3>();
                orientation = reader.Read<float>();
                patrolPositions = reader.Read<vector<Vector3>>(reader.Read<int32_t>());
            }
        };

        struct Wld
        {
                WldType wldType = WldType::Field;
                uint32_t mapSize;
                vector<unsigned char> terrainHeightMap;
                vector<unsigned char> terrainTextureMap;
                vector<WldTerrainLayer> terrainLayers;
                String256 layoutName;
                vector<String256> buildingAssets;
                vector<WldObjectInstance> buildingInstances;
                vector<String256> shapeAssets;
                vector<WldObjectInstance> shapeInstances;
                vector<String256> treeAssets;
                vector<WldObjectInstance> treeInstances;
                vector<String256> grassAssets;
                vector<WldObjectInstance> grassInstances;
                vector<String256> primaryVaniAssets;
                vector<WldObjectInstance> primaryVaniInstances;
                vector<String256> secondaryVaniAssets;
                vector<WldObjectInstance> secondaryVaniInstances;
                vector<String256> dungeonAssets;
                vector<WldObjectInstance> dungeonInstances;
                vector<String256> maniAssets;
                vector<WldManiCoordinate> maniInstances;
                String256 effectFileName;
                vector<WldEffectInstance> effectInstances;
                int32_t unknown1;
                int32_t unknown2;
                int32_t unknown3;
                vector<String256> objectAssets;
                vector<WldObjectInstance> objectInstances;
                vector<String256> musicAssets;
                vector<WldMusicZone> musicZoneInstances;
                vector<String256> soundEffectAssets;
                vector<WldZone> zones;
                vector<WldSoundEffect> soundEffectInstances;
                vector<WldMonsterRestrictedZone> wldMonsterRestrictedZones;
                vector<WldPortal> portalInstances;
                vector<WldSpawn> spawnInstances;
                vector<WldNamedArea> namedAreaInstances;
                vector<WldNpc> npcInstances;
                String256 skyFileName;
                String256 primaryCloudFileName;
                String256 secondaryCloudFileName;
                Color unusedColor1;
                Color unusedColor2;
                Color fogColor;
                float fogStartDistance;
                float fogEndDistance;
                string extension = "wld";

                Wld()
                {}

                Wld(string filePath)
                {
                        BinaryReader reader(filePath, extension);
                        this->Read(reader);
                }

                void Read(BinaryReader &reader)
                {
                        string worldType = reader.Read<string>(4);
                        std::cout << worldType << std::endl;
                        if (worldType == "DUN")
                                wldType = WldType::Dungeon;
                        else // else if FLD...
                        {
                                wldType = WldType::Field;
                        }

                        if (wldType == WldType::Field)
                        {
                                mapSize = reader.Read<uint32_t>();
                                auto mappingSize = (int)pow(mapSize / 2. + 1, 2);
                                terrainHeightMap = reader.Read<vector<unsigned char>>(mappingSize * 2);
                                terrainTextureMap = reader.Read<vector<unsigned char>>(mappingSize);

                                std::cout << "fetchin terrainLayers" << endl;

                                terrainLayers = reader.Read<vector<WldTerrainLayer>>(reader.Read<int32_t>());
                             
                                std::cout << "DONE fetchin terrainLayers" << endl;

                                if (!terrainLayers.size()) {
                                        std::cout << terrainLayers.size() << " layers." << endl;
                                        throw runtime_error("err");
                                } else {
                                        std::cout << terrainLayers.size() << " layers." << endl;
                                }
                        }

                        layoutName = reader.Read<String256>();
                        ReadNamesAndCoordinates(reader, buildingAssets, buildingInstances);
                        ReadNamesAndCoordinates(reader, shapeAssets, shapeInstances);
                        ReadNamesAndCoordinates(reader, treeAssets, treeInstances);
                        ReadNamesAndCoordinates(reader, grassAssets, grassInstances);
                        ReadNamesAndCoordinates(reader, primaryVaniAssets, primaryVaniInstances);
                        ReadNamesAndCoordinates(reader, secondaryVaniAssets, secondaryVaniInstances);
                        ReadNamesAndCoordinates(reader, dungeonAssets, dungeonInstances);
                        ReadNames(reader, maniAssets);
                        maniInstances = reader.Read<vector<WldManiCoordinate>>(reader.Read<int32_t>());
                        effectFileName = reader.Read<String256>();
                        effectInstances = reader.Read<vector<WldEffectInstance>>(reader.Read<int32_t>());
                        unknown1 = reader.Read<int32_t>();
                        unknown2 = reader.Read<int32_t>();
                        unknown3 = reader.Read<int32_t>();
                        ReadNamesAndCoordinates(reader, objectAssets, objectInstances);
                        ReadNames(reader, musicAssets);
                        musicZoneInstances = reader.Read<vector<WldMusicZone>>(reader.Read<int32_t>());
                        ReadNames(reader, soundEffectAssets);
                        zones = reader.Read<vector<WldZone>>(reader.Read<int32_t>());
                        soundEffectInstances = reader.Read<vector<WldSoundEffect>>(reader.Read<int32_t>());
                        wldMonsterRestrictedZones = reader.Read<vector<WldMonsterRestrictedZone>>(reader.Read<int32_t>());
                        portalInstances = reader.Read<vector<WldPortal>>(reader.Read<int32_t>());
                        spawnInstances = reader.Read<vector<WldSpawn>>(reader.Read<int32_t>());
                        namedAreaInstances = reader.Read<vector<WldNamedArea>>(reader.Read<int32_t>());
                        // NOTE: npcCount is the real npc count + the patrol coordinates count
                        int32_t npcCount = reader.Read<int32_t>();
                        while (npcCount > 0)
                        {
                                auto npc = reader.Read<WldNpc>();
                                npcInstances.push_back(npc);
                                npcCount -= npc.patrolPositions.size();
                                npcCount--;
                        }
                        if (wldType == WldType::Field)
                        {
                                skyFileName = reader.Read<String256>();
                                primaryCloudFileName = reader.Read<String256>();
                                secondaryCloudFileName = reader.Read<String256>();
                        }
                        unusedColor1 = reader.Read<Color>();
                        unusedColor2 = reader.Read<Color>();
                        fogColor = reader.Read<Color>();
                        fogStartDistance = reader.Read<float>();
                        fogEndDistance = reader.Read<float>();
                }

                void ReadNames(BinaryReader &reader, vector<String256> &namesList)
                {
                        auto namesCount = reader.Read<int32_t>();
                        for (auto i = 0; i < namesCount; i++)
                        {
                                auto str256 = reader.Read<String256>();
                                namesList.push_back(str256);
                        }
                }

                 void ReadNamesAndCoordinates(BinaryReader &reader, vector<String256> &namesList,
                        vector<WldObjectInstance> &coordinatesList)
                {
                        ReadNames(reader, namesList);
                        auto coordinatesCount = reader.Read<int32_t>();
                        for (auto i = 0; i < coordinatesCount; i++)
                        {
                                auto coordinate = reader.Read<WldObjectInstance>();
                                coordinatesList.push_back(coordinate);
                        }
                }
        };
    }