# Change Log

- Collisions separation
- Asset manager
- Rendering individual hitboxes
- Loading buttons, platforms, decorations with and without collisions
- Platforms semi-correctly load their target positions
- Buttons connect to platforms

## [0.0.12] - 2023-05-02
### Added
- added random scrolling background

## [0.0.11] - 2023-04-17
### Added
- text rendering using freetype2 lib

## [0.0.10] - 2023-04-13
### Added
- OpenAl lib
- freetype2 lib

## [0.0.9] - 2023-04-12
### Added
- HUD
- second player
- players controlled by InputSystem

## [0.0.8] - 2023-04-03
### Fixed
- different models can use different shaders each
- closing program with escape
- closing program with the closing cross
- object moves with camera

### Added
- debug shape - draw cube, arrow or obj on given coordinates (arrow and obj not tested yet)
- parsing and saving all light types to .json file
- very primitive way of loading lights form .json



## [0.0.7] - 2023-04-02
### Added
- counting and displaying deltaTime and FramesPerSecond
- MovingObject class
- MovingObject has velocity in all axes, its position changes depending on deltaTime 
- InputSystem class
- InputSystem has a GetKey method to determine whether key is pressed or not

## [0.0.6] - 2023-03-31

### Added
- rapidjson lib
- effolkronium/random lib
- wqking/eventpp lib
- Neargye/magic_enum lib
- fmt lib

### Fixed
- removed openAL lib from cmake and Game.cpp because its fucked

## [0.0.5] - 2023-03-30
### Added
- point light
- camera 

### Changed
- frag shader
- lights are working with obj

### Fixed
- spot light

## [0.0.4] - 2023-03-29
### Added
- class Transform with Position, Rotation and Scale
- class Object3D containing Transform and Model
- rendering 3d object affected by light

- directional light
- spot light (but i cant locate it on the cube, so i'm not sure if it works tbh)
### Changed
- both shaders

## [0.0.3] - 2023-03-28
### Tip
you can install GLSL extension to Clion easily, and it helps a lot


- lights hierarchy
- loading and rendering 2 obj Models

### Todo
- fix models' textures
- add transform to each individual one

### Changed
- in Sierpinski.draw() changed 12 to 36
- drawing cube instead of triangle

## [0.0.2] - 2023-03-27

### Added
- displaying Sierpinski's Tetrahedron instead of Hello World Triangle
- can change color, rotation and recursion depth
- class Mesh
- class Model loading Meshes and Textures via Assimp
### Changed
- moved project to CLion

## [0.0.1] - 2023-03-21

### Added
- displaying "Hello world"

