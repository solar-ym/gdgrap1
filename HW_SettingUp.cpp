#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Yza Montuerto", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    GLfloat vertices[]{
        -0.425f, 1.f, 0.f,
        0.425, 1.f, 0.f,
        1.0f, 0.425f, 0.f,
        1.0f, -0.425f, 0.f,
        0.425f, -1.0f, 0.f,
        -0.425f, -1.0f, 0.f,
        -1.0f, -0.425f, 0.f,
        -1.0f, 0.425f, 0.f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO); 

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, // VBO index
        3, // num of components
        GL_FLOAT, //datatype
        GL_FALSE, // normalized or not
        3 * sizeof(GL_FLOAT), // size of components in bytes
        (void*)0 //stride value
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
                    // SHAPE, 1st INDEX, NUM OF POINTS
        glDrawArrays(GL_POLYGON, 0, 8);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}