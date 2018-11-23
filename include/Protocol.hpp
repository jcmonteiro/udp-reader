#pragma once


namespace udp_socket
{


/** Helper function to read a 16 bit value from a little-endian byte stream */
template<typename T, typename InputIterator>
inline T read16_le(InputIterator it)
{
    static_assert(sizeof(T) == 2, "return type is not a 2-byte data type");
    uint16_t b0 = *it;
    uint16_t b1 = *(++it);
    uint16_t r = b0 | b1 << 8;
    return reinterpret_cast<T const&>(r);
}

/** Helper function to read a 32 bit value from a little-endian byte stream */
template<typename T, typename InputIterator>
inline T read32_le(InputIterator it)
{
    static_assert(sizeof(T) == 4, "return type is not a 4-byte data type");
    uint32_t b0 = *it;
    uint32_t b1 = *(++it);
    uint32_t b2 = *(++it);
    uint32_t b3 = *(++it);
    uint32_t r = b0 | b1 << 8 | b2 << 16 | b3 << 24;
    return reinterpret_cast<T const&>(r);
}

/** Helper function to read a 64 bit value from a little-endian byte stream */
template<typename T, typename InputIterator>
inline T read64_le(InputIterator it)
{
    static_assert(sizeof(T) == 8, "return type is not a 8-byte data type");
    uint64_t b0 = *it;
    uint64_t b1 = *(++it);
    uint64_t b2 = *(++it);
    uint64_t b3 = *(++it);
    uint64_t b4 = *(++it);
    uint64_t b5 = *(++it);
    uint64_t b6 = *(++it);
    uint64_t b7 = *(++it);
    uint64_t r = b0 | b1 << 8 | b2 << 16 | b3 << 24 | b4 << 32 | b5 << 40 | b6 << 48 | b7 << 56;
    return reinterpret_cast<T const&>(r);
}

/** Helper function to read a 16 bit value from a big-endian byte stream */
template<typename T, typename InputIterator>
inline T read16_be(InputIterator it)
{
    static_assert(sizeof(T) == 2, "return type is not a 2-byte data type");
    uint16_t b1 = *it;
    uint16_t b0 = *(++it);
    uint16_t r = b0 | b1 << 8;
    return reinterpret_cast<T const&>(r);
}

/** Helper function to read a 32 bit value from a big-endian byte stream */
template<typename T, typename InputIterator>
inline T read32_be(InputIterator it)
{
    static_assert(sizeof(T) == 4, "return type is not a 4-byte data type");
    uint32_t b3 = *it;
    uint32_t b2 = *(++it);
    uint32_t b1 = *(++it);
    uint32_t b0 = *(++it);
    uint32_t r = b0 | b1 << 8 | b2 << 16 | b3 << 24;
    return reinterpret_cast<T const&>(r);
}

/** Helper function to read a 64 bit value from a big-endian byte stream */
template<typename T, typename InputIterator>
inline T read64_be(InputIterator it)
{
    static_assert(sizeof(T) == 8, "return type is not a 8-byte data type");
    uint64_t b7 = *it;
    uint64_t b6 = *(++it);
    uint64_t b5 = *(++it);
    uint64_t b4 = *(++it);
    uint64_t b3 = *(++it);
    uint64_t b2 = *(++it);
    uint64_t b1 = *(++it);
    uint64_t b0 = *(++it);
    uint64_t r = b0 | b1 << 8 | b2 << 16 | b3 << 24 | b4 << 32 | b5 << 40 | b6 << 48 | b7 << 56;
    return reinterpret_cast<T const&>(r);
}


}
