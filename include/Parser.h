#ifndef APENGINE_PARSER_H
#define APENGINE_PARSER_H

#include <string>
#include "document.h"
#include "filewritestream.h"
#include <writer.h>
#include <sstream>

class Parser {
private:
    std::string savePath;
public:
    Parser(std::string savePath);

    void saveJSON(rapidjson::Document d, std::string fileName);

    

};


#endif //APENGINE_PARSER_H
