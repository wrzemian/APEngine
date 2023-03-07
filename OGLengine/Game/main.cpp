#include <iostream>
#include "Window.h"



int main()
{
    Window window;
    window.init();
    // render loop
    // -----------
    while (window.isOpen())
    {
        // input
        // -----
        window.processInput();

        // render
        // ------
        window.clear();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        
        window.swap();
        window.pollEvents();
        
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    window.terminate();
    return 0;
}



