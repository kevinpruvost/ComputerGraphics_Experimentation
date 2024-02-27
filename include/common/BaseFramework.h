#pragma once

class BaseFramework {
public:
    virtual ~BaseFramework() {}

    // Initialization function
    virtual bool Init() = 0;

    // Destruction function
    virtual void Destroy() = 0;

    // Launch function
    virtual void Launch() = 0;
};