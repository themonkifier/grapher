#include "shader.h"

unsigned int shaderID;

void set_shaders(const char* vertexPath, const char* fragmentPath)
{
    /* open files */
    FILE* vertexFile;
    FILE* fragmentFile;
    
    if ((vertexFile = fopen(vertexPath, "r")) == NULL)
    {
        printf("Failed to open vertex shader\n");
        return;
    }
    else if ((fragmentFile = fopen(fragmentPath, "r")) == NULL)
    {
        fclose(vertexFile);
        printf("Failed to open fragmentation shader\n");
        return;
    }
    
    /* get length of files */
    fseek(vertexFile, 0, SEEK_END);
    long int vertexLength = ftell(vertexFile);
    rewind(vertexFile);
    
    fseek(fragmentFile, 0, SEEK_END);
    long int fragmentLength = ftell(fragmentFile);
    rewind(fragmentFile);
    
    /* read files into buffers */
    char* vertexCode = (char*) malloc((vertexLength + 1) * sizeof(char));
    char* fragmentCode = (char*) malloc((fragmentLength + 1) * sizeof(char));
        
    fread((void*) vertexCode, sizeof(char), vertexLength, vertexFile);
    vertexCode[vertexLength] = '\0';
    fclose(vertexFile);
    
    fread((void*) fragmentCode, sizeof(char), fragmentLength, fragmentFile);
    fragmentCode[fragmentLength] = '\0';
    fclose(fragmentFile);
    
    /* setup shaders */
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char* const*) &vertexCode, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Failed to compile vertex shader\n%s\n", infoLog);
    }
    free(vertexCode);
    
    /* fragment shader */
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char* const*) &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Failed to compile fragmentation shader\n%s\n", infoLog);
    }
    free(fragmentCode);
    
    /* link shaders */
    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);
    glLinkProgram(shaderID);
    // check for linking errors
    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
        printf("Failed to link shader program\n%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void use_shaders()
{
    glUseProgram(shaderID);
}

void set_int(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shaderID, name), value);
}

void set_float(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shaderID, name), value);
}
