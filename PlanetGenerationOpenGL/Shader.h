#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const { glUseProgram(ID); }

    void setBool(const std::string& name, bool v) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)v);
    }

    void setInt(const std::string& name, int v) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), v);
    }

    void setFloat(const std::string& name, float v) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), v);
    }

    void setVec3(const std::string& name, const glm::vec3& v) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
    }

    void setMat4(const std::string& name, const glm::mat4& m) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
    }
};