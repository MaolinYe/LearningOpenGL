#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <cmath>
#include "shader.h"
GLFWwindow *window;

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


int main() {
    init();
    float vertices[] = { // 位置              // 颜色
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
    };
    unsigned int VAO; //
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO; // 将顶点数据绑定到顶点缓冲对象VBO中
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 顶点属性位置索引、属性组件数量、组件类型、是否进行归一化处理、属性之间的偏移量、起始偏移量
    glEnableVertexAttribArray(0);// 位置属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);// 颜色属性
    Shader shader("C:\\Users\\Yezi\\Desktop\\LearningOpenGL\\shaders\\shader.vs","C:\\Users\\Yezi\\Desktop\\LearningOpenGL\\shaders\\shader.fs");
    while (!glfwWindowShouldClose(window)) {
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window); // 交换在此渲染迭代期间用于渲染的颜色缓冲区
        glfwPollEvents(); // 检查是否触发了任何事件（如键盘输入或鼠标移动事件）
    }
    glfwTerminate(); // 删除所有已分配的 GLFW 资源
    return 0;
}