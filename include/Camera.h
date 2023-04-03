#ifndef APENGINE_CAMERA_H
#define APENGINE_CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include <vector>


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
bool isBlocked = false;
bool isFound = false;


class vec3;

class vec3;

class vec3;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Look = glm::vec3(0, 0, -6);
    bool canBeRotated;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
//        updateCameraPosition();
        updateCameraVectors();
    }


    void Imgui(){

        ImGui::Begin("pozycja kamery");
        ImGui::SetWindowSize(ImVec2(250, 100));

        ImGui::SliderFloat("pozycja kamery - X", &Position.x, -100.0f, 100.0f);
        ImGui::SliderFloat("pozucja kamery - Y", &Position.y, -100.0f, 100.0f);
        ImGui::SliderFloat("pozycja kamery - Z", &Position.z, -100.0f, 100.f);
        ImGui::End();

        ImGui::Begin("Punkt na ktory patrzy kamera");
        ImGui::SetWindowSize(ImVec2(300, 150));
        ImGui::SliderFloat("X", &Look.x, -6.0f, 6.0f);
        ImGui::SliderFloat("Y", &Look.y, -6.0f, 6.0f);
        ImGui::SliderFloat("Z", &Look.z, -6.0f, 6.f);
        ImGui::Checkbox("zablokuj punkt patrzenia", &isBlocked);
        ImGui::Checkbox("znajdz obiekt", &isFound);
        ImGui::End();
    }

    glm::mat4 getView(glm::vec3 oldLook, glm::vec3 oldPos){
        if(oldLook + oldPos != Look + Position){
            Look = oldLook + oldPos;
        }
        if(isBlocked){

            return glm::lookAt(glm::vec3(Position.x, Position.y, Position.z),
                                         glm::vec3(Look.x, Look.y, Look.z),
                                         glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else {
            return glm::lookAt(glm::vec3(Position.x, Position.y, Position.z),
                               glm::vec3(Position.x + Look.x, Position.y + Look.y, Position.z + Look.z),
                               glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
    void findObject(MovingObject kostka){
        if(isFound){
            Position.x = kostka._transform._position.x;
            Position.y = kostka._transform._position.y;
            Position.z = kostka._transform._position.z + 20.f;
            Look = glm::vec3(0, 0, -6);
        };
    }


//    void ProcessMouseScroll(float yoffset)
//    {
//        Zoom -= (float)yoffset;
//        if (Zoom < 1.0f)
//            Zoom = 1.0f;
//        if (Zoom > 45.0f)
//            Zoom = 45.0f;
//    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }


};
#endif //APENGINE_CAMERA_H
