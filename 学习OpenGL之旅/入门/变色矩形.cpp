#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <cmath>

GLFWwindow *window;
unsigned int shaderProgram;

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
    glViewport(0, 0, width, height); // 设置 OpenGL 渲染窗口的大小，前两个参数设置窗口左下角的位置，第三个和第四个参数以像素为单位设置渲染窗口的宽度和高度
}

void init() {
    glfwInit();  // 初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 指定创建的内容必须兼容的客户端 API 版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 指定创建的内容必须兼容的客户端 API 版本
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 指定要为其创建内容的 OpenGL 配置文件
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 指定 OpenGL 上下文是否应向前兼容
    window = glfwCreateWindow(2048, 1024, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window); // 告诉 GLFW 将窗口的内容作为当前线程上的主要内容
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 注册窗口调整调用函数
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) { // 初始化 GLAD
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

void main()
{
    FragColor = ourColor;
}
)";

void shaderCompile() { // 创建顶点着色器和片元着色器，编译链接成为一个着色器程序
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

int main() {
    init();
    shaderCompile();
    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 这样可以由下标代表顶点组合成矩形
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };
    unsigned int VAO; //
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO; // 将顶点数据绑定到顶点缓冲对象VBO中
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    unsigned int EBO; // 复制索引数组到一个索引缓冲中，供OpenGL使用
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0); // 告诉OpenGL该如何解析顶点数据
    glEnableVertexAttribArray(0); // 以顶点属性位置值作为参数，启用顶点属性
    while (!glfwWindowShouldClose(window)) {
        glUseProgram(shaderProgram);
        float timeValue = glfwGetTime();
        float blue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // 查询uniform ourColor的位置值
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, blue, 1.0f); // 更新uniform颜色
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制类型、顶点数目、索引类型、偏移量
        glfwSwapBuffers(window); // 交换在此渲染迭代期间用于渲染的颜色缓冲区
        glfwPollEvents(); // 检查是否触发了任何事件（如键盘输入或鼠标移动事件）
    }
    glfwTerminate(); // 删除所有已分配的 GLFW 资源
    return 0;
}