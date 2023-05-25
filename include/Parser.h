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
#include "Objects/Object3D.h"
#include "Objects/MovingObject.h"
#include "Objects/Level.h"
#include "Camera.h"

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

    Object3D CreateFromJSONObj3D(std::string fileName);

    MovingObject CreateFromJSONMovingObject(std::string fileName);

    Level CreateFromJSONWalls(std::string fileName);

    Camera CreateFromJSONCam(std::string fileName);

    rapidjson::Document openJSON (std::string fileName);


};


#endif //APENGINE_PARSER_H
