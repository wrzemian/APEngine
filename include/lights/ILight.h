#ifndef APENGINE_ILIGHT_H
#define APENGINE_ILIGHT_H
#include "../Shader.h"

class ILight
{
public:
    ILight() = default;
    virtual ~ILight() = default;

    virtual void sendToShader(Shader shader, std::string type) = 0;
};
#endif //APENGINE_ILIGHT_H
