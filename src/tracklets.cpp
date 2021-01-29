#include "../include/tracklets.h"
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

Tracklets::Tracklets(){
    
}

bool Tracklets::readXml(std::string directory){
    tinyxml2::XMLError eResult = xml_doc.LoadFile(directory.c_str());

    if (eResult != tinyxml2::XML_SUCCESS){
        return false;
    }

    tinyxml2::XMLNode* root = xml_doc.FirstChildElement("boost_serialization");
    if (root == nullptr){
        return false;
    }

    /*************************
	 * Tracklets meta info
	*************************/
    tinyxml2::XMLElement* element = root->FirstChildElement("tracklets");
    if (element == nullptr){
        return false;
    }

    int count = -1;
    element->FirstChildElement("count")->QueryIntText(&count);
    trackletVector.reserve(count);

    /*************************
	 * Items
	*************************/
    for(unsigned int i = 0; i < count; i++){
        if(i == 0){
            element = element->FirstChildElement("item");
        }else{
            element = element->NextSiblingElement("item");
        }

        Tracklet t;

        element->FirstChildElement("w")->QueryFloatText(&t.width);
        element->FirstChildElement("h")->QueryFloatText(&t.height);
        element->FirstChildElement("l")->QueryFloatText(&t.length);
        element->FirstChildElement("first_frame")->QueryIntText(&t.firstFrame);

        t.type = std::string(element->FirstChildElement("objectType")->GetText());
        t.color = glm::vec4(colorForType(t.type), 1.0f);

        /*Find all poses for tracklet*/
        tinyxml2::XMLElement* elementPoses = element->FirstChildElement("poses");
        elementPoses->FirstChildElement("count")->QueryIntText(&t.poses);

        for(unsigned int j = 0; j < t.poses; j++){
            if(j == 0){
                elementPoses = elementPoses->FirstChildElement("item");
            }else{
                elementPoses = elementPoses->NextSiblingElement("item");
            }

            glm::vec3 rot, trans;
            elementPoses->FirstChildElement("tx")->QueryFloatText(&trans.x);
            elementPoses->FirstChildElement("ty")->QueryFloatText(&trans.y);
            elementPoses->FirstChildElement("tz")->QueryFloatText(&trans.z);

            elementPoses->FirstChildElement("rx")->QueryFloatText(&rot.x);
            elementPoses->FirstChildElement("ry")->QueryFloatText(&rot.y);
            elementPoses->FirstChildElement("rz")->QueryFloatText(&rot.z);

            t.translations.push_back(trans);
            t.rotations.push_back(rot);
        }

        trackletVector.push_back(t);
    }
    return true;
}

void Tracklets::drawTrackletOptions(unsigned int frame, glm::mat4 proj){
    if (ImGui::CollapsingHeader("Tracklet options")){
        for(unsigned int i = 0; i < trackletVector.size(); i++){
            std::string n = trackletVector[i].type + " " + std::to_string(i);
            if (ImGui::TreeNode(n.c_str())){
                ImGui::Text("Color");
                ImGui::SameLine();
                std::string s = "Tracklet color" + std::to_string(i);
                ImGui::ColorEdit4(s.c_str(), (float*)&trackletVector[i].color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                ImGui::Button(std::string("w: " + std::to_string(trackletVector[i].width)).c_str());

                ImGui::SameLine();
                ImGui::Button(std::string("h: " + std::to_string(trackletVector[i].height)).c_str());

                ImGui::SameLine(); 
                ImGui::Button(std::string("l: " + std::to_string(trackletVector[i].length)).c_str());


                ImGui::Button(std::string("First frame: " + std::to_string(trackletVector[i].firstFrame)).c_str());

                ImGui::SameLine();
                ImGui::Button(std::string("Last frame:: " + std::to_string(trackletVector[i].firstFrame + trackletVector[i].poses - 1)).c_str());

                if(frame >= trackletVector[i].firstFrame && frame < trackletVector[i].firstFrame + trackletVector[i].poses){
                    glm::vec3 vPosition = glm::vec3(-0.5f, 0.f, 0.5f);
                    
                    glm::mat4 uModelMatrix = glm::mat4(1.0f);
                    uModelMatrix = glm::scale(uModelMatrix, glm::vec3(
                                            trackletVector[i].width,
                                            trackletVector[i].length,
                                            trackletVector[i].height));
                    uModelMatrix = glm::translate(uModelMatrix, trackletVector[i].translations[frame - trackletVector[i].firstFrame]);
                    uModelMatrix = glm::rotate(uModelMatrix, trackletVector[i].rotations[frame - trackletVector[i].firstFrame].z, glm::vec3(0.0f, 0.0f, 1.0f));


                    glm::mat4 uProjectionVideo = proj;
                    glm::vec4 vPosW;
                    glm::vec4 posInVideoSpace;

                    /*Shadercode*/
                    vPosW = uModelMatrix * glm::vec4(vPosition, 1.0);
                    posInVideoSpace = uProjectionVideo * vPosW; 

                    posInVideoSpace.x /= posInVideoSpace.z;
                    posInVideoSpace.y /= posInVideoSpace.z;
                    posInVideoSpace.z /= posInVideoSpace.z;
                    
                    posInVideoSpace.z = 0.0;
                    posInVideoSpace.w = 1.0;
                    posInVideoSpace.x = posInVideoSpace.x / 1242.0;
                    posInVideoSpace.y = posInVideoSpace.y / 375.0;
                    posInVideoSpace.y = 1.0-posInVideoSpace.y;
                    posInVideoSpace.x = 2.0 * posInVideoSpace.x - 1.0;
                    posInVideoSpace.y = 2.0 * posInVideoSpace.y - 1.0;

                    /*shader De-Homogenisation*/
                    posInVideoSpace.x = posInVideoSpace.x / posInVideoSpace.w;
                    posInVideoSpace.y = posInVideoSpace.y / posInVideoSpace.w;
                    posInVideoSpace.z = posInVideoSpace.z / posInVideoSpace.w;
                    posInVideoSpace.w = posInVideoSpace.w / posInVideoSpace.w;

                    ImGui::Button( ("Video Space: " + std::to_string(posInVideoSpace.r)).c_str());
                    ImGui::Button( ("Video Space: " + std::to_string(posInVideoSpace.g)).c_str());
                    ImGui::Button( ("Video Space: " + std::to_string(posInVideoSpace.b)).c_str());
                    ImGui::Button( ("Video Space: " + std::to_string(posInVideoSpace.a)).c_str());

                    ImGui::Button( ("tx: " + std::to_string(trackletVector[i].translations[frame - trackletVector[i].firstFrame].x)).c_str());
                    ImGui::SameLine();
                    ImGui::Button( ("ty: " + std::to_string(trackletVector[i].translations[frame - trackletVector[i].firstFrame].y)).c_str());
                    ImGui::SameLine();                    
                    ImGui::Button( ("tz: " + std::to_string(trackletVector[i].translations[frame - trackletVector[i].firstFrame].z)).c_str());

                    ImGui::Button( ("rx: " + std::to_string(trackletVector[i].rotations[frame - trackletVector[i].firstFrame].x)).c_str());
                    ImGui::SameLine();
                    ImGui::Button( ("ry: " + std::to_string(trackletVector[i].rotations[frame - trackletVector[i].firstFrame].y)).c_str());
                    ImGui::SameLine();                    
                    ImGui::Button( ("rz: " + std::to_string(trackletVector[i].rotations[frame - trackletVector[i].firstFrame].z)).c_str());
                }

                ImGui::TreePop();
            }
        }
    }
}

glm::vec3 Tracklets::colorForType(std::string type){
        if(type == "Car"){ return glm::vec3(0.0f, 1.0f, 0.0f); }
        else if(type == "Van"){ return glm::vec3(0.0f, 0.0f, 1.0f);}
        else if(type == "Truck"){ return glm::vec3(0.0f, 1.0f, 1.0f);}
        else if(type == "Pedestrian"){ return glm::vec3(1.0f, 0.6f, 0.0f);}
        else if(type == "Sitter"){ return glm::vec3(1.0f, 1.0f, 1.0f);}
        else if(type == "Cyclist"){ return glm::vec3(1.0f, 0.0f, 0.0f);}
        else if(type == "Tram"){ return glm::vec3(1.0f, 1.0f, 0.0f);}
        else if(type == "Misc"){ return glm::vec3(0.6f);}
        else{ return glm::vec3(1.0f, 0.0f, 1.0f); }   
}