#include "GameLoop.h"

#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Graphics/DrawnObject.h"
#include "WindowUI.h"
#include "Graphics/Renderer.h"
#include "Graphics/VertexBufferLayout.h"
#include "Graphics/Texture.h"
#include "Graphics/Coordinating.h"
#include "LevelLoad.h"
#include "Items/Entity.h"
#include "Items/Player.h"
#include "Items/Consts.h"

#define FPS_LIMIT 60


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        //std::cout << "Pressed A" << std::endl;
        Player::GetPlayerPtr()->StartMovement(Direction::LEFT);
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        //std::cout << "Released A" << std::endl;
        Player::GetPlayerPtr()->StopMovement(Direction::LEFT);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        //std::cout << "Pressed D" << std::endl;
        Player::GetPlayerPtr()->StartMovement(Direction::RIGHT);
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        //std::cout << "Released D" << std::endl;
        Player::GetPlayerPtr()->StopMovement(Direction::RIGHT);
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        //std::cout << "Pressed W" << std::endl;
        //Player::GetPlayerPtr()->StartMovement(Direction::UP);
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        //std::cout << "Released W" << std::endl;
        //Player::GetPlayerPtr()->StopMovement(Direction::UP);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        //std::cout << "Pressed S" << std::endl;
        //Player::GetPlayerPtr()->StartMovement(Direction::DOWN);
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        //std::cout << "Released S" << std::endl;
        //Player::GetPlayerPtr()->StopMovement(Direction::DOWN);
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        Player::GetPlayerPtr()->Jump();
        //std::cout << "Pressed Space" << std::endl;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        //Player::GetPlayerPtr()->Jump();
        //std::cout << "Pressed Space" << std::endl;
    }
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        //std::cout << "Pressed Escape" << std::endl;
    }
}

bool GameLoop::StartLoop()
{
    GLFWwindow* window = WindowUI::GetWindowPtr();

    glfwSetKeyCallback(window, key_callback);

    const glm::mat4 proj = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.7f, 1.0f);
    shader.SetUniformMat4f("u_MVP", proj);

    Coordinates::SetGridIndices();

    const int playerIndex = DrawnObject::CreateObject(new UniformPosition{1,10}, Coordinates::SnapToGrid(1, 10), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_PLAYER], shader);

	Level test("test.level");
    test.Load(shader);

    Player player(DrawnObject::objects[playerIndex]);

    auto timePtOne = std::chrono::system_clock::now();

    while (!glfwWindowShouldClose(window))
    {
        //TODO SETUP CHRONO CLOCK TIMER WITH NO VSYNC

        auto timePtTwo = std::chrono::system_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(timePtTwo - timePtOne).count();

    	if (time > (1000000 / FPS_LIMIT))
        {
            Renderer::Clear();

            test.Render();
            DrawnObject::objects[playerIndex]->DrawObject();

            player.PollPlayerEvents();
            Entity::PollEntitiesEvents();

            GLCall(glfwSwapBuffers(window));
			GLCall(glfwPollEvents());

            timePtOne = timePtTwo;
        }
    }

    glfwTerminate();
    DrawnObject::UnloadAll();
    Entity::UnloadAll();
    TextureList::ClearTextures();
    Coordinates::clearCoords();
	return false;
}



/*TODO
	Gravity
	Basic enemy AI
	Screen scroll?
	Main Menu
	Collisions
	Acceleration and Deceleration
*/