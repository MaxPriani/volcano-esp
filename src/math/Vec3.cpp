#include "Vec3.h"

Vector3 Vector3::operator-(const Vector3& other) const noexcept {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator+(const Vector3& other) const noexcept {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator*(float factor) const noexcept {
    return Vector3(x * factor, y * factor, z * factor);
}

Vector3 Vector3::operator/(float factor) const noexcept {
    return Vector3(x / factor, y / factor, z / factor);
}

float Vector3::distance(const Vector3& other) const noexcept {
    Vector3 diff = *this - other;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

const float* ViewMatrix::operator[](int index) const {
    return matrix[index];
}

bool WorldToScreen::transform(const Vector3& world, ImVec2& screen) const {
    float w = viewMatrix[3][0] * world.x + viewMatrix[3][1] * world.y + viewMatrix[3][2] * world.z + viewMatrix[3][3];

    if (w < 0.01f)
        return false;

    float x = viewMatrix[0][0] * world.x + viewMatrix[0][1] * world.y + viewMatrix[0][2] * world.z + viewMatrix[0][3];
    float y = viewMatrix[1][0] * world.x + viewMatrix[1][1] * world.y + viewMatrix[1][2] * world.z + viewMatrix[1][3];

    float inv_w = 1.0f / w;

    x *= inv_w;
    y *= inv_w;

    screen.x = (screenWidth * 0.5f) + (x * screenWidth * 0.5f);  // playerScreenPos
    screen.y = (screenHeight * 0.5f) - (y * screenHeight * 0.5f); // playerScreenHead

    return true;
}

