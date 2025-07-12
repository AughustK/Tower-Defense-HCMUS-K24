#pragma once
#include <cstdint>
#include <vector>

/// Types of buffs/debuffs
enum class BuffType : uint8_t 
{
    Stun,           // freeze enemy for a duration
    Slow            // slow enemy 
};

/// Definition of a buff: parameters per BuffType
struct BuffDef 
{
    BuffType type;
    float    duration;    // total seconds of effect
    int      tickCount;   // number of ticks (for Slow, subdivisions)
    float    magnitude;   // for Slow: speed multiplier (e.g., 0.5f = 50%), for Stun unused
};

// Data-driven table for all buffs
static constexpr BuffDef BUFF_DEFS[] = 
{
    { BuffType::Stun, 1.0f,  1,  0.0f }, // Mage freeze: 1s stun
    { BuffType::Slow, 2.0f, 10,  0.5f }  // Cannon slow: 2s at 50% speed
};
static_assert(sizeof(BUFF_DEFS) / sizeof(*BUFF_DEFS) == 2, "Update BUFF_DEFS");

struct Buff 
{
    BuffType type;      // which buff
    float    elapsed;   // time elapsed since application
    float    tickTimer; // time since last tick
};

/// Component attached to entities that can be buffed
struct BuffComponent 
{
    std::vector<Buff> buffs;
};

