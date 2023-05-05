#ifndef APENGINE_CAMERA_H
#define APENGINE_CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "MovingObject.h"
#include <vector>


extern int SCR_WIDTH;
extern int SCR_HEIGHT;


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
extern float YAW;
extern float PITCH;
extern float SPEED;
extern float SENSITIVITY;
extern float ZOOM;
extern bool isBlocked;
extern bool isFound;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public IGui
{


public:

    glm::mat4 viewProjection = projection * view;
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Look = glm::vec3(0, 0, -6);
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        spdlog::warn("camera constructor");

        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;


        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        spdlog::warn("camera constructor");

        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
//        updateCameraPosition();
        updateCameraVectors();
    }

    ~Camera() {
        spdlog::warn("camera destructor");
    }

     void ImGui(){

        ImGui::Begin("Camera");
        ImGui::SetWindowSize(ImVec2(300, 400));

        ImGui::SliderFloat("position X", &Position.x, -100.0f, 100.0f);
        ImGui::SliderFloat("position Y", &Position.y, -100.0f, 100.0f);
        ImGui::SliderFloat("position Z", &Position.z, -100.0f, 100.f);

        ImGui::SliderFloat("target X", &Front.x, -1.0f, 1.0f);
        ImGui::SliderFloat("target Y", &Front.y, -1.0f, 1.0f);
        ImGui::SliderFloat("target Z", &Front.z, -1.0f, 1.f);
         ImGui::SliderFloat("zoom", &Zoom, 0.0f, 100.f);
        ImGui::Checkbox("Lock target point", &isBlocked);
        if(ImGui::Button("Calculate Lock target Point")) {
            Look = Look - Position;
        }
        ImGui::Checkbox("Follow object / reset kamery", &isFound);

        ImGui::End();
    }


    glm::mat4 getViewProjection(){
        return projection * view;
    }

    glm::mat4 getView(MovingObject& movingObject){
        if(isBlocked){

            return glm::lookAt(glm::vec3(Position.x, Position.y, Position.z),
                               glm::vec3(movingObject._transform._position.x, movingObject._transform._position.y, movingObject._transform._position.z),
                               glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else {
            return glm::lookAt(glm::vec3(Position.x, Position.y, Position.z),
                               glm::vec3(Position.x + Look.x, Position.y + Look.y, Position.z + Look.z),
                               glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }



    void followObject(MovingObject& kostka){
        if(isFound){
            Position.x = kostka._transform._position.x;
            Position.y = kostka._transform._position.y + 5;
            Position.z = kostka._transform._position.z + 40.f;
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

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
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
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -22.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    }



};
#endif //APENGINE_CAMERA_H
