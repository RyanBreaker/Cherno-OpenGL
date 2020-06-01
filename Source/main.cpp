#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderSource ParseShader(const std::string &filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int) type] << line << '\n';
        }
    }

    const auto a = ss[0].str();

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int shaderType, const std::string &shaderSource) {
    const auto shaderId = glCreateShader(shaderType);
    const auto shaderSourceC = shaderSource.c_str();

    glShaderSource(shaderId, 1, &shaderSourceC, nullptr);
    glCompileShader(shaderId);

    int compileResult;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        int messageLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &messageLength);

        std::vector<char> message(messageLength);
        glGetShaderInfoLog(shaderId, messageLength, &messageLength, &message[0]);

        std::cout << "Failed to compile shader!" << std::endl;
        for (auto c : message)
            std::cout << c;
        std::cout << std::endl;

        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

static unsigned int CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    const auto programId = glCreateProgram();
    const auto vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    const auto fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);
    glValidateProgram(programId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return programId;
}

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW init error!" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);

    // Vertex buffer.
    const auto vertices = {
            -0.5f, -0.5f,
            0.f, 0.5f,
            0.5f, -0.5f
    };

    // Number of elements in each vertex.
    const auto vertexSize = 2;

    // Number of vertices in the buffer.
    const auto vertexCount = vertices.size() / vertexSize;

    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vertexSize, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), nullptr);

    const auto shaderSource = ParseShader("../Resources/Shaders/Basic.shader");
    const auto shaderId = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);

    glUseProgram(shaderId);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderId);
    glfwTerminate();
}
