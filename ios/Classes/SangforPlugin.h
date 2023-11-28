#import <Flutter/Flutter.h>

@interface SangforPlugin : NSObject<FlutterPlugin>

// FlutterEventSink
@property (nonatomic, strong) FlutterEventSink eventSink;

// 定时器
@property (nonatomic, strong) NSTimer *timer;
@end
