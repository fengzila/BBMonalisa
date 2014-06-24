//
//  AdBannerManager.m
//  puzzle
//
//  Created by FengZi on 14-3-3.
//
//

#import "AdBannerManager.h"
#import "AdBanner.h"

@implementation AdBannerManager

+ (void)show {
    
    UIWindow *window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    if (adBannerView) {
        [adBannerView setFrame:CGRectMake([window bounds].size.width/2 - 320.0/2, 0, 320, 50)];
    }
}

+ (void)hiden{
    if (adBannerView) {
        [adBannerView setFrame:CGRectMake(0, -50, 320, 50)];
    }
}

@end
