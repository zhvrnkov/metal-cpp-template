//
//  Renderer.h
//  metal-cpp-template
//
//  Created by Zhavoronkov Vlad on 11/15/22.
//

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "MTLContext.hpp"
#include "FillKernel.cpp"

class Renderer : public MTK::ViewDelegate
{
public:
    Renderer(MTLContext& context) : context(context) {}
    
    virtual void drawInMTKView( MTK::View* pView ) override {
        
        NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
        
        auto drawable = pView->currentDrawable();
        MTL::CommandBuffer* pCmd = context.commandQueue->commandBuffer();
        fillKernel.encode(pCmd, drawable->texture());
        pCmd->presentDrawable(drawable);
        pCmd->commit();
        
        pPool->release();
    }
    
private:
    MTLContext& context;
    FillKernel fillKernel = FillKernel(context);
};
