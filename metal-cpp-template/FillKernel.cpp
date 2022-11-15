//
//  FillKernel.cpp
//  metal-cpp-template
//
//  Created by Zhavoronkov Vlad on 11/15/22.
//

#include <stdio.h>
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include "MTLContext.hpp"
#include <simd/simd.h>

struct Kernel {
    
    Kernel(MTLContext& context) : context(context) {};
    ~Kernel() {};
    
    void dispatchThreads(MTL::ComputeCommandEncoder& encoder, MTL::Size size) {
        auto width = pipelineState->threadExecutionWidth();
        auto height = pipelineState->maxTotalThreadsPerThreadgroup() / width;
        MTL::Size tgSize = MTL::Size(width, height, 1);
        encoder.setComputePipelineState(pipelineState);
        encoder.dispatchThreads(size, tgSize);
    }
    
    MTLContext& context;
    MTL::ComputePipelineState* pipelineState = nullptr;
};

struct FillKernel : public Kernel {
    
    FillKernel(MTLContext& context) : Kernel(context) {
        pipelineState = context.makeComputePipelineState("fill");
    }
    
    ~FillKernel() {
        pipelineState->release();
    }
    
    void encode(MTL::CommandBuffer *commandBuffer,
                MTL::Texture* destinationTexture) {
        auto color = simd_make_float4(1.0, 1.0, 0, 1.0);
        NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
        
        auto encoder = commandBuffer->computeCommandEncoder();
        encoder->setTexture(destinationTexture, 0);
        encoder->setBytes(&color, sizeof(color), 0);
        auto size = MTL::Size(destinationTexture->width(), destinationTexture->height(), destinationTexture->depth());
        dispatchThreads(*encoder, size);
        encoder->endEncoding();
        
        pPool->release();
    };
};
