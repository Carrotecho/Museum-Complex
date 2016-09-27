#ifndef DATA_HOLDER
#define DATA_HOLDER


#import <Foundation/Foundation.h>


@interface DataHolder : NSObject
{
	NSArray*	objectId;
	NSArray*	audio;
	NSArray*	excursionMode;//Main, Audio, 3D, 3DAudio, Short
	NSArray*	caption;
	NSArray*	text;
	NSArray*	descriptions;
	NSArray*	images;
	NSArray*	mapCoords;
}

-(void)FillData:(NSString*)objectID;
-(id)init;

@end


#endif //DATA_HOLDER
