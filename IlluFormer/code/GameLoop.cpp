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
    if (Level::GetActiveLevel()!=nullptr)
    {
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            Player::GetPlayerPtr()->StartMovement(Direction::LEFT);
        else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
            Player::GetPlayerPtr()->StopMovement(Direction::LEFT);

        else if (key == GLFW_KEY_D && action == GLFW_PRESS)
            Player::GetPlayerPtr()->StartMovement(Direction::RIGHT);
        else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
            Player::GetPlayerPtr()->StopMovement(Direction::RIGHT);

        else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            Player::GetPlayerPtr()->isJumping = true;
        else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
            Player::GetPlayerPtr()->isJumping = false;

        else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            Menu::isPauseMenuActive = !Menu::isPauseMenuActive;
        }

        else if (key == GLFW_KEY_R && action == GLFW_PRESS)
            Level::GetActiveLevel()->ResetLevel();
        else if(key == GLFW_KEY_TAB && action == GLFW_PRESS)
        {
            int newLevel = Level::GetActiveLevel()->index + 1;
            new Level{ (std::to_string(newLevel)) + ".level" };
            Level::SetActiveLevel(newLevel);
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double cursorX;
        double cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        if (Menu::ProcessClick(cursorX, cursorY))
            glfwSetWindowShouldClose(window, 1);
    }
}

bool GameLoop::StartLoop()
{
    GLFWwindow* window = WindowUI::GetWindowPtr();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    const glm::mat4 proj = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);

    Shader* shader = new Shader("res/shaders/Basic.shader");
    shader->Bind();
    shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.7f, 1.0f);
    shader->SetUniformMat4f("u_MVP", proj);

    Coordinates::SetGridIndices();

    Menu::InitMenus(shader);

    auto timePtOne = std::chrono::system_clock::now();

    while (!glfwWindowShouldClose(window))
    { //TODO FORMAT LEVELS AS SPEEDRUN, END RESULTS: TIME, DEATHS, COLLECTED COINS.

        auto timePtTwo = std::chrono::system_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(timePtTwo - timePtOne).count();

    	if (time > (1000000 / FPS_LIMIT))
        {
            Renderer::Clear();
            Level::InitLevels(shader);
            //test.Render();
            if (Level::GetActiveLevel() == nullptr)
            {
                double cursorX;
                double cursorY;
                glfwGetCursorPos(window, &cursorX, &cursorY);
                Menu::ShowMainMenu(cursorX, cursorY);
            }
            else
            {
                Level::GetActiveLevel()->Render();
                if(Menu::isPauseMenuActive)
                {
                    double cursorX;
                    double cursorY;
                    glfwGetCursorPos(window, &cursorX, &cursorY);
                    Menu::ShowPauseMenu(cursorX, cursorY);
                }
                else if(Menu::isEndMenuActive)
                {
                    double cursorX;
                    double cursorY;
                    glfwGetCursorPos(window, &cursorX, &cursorY);
                    Menu::ShowEndMenu(cursorX, cursorY);
                }
                else
                {
                    Player::GetPlayerPtr()->PollPlayerEvents();
                    Entity::PollEntitiesEvents();
                }
            }

            GLCall(glfwSwapBuffers(window))
			GLCall(glfwPollEvents())

            timePtOne = timePtTwo;
        }
    }

    Coordinates::clearCoords();
    Level::UnloadAllLevels();
    Menu::UnloadMenus();
    TextureList::ClearTextures();
    delete shader;
    glfwTerminate();
	return false;                 
}


/*TODO
*/
