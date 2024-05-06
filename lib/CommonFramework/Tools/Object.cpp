#include <common/ObjectPool.h>
#include <common/Object.h>

#include <random>
#include <cstring>

static const char* GenerateObjectName(const char const * name) {
    if (name != nullptr) return name;
    // Define a character set for random string generation
    constexpr char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
    constexpr int length = 16;

    // Generate a random string
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local std::uniform_int_distribution<int> dist(0, sizeof(charset) - 2); // -2 to avoid including the string terminator

    static thread_local char randomString[length + 1]; // +1 for the null terminator
    for (int i = 0; i < length; ++i) {
        randomString[i] = charset[dist(gen)];
    }
    randomString[length] = 0;

    // Construct the object name
    static char objectName[length + 8] = "Object_\0"; // +8 for "Object_" prefix and null terminator
    memcpy(objectName + (sizeof(char) * 7), randomString, sizeof(char) * length);

    // Return the object name as const char*
    return objectName;
}

Object::Object(const char const* name)
    : _objectName{GenerateObjectName(name)}
    , _guiCallback(nullptr)
{
    ObjectPool::AddObject(this);
}

Object::~Object()
{
}

void * Object::Delete()
{
    ObjectPool::DeleteObject(this);
    return nullptr;
}