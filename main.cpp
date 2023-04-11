#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

static unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*)alloca(len * sizeof(char));
        glad_glGetShaderInfoLog(id, len, &len, message);
        std::cout<<'Failed to compile '
                 << (type == GL_VERTEX_SHADER ? 'vertex' : 'fragment') << 'shader:\n'<<message<<'\n';
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

int main() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    const int width = 640, height = 640;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
    if (!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    float x_pos=1.0, y_pos=1.0;
    bool back = false;

    int vertex_size = 12;
    int vertex_atrib_size = 2;

    float vertex[12] = {
             x_pos,  y_pos,
            -x_pos,  y_pos,
            -x_pos, -y_pos,

             x_pos,  y_pos,
             x_pos, -y_pos,
            -x_pos, -y_pos,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(float), vertex, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * vertex_atrib_size, 0);

    std::string vertexCode = get_file_contents("../Shaders/default.vert");
    std::string fragmentCode = get_file_contents("../Shaders/default.frag");

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    unsigned int shader = CreateShader(vertexSource, fragmentSource);
    glUseProgram(shader);

    int table_size = 676;

    int pixelSize = 32;
    int ps_loc = glGetUniformLocation(shader, "u_PixelSize");
    glUniform1i(ps_loc, pixelSize);

    int table_width = sqrt(table_size);
    int tw_loc = glGetUniformLocation(shader, "u_TableWidth");
    glUniform1i(tw_loc, table_width);

    int table_height = sqrt(table_size);
    int th_loc = glGetUniformLocation(shader, "u_TableHeight");
    glUniform1i(th_loc, table_height);

    srand((unsigned)time(NULL));

    float table[676] = {0};
    // small windmill
//    int starting_pos = 300;
//    table[starting_pos] = (float) true;
//    table[starting_pos+1] = (float) true;
//    table[starting_pos+2] = (float) true;

      // test shape
//    int starting_pos = 300;
//    table[starting_pos] = (float) true;
//    table[starting_pos+1+table_width] = (float) true;
//    table[starting_pos-1+table_width] = (float) true;
//    table[starting_pos+1-table_width] = (float) true;
//    table[starting_pos-1-table_width] = (float) true;

    // flying machine
//    int starting_pos = 300;
//    table[starting_pos] = (float) true;
//    table[starting_pos+1] = (float) true;
//    table[starting_pos+2] = (float) true;
//    table[starting_pos+2+table_width] = (float) true;
//    table[starting_pos+1+table_width+table_width] = (float) true;

    // random grid
    for (int i=0; i < table_size; i++){
        table[i] = (float)(rand()%2 == 0);
    }


    int location = glGetUniformLocation(shader, "u_Table");
    glUniform1fv(location, table_size, table);

    int timer = 0;

    int update_speed = 10;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (timer == update_speed) {
            float fTrue = (float) true;
            float fFalse = (float) false;
            std::vector<int> updateIndexes;
            for (int i = 0; i < table_size; i++) {
                bool alive = table[i];
                int alive_neighbour_counter = 0;
                if (table[i + 1] == fTrue)
                    alive_neighbour_counter++;
                if (table[i - 1] == fTrue)
                    alive_neighbour_counter++;
                if (table[i - table_width] == fTrue)
                    alive_neighbour_counter++;
                if (table[i + table_width] == fTrue)
                    alive_neighbour_counter++;
                if (table[i + 1 + table_width] == fTrue)
                    alive_neighbour_counter++;
                if (table[i - 1 + table_width] == fTrue)
                    alive_neighbour_counter++;
                if (table[i + 1 - table_width] == fTrue)
                    alive_neighbour_counter++;
                if (table[i - 1 - table_width] == fTrue)
                    alive_neighbour_counter++;

                if (alive && (alive_neighbour_counter == 1||alive_neighbour_counter == 4)) {
                    updateIndexes.push_back(i);
                } else if (!alive && alive_neighbour_counter == 3) {
                    updateIndexes.push_back(i);
                }
            }

            for (int i = 0; i < updateIndexes.size(); ++i) {
                table[updateIndexes.at(i)] == fTrue ? table[updateIndexes.at(i)] = fFalse : table[updateIndexes.at(i)] = fTrue;
            }
            timer=0;
            glUniform1fv(location, table_size, table);
        }
        timer++;

        glDrawArrays(GL_TRIANGLES, 0, vertex_size);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
