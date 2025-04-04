#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <HgLogger.h>

using namespace io;
using namespace core;
using namespace math;
//TODO: this class needs to be tested... I just copied it over from another project.
//Could be a good one for unit tests
HgError OBJLoader::loadFromFile(const char* filePath, Mesh& mesh){
    std::string line;
    std::ifstream file (filePath);

    PointArray vertices;
    PointArray normals;
    std::vector<unsigned int> UVCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;

    int unhandledLines = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            
            //if it starts with a # it is a comment, and we can ignore the line
            if (line.find("#") == 0)
                continue;
            
            //it is a vertex, add it to the vertex list
            if (line.find("v ") == 0) {
                //remove the v from the beginning
                line = line.substr(2);
                float x = std::stof(line.substr(0, line.find(' ')));
                line = line.substr(line.find(' ') + 1);
                float y = std::stof(line.substr(0, line.find(' ')));
                line = line.substr(line.find(' ') + 1);
                float z = std::stof(line.substr(0, line.find(' ')));
                line = line.substr(line.find(' ') + 1);
                
                vertices.push_back(Point(x, y, z));
                continue;
            }

            //texture coords
            if (line.find("vt ") == 0) {
                line = line.substr(3);
                
                std::size_t pos = line.find(" ");

                unsigned int u = std::stoul(line.substr(0,pos));

                unsigned int v = std::stoul(line.substr(pos + 1));

                UVCoords.push_back(u);
                
                UVCoords.push_back(v);
                continue;
            }

            if (line.find("vn ") == 0) {
                //remove the v from the beginning
                line = line.substr(3);

                float x = std::stof(line.substr(0, line.find(' ')));
                line = line.substr(line.find(' ') + 1);
                float y = std::stof(line.substr(0, line.find(' ')));
                line = line.substr(line.find(' ') + 1);
                float z = std::stof(line.substr(0, line.find(' ')));

                normals.push_back(Point(x, y, z));
                continue;
            }


            if (line.find("f") == 0) {
                //remove the f from the beginning
                line = line.substr(2);
                while(!line.empty()) {
                    std::string face;

                    if(line.find(' ') != std::string::npos){
                        face = line.substr(0, line.find(' '));
                        line = line.substr(line.find(' ') + 1);
                    }else {
                        face = line;
                        line.clear();
                    }

                    Face* index = parseFaceIndex(face);
                    //indexes start at 1, but we need them to start at 0
                    indices.push_back(index->vertexIndex - 1);
                    
                    //if it is 0 it means the file didn't have it
                    if(index->uvIndex > 0)
                        textureIndices.push_back(index->uvIndex - 1);
                    
                    if(index->normalIndex > 0)
                        normalIndices.push_back(index->normalIndex - 1);
                }
                continue;
            }
            unhandledLines++;

        }

        file.close();
    } else {
        HgLogger::logWarning("Can't open file!\nPath:  %s", filePath);
        return HgError::eFailure;
    }
    //TODO: these return error codes, we should probably check to see that they return success 
    //(though, at the moment that is the only option for these functions.)
    mesh.setPoints(vertices, false);
    mesh.setNormals(normals);
    mesh.setUV(UVCoords);
    mesh.setIndices(indices);
    mesh.setTextureIndices(textureIndices);
    mesh.setNormalIndices(normalIndices);
    return HgError::eSuccess;
}

OBJLoader::Face* OBJLoader::parseFaceIndex(const std::string& face) {
    std::size_t index = face.find('/');
    std::size_t index1 = face.rfind('/');
    Face* faceStruct = new Face();

    if (index == std::string::npos) {
        faceStruct->vertexIndex = std::stoul(face);
    } else {
        faceStruct->vertexIndex = std::stoul(face.substr(0, index1));
        if (index == index1) {
            faceStruct->uvIndex = std::stoul(face.substr(index + 1));
        } else {
            faceStruct->uvIndex = std::stoul(face.substr(index + 1, index1 - index - 1));
            faceStruct->normalIndex = std::stoul(face.substr(index1 + 1));
        }
    }
    return faceStruct;
}