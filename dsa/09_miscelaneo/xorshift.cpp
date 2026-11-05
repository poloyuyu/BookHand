uint32_t xorshift() {
    static uint32_t seed = static_cast<uint32_t>(time(nullptr));
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}
