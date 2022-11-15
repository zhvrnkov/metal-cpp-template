//
//  Shaders.metal
//  metal-cpp-template
//
//  Created by Zhavoronkov Vlad on 11/15/22.
//

#include <metal_stdlib>
using namespace metal;

kernel void fill(texture2d<float, access::write> destination [[ texture(0) ]],
                 constant const float4& color [[ buffer(0) ]],
                 const uint2 pos [[ thread_position_in_grid ]]) {
    destination.write(color, pos);
}
