#ifndef APENGINE_PARSER_H
#define APENGINE_PARSER_H

#include <string>
#include "document.h"
#include "filewritestream.h"
#include <writer.h>
#include <sstream>
#include "lights/ILight.h"
#include "lights/AmbientLight.h"
#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"

class Parser {
private:
    std::string savePath;
public:
    Parser(std::string savePath);

    void saveJSON(rapidjson::Document d, std::string fileName);

    DirectionalLight createFromJSONDir(std::string fileName);

    PointLight createFromJSONPoint(std::string fileName);

    SpotLight createFromJSONSpot(std::string fileName);

    rapidjson::Document openJSON (std::string fileName);

};


#endif //APENGINE_PARSER_H
