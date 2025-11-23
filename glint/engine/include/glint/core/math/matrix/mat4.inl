#pragma once

// --- factories ---
constexpr mat4 mat4::identity() {
    mat4 result{};
    for (size_t i = 0; i < 4; ++i) {
        result[i][i] = 1.0f;
    }
    return result;
}

mat4 mat4::translate(const vec3& val) {
    mat4 result = identity();
    result[3][0] = val.x();
    result[3][1] = val.y();
    result[3][2] = val.z();
    return result;
}

mat4 mat4::rotate(const quaternion& val) {
    float xx = val.x * val.x;
    float yy = val.y * val.y;
    float zz = val.z * val.z;
    float xy = val.x * val.y;
    float xz = val.x * val.z;
    float yz = val.y * val.z;
    float wx = val.w * val.x;
    float wy = val.w * val.y;
    float wz = val.w * val.z;

    mat4 result = mat4::identity();

    result[0][0] = 1.0f - 2.0f * (yy + zz);
    result[0][1] = 2.0f * (xy - wz);
    result[0][2] = 2.0f * (xz + wy);
    result[0][3] = 0.0f;

    result[1][0] = 2.0f * (xy + wz);
    result[1][1] = 1.0f - 2.0f * (xx + zz);
    result[1][2] = 2.0f * (yz - wx);
    result[1][3] = 0.0f;

    result[2][0] = 2.0f * (xz - wy);
    result[2][1] = 2.0f * (yz + wx);
    result[2][2] = 1.0f - 2.0f * (xx + yy);
    result[2][3] = 0.0f;

    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;

    return result;
}

mat4 mat4::scale(const vec3& val) {
    mat4 result = identity();
    result[0][0] = val.x();
    result[1][1] = val.y();
    result[2][2] = val.z();
    return result;
}