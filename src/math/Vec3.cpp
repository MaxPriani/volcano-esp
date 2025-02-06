#include "Vec3.h"

constexpr Vector3::Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

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

// Transforms 3D world coordinates into 2D screen coordinates using the view matrix
bool WorldToScreen::transform(const Vector3& world, ImVec2& screen) const {
    // Step 1: Compute 'w', the depth component, representing the relative distance from the object to the camera
    // This is derived from the 4th row of the view matrix
    float w = viewMatrix[3][0] * world.x + viewMatrix[3][1] * world.y + viewMatrix[3][2] * world.z + viewMatrix[3][3];

    // Step 2: Check if the object is outside the camera's view frustum
    // If 'w' is too small (e.g., less than 0.01), the object is either behind the camera or too far
    if (w < 0.01f)
        return false; // Return false to indicate the object is not visible.

    // Step 3: Calculate the transformed X coordinate in clip space.
    // This is derived from the first row of the view matrix.
    float x = viewMatrix[0][0] * world.x + viewMatrix[0][1] * world.y + viewMatrix[0][2] * world.z + viewMatrix[0][3];

    // Step 4: Calculate the transformed Y coordinate in clip space.
    // This is derived from the second row of the view matrix.
    float y = viewMatrix[1][0] * world.x + viewMatrix[1][1] * world.y + viewMatrix[1][2] * world.z + viewMatrix[1][3];

    // Step 5: Normalize the clip space coordinates using the depth component ('w').
    // Perspective division is performed here.
    float inv_w = 1.0f / w; // Calculate the reciprocal of 'w'.
    x *= inv_w; // Normalize the X coordinate.
    y *= inv_w; // Normalize the Y coordinate.

    // Step 6: Convert the normalized coordinates to screen space.
    // The screen space coordinates are calculated based on the screen resolution.

    // Horizontal screen position (X-axis):
    // The normalized X coordinate is scaled and offset to fit the screen width.
    screen.x = (screenWidth * 0.5f) + (x * screenWidth * 0.5f);

    // Vertical screen position (Y-axis):
    // Note: The Y-axis is inverted in most rendering systems, so subtraction is used here.
    screen.y = (screenHeight * 0.5f) - (y * screenHeight * 0.5f);

    // Step 7: Return true to indicate that the transformation was successful and the object is visible on screen.
    return true;
}

