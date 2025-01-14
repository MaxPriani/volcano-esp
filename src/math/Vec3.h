#pragma once
#include <Windows.h>
#include <imgui.h>
#include <cmath> 

class Vector3 {
public:
    float x, y, z;

public:
    constexpr Vector3(float x = 0.f, float y = 0.f, float z = 0.f) noexcept : x(x), y(y), z(z) {}
    Vector3 operator-(const Vector3& other) const noexcept;
    Vector3 operator+(const Vector3& other) const noexcept;
    Vector3 operator*(float factor) const noexcept;
    Vector3 operator/(float factor) const noexcept;
    float distance(const Vector3& other) const noexcept;
};

class ViewMatrix {
public:
    float matrix[4][4];
    const float* operator[](int index) const;
};

class WorldToScreen {
public:
    const ViewMatrix& viewMatrix;
    int screenWidth;
    int screenHeight;

public:
    WorldToScreen(const ViewMatrix& vm, int resX, int resY) : viewMatrix(vm), screenWidth(resX), screenHeight(resY) {}
    bool transform(const Vector3& world, ImVec2& screen) const;
};