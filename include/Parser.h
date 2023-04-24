#ifndef APENGINE_PARSER_H
#define APENGINE_PARSER_H

#include <string>
#include "document.h"
#include "filewritestream.h"
#include <writer.h>
#include <sstream>
#include "lights/AmbientLight.h"
#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"

class Parser {
private:
    std::string savePath;
public:
    Parser(std::string savePath);

    Parser();

    void SaveJSON(rapidjson::Document d, std::string fileName);

    DirectionalLight CreateFromJSONDir(std::string fileName);

    PointLight CreateFromJSONPoint(std::string fileName);

    SpotLight CreateFromJSONSpot(std::string fileName);

private:
    rapidjson::Document openJSON (std::string fileName);

};


#endif //APENGINE_PARSER_H
