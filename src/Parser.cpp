//
// Created by wrzos on 03.04.2023.
//

#include <iostream>
#include <fstream>
#include "../include/Parser.h"
#include "istreamwrapper.h"
#include "../include/Engine.h"


Parser::Parser(std::string savePath) {
    spdlog::warn("parser constructor");

    this->savePath = savePath;
}

Parser::Parser() {
    spdlog::warn("parser constructor");

}

void Parser::SaveJSON(rapidjson::Document d, std::string fileName) {
    std::stringstream fullPath;
    fullPath << savePath << "/" << fileName << ".json";
    //
    std::string temp = fullPath.str();
    const char* cstr = temp.c_str();
    FILE* fp = fopen(cstr, "wb");

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    d.Accept(writer);

    fclose(fp);
    std::cout << "\nsaved " << fullPath.str();
}

DirectionalLight Parser::CreateFromJSONDir(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "dirLight") {
        glm::vec3 tempAmbient(d["ambientX"].GetFloat(), d["ambientY"].GetFloat(), d["ambientZ"].GetFloat());
        glm::vec3 tempDiffuse(d["diffuseX"].GetFloat(), d["diffuseY"].GetFloat(), d["diffuseZ"].GetFloat());
        glm::vec3 tempSpecular(d["specularX"].GetFloat(), d["specularY"].GetFloat(), d["specularZ"].GetFloat());
        glm::vec3 tempDirection(d["directionX"].GetFloat(), d["directionY"].GetFloat(), d["directionZ"].GetFloat());
        return {tempAmbient, tempDiffuse, tempSpecular, tempDirection};
    }
    std::cerr << "No type matches!\n";
    return {};
}

PointLight Parser::CreateFromJSONPoint(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "pointLight") {
        glm::vec3 tempAmbient(d["ambientX"].GetFloat(), d["ambientY"].GetFloat(), d["ambientZ"].GetFloat());
        glm::vec3 tempDiffuse(d["diffuseX"].GetFloat(), d["diffuseY"].GetFloat(), d["diffuseZ"].GetFloat());
        glm::vec3 tempSpecular(d["specularX"].GetFloat(), d["specularY"].GetFloat(), d["specularZ"].GetFloat());
        glm::vec3 tempPosition(d["positionX"].GetFloat(), d["positionY"].GetFloat(), d["positionZ"].GetFloat());
        float tempConstant = d["constant"].GetFloat();
        float tempLinear = d["linear"].GetFloat();
        float tempQuadratic = d["quadratic"].GetFloat();
        return {tempAmbient, tempDiffuse, tempSpecular, tempPosition, tempConstant, tempLinear, tempQuadratic};
    }
    std::cerr << "No type matches!\n";
    return {};
}

SpotLight Parser::CreateFromJSONSpot(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "spotLight") {
        glm::vec3 tempAmbient(d["ambientX"].GetFloat(), d["ambientY"].GetFloat(), d["ambientZ"].GetFloat());
        glm::vec3 tempDiffuse(d["diffuseX"].GetFloat(), d["diffuseY"].GetFloat(), d["diffuseZ"].GetFloat());
        glm::vec3 tempSpecular(d["specularX"].GetFloat(), d["specularY"].GetFloat(), d["specularZ"].GetFloat());
        glm::vec3 tempPosition(d["positionX"].GetFloat(), d["positionY"].GetFloat(), d["positionZ"].GetFloat());
        float tempConstant = d["constant"].GetFloat();
        float tempLinear = d["linear"].GetFloat();
        float tempQuadratic = d["quadratic"].GetFloat();
        glm::vec3 tempDirection(d["directionX"].GetFloat(), d["directionY"].GetFloat(), d["directionZ"].GetFloat());
        float tempCutOff = d["cutOff"].GetFloat();
        float tempOuterCutOff = d["outerCutOff"].GetFloat();
        return {tempAmbient, tempDiffuse, tempSpecular, tempPosition, tempConstant, tempLinear, tempQuadratic, tempDirection,tempCutOff, tempOuterCutOff};
    }
    std::cerr << "No type matches!\n";
    return {};
}

Object3D Parser::CreateFromJSONObj3D(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "object3D") {
        glm::vec3 tempScale(d["scaleX"].GetFloat(), d["scaleY"].GetFloat(), d["scaleZ"].GetFloat());
        glm::vec3 tempRotation(d["rotationX"].GetFloat(), d["rotationY"].GetFloat(), d["rotationZ"].GetFloat());
        glm::vec3 tempPosition(d["positionX"].GetFloat(), d["positionY"].GetFloat(), d["positionZ"].GetFloat());
        std::string tempModel = d["model"].GetString();

        Object3D temp;
        temp.loadModel(tempModel);
        temp._transform._scale = tempScale;
        temp._transform._rotation = tempRotation;
        temp._transform._position = tempPosition;
        return temp;
    }
    std::cerr << "No type matches!\n";
    return {};
}

MovingObject Parser::CreateFromJSONMovingObject(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "object3D") {
        glm::vec3 tempScale(d["scaleX"].GetFloat(), d["scaleY"].GetFloat(), d["scaleZ"].GetFloat());
        glm::vec3 tempRotation(d["rotationX"].GetFloat(), d["rotationY"].GetFloat(), d["rotationZ"].GetFloat());
        glm::vec3 tempPosition(d["positionX"].GetFloat(), d["positionY"].GetFloat(), d["positionZ"].GetFloat());
        glm::vec3 tempVelocity(d["velocityX"].GetFloat(), d["velocityY"].GetFloat(), d["velocityZ"].GetFloat());
        std::string tempModel = d["model"].GetString();

        MovingObject temp;
//        temp._model = tempModel;
        temp.loadModel(tempModel);
        temp._transform._scale = tempScale;
        temp._transform._rotation = tempRotation;
        temp._transform._position = tempPosition;
        temp._velocity = tempVelocity;
        return temp;
    }
    std::cerr << "No type matches!\n";
    return {};
}

Walls Parser::CreateFromJSONWalls(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "object3D") {
        glm::vec3 tempScale(d["scaleX"].GetFloat(), d["scaleY"].GetFloat(), d["scaleZ"].GetFloat());
        glm::vec3 tempRotation(d["rotationX"].GetFloat(), d["rotationY"].GetFloat(), d["rotationZ"].GetFloat());
        glm::vec3 tempPosition(d["positionX"].GetFloat(), d["positionY"].GetFloat(), d["positionZ"].GetFloat());
        std::string tempModel = d["model"].GetString();

        Walls temp;
//        temp._model = tempModel;
        temp.loadModel(tempModel);
        temp._transform._scale = tempScale;
        temp._transform._rotation = tempRotation;
        temp._transform._position = tempPosition;
        return temp;
    }
    std::cerr << "No type matches!\n";
    return {};
}

Camera Parser::CreateFromJSONCam(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "camera") {
        glm::vec3 tempPos(d["posX"].GetFloat(), d["posY"].GetFloat(), d["posZ"].GetFloat());
        float tempRotX = d["rotX"].GetFloat();
        float tempRotY = d["rotY"].GetFloat();
        float tempZoom = d["zoom"].GetFloat();
        return {tempPos, tempRotX, tempRotY, tempZoom};
    }
    std::cerr << "No type matches!\n";
    return {};
}


rapidjson::Document Parser::openJSON(std::string fileName) {
    std::stringstream fullPath;
    fullPath << savePath << "/" << fileName << ".json";
    //
    std::string temp = fullPath.str();
    const char* cstr = temp.c_str();
    std::ifstream ifs { cstr };
    if ( !ifs.is_open() )
    {
        std::cerr << "Could not open file for reading!\n";
        //return EXIT_FAILURE;
    }

    rapidjson::IStreamWrapper isw { ifs };

    rapidjson::Document doc {};
    doc.ParseStream( isw );

    rapidjson::StringBuffer buffer {};
    rapidjson::Writer<rapidjson::StringBuffer> writer { buffer };
    doc.Accept( writer );

    if ( doc.HasParseError() )
    {
        std::cout << "\n\nError  : " << doc.GetParseError()  << '\n'
                  << "Offset : " << doc.GetErrorOffset() << '\n';
        //return EXIT_FAILURE;
    }

    //const std::string jsonStr { buffer.GetString() };

    ///std::cout << '\n\n' << jsonStr ;
    return doc;
}





