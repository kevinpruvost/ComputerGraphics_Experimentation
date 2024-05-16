#pragma once

#include <common/Model.h>
#include <glad/glad.h>

class Model_OGL : public Model
{
public:
    Model_OGL();

    void SetIndices(const TriangleArray& indices) override;
    void Draw() const;

    Venom::ErrorCode ReloadObjectFromEngine() override;

private:
};

extern "C"
{
    EXPORT Model * createModel();
}