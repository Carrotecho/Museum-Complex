//
//  MainController.m
//  UINew
//
//  Created by  Roman on 24.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "MainController.h"
#import "DataSenderServer.h"

@implementation Exhibit

-(id)init
{
	self->parentId = nil;
	self->floor = 0;
	self->Id = nil;
	self->state = NOT_VISITED;
	return self;
}

@end

@implementation MainController

@synthesize objectId;
@synthesize objectList;
@synthesize mapImages;
@synthesize currentFloor;
@synthesize description;
@synthesize details;
@synthesize images;
@synthesize audio;
@synthesize caption;
@synthesize currentLang;
@synthesize text;
@synthesize descriptions;
@synthesize excursionMode;
@synthesize index;
@synthesize prevObjectId;
@synthesize mapCoords;
@synthesize lastDetectedIndex;
@synthesize detectedId;
@synthesize globalObjectList;
@synthesize objectStack;
@synthesize hasNestedObjects;
@synthesize socials;

+(id) sharedController
{
	static MainController *sharedCntrl = nil;
	static dispatch_once_t onceToken;
	dispatch_once ( &onceToken, ^{
		sharedCntrl = [[self alloc] init];
	});
	
	return sharedCntrl;
}

-(id)init
{
	if ( self =[super init] )
	{
		objectId = nil;
		currentLang = 0;
		prevObjectId = nil;
		lastDetectedIndex = -1;
        globalObjectList = nil;
		currentFloor = 1;

		mapImages =	@[
						@"(podval)-granovitaja-palata-11.jpg",
						@"1-spasa-preobrazhenija-na-Iline.jpg",
						@"2-spasa-preobrazhenija-na-Iline.jpg",
						@"(ioanovsky-korpus)-granovitaja-palata-14.jpg"
					];
		
		images =	@[
						@"KremlinMap.png",
						@"GranovitayaPalata.png"
					];
		
	}
    isXMLContentLoaded = false;
	holder = [[DataHolder alloc] init];
    _dataSize = [[NSMutableDictionary alloc]init];
    _markerIdObjectId= [[NSMutableDictionary alloc]init];
    objectStack = [NSMutableArray array];
	
	socials = [[Socials alloc] init];
	self.socials = socials;
    
    linksToDownload = [[NSMutableArray alloc] init];
    
    senderRequest = [[DataSenderServer alloc] init];
    _downloadObjects = [[NSMutableArray alloc] init];

    
	return self;
}

-(void)retrieve3Dlinks
{
            [senderRequest send:[self createReqString:@"" data:nil] callBackTarget:self callBackAction:@selector(gettedContentLinks:) callBackActionFail:@selector(failGettedContentLinks:) succesParrams:[NSNumber numberWithInt:1] cookies:nil];
};

-(void)createXMlContent:(NSString*)textXML {
    isXMLContentLoaded = true;
    contentXMLObj.load([textXML UTF8String]);
    
    
    std::string xpathParent = "/content/object";
    
    enum exhibitState state = NOT_VISITED;
    
    pugi::xpath_node_set parents = contentXMLObj.select_nodes(xpathParent.c_str());
    if (!globalObjectList)
    {
        globalObjectList = [NSArray array];
    }
    
    switch (currentLang)
    {
        case 0:
            LocalizationSetLanguage(@"ru");
            break;
        case 1:
            LocalizationSetLanguage(@"en");
            break;
    }
    BOOL dataExists = NO;
    if (globalObjectList.count > 0)
    {
        dataExists = YES;
    }
    NSInteger i = 0;
    
    for (pugi::xpath_node_set::const_iterator it = parents.begin(); it != parents.end(); ++it)
    {
        pugi::xpath_node node = *it;
        Exhibit* exhibit;
        
        if (!dataExists)
        {
            exhibit = [[Exhibit alloc] init];
            
            exhibit->Id = [NSString stringWithUTF8String:node.node().attribute("id").value()];
            
            exhibit->state = state;
            
            float dx  = 0.0;
            float dy  = 0.0;
            int floor = 0;
            bool hasNestedObj = false;
            
            NSString*parentID = @"";
            
            if (!node.node().child("coordX").empty()) {
                dx = node.node().child("coordX").text().as_float();
                dy = node.node().child("coordY").text().as_float();
            }
            
            //if (!node.node().child("coordX").empty())
            {
                floor = node.node().child("floor").text().as_int();
            }
            
            
            if (!node.node().attribute("parentId").empty()) {
                parentID = [NSString stringWithUTF8String:node.node().attribute("parentId").as_string()];
            }
            
            if (!node.node().attribute("hasNestedObjects").empty()) {
                hasNestedObj = node.node().attribute("hasNestedObjects").as_bool();
            }
             exhibit->isUnesco = false;
            if (!node.node().attribute("unesco").empty()) {
                bool isUnesco = node.node().attribute("unesco").as_bool();
                if (isUnesco)
                {
                    exhibit->isUnesco = true;
                }
            }
            
            exhibit->hasMarker = false;
            exhibit->markerId = @"";
           
            
            fastMarkerObject[node.node().attribute("id").as_int()] = false;
            if (!node.node().child("markerId").empty()) {
                if (!node.node().child("markerId").text().empty()) {
                    fastMarkerObject[node.node().attribute("id").as_int()] = true;
                    exhibit->markerId = [NSString stringWithUTF8String: node.node().child("markerId").text().as_string()];
                    exhibit->hasMarker = true;
                }
                
            }
            
            exhibit->coord.dx = dx;
            exhibit->coord.dy = dy;
            exhibit->floor = floor;
            exhibit->parentId = parentID;
            exhibit->hasNestedObjects = hasNestedObj;
            NSString* icoPath = @"";
            if (!node.node().child("ico").text().empty()) {
                
                icoPath = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String:node.node().child("ico").text().as_string()]];
                icoPath = [CacheControll getLocalPathFromLocation:icoPath];
            }
            
            
            exhibit->icon = icoPath;
        }
        else
        {
            exhibit = globalObjectList[i];
        }
        
        NSString *detailsExhibit = @"";
        if (!node.node().child("details").empty())
        {
            detailsExhibit = [NSString stringWithUTF8String:node.node().child("details").text().as_string()];
        }
        
        NSString*captionF = @"";
        
        
        pugi::xml_node textNode = node.node().child("textid");
        
        if (!textNode.text().empty()) {
            captionF = [NSString stringWithUTF8String:textNode.text().as_string()];
        }
        
        exhibit->caption = AMLocalizedString(captionF ,@"");//[NSString stringWithUTF8String:node.node().child("floor").text().as_string()];
        
        exhibit->details = AMLocalizedString(detailsExhibit ,@"");
        
        
        if (!dataExists)
        {
            globalObjectList = [globalObjectList arrayByAddingObject:exhibit];
        }
        i++;
        
    }
    
    xpathParent = "/content/fileContent/file";

    parents = contentXMLObj.select_nodes(xpathParent.c_str());
    for (pugi::xpath_node_set::const_iterator it = parents.begin(); it != parents.end(); ++it)
    {
        pugi::xpath_node node = *it;
        NSString * path = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String: node.node().attribute("path").as_string()]];
        int size =  node.node().attribute("size").as_int();
        [_dataSize setObject:[NSNumber numberWithInt:size] forKey:path];
    }

}

-(BOOL)Determine3DStatus
{
    if ([self.excursionMode isEqualToString:@"Short"])
    {
        return false;
    }
    
    if (self.isUnesco)
    {
        return  false;
    }
    
    NSArray *churchesIds = [NSArray arrayWithObjects: @"1", @"32", @"93", @"61_0", @"270", @"409", nil];
    if ([churchesIds containsObject:self.parentId])
    {
        return false;
    }
    
    if ([churchesIds containsObject:self.objectId])
    {
        return false;
    }
    
    return true;
};

-(void)CreateDownloadableObjectArray
{
    
    if (![_downloadObjects count])
    {
        DownloadableObject *obj1 = [[DownloadableObject alloc] initWithData:[self getAllResourcesForID:@"151"] ];
        obj1.name = @"GranovitayaPalata";
        
        [_downloadObjects addObject:obj1];
        
        DownloadableObject *obj2 = [[DownloadableObject alloc] initWithData: [self getAllResourcesForID:@"152"]];
        obj2.name = @"ZdaniePrisutMest";
        
        [_downloadObjects addObject:obj2];
        
        DownloadableObject *obj3 = [[DownloadableObject alloc] initWithData: [self getAllResourcesForID:@"999"]];
        
        obj3.name = @"KremlinMap";
        [_downloadObjects addObject:obj3];
        
        DownloadableObject *obj4 = [[DownloadableObject alloc] initWithData: [self getAllResourcesForID:@"1"]];
        
        obj4.name = @"CerkovNaIline";
        [_downloadObjects addObject:obj4];
        
        DownloadableObject *obj5 = [[DownloadableObject alloc] initWithData: [self getAllResourcesForID:@"32"]];
        obj5.name = @"CerkovNaNeredice";
        [_downloadObjects addObject:obj5];
        
        DownloadableObject *obj6 = [[DownloadableObject alloc] initWithData: [self getAllResourcesForID:@"93"]];
        obj6.name =  @"CerkovNaKovaleve";
        [_downloadObjects addObject:obj6];
        
        DownloadableObject *obj7 = [[DownloadableObject alloc] initWithData: [self getAllResourcesForID:@"61_0"]];
        obj7.name = @"CerkovVKoshevnikah";
        [_downloadObjects addObject:obj7];
    }
}

-(bool*)getChecker {
    return fastMarkerObject;
}

-(NSString*)getExcursionMode:(pugi::xml_node)node {
    
    
    if (!node.attribute("main").empty()) {
        return @"Main";
    }else if (!node.child("markerID").text().empty()) {
        if (!node.child("soundRus").text().empty()) {
            return @"3DAudio";
        }
        return @"3D";
    }
    else if (!node.child("soundRus").text().empty()) {
        return @"Audio";
    }
    if (!node.attribute("parentId").empty())
    {
        NSString *parentID = [NSString stringWithUTF8String:node.attribute("parentId").as_string()];
        if ( [parentID isEqualToString:@"1000"] || [parentID isEqualToString:@"999"])
        return  @"Short";
    }
    
    return @"3D";
};



-(pugi::xml_node)getNodeByID:(NSString*)nodeId {
    
    std::string xpathParent = "/content/object[@id='";
    xpathParent+=[nodeId UTF8String];
    xpathParent+="']";
    
    pugi::xpath_node_set object = contentXMLObj.select_nodes(xpathParent.c_str());
    
    pugi::xpath_node_set::const_iterator it = object.begin();
    pugi::xpath_node node = *it;
    
    return node.node();
}



-(void)fillImageData:(pugi::xml_node)node {
    
    NSMutableArray* imagePaths = [[NSMutableArray alloc] init];
    NSMutableArray* imagePathsO = [[NSMutableArray alloc] init];
    
    NSMutableArray* imageDescr = [[NSMutableArray alloc] init];
    for (pugi::xml_node image = node.child("image"); image; image = image.next_sibling("image")) {
        NSString*imagePath = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String:image.child("path").text().as_string()]];
        
        imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
        imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
        
        [imagePathsO addObject:imagePath];
        [imagePaths addObject:[CacheControll getLocalPathFromLocation:imagePath]];
        [imageDescr addObject:AMLocalizedString([NSString stringWithUTF8String:image.child("textid").text().as_string()],@"")];
    }
    self.descriptions = [imageDescr copy];
    self.imagesO = [imagePathsO copy];
    self.images = [imagePaths copy];
}


-(CGVector)getMapCoord:(pugi::xml_node)node {
    float dx = 0.0;
    float dy = 0.0;
    
    if (!node.child("coordX").empty()) {
        dx = node.child("coordX").text().as_float();
        dy = node.child("coordY").text().as_float();
    }
    return CGVectorMake(dx,dy);
}




-(void)fillMapImages:(pugi::xml_node)node {
    
    NSMutableArray* arrayResources = [[NSMutableArray alloc] init];
    if (!node.child("mapImages").text().empty()) {
        NSString* mapsNames = [NSString stringWithUTF8String: node.child("mapImages").text().as_string()];
        NSArray *myWords = [mapsNames componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@":"]];
        
        
        for (int i = 0; i < [myWords count]; i++) {
            NSString*mapPath = [self.serverPath stringByAppendingString:[myWords objectAtIndex:i]];
            [arrayResources addObject:[CacheControll getLocalPathFromLocation:mapPath]];
        }
        
        self.mapImages = arrayResources;
    }
    
    
    
    
}


-(void)preloadResForID:(NSString*)idNeedle objectNode:(pugi::xml_node)node
{
    NSMutableArray* arrayResources = [[NSMutableArray alloc] init];
    
    
    if (!node.child("mapImages").text().empty()) {
        NSString* mapsNames = [NSString stringWithUTF8String: node.child("mapImages").text().as_string()];
        NSArray *myWords = [mapsNames componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@":"]];
        for (int i = 0; i < [myWords count]; i++) {
            NSString*mapPath = [self.serverPath stringByAppendingString:[myWords objectAtIndex:i]];
            if (![CacheControll isCachedAtPath:mapPath]) {
                mapPath = [mapPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
                mapPath = [mapPath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
                [arrayResources addObject:mapPath];
            }
        }
    }
    
    
    for (pugi::xml_node image = node.child("image"); image; image = image.next_sibling("image")) {
        
        NSString* imagePath = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String:image.child("path").text().as_string()]];
        
        if (![CacheControll isCachedAtPath:imagePath]) {
            imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            [arrayResources addObject:imagePath];
        }
    }
    
    std::string xpathParent = "/content/object[@parentId='";
    xpathParent+=[idNeedle UTF8String];
    xpathParent+="']";
    
    pugi::xpath_node_set parents = contentXMLObj.select_nodes(xpathParent.c_str());
    
    
    for (pugi::xpath_node_set::const_iterator it = parents.begin(); it != parents.end(); ++it)
    {
        pugi::xpath_node node = *it;
        
        for (pugi::xml_node image = node.node().child("ico"); image; image = image.next_sibling("ico")) {
            
            
            NSString* ico = [NSString stringWithUTF8String:image.text().as_string()];
            NSString* imagePath = [self.serverPath stringByAppendingString:ico];
            imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            if (![CacheControll isCachedAtPath:imagePath]) {
                [arrayResources addObject:imagePath];
            }
        }
    }
    
    

    
    if (self.audio) {
        NSString* soundPath = [self.serverPath stringByAppendingString:self.audio];
        
        self.audio = [CacheControll getLocalPathFromLocation:soundPath];
        
        if (![CacheControll isCachedAtPath:soundPath]) {
            soundPath = [soundPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            soundPath = [soundPath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            [arrayResources addObject:soundPath];
        }
    }
    
    if ([arrayResources count]) {
        
        [self fillPreloader];
        
        
        Loader*loader = [[Loader alloc] init];
        loader.delegate = self;
        [loader load:arrayResources];
        
        //[self.mainNavController.view setUserInteractionEnabled:NO];
    }else {
        [self updateLoaderDelegate:100.0 sender:nil];
    }
    
}

-(void)failGettedContentLinks:(id)data {
    
}

-(NSString*)createReqString:(NSString*)req data:(NSArray*)data {
    NSLog(@"req send to server");
    NSString * baseString = [NSString stringWithFormat:@"%@/contentGetter.php?request=%@",@"http://f.devar.org/",req];
    if (data) {
        NSMutableString *s = [[NSMutableString alloc] initWithString:baseString] ;
        for (int i = 0; i < [data count]; i++) {
            
            NSString*param = [data objectAtIndex:i];
            
            NSCharacterSet *set = [NSCharacterSet URLQueryAllowedCharacterSet];
            NSString *result = [param stringByAddingPercentEncodingWithAllowedCharacters:set];
            
            
            [s appendFormat:@"&d%d=%@",i,result];
        }
        
        return s;
    }
    return baseString;
}



-(void)gettedContentLinks:(NSArray*)data {
    
    NSString*paths = [data objectAtIndex:0];
    
    NSArray *myWords = [paths componentsSeparatedByCharactersInSet:
                        [NSCharacterSet characterSetWithCharactersInString:@":"]
                        ];
    
    for (int i = 0; i < [myWords count]; i++)
    {
        
        NSString*realPath = [@"http://f.devar.org/" stringByAppendingString:[myWords objectAtIndex:i]];
       //   if (![CacheControll isCachedAtPath:realPath])
        {
            
            realPath = [realPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            realPath = [realPath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            [linksToDownload addObject:realPath];
        }
    }
    
    [self CreateDownloadableObjectArray];
    
}


-(NSMutableArray*)getResourcesForID:(NSString*)idNeedle
{

    NSMutableArray* arrayResources = [[NSMutableArray alloc] init];

    pugi::xml_node node = [self getNodeByID:idNeedle];
    
    if (!node.child("mapImages").text().empty())
    {
        NSString* mapsNames = [NSString stringWithUTF8String: node.child("mapImages").text().as_string()];
        NSArray *myWords = [mapsNames componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@":"]];
        for (int i = 0; i < [myWords count]; i++) {
            NSString*mapPath = [self.serverPath stringByAppendingString:[myWords objectAtIndex:i]];
            mapPath = [mapPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            mapPath = [mapPath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            mapPath = [mapPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
            //if (![CacheControll isCachedAtPath:mapPath])
            {

                [arrayResources addObject:mapPath];
            }
        }
    }
    
    if (!node.child("markerId").text().empty())
    {
        NSString *markerId = [NSString stringWithUTF8String:node.child("markerId").text().as_string()];
        for (int i = 0; i < [linksToDownload count]; i++)
        {
            NSString *tmp = linksToDownload[i];
            if ( ([tmp rangeOfString:markerId].location != NSNotFound ) && (([tmp rangeOfString:@".dataModified"].location == NSNotFound )))
            {
                //if (![arrayResources containsObject:linksToDownload[i]])
                {
                    [arrayResources addObject:linksToDownload[i]];
                }
            }
                
        }
    }
     NSString *ico = @"";
    
    if (!node.child("ico").text().empty())
    {
        ico = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String:node.child("ico").text().as_string()]];
        if (![ico isEqualToString:@""])
        {
            ico = [ico stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            ico = [ico stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            ico = [ico stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
            //if (![CacheControll isCachedAtPath:ico])
            {

                [arrayResources addObject:ico];
            }
            
        }
    }
   
    
    for (pugi::xml_node image = node.child("image"); image; image = image.next_sibling("image"))
    {
        
        NSString* imagePath = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String:image.child("path").text().as_string()]];
        
        imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
        imagePath = [imagePath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
        imagePath = [imagePath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        //if (![CacheControll isCachedAtPath:imagePath])
        {

            NSString *fullImageName = [imagePath stringByReplacingOccurrencesOfString:@".jpg" withString:@"_full.jpg"];
            [arrayResources addObject:imagePath];
            [arrayResources addObject:fullImageName];
        }
    }
    
    NSString *soundRusPath = @"";
    NSString *soundEngPath = @"";
    
    if (currentLang == 0)
    {
        {
            if (!node.child("soundRus").text().empty())
            {
                soundRusPath = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String:node.child("soundRus").text().as_string()]];
            }
        }
        if (![soundRusPath isEqualToString:@""])
        {
            soundRusPath = [soundRusPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            soundRusPath = [soundRusPath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            soundRusPath = [soundRusPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
           // if (![CacheControll isCachedAtPath:soundRusPath])
            {

                [arrayResources addObject:soundRusPath];
            }
            
        }
    }
    else if (currentLang == 1)
    {
        if (!node.child("soundEng").text().empty())
        {
            soundEngPath = [self.serverPath stringByAppendingString:[NSString stringWithUTF8String:node.child("soundEng").text().as_string()]];
        }
        if (![soundEngPath isEqualToString:@""])
        {
            soundEngPath = [soundEngPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
            soundEngPath = [soundEngPath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
            soundEngPath = [soundEngPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
          //  if (![CacheControll isCachedAtPath:soundEngPath])
            {
                [arrayResources addObject:soundEngPath];
            }
            
        }
        
    }
    return arrayResources;
}


-(NSMutableArray*)getAllResourcesForID:(NSString*)idNeedle
{
    NSMutableArray* arrayResources = [[NSMutableArray alloc] init];
    
    arrayResources = [self getResourcesForID:idNeedle];
    
    std::string xpathParent = "/content/object[@parentId='";
    xpathParent+=[idNeedle UTF8String];
    xpathParent+="']";
    
    pugi::xpath_node_set parents = contentXMLObj.select_nodes(xpathParent.c_str());
    
     for (pugi::xpath_node_set::const_iterator it = parents.begin(); it != parents.end(); ++it)
     {
         pugi::xpath_node node = *it;
         NSString * objId = @"";
         if (!node.node().attribute("id").empty())
         {
             objId = [NSString stringWithUTF8String:node.node().attribute("id").as_string()];
             [ arrayResources addObjectsFromArray:[self getResourcesForID:objId ]];
             if ( (!node.node().attribute("hasNestedObjects").empty()))
             {
                 xpathParent = "/content/object[@parentId='";
                 xpathParent+=[objId UTF8String];
                 xpathParent+="']";
                 
                  pugi::xpath_node_set parents = contentXMLObj.select_nodes(xpathParent.c_str());
                 
                 for (pugi::xpath_node_set::const_iterator it = parents.begin(); it != parents.end(); ++it)
                 {
                     pugi::xpath_node node = *it;
                     NSString * objId = @"";
                     if (!node.node().attribute("id").empty())
                     {
                         objId = [NSString stringWithUTF8String:node.node().attribute("id").as_string()];
                         [ arrayResources addObjectsFromArray:[self getResourcesForID:objId ]];
                     }

                 }

             }
             
         }
         
     }
    NSOrderedSet *orderedSet = [NSOrderedSet orderedSetWithArray:arrayResources];
    return [NSMutableArray arrayWithArray:[orderedSet array]];
}


-(void)fillPreloader {
    backgroundProgress = [[UIView alloc] initWithFrame:self.mainNavController.view.frame];
    backgroundProgress.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    [self.mainNavController.view addSubview:backgroundProgress];
    
    progressLoadedContent = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
    progressLoadedContent.progress = 0;
    [progressLoadedContent setFrame:CGRectMake(self.mainNavController.view.frame.size.width/2 - 100, self.mainNavController.view.frame.size.height/2-25, 200, 50)];
    [self.mainNavController.view addSubview:progressLoadedContent];
}

-(void)retrieveMarkerId
{
    NSMutableArray *groupArray = [[NSMutableArray alloc]init];
    NSMutableArray *objArray = [[NSMutableArray alloc] init];
    
    for (int i = 0 ; i<[globalObjectList count]; i++)
    {
        Exhibit *exhibit = globalObjectList[ i ];
        if (exhibit->hasMarker)
        {
            if (exhibit->hasNestedObjects)
            {
                [groupArray addObject:exhibit];
            }
            else
            {
                [objArray addObject:exhibit];
            }
        }
    }
    
    for (int i = 0; i<[groupArray count]; i++)
    {
        Exhibit *exhibit = groupArray[ i ];
        [_markerIdObjectId setObject:exhibit->Id forKey:exhibit->markerId];
    }
    
    for (int i = 0; i<[objArray count]; i++)
    {
        Exhibit *exhibit = objArray[ i ];
        
        if ([_markerIdObjectId objectForKey:exhibit->markerId] == nil)
        {
            [_markerIdObjectId setObject:exhibit->Id forKey:exhibit->markerId];
        }
    }
    
}

-(void)loadFiles:(NSArray*)paths sender:(id<MainControllerDelegate>)sender {
    self.senderForCallbackLoad = sender;
    
    [self fillPreloader];
    NSMutableArray * res = [[NSMutableArray alloc] init];
    NSString *path;
    for (path in paths)
    {
        path = [path stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
        path = [path stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
        [res addObject:path];
    }
    
    Loader*loader = [[Loader alloc] init];
    loader.delegate = self;
    [loader load:res];
    
}

-(bool)taskWithError:(NSURL *)errorurl {
    [progressLoadedContent removeFromSuperview];
    [backgroundProgress removeFromSuperview];
    return false;
}


-(void)updateLoaderDelegate:(double)progress sender:(id)sender {
  
    
    
    if (progress >= 100.0) {
        
        if (self.senderForCallbackLoad && [self.senderForCallbackLoad respondsToSelector:@selector(loadEnd)]) {
            [self.senderForCallbackLoad loadEnd];
        }
        //[self.mainNavController.view setUserInteractionEnabled:YES];
        [progressLoadedContent removeFromSuperview];
        [backgroundProgress removeFromSuperview];
    }else {
        float progressF = progress/100.0;
        [progressLoadedContent setProgress:progressF];
    }
    
    
}

//sets appropriate fields when object id is updated
-(void)ObjectIdUpdated:(id<MainControllerDelegate>)sender
{
	//TBD
    
    //[self.mainNavController.view setUserInteractionEnabled:YES];
    if (!isXMLContentLoaded) {
        return;
    }
    
    if (!self.objectId) {
        return;
    }
    
    self.senderForCallbackLoad = sender;
    
    pugi::xml_node node = [self getNodeByID:self.objectId];
    
    
    
    
    // here need load pack images )
    
    
    
    
    
    
    self.objectId = self.objectId;
    
    if (currentLang == 0) {
        if (!node.child("soundRus").text().empty()) {
            self.audio = [NSString stringWithUTF8String:node.child("soundRus").text().as_string()];
        }else {
            self.audio = nil;
        }
        
    }else if (currentLang == 1){
        if (!node.child("soundEng").text().empty()) {
            self.audio = [NSString stringWithUTF8String:node.child("soundEng").text().as_string()];
        }else {
            self.audio = nil;
        }
    }

    
    NSString*captionF = @"";
    NSString*fullText = @"";
    
    
    pugi::xml_node textNode = node.child("textid");
    
    if (!textNode.text().empty()) {
        captionF = [NSString stringWithUTF8String:textNode.text().as_string()];
    }
    
    textNode = textNode.next_sibling("textid");
    
    if (!textNode.text().empty()) {
        fullText = [NSString stringWithUTF8String:textNode.text().as_string()];
    }
    
    NSString *detailsExhibit = @"";
    if (!node.child("details").empty())
    {
        detailsExhibit = [NSString stringWithUTF8String:node.child("details").text().as_string()];
    }

    
    
    contentXMLObj.child("content").children("object");
    
    self.excursionMode = [self getExcursionMode:node];
    self.caption = AMLocalizedString(captionF, @"");
    self.text    = AMLocalizedString(fullText, @"");
    self.details = AMLocalizedString (detailsExhibit, @"");
    [self fillImageData:node];
    
    self.mapCoords = [self getMapCoord:node];
    
    NSString*parentID = @"0";
    
    if (!node.attribute("parentId").empty()) {
        parentID = [NSString stringWithUTF8String:node.attribute("parentId").as_string()];
    }
    
    self.parentId = parentID;
    
    self.isUnesco = false;
    if (!node.attribute("unesco").empty()) {
        bool isUnesco = node.attribute("unesco").as_bool();
        if (isUnesco)
        {
            self.isUnesco = true;
        }
    }

    
    if (!node.attribute("hasNestedObjects").empty()) {
        hasNestedObjects = node.attribute("hasNestedObjects").as_bool();
    }
    else
    {
        hasNestedObjects = NO;
    }
    
    [self fillMapImages:node];

	
	if ( hasNestedObjects == true || [excursionMode isEqualToString:@"Main"] )
    {
        [self FillObjectList];
	}
    
    
    
    [self preloadResForID:self.objectId objectNode:node];
    
}

//only for nested objects
-(void)FillGlobalObjectList
{
	/*
	for (int i = 0; i < EXHIBITS_TOTAL_NUM; i++)
	{
		Exhibit *exhibit = [[Exhibit alloc] init];

		//TBD
		/*
		exhibit->Id =
		exhibit->floor =
		exhibit->coord.dx = ;
		exhibit->coord.dy = ;
		exhibit->caption = ;
		exhibit->parentId = ;
		*/
		/*
		exhibit->state = NOT_VISITED;
		globalObjectList = [globalObjectList arrayByAddingObject:exhibit];
	}
	*/
}

-(BOOL) checkIsChildObject: (NSInteger)Id
{
    Exhibit *exhibit = globalObjectList[ Id ];

    for (int j = 0; j < globalObjectList.count; j++)
    {
        Exhibit *parentExhibit = globalObjectList[ j ];
        if ( [parentExhibit->Id isEqualToString: exhibit->parentId] )
        {
            parentExhibit = globalObjectList[ j ];
           return parentExhibit->hasNestedObjects;
        }
    }
    
    return NO;
}

-(void)UpdateObjectStatus
{
	if ( lastDetectedIndex != -1 )
	{
		Exhibit *exhibit = globalObjectList[ lastDetectedIndex ];
		exhibit->state = VISITED;
	}
	
	for ( int i = 0; i < globalObjectList.count; i++)
	{
		Exhibit *exhibit = globalObjectList[ i ];
		if ( [exhibit->Id isEqualToString: detectedId] )
		{
            if ([self checkIsChildObject:i])
            {
                for (int j = 0; j < globalObjectList.count; j++)
                {
                    Exhibit *parentExhibit = globalObjectList[ j ];
                    if ( [parentExhibit->Id isEqualToString: exhibit->parentId] )
                    {
                        exhibit = globalObjectList[j];
                        i = j;
                        break;
                    }
                }
                
            }
			exhibit->state = LAST_VISITED;
			lastDetectedIndex = i;
			break;
		}
	}
}

-(void)FillObjectList
{
	objectList = [NSArray array];
	for ( int i = 0; i < [globalObjectList count]; i++ )
	{
		Exhibit *exhibit = globalObjectList[ i ];
		
        if ( [exhibit->parentId isEqualToString: objectId] ) {
            objectList = [objectList arrayByAddingObject:exhibit];
        }
			
	}
}

-(void)PushToStack: (NSString*) objId
{
    if ( objId != nil)
        [objectStack addObject: objId];

}

@end
