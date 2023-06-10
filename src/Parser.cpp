//
// Created by wrzos on 03.04.2023.
//

#include <iostream>
#include <fstream>
#include "../include/Parser.h"
#include "istreamwrapper.h"
#include "../include/Engine.h"
#include "prettywriter.h"


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

    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
    d.Accept(writer);

    fclose(fp);
    spdlog::info("saved " + fullPath.str());
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

void Parser::ReadEffectColor( aiColor4D& pColor, Sampler& pSampler)
{
//    // Save current element name
//    const std::string curElem = mReader->getNodeName();
//
//    while( mReader->read())
//    {
//        if( mReader->getNodeType() == irr::io::EXN_ELEMENT) {
//            if( IsElement( "color"))
//            {
//                // text content contains 4 floats
//                const char* content = GetTextContent();
//
//                content = fast_atoreal_move<float>( content, (float&)pColor.r);
//                SkipSpacesAndLineEnd( &content);
//
//                content = fast_atoreal_move<float>( content, (float&)pColor.g);
//                SkipSpacesAndLineEnd( &content);
//
//                content = fast_atoreal_move<float>( content, (float&)pColor.b);
//                SkipSpacesAndLineEnd( &content);
//
//                content = fast_atoreal_move<float>( content, (float&)pColor.a);
//                SkipSpacesAndLineEnd( &content);
//                TestClosing( "color");
//            }
//            else if( IsElement( "texture"))
//            {
//                // get name of source textur/sampler
//                int attrTex = GetAttribute( "texture");
//                pSampler.mName = mReader->getAttributeValue( attrTex);
//
//                // get name of UV source channel
//                attrTex = GetAttribute( "texcoord");
//                pSampler.mUVChannel = mReader->getAttributeValue( attrTex);
//                //SkipElement();
//            }
//            else if( IsElement( "technique"))
//            {
//                const int _profile = GetAttribute( "profile");
//                const char* profile = mReader->getAttributeValue( _profile );
//
//                // Some extensions are quite useful ... ReadSamplerProperties processes
//                // several extensions in MAYA, OKINO and MAX3D profiles.
//                if (!::strcmp(profile,"MAYA") || !::strcmp(profile,"MAX3D") || !::strcmp(profile,"OKINO"))
//                {
//                    // get more information on this sampler
//                    ReadSamplerProperties(pSampler);
//                }
//                else SkipElement();
//            }
//            else if( !IsElement( "extra"))
//            {
//                // ignore the rest
//                SkipElement();
//            }
//        }
//        else if( mReader->getNodeType() == irr::io::EXN_ELEMENT_END){
//            if (mReader->getNodeName() == curElem)
//                break;
//        }
//    }
}


MovingObject Parser::CreateFromJSONMovingObject(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

//    if(d["newparam"].GetString() == "newparam")	{
//        // save ID
//        int attrSID = GetAttribute( "sid");
//        std::string sid = mReader->getAttributeValue( attrSID);
//        pEffect.mParams[sid] = EffectParam();
//        ReadEffectParam( pEffect.mParams[sid]);
//    }
//    else if( d["technique"].GetString() == "technique" || d["extra"].GetString() == "extra")
//    {
//        // just syntactic sugar
//    }

//    if( d["phong"].GetString() == "phong")
//        pEffect.mShadeType = Shade_Phong;
//    else if( d["constant"].GetString() == "constant")
//        pEffect.mShadeType = Shade_Constant;
//    else if( d["lambert"].GetString() == "lambert")
//        pEffect.mShadeType = Shade_Lambert;
//    else if( d["blinn"].GetString() == "blinn")
//        pEffect.mShadeType = Shade_Blinn;

//    else if( d["emission"].GetString() == "emission")
//        ReadEffectColor( pEffect.mEmissive, pEffect.mTexEmissive);
//    else if( IsElement( "ambient"))
//        ReadEffectColor( pEffect.mAmbient, pEffect.mTexAmbient);
//    else if( IsElement( "diffuse"))
//        ReadEffectColor( pEffect.mDiffuse, pEffect.mTexDiffuse);
//    else if( IsElement( "specular"))
//        ReadEffectColor( pEffect.mSpecular, pEffect.mTexSpecular);
//    else if( IsElement( "reflective")) {
//        ReadEffectColor( pEffect.mReflective, pEffect.mTexReflective);
//    }
//    else if( IsElement( "transparent")) {
//        ReadEffectColor( pEffect.mTransparent,pEffect.mTexTransparent);
//    }
//    else if( IsElement( "shininess"))
//        ReadEffectFloat( pEffect.mShininess);
//    else if( IsElement( "reflectivity"))
//        ReadEffectFloat( pEffect.mReflectivity);
//
//    else if( IsElement( "transparency"))
//        ReadEffectFloat( pEffect.mTransparency);
//    else if( IsElement( "index_of_refraction"))
//        ReadEffectFloat( pEffect.mRefractIndex);

        // FCOLLADA extensions
        // -------------------------------------------------------
//    if( d["bump"].GetString() == "bump") {
//        aiColor4D dummy;
//        ReadEffectColor( dummy,pEffect.mTexBump);
//    }

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

Level Parser::CreateFromJSONWalls(std::string fileName) {
    rapidjson::Document d = Parser::openJSON(fileName);
    std::string type = d["type"].GetString();

    if(type == "object3D") {
        glm::vec3 tempScale(d["scaleX"].GetFloat(), d["scaleY"].GetFloat(), d["scaleZ"].GetFloat());
        glm::vec3 tempRotation(d["rotationX"].GetFloat(), d["rotationY"].GetFloat(), d["rotationZ"].GetFloat());
        glm::vec3 tempPosition(d["positionX"].GetFloat(), d["positionY"].GetFloat(), d["positionZ"].GetFloat());
        std::string tempModel = d["model"].GetString();

        Level temp;
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





