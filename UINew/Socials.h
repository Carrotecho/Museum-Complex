#ifndef SOCIALS
#define SOCIALS


#import <UIKit/UIKit.h>
#import "DefaultSHKConfigurator.h"


enum eSocialNetwork
{
	SOCIALS_VK,
	SOCIALS_INSTAGRAM,
	SOCIALS_FACEBOOK,
	SOCIALS_TWITTER
};

@interface Socials : DefaultSHKConfigurator
{
	NSString*	iosLink;
	NSString*	androidLink;
	NSString*	message;
}

-(id)init;
-(void)dealloc;

-(void)PostAdvertising: (enum eSocialNetwork)social withScreenshot: (UIImage*)screenshot;
-(void)UpdateLanguage;

-(NSString*)appName;
-(NSString*)appURL;

-(NSArray*)defaultFavoriteURLSharers;
-(NSArray*)defaultFavoriteImageSharers;
-(NSArray*)defaultFavoriteTextSharers;
-(NSArray*)defaultFavoriteFileSharers __attribute__( ( deprecated ( "use defaultFavoriteSharersForFile: instead" ) ) );
-(NSArray*)defaultFavoriteSharersForMimeType:(NSString *)mimeType __attribute__ ( ( deprecated ( "use defaultFavoriteSharersForFile: instead" ) ) );
-(NSArray*)defaultFavoriteSharersForFile:(SHKFile *)file;

-(NSString*)vkontakteAppId;

-(NSString*)facebookAppId;

-(NSString*)twitterConsumerKey;
-(NSString*)twitterSecret;


@end


#endif //SOCIALS
