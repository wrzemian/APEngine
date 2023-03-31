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
    glm::vec3 Look;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
//        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
//        updateCameraPosition();
//        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void Imgui(){

        ImGui::Begin("Camera");
        ImGui::SetWindowSize(ImVec2(250, 150));

        ImGui::SliderFloat("X", &Position.x, -200.0f, 100.0f);
        ImGui::SliderFloat("Y", &Position.y, -200.0f, 100.0f);
        ImGui::SliderFloat("Z", &Position.z, -200.0f, 100.f);

        ImGui::SliderFloat("Kierunek - X", &Look.x, -200.0f, 100.0f);
        ImGui::SliderFloat("Kierunek - Y", &Look.y, -200.0f, 100.0f);
        ImGui::SliderFloat("Kierunek - Z", &Look.z, -200.0f, 100.f);


        ImGui::End();
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

    void updateCameraPosition(){
//        Position.x = x;
//        Position.y = y;
//        Position.z = z;
    }

    void setView(){
        glm::mat4 view = glm::lookAt(glm::vec3(Position.x, Position.y, Position.z),  // pozycja kamery
                                     glm::vec3(Look.x, Look.y, Look.z),  // punkt na który patrzy kamera
                                     glm::vec3(0.0f, 1.0f, 0.0f)); // wektor wskazujący kierunek "góry"
    }


};
#endif //APENGINE_CAMERA_H