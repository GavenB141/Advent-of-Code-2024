struct Vec2 {
    int x;
    int y;
};

inline Vec2 operator+(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

inline Vec2 operator-(Vec2 const& lhs, Vec2 const& rhs) {
    return Vec2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

inline bool operator==(Vec2 const& lhs, Vec2 const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
