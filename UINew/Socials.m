#import <Foundation/Foundation.h>
#import "Socials.h"
#import "SHK.h"
#import "SHKItem.h"
#import "SHKVkontakte.h"
#import "SHKInstagram.h"
#import "SHKiOSFacebook.h"
#import "SHKiOSTwitter.h"
#import "SHKConfiguration.h"
#import "LocalizationSystem.h"


@implementation Socials

-(id)init
{
	self = [super init];
	
	iosLink = @"https://itunes.apple.com/ru/genre/ios/id36?mt=8";
	androidLink = @"https://play.google.com/store";

	[self UpdateLanguage];

	[SHKConfiguration sharedInstanceWithConfigurator:self];
	
	return self;
}

-(void)dealloc
{
	//NOTHING
}

-(void)PostAdvertising: (enum eSocialNetwork)social withScreenshot: (UIImage*)screenshot
{
	SHKItem* item = [SHKItem image:screenshot title:message];
	//SHKItem* item = [SHKItem text: message];
	
	SHKSharer* sharer = nil;
	
	switch ( social )
	{
		case SOCIALS_VK:
			sharer = [[SHKVkontakte alloc] init];
		break;
			
		case SOCIALS_INSTAGRAM:
			sharer = [[SHKInstagram alloc] init];
		break;
			
		case SOCIALS_FACEBOOK:
			sharer = [[SHKiOSFacebook alloc] init];
		break;
			
		case SOCIALS_TWITTER:
			sharer = [[SHKiOSTwitter alloc] init];
		break;
			
		default:
		break;
	}
	
	if ( !sharer )
	{
		NSLog ( @"Socials::PostAdvertising:withScreenshot::Error - Sharer is nil" );
		return;
	}
	
	[sharer loadItem:item];
	[sharer share];
}

-(void)UpdateLanguage
{
	message = AMLocalizedString ( @"Socials_Message", @"" );
}

-(BOOL)schemeAvailable:(NSString*)scheme
{
	UIApplication *application = [UIApplication sharedApplication];
	NSURL* URL = [NSURL URLWithString:scheme];
	
	return [application canOpenURL:URL];
}

-(NSString*)appName
{
	return @"Museum";
}

-(NSString*)appURL
{
	return iosLink;
}

-(NSArray*)defaultFavoriteURLSharers
{
	return [NSArray arrayWithObjects:@"SHKVkontakte", @"SHKInstagram", @"SHKiOSFacebook", @"SHKFacebook", @"SHKTwitter",@"SHKiOSTwitter", nil];
}

-(NSArray*)defaultFavoriteImageSharers
{
	return [NSArray arrayWithObjects:@"SHKVkontakte", @"SHKInstagram", @"SHKiOSFacebook", @"SHKFacebook", @"SHKTwitter",@"SHKiOSTwitter", nil];
}

-(NSArray*)defaultFavoriteTextSharers
{
	return [NSArray arrayWithObjects:@"SHKVkontakte", @"SHKInstagram", @"SHKiOSFacebook", @"SHKFacebook", @"SHKTwitter",@"SHKiOSTwitter", nil];
}

-(NSArray*)defaultFavoriteFileSharers __attribute__( ( deprecated ( "use defaultFavoriteSharersForFile: instead" ) ) )
{
	return [NSArray arrayWithObjects:@"SHKVkontakte", @"SHKInstagram", @"SHKiOSFacebook", @"SHKFacebook", @"SHKTwitter",@"SHKiOSTwitter", nil];
}

-(NSArray*)defaultFavoriteSharersForMimeType:(NSString *)mimeType
{
	return [self defaultFavoriteSharersForFile:nil];
}

-(NSArray*)defaultFavoriteSharersForFile:(SHKFile *)file
{
	return [NSArray arrayWithObjects:@"SHKVkontakte", @"SHKInstagram", @"SHKiOSFacebook", @"SHKFacebook", @"SHKTwitter",@"SHKiOSTwitter", nil];
}

-(NSString*)vkontakteAppId
{
	return @"5104767";
}

-(NSString*)facebookAppId
{
	return @"980257262035014";
}

-(NSNumber*)forcePreIOS6FacebookPosting
{
	return [NSNumber numberWithBool:true];
}

-(NSNumber*)forcePreIOS5TwitterAccess
{
	return [super forcePreIOS5TwitterAccess];
}

-(NSString*)twitterConsumerKey
{
	return @"hxDhwhnDHrTJ4OqQj4QvYbPrp";
}

-(NSString*)twitterSecret
{
	return @"FuvlYq9jZPg2L6LmioKD9RUPeBUqZLdVEF86vIl2CxPD0mg6K3";
}

@end
