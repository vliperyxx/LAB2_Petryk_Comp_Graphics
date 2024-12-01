#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // Переконатися, що файли можуть бути відкриті
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // Відкрити файли
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // Прочитати дані з файлів у потоки
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // Закрити файли
            vShaderFile.close();
            fShaderFile.close();

            // Перетворити потоки в рядки коду шейдерів
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        // Перевірити, чи шейдерний код не порожній
        if (vertexCode.empty()) {
            std::cerr << "ERROR::SHADER::VERTEX_SHADER_CODE_IS_EMPTY" << std::endl;
        }
        if (fragmentCode.empty()) {
            std::cerr << "ERROR::SHADER::FRAGMENT_SHADER_CODE_IS_EMPTY" << std::endl;
        }

        const char* vShaderCode = vertexCode.empty() ? nullptr : vertexCode.c_str();
        const char* fShaderCode = fragmentCode.empty() ? nullptr : fragmentCode.c_str();

        if (!vShaderCode || !fShaderCode) {
            std::cerr << "ERROR: Shader code is empty or invalid." << std::endl;
            return;
        }

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        if (vertex == 0) {
            std::cerr << "ERROR::SHADER::VERTEX::CREATION_FAILED" << std::endl;
            return;
        }
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        if (fragment == 0) {
            std::cerr << "ERROR::SHADER::FRAGMENT::CREATION_FAILED" << std::endl;
            return;
        }
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        if (ID == 0) {
            std::cerr << "ERROR::SHADER::PROGRAM::CREATION_FAILED" << std::endl;
            return;
        }
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);


        // Видалити шейдери після лінкування
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(ID);
    }

    void deleteProgram() {
        glDeleteProgram(ID);
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- --" << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- --" << std::endl;
            }
        }
    }
};

#endif
