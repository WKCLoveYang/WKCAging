//
//  WKCAgingViewController.m
//  WKCAgingView
//
//  Created by WeiKunChao on 2019/3/27.
//  Copyright © 2019 SecretLisa. All rights reserved.
//

#import "WKCAgingViewController.h"
#include "mptypes.h"
#import <time.h>
#import <sys/timeb.h>
#import "mprender.h"
#import "mpface.h"
#import "mpsynth.h"
#import "mpctlanimation.h"
#import "mpctlitem.h"

#import "WKCImageUtil.h"
#import "WKCCreateAvatar.h"

#define BG_R (1.f)
#define BG_G (1.f)
#define BG_B (1.f)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"


@interface WKCAgingViewController ()
<WKCCreateAvatarDelegate>

{
    // MP instance
    motionportrait::MpRender * _render;
    motionportrait::MpFace   * _face;
    
    // MP controller
    motionportrait::MpCtlAnimation * _ctlAnim;
    motionportrait::MpCtlItem      * _ctlBeard;
    
    // aging mask
    motionportrait::MpCtlItem::ItemId  _agingMaskY;
    motionportrait::MpCtlItem::ItemId  _agingMaskO;
    NSString * _pathAgingMask;
}

@property (nonatomic, strong) EAGLContext * context;

@end

@implementation WKCAgingViewController

- (void)postError:(WKCAgingError)error
{
    dispatch_queue_t mainQueue = dispatch_get_main_queue();
    dispatch_async(mainQueue, ^{
        if (self.agingDelegate && [self.agingDelegate respondsToSelector:@selector(agingError:)])
        {
            [self.agingDelegate agingError:error];
        }
    });
}

- (instancetype)init
{
    if (self = [super init])
    {
        self.view.backgroundColor = nil;
        _pathAgingMask = WKCFutureAgingPath();
        
        [self initContext];
        [self initParameters];
        [self initGesture];
        
    }
    return self;
}

- (void)initContext
{
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!self.context)
    {
        [self postError:WKCAgingErrorFaceOpenGL];
        return;
    }
}

- (void)initParameters
{
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    
    _agingMaskY = 0;
    _agingMaskO = 0;
    
    _render = new motionportrait::MpRender();
    _render -> Init();
    _render -> EnableDrawBackground(true);
}

- (void)initGesture
{
    self.view.userInteractionEnabled = YES;
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPan:)];
    [self.view addGestureRecognizer:pan];
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTap:)];
    [self.view addGestureRecognizer:tap];
}

- (void)initFace
{
    if (!_face) {
        _face = new motionportrait::MpFace();
        _ctlAnim = _face -> GetCtlAnimation();
        _ctlBeard = _face -> GetCtlItem(motionportrait::MpFace::ITEM_TYPE_BEARD);
    }
}

- (void)loadFaceWithFile:(NSString *)file
{
    [EAGLContext setCurrentContext:self.context];
    
    if (_agingMaskY) {
        _ctlBeard -> Destroy(_agingMaskY);
        _agingMaskY = 0;
    }
    
    if (_agingMaskO) {
        _ctlBeard -> Destroy(_agingMaskO);
        _agingMaskO = 0;
    }
    
    int ret = _face -> Load([file UTF8String]);
    if (ret) {
        [self postError:WKCAgingErrorFaceDemo];
        return;
    }
    
    _render -> SetFace(_face);
    
    _ctlAnim -> SetParamf(motionportrait::MpCtlAnimation::NECK_X_MAX_ROT, 2.0f);
    _ctlAnim -> SetParamf(motionportrait::MpCtlAnimation::NECK_Y_MAX_ROT, 2.0f);
    _ctlAnim -> SetParamf(motionportrait::MpCtlAnimation::NECK_Z_MAX_ROT, 0.3f);
}


- (void)loadFaceObject:(motionportrait::mpFaceObject)faceObject
{
    [EAGLContext setCurrentContext:self.context];
    
    if (_agingMaskY) {
        _ctlBeard -> Destroy(_agingMaskY);
        _agingMaskY = 0;
    }
    
    if (_agingMaskO) {
        _ctlBeard -> Destroy(_agingMaskO);
        _agingMaskO = 0;
    }
    
    int ret = _face -> Load(faceObject);
    motionportrait::MpSynth::DestroyFaceBin(faceObject);
    if (ret) {
        [self postError:WKCAgingErrorFaceObject];
        return;
    }
    
    _render -> SetFace(_face);
    
    _ctlAnim -> SetParamf(motionportrait::MpCtlAnimation::NECK_X_MAX_ROT, 2.0f);
    _ctlAnim -> SetParamf(motionportrait::MpCtlAnimation::NECK_Y_MAX_ROT, 2.0f);
    _ctlAnim -> SetParamf(motionportrait::MpCtlAnimation::NECK_Z_MAX_ROT, 0.3f);
}

- (void)loadYourAvatar:(NSValue *)value
{
    [self initFace];
    
    motionportrait::mpFaceObject faceObject = (motionportrait::mpFaceObject)[value pointerValue];
    
    [self loadFaceObject:faceObject];
    [self loadAgingMask:_pathAgingMask];
    
    NSString * stringPath = [WKCFutureAgingPath() stringByAppendingPathComponent:@"faceanim_HourFace.txt"];
    _ctlAnim -> SetExprData((char *)[stringPath UTF8String]);
}

- (void)loadItem:(NSString *)path
             ctl:(motionportrait::MpCtlItem *)ctl
            item:(motionportrait::MpCtlItem::ItemId *)item
{
    if (*item) {
        ctl -> UnsetItem(*item);
        ctl -> Destroy(*item);
        *item = 0;
    }
    if (path == NULL) return;
    
    *item = ctl -> Create((char *)[path UTF8String]);
    if (*item == 0) {
        [self postError:WKCAgingErrorFaceAging];
        return;
    }
    
    ctl -> SetItem(*item);
}

- (void)loadAgingMask:(NSString *)maskPath
{
    NSString *maskY = [maskPath stringByAppendingPathComponent:@"mask0_young"];
    NSString *maskO = [maskPath stringByAppendingPathComponent:@"mask0_old"];
    
    [self loadItem:maskY ctl:_ctlBeard item:&_agingMaskY];
    [self loadItem:maskO ctl:_ctlBeard item:&_agingMaskO];
    
    _ctlBeard -> SetAlpha(_agingMaskY, 0.0f);
    _ctlBeard -> SetAlpha(_agingMaskO, 0.0f);
}

- (long)qn_getmsec
{
    static bool first = true;
    static double start;
    double now;
    struct timeb time;
    
    ftime(&time);
    now = (double)time.time * 1000 + time.millitm;
    if(first) {
        start = now;
        first = false;
    }
    return (long)(now - start);
}

#pragma mark -OutsideMethod
- (void)loadExample
{
    [self view];
    
    [self initFace];
    
    NSString * stringPath = [WKCFutureFacePath() stringByAppendingPathComponent:@"face0.bin"];
    [self loadFaceWithFile:stringPath];
    
    stringPath = WKCFutureAgingPath();
    [self loadAgingMask:stringPath];
    
    stringPath = [WKCFutureAgingPath() stringByAppendingPathComponent:@"faceanim_HourFace.txt"];
    _ctlAnim -> SetExprData((char *)[stringPath UTF8String]);
    
    [self showAgingProgress:0.5];
    
    if (self.agingDelegate && [self.agingDelegate respondsToSelector:@selector(agingFaceLoaded)])
    {
        dispatch_queue_t mainQueue = dispatch_get_main_queue();
        dispatch_async(mainQueue, ^{
            [self.agingDelegate agingFaceLoaded];
        });
    }
}


- (void)loadImage:(UIImage *)image
{
    [self view];
    [self initFace]; //不要Demo时单独加载
    
    image = [WKCImageUtil fixrotation:image];
    
    if (!image) {
        return;
    }
    
    WKCCreateAvatar * genAvatar;
    genAvatar = [[WKCCreateAvatar alloc] init];
    genAvatar.delegate = self;
    [genAvatar createAvatar:image];
    
    [self showAgingProgress:0.5];
}

- (void)showAgingProgress:(CGFloat)progress
{
    if (_agingMaskY == 0 || _agingMaskO == 0) return;
    
    float curAge = MAX(0, MIN(1, progress));
    
    float gainY = (curAge < 0.5f) ? (0.5f - curAge) * 2.0f : 0.0f;
    float gainO   = (curAge < 0.5f) ? 0.0f : (curAge - 0.5f) * 2.0f;
    
    _ctlBeard -> SetAlpha(_agingMaskY, gainY);
    _ctlBeard -> SetAlpha(_agingMaskO, gainO);
    
    float gains[32];
    for (int i = 0; i < 32; i++) gains[i] = 0.0f;
    static const int SLOT_YOUNG = 14;
    static const int SLOT_OLD   = 13;
    gains[SLOT_YOUNG] = gainY;
    gains[SLOT_OLD]   = gainO;
    int msec = 10;
    float weight = 1.0f;
    
    _ctlAnim -> Express(msec, gains, weight);
}

#pragma mark -WKCCreateAvatarDelegate
- (void)agingAvatarFinished:(motionportrait::mpFaceObject)faceObject
{
    if (faceObject) {
        [self performSelectorOnMainThread:@selector(loadYourAvatar:)
                               withObject:[NSValue valueWithPointer:faceObject]
                            waitUntilDone:NO];
    }
    
    if (self.agingDelegate && [self.agingDelegate respondsToSelector:@selector(agingFaceLoaded)])
    {
        dispatch_queue_t mainQueue = dispatch_get_main_queue();
        dispatch_async(mainQueue, ^{
            [self.agingDelegate agingFaceLoaded];
        });
    }
}

- (void)agingAvatarError:(WKCAvatarError)error
{
    if (error == WKCAvatarErrorRecogintionFailure)
    {
        [self postError:WKCAgingErrorFaceRecogintion];
    }
    else
    {
        [self postError:WKCAgingErrorFaceAvatar];
    }
}

#pragma mark -GLMethod
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if (!_face) {
        return;
    }
    
    [EAGLContext setCurrentContext:self.context];
    
    int width = 0;
    int height = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,  &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    int screen = width;
    
    motionportrait::mpRect viewport;
    viewport.x = 0;
    viewport.y = height - width;
    viewport.width  = screen;
    viewport.height = screen;
    _render -> SetViewport(viewport);
    
    motionportrait::MpCtlAnimation *anim = _face -> GetCtlAnimation();
    long cTime = [self qn_getmsec];
    
    glClearColor(BG_R, BG_G, BG_B, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    anim -> Update(cTime);
    _render -> Draw();
}


#pragma mark -GestureMethod
- (void)onPan:(UIPanGestureRecognizer *)pan
{
    if (!_face) {
        return;
    }
    
    switch (pan.state) {
        case UIGestureRecognizerStateBegan:
        case UIGestureRecognizerStateChanged:
        {
            motionportrait::mpVector2 pos;
            CGRect r = [self.view bounds];
            
            CGPoint location = [pan locationInView:self.view];
            pos.x = location.x / r.size.width;
            pos.y = 1.0f - location.y / r.size.height;
            _ctlAnim -> LookAt(0, pos, 1.0f);
        }
            break;
        case UIGestureRecognizerStateEnded:
        case UIGestureRecognizerStateCancelled:
        {
            motionportrait::mpVector2 pos = {0.5f, 0.5f};
            _ctlAnim -> LookAt(500, pos, 1.0f);
        }
            break;
        default:
            break;
    }
}

- (void)onTap:(UITapGestureRecognizer *)tap
{
    if (!_face) {
        return;
    }
    
    motionportrait::mpVector2 pos;
    CGRect r = [self.view bounds];
    
    CGPoint location = [tap locationInView:self.view];
    pos.x = location.x / r.size.width;
    pos.y = 1.0f - location.y / r.size.height;
    _ctlAnim -> LookAt(50, pos, 1.0f);
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)),
                   dispatch_get_main_queue(),
                   ^{
                       motionportrait::mpVector2 pii;
                       pii.x = 0.5f;
                       pii.y = 0.5f;
                       self -> _ctlAnim -> LookAt(500, pii, 1.0f);
                   });
}

#pragma mark - Dealloc
- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    if (_render) {
        delete _render;
    }
    
    if (_face) {
        delete _face;
    }
    
    NSLog(@">>>>>>>>>>>>>>>>>>>变老被销毁了");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    NSLog(@">>>>>>>>>>>>>>>>>>>内存警告");
}


@end

#pragma clang diagnostic pop
