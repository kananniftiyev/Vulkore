#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

struct IDrawable
{
    virtual ~IDrawable() = default;
    virtual void draw() = 0;
};

struct Mesh
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Object : IDrawable
{
    uint16_t id;
    std::string name;
    std::vector<Mesh> vertecies;
    std::vector<unsigned int> indices;
    bool is_static = true;
    bool is_active = false;

    void draw() override
    {
        if (!is_active)
        {
            return;
        }
    }
};

class ObjectManager
{
private:
    std::vector<Object> objects;

public:
    ObjectManager();
    ~ObjectManager();

    bool addObject(Object &object);
    bool removeObject(uint16_t id);
    bool readObjectsFromFile();

    void renderObjects();
};
