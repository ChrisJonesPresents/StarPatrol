#include "AppDelegate.h"
#include "Mode_Spacebattle.h"
#include "Mode_Starscape.h"
#include "Mode_Awayteam.h"
#include "Mode_Explore.h"
#include "ModeMenu.h"
#include "ReferenceLibrary.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	GameManager::FreeInstance();
	ReferenceLibrary::FreeInstance();

}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLViewImpl::create("Star Patrol");
        director->setOpenGLView(glview);
    }
	Director::getInstance()->getOpenGLView()->setDesignResolutionSize(800, 640, ResolutionPolicy::SHOW_ALL);
    // turn on display FPS
  //  director->setDisplayStats(true);

	time_t t; 
	time(&t);
	srand(t);


    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	ReferenceLibrary::getInstance();

	
	//auto scene = Mode_Spacebattle::DebugcreateScene();
	//director->runWithScene(scene);
	
	if (GameManager::getInstance()->Load() == false)
	{
		GameManager::getInstance()->Newgame();
	}

	if (GameManager::getInstance()->getActive()==false)
	{
		auto scene = ModeMenu::createScene();
		//Mode_Awayteam::DebugcreateScene();
		//	Mode_Spacebattle::DebugcreateScene();
		//	Mode_Explore::createDebugScene();
		// run
		director->runWithScene(scene);
	}
	
    // create a scene. it's an autorelease object


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
