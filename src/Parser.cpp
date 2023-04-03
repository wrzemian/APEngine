//
// Created by wrzos on 03.04.2023.
//

#include <iostream>
#include "../include/Parser.h"
Parser::Parser(std::string savePath) {
this->savePath = savePath;
}

void Parser::saveJSON(rapidjson::Document d, std::string fileName) {
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




