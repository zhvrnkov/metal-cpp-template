//
//  MTLContext.cpp
//  metal-cpp-template
//
//  Created by Zhavoronkov Vlad on 11/15/22.
//

#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <Metal/Metal.hpp>

class MTLContext {
public:
    std::string name = "FooBarBaz";
    MTL::Device *device;
    MTL::CommandQueue *commandQueue;
    MTL::Library *library;
    
    MTLContext() : device(MTL::CreateSystemDefaultDevice()) {
        commandQueue = device->newCommandQueue();
        library = device->newDefaultLibrary();
    }
    
    ~MTLContext() {
        library->release();
        commandQueue->release();
        device->release();
    }
    
    MTL::ComputePipelineState *makeComputePipelineState(const std::string functionName) {
        auto fn = NS::String::alloc()->init(functionName.c_str(), NS::StringEncoding::UTF8StringEncoding);
        auto function = library->newFunction(fn);
        fn->release();
        NS::Error *error;
        auto pipelineState = device->newComputePipelineState(function, &error);
        if (error != nil) {
            throw error;
        }
        function->release();
        return pipelineState;
    }
};
