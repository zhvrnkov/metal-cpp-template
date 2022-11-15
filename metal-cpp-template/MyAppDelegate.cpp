//
//  MyAppDelegate.hpp
//  metal-cpp-template
//
//  Created by Zhavoronkov Vlad on 11/15/22.
//

#include "Renderer.cpp"

class MyAppDelegate : public NS::ApplicationDelegate
{
public:
    ~MyAppDelegate() {
        _pMtkView->release();
        _pWindow->release();
        delete _pRenderer;
    }
    
    NS::Menu* createMenuBar() {
        using NS::StringEncoding::UTF8StringEncoding;
        
        NS::Menu* pMainMenu = NS::Menu::alloc()->init();
        NS::MenuItem* pAppMenuItem = NS::MenuItem::alloc()->init();
        NS::Menu* pAppMenu = NS::Menu::alloc()->init( NS::String::string( "Appname", UTF8StringEncoding ) );
        
        NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
        NS::String* quitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( appName );
        SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* pSender){
            auto pApp = NS::Application::sharedApplication();
            pApp->terminate( pSender );
        } );
        
        NS::MenuItem* pAppQuitItem = pAppMenu->addItem( quitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
        pAppQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
        pAppMenuItem->setSubmenu( pAppMenu );
        
        NS::MenuItem* pWindowMenuItem = NS::MenuItem::alloc()->init();
        NS::Menu* pWindowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );
        
        SEL closeWindowCb = NS::MenuItem::registerActionCallback( "windowClose", [](void*, SEL, const NS::Object*){
            auto pApp = NS::Application::sharedApplication();
            pApp->windows()->object< NS::Window >(0)->close();
        } );
        NS::MenuItem* pCloseWindowItem = pWindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding ) );
        pCloseWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
        
        pWindowMenuItem->setSubmenu( pWindowMenu );
        
        pMainMenu->addItem( pAppMenuItem );
        pMainMenu->addItem( pWindowMenuItem );
        
        pAppMenuItem->release();
        pWindowMenuItem->release();
        pAppMenu->release();
        pWindowMenu->release();
        
        return pMainMenu->autorelease();
    }
    
    virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override {
        NS::Menu* pMenu = createMenuBar();
        NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
        pApp->setMainMenu( pMenu );
        pApp->setActivationPolicy( NS::ActivationPolicy::ActivationPolicyRegular );
    }

    virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override {
        CGRect frame = (CGRect){ {100.0, 100.0}, {512.0, 512.0} };
        
        _pWindow = NS::Window::alloc()->init(
                                             frame,
                                             NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled,
                                             NS::BackingStoreBuffered,
                                             false );
        
//        context = MTLContext();
        
        _pMtkView = MTK::View::alloc()->init( frame, context.device );
        _pMtkView->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
        _pMtkView->setClearColor( MTL::ClearColor::Make( 1.0, 0.0, 0.0, 1.0 ) );
        _pMtkView->setFramebufferOnly(false);
        
        _pRenderer = new Renderer(context);
        _pMtkView->setDelegate( _pRenderer );
        
        _pWindow->setContentView( _pMtkView );
        _pWindow->setTitle( NS::String::string( "00 - Window", NS::StringEncoding::UTF8StringEncoding ) );
        
        _pWindow->makeKeyAndOrderFront( nullptr );
        
        NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
        pApp->activateIgnoringOtherApps( true );
    }
    virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override {
        return true;
    };
    
private:
    NS::Window* _pWindow;
    MTK::View* _pMtkView;
    MTLContext context = MTLContext();
    Renderer* _pRenderer = nullptr;
};
