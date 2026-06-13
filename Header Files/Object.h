#pragma once
class Object {
public:
    Object() = default;
    virtual ~Object() = default;
    virtual void update() = 0;
    virtual void draw(unsigned int shaderProgram) = 0;
};
