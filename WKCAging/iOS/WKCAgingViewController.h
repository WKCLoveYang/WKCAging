//
//  WKCAgingViewController.h
//  WKCAgingView
//
//  Created by WeiKunChao on 2019/3/27.
//  Copyright © 2019 SecretLisa. All rights reserved.
//

#import <GLKit/GLKit.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

typedef NS_ENUM(NSInteger, WKCAgingError) {
    /** 创建ES失败*/
    WKCAgingErrorFaceOpenGL,
    /** Demo加载失败*/
    WKCAgingErrorFaceDemo,
    /** 头像创建失败*/
    WKCAgingErrorFaceAvatar,
    /** 脸识别失败*/
    WKCAgingErrorFaceRecogintion,
    /** 脸数据加载失败*/
    WKCAgingErrorFaceObject,
    /** 变老处理失败*/
    WKCAgingErrorFaceAging
};

@protocol WKCAgingViewControllerDelegate <NSObject>

@optional

- (void)agingError:(WKCAgingError)error;
- (void)agingFaceLoaded;

@end

@interface WKCAgingViewController : GLKViewController

@property (nonatomic, weak) id<WKCAgingViewControllerDelegate> agingDelegate;

- (void)loadExample;

- (void)loadImage:(UIImage *)image;

- (void)showAgingProgress:(CGFloat)progress;


@end

#pragma clang diagnostic pop
