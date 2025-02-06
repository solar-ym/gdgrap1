#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

float x = 0, y = 0, z = -1.f;
float scale_x = 1, scale_y = 1, scale_z = 1;
float theta = 0, av_x = 0, av_y = 0, av_z = 0;

void key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {


    //Moving the camera position
    if (key == GLFW_KEY_D && action == GLFW_REPEAT) x += 0.5f;
    if (key == GLFW_KEY_A && action == GLFW_REPEAT) x -= 0.5f;
    if (key == GLFW_KEY_W && action == GLFW_REPEAT) y += 0.5f;
    if (key == GLFW_KEY_S && action == GLFW_REPEAT) y -= 0.5f;
    if (key == GLFW_KEY_Z && action == GLFW_REPEAT) z -= 0.5f;
    if (key == GLFW_KEY_C && action == GLFW_REPEAT) z += 0.5f;

    //Scaling
    if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
        scale_x += 0.5f;
        scale_y += 0.5f;
        scale_z += 0.5f;
    }
    if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
        scale_x -= 0.5f;
        scale_y -= 0.5f;
        scale_z -= 0.5f;
    }

    //Rotating
    /* Change the theta of rotation */
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
        av_y = 0.5;
        av_x = 0;
        theta -= 5.0f;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
        av_y = 0.5;
        av_x = 0;
        theta += 5.0f;
    }
    if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
        av_x = 0.5;
        av_y = 0;
        theta -= 5.0f;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        av_x = 0.5;
        av_y = 0;
        theta += 5.0f;
    }

}

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    float windowWidth = 640.f;
    float windowHeight = 480.f;

    window = glfwCreateWindow(640, 480, "Zoom", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    /* Screen Space is usually:
        From 0 to screen width
        From 0 to screen height
    */
    //min x //min y, //max x // max y
    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetKeyCallback(window, key_Callback);

    /* Load and create a file */
    fstream vertSrc("Shaders/Shader.vert");
    stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();

    /* Convert stream into a character array */
    string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    fstream fragSrc("Shaders/Shader.frag");
    stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    /* Create a fragment shader
    *  Assign source to fragment shader
    *  Compile the Fragment Shader
    */
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    /*
    * Create the shader program
    * Attach the compiled vertex & fragment shader
    */
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    /*Initialize the FF variables*/
    string path = "3D/bunny.obj";
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> material;
    string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes, //Overall def
        &shapes,  //Refers to the object itself
        &material, //Refers to the texture/image
        &warning,
        &error,
        path.c_str()
    );
    /* We need to instruct the EBO from the Mesh Data */
    vector<GLuint> meshIndices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        meshIndices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[]{
         0.f, 1.f, 0.f,
         -0.5f, 0.f, 0.f,
         0.5f, 0.f, 0.f
    };

    GLuint indices[]{ // the points
        0, 1, 2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /* Render loop until the user closes the window */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE, 3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * meshIndices.size(),
        meshIndices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProg);
    glBindVertexArray(VAO);

    mat4 identity_matrix = mat4(1.0f);

    //Projection Matrix
    //mat4 projectionMatrix = ortho(
    //    -2.0f,
    //    2.0f,
    //    -2.0f,
    //    2.0f,
    //    -5.0f,
    //    5.0f
    //);

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Position of camera
        vec3 cameraPos = vec3(0.f + x, 0.f + y, 5.f); //Eye
        mat4 cameraPosMatrix = translate(mat4(1.0f), cameraPos * -1.0f);

        cout << "X: " << x << endl;

        //Orientation, points upward
        vec3 worldUp = normalize(vec3(0.f, 1.f, 0.f)); //Pointing upward
        vec3 cameraCenter = vec3(0.f + x, 3.f + y, 0.f); //A bit on top of the bunny

        //Forward
        vec3 F = cameraCenter - cameraPos;
        F = normalize(F);
        //R = F x WorldUp
        vec3 R = cross(F, worldUp);
        //U = R x F
        vec3 U = cross(R, F);
        //R
        mat4 cameraOrientation = mat4(1.0f); //creates an identity matrix
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;
        //U
        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;
        //F
        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;
        //End of Orientation matrix

        //mat4 viewMatrix = (cameraOrientation * cameraPosMatrix);
        mat4 viewMatrix = lookAt(cameraPos, cameraCenter, worldUp);

        mat4 projectionMatrix = perspective(
            radians(60.f + z), //This is your FOV
            windowHeight / windowWidth, //Aspect ratio
            0.1f, //z-Near, should never be <= 0
            100.f //z-Far
        );

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewMatrix));

        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projectionMatrix));

        unsigned int transformLocation = glGetUniformLocation(shaderProg, "transform");
        mat4 transformation_matrix = translate(identity_matrix, vec3(0, 0, z));
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(transformation_matrix));

        transformation_matrix = scale(transformation_matrix, vec3(scale_x, scale_y, scale_z));
        if (av_x != 0 || av_y != 0 || av_z != 0)
            transformation_matrix = rotate(transformation_matrix, radians(theta), normalize(vec3(av_x, av_y, av_z)));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, meshIndices.size(), GL_UNSIGNED_INT, 0);

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}