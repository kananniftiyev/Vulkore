#include "core/ObjectManager.hpp"

ObjectManager::ObjectManager()
{
    objects.reserve(50);
}

ObjectManager::~ObjectManager()
{
}

bool ObjectManager::addObject(Object &object)
{
    return false;
}

bool ObjectManager::removeObject(uint16_t id)
{
    return false;
}

bool ObjectManager::readObjectsFromFile()
{
    return false;
}
