#include "GameLoop.h"
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        std::cout << "Pressed A" << std::endl;
        Player::GetPlayerPtr()->StartMovement(Direction::LEFT);
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        std::cout << "Released A" << std::endl;
        Player::GetPlayerPtr()->StopMovement(Direction::LEFT);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        std::cout << "Pressed D" << std::endl;
        Player::GetPlayerPtr()->StartMovement(Direction::RIGHT);
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        std::cout << "Released D" << std::endl;
        Player::GetPlayerPtr()->StopMovement(Direction::RIGHT);
    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        std::cout << "Pressed W" << std::endl;
        Player::GetPlayerPtr()->StartMovement(Direction::UP);
    }
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        std::cout << "Released W" << std::endl;
        Player::GetPlayerPtr()->StopMovement(Direction::UP);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        std::cout << "Pressed S" << std::endl;
        Player::GetPlayerPtr()->StartMovement(Direction::DOWN);
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        std::cout << "Released S" << std::endl;
        Player::GetPlayerPtr()->StopMovement(Direction::DOWN);
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        std::cout << "Pressed Space" << std::endl;
    }
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout << "Pressed Escape" << std::endl;
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

    const int playerIndex = DrawnObject::CreateObject(new UniformPosition{1,2}, Coordinates::SnapToGrid(1, 2), 16, Coordinates::GetGridIndices(), 6, TextureList::textures[TEXTURE_PLAYER], shader);

	Level test("test.level");
    test.Load(shader);

    Player player(DrawnObject::objects[playerIndex]);

    while (!glfwWindowShouldClose(window))
    {
        Renderer::Clear();
        /* Render here */
        
        //DrawnObject::objects[wallIndex]->DrawObject();
        //DrawnObject::objects[playerIndex+1]->DrawObject();

        test.Render();

        DrawnObject::objects[playerIndex]->DrawObject();
        //DrawnObject::objects[playerIndex]->ChangePosition(positions);
        //Entity::entityList[0]->moveEntity(Direction::RIGHT);
        player.PollPlayerEvents();
        Entity::PollEntitiesEvents();
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));
        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glfwTerminate();
    DrawnObject::UnloadAll();
    Entity::UnloadAll();
    TextureList::ClearTextures();
    Coordinates::clearCoords();
	return false;
}



//TO-DO
//Gravity
//Basic enemy AI
//Screen scroll?
//Main Menu
//Collisions
//Acceleration and Deceleration