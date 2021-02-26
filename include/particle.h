//
// Created by Johannes Martin on 23.02.21.
//

#ifndef HASHEDOCTREE_PARTICLE_H
#define HASHEDOCTREE_PARTICLE_H

#include <cstdint>
#include <bitset>

// if memory turns out to be the limiting factor here prefer int_least#_t over int_fast#_t
class Particle {

public:
    static constexpr std::uint_fast32_t MASK_COORD2KEY { 0x001FFFFF };
    Particle(std::uint_fast32_t _i_x, std::uint_fast32_t _i_y, std::uint_fast32_t _i_z);
    std::uint64_t getKey(); // using int64_t with fixed size as the key must be exactly 64 bits
    // Coordinate getter
    const std::uint_fast32_t getX() const {return i_x;}
    const std::uint_fast32_t getY() const {return i_y;}
    const std::uint_fast32_t getZ() const {return i_z;}

private:
    std::uint_fast32_t i_x, i_y, i_z;

};

#endif //HASHEDOCTREE_PARTICLE_H
