#include "maths.h"
#include "types.h"

static u16 atan2_lookup(f32 y, f32 x) {
    if (x == 0) {
        return arctan_table[0];
    } else {
        return arctan_table[(s32)(y / x * 1024 + 0.5f)];
    }
}
s16 atan2s(f32 y, f32 x) {
    if (x >= 0) {
        if (y >= 0) {
            if (y >= x) {
                return atan2_lookup(x, y);
            } else {
                return 0x4000 - atan2_lookup(y, x);
            }
        } else {
            y = -y;
            if (y < x) {
                return 0x4000 + atan2_lookup(y, x);
            } else {
                return 0x8000 - atan2_lookup(x, y);
            }
        }
    } else {
        x = -x;
        if (y < 0) {
            y = -y;
            if (y >= x) {
                return 0x8000 + atan2_lookup(x, y);
            } else {
                return 0xC000 - atan2_lookup(y, x);
            }
        } else {
            if (y < x) {
                return 0xC000 + atan2_lookup(y, x);
            } else {
                return -atan2_lookup(x, y);
            }
        }
    }
}
/// Copy vector 'src' to 'dest'
void *vec3f_copy(f32 dest[3], f32 src[3]) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    return dest; //! warning: function returns address of local variable
}

/// Set vector 'dest' to (x, y, z)
void *vec3f_set(f32 dest[3], f32 x, f32 y, f32 z) {
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return dest; //! warning: function returns address of local variable
}

s32 approach_s32(s32 current, s32 target, s32 inc, s32 dec) {
    //! If target is close to the max or min s32, then it's possible to overflow
    // past it without stopping.

    if (current < target) {
        current += inc;
        if (current > target) {
            current = target;
        }
    } else {
        current -= dec;
        if (current < target) {
            current = target;
        }
    }
    return current;
}