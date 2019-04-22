//
//  WKCCreateAvatar.h
//  WKCAgingView
//
//  Created by WeiKunChao on 2019/3/27.
//  Copyright © 2019 SecretLisa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "mptypes.h"

static inline NSString * WKCFutureAgingPath()
{
    NSString * futurePath = [[NSBundle mainBundle] pathForResource:@"Future" ofType:@"bundle"];
    NSString * agingPath = [[futurePath stringByAppendingPathComponent:@"future_data"] stringByAppendingPathComponent:@"aging"];
    return agingPath;
}

static inline NSString * WKCFutureFacePath()
{
    NSString * futurePath = [[NSBundle mainBundle] pathForResource:@"Future" ofType:@"bundle"];
    NSString * facePath = [[futurePath stringByAppendingPathComponent:@"future_data"] stringByAppendingPathComponent:@"face"];
    return facePath;
}

static inline NSString * WKCFutureResPath()
{
    NSString * futurePath = [[NSBundle mainBundle] pathForResource:@"Future" ofType:@"bundle"];
    NSString * resPath = [[futurePath stringByAppendingPathComponent:@"future_data"] stringByAppendingPathComponent:@"res"];
    return resPath;
}

typedef NS_ENUM(NSInteger, WKCAvatarError) {
    /** 初始化工具失败*/
    WKCAvatarErrorRecogintionEngine,
    /** 脸识别失败*/
    WKCAvatarErrorRecogintionFailure,
    /** 头像解析失败*/
    WKCAvatarErrorSynthesizeFailure,
    /** 加载遮罩失败 */
    WKCAvatarErrorMaskFailure
};


@protocol WKCCreateAvatarDelegate <NSObject>

- (void)agingAvatarFinished:(motionportrait::mpFaceObject)faceObject;
- (void)agingAvatarError:(WKCAvatarError)error;

@end

@interface WKCCreateAvatar : NSObject

@property (nonatomic, weak) id<WKCCreateAvatarDelegate> delegate;

- (void)createAvatar:(UIImage *)image;

@end
