struct Vec2 {
    long x;
    long y;
};

inline Vec2 operator+(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

inline Vec2 operator-(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

inline Vec2 operator*(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x * rhs.x, lhs.y * rhs.y };
}

inline Vec2 operator/(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x / rhs.x, lhs.y / rhs.y};
}

inline bool operator==(Vec2 const& lhs, Vec2 const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator<(Vec2 const& lhs, Vec2 const& rhs) {
    if (lhs.x < rhs.x) {
        return true;
    } else if (lhs.x == rhs.x && lhs.y < rhs.y) {
        return true;
    }
    return false;
}
