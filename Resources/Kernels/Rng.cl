// ---- rng.cl ----

// Replaces your RNG class — state is per-thread
typedef struct {
    u64 state;
    u64 inc;
} RNG;

RNG rng_create(u64 seed) {
    RNG rng;
    rng.state = seed;
    rng.inc   = 1442695040888963407ul;
    return rng;
}

u32 rng_next_uint(RNG* rng) {
    u64 oldState  = rng->state;
    rng->state    = oldState * 6364136223846793005ul + rng->inc;
    u32 xorshifted = ((oldState >> 18u) ^ oldState) >> 27u;
    u32 rot        = oldState >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

f32 rng_next_float(RNG* rng, f32 min, f32 max) {
    float r = (float)rng_next_uint(rng) / (float)(0xFFFFFFFFu);
    return min + r * (max - min);
}

s32 rng_next_int(RNG* rng, s32 min, s32 max) {
    return min + (int)(rng_next_uint(rng) % (max - min + 1));
}

float3 rng_next_vec3(RNG* rng, f32 min, f32 max) {
    return (float3)(
        rng_next_float(rng, min, max),
        rng_next_float(rng, min, max),
        rng_next_float(rng, min, max)
    );
}

float3 rng_next_vec3_unit(RNG* rng) {
    while (true) {
        float3 p    = rng_next_vec3(rng, -1.0f, 1.0f);
        float lensq = dot(p, p);  // dot() replaces GetMagnitudeSquared()
        if (1e-10f < lensq && lensq <= 1.0f)
            return p / sqrt(lensq);  // replaces GetMagnitude()
    }
}

float3 rng_next_on_hemisphere(RNG* rng, float3 normal) {
    float3 on_unit_sphere = rng_next_vec3_unit(rng);
    if (dot(on_unit_sphere, normal) > 0.0f)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}