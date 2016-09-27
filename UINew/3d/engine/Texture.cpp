//
//  Textures.cpp
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 13.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//

#include "Texture.h"

#include "XTypes.h"
#include "XTECH-C-Interface.h"
#include "png.h"

#include "ResourceManager.h"


void Texture::createTexture() {
    GLubyte pixels[4*3] = {
        0, 0, 0,
        225, 225, 225,
        155, 155, 155,
        255, 255, 255
    };
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &this->texID);
    glBindTexture(GL_TEXTURE_2D, this->texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    XLOG("texture alloc %d\n",texID);

    
    countUsed = 1;
	_isDamaged = false;
    this->width  = 2;
    this->height = 2;
    this->formatTexture = GL_TEXTURE_2D;
}

void Texture::createTextureSimple() {
    GLubyte pixels[4*3] = {
        155, 155, 155,
        225, 225, 225,
        155, 155, 155,
        255, 255, 255
    };
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &this->texID);
    glBindTexture(GL_TEXTURE_2D, this->texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    countUsed = 1;
	_isDamaged = false;
    this->width  = 2;
    this->height = 2;
    this->formatTexture = GL_TEXTURE_2D;
}

void Texture::reloadTexture() {
    createTexture(pathForTexture);
}


void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    printf("%s\n",error_msg);
    
}
void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    printf("%s\n",warning_msg);
}
/*
void Texture::read_png_file(char* file_name)
{
    
    int x, y;
    
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    
    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;
    
    char header[8];    // 8 is the maximum size that can be checked
    
 
    FILE *fp = fopen(file_name, "rb");
    if (!fp)
        return;
    fread(header, 1, 8, fp);
    
    
    
    if (png_sig_cmp((png_const_bytep)header, 0, 8))
        return;
    
 
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,user_error_fn, user_warning_fn);
    
    if (!png_ptr)
        return;
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        return;
    
    if (setjmp(png_jmpbuf(png_ptr)))
        return;
    
    
    
    
    
    
    
    
    
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    
    png_read_info(png_ptr, info_ptr);
    
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
    
    
 
    if (setjmp(png_jmpbuf(png_ptr)))
        return;
    
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y=0; y<height; y++)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
    
    png_read_image(png_ptr, row_pointers);
    
    
    
    
    
    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_width, temp_height, 0, GL_RGB, GL_UNSIGNED_BYTE, row_pointers);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);

    
    
    
    fclose(fp);
}

*/

int Texture::read_png_file(const char * file_name, int * width, int * height)
{
    png_byte header[8];
    
    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }
    
    // read the header
    fread(header, 1, 8, fp);
    
    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }
    
    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }
    
    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }
    
    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    
    // init png reading
    png_init_io(png_ptr, fp);
    
    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);
    
    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);
    
    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;
    
    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
                 NULL, NULL, NULL);
    
    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }
    
    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);
    
    // Row size in bytes.
    unsigned long rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);
    
    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    
    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers =(png_bytep *) malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }
    
    // set the individual row_pointers to point at the correct offsets of image_data
    unsigned int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }
    
    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);
    
    // Generate the OpenGL texture object
    
    uint formatColor = 0;
    
    switch (color_type) {
        case PNG_COLOR_TYPE_RGBA:
            formatColor = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            formatColor = GL_RGB;
        default:
        break;
    }
    
    
    if (!formatColor) {
        _isDamaged = false;
        countUsed = 1;
        createTexture();
        return 0;
    }
    
    glGenTextures(1, &this->texID);
    glBindTexture(GL_TEXTURE_2D, this->texID);

    glTexImage2D(GL_TEXTURE_2D, 0, formatColor, temp_width, temp_height, 0, formatColor, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    
    
    
    countUsed = 1;
    _isDamaged = false;
    this->width  = temp_width;
    this->height = temp_height;
    this->formatTexture = GL_TEXTURE_2D;
    
    
    return 1;
}


int Texture::createTexture(const std::string &path) {
 
    
    
    pathForTexture.assign(path.begin(), path.end());
    

    long m_Size = 0;
    bool m_bOpen = false;
    char* m_pData = NULL;
    
    std::string ext = path.substr(path.find_last_of(".") + 1);
    if (std::strcmp(  ext.c_str(),"png")==0) {
        // here need os load png files
        int width = 0;
        int height = 0;
        return read_png_file((char*)path.c_str(),&width,&height);
    }
    
    
    FILE* pFile = fopen(path.c_str(), "rb");
    if (pFile)
    {
        // Get the file size
        fseek(pFile, 0, SEEK_END);
        m_Size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        
        // read the data, append a 0 byte as the data might represent a string
        char* pData = new char[m_Size + 1];
        pData[m_Size] = '\0';
        size_t BytesRead = fread(pData, 1, m_Size, pFile);
        
        if (BytesRead != m_Size)
        {
            delete [] pData;
            m_Size = 0;
        }
        else
        {
            m_pData = pData;
            m_bOpen = true;
        }
        fclose(pFile);
    }else {
        // cant open need create clear texture // mb after some time system change data and we will see normal tex ))
        _isDamaged = false;
        countUsed = 1;
        createTexture();
        return 2;
    }
    
    if (!m_bOpen) {
        texID = 0;
        printf("load texture error\n");
        return 0;
    }
    
    
    
    
    
    
    uint ret = 0;
	if (std::strcmp(  ext.c_str(),"frame")==0) {
        
        FrameImageHeader*textureHeader =(FrameImageHeader*)malloc(sizeof(FrameImageHeader));
        textureHeader=(FrameImageHeader*)m_pData;
        this->width  = textureHeader->width;
        this->height = textureHeader->height;
        this->formatTexture = GL_TEXTURE_2D;
        
        if (texID != 0) {
            glBindTexture(GL_TEXTURE_2D, texID);
        }else {
            
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        
        
        unsigned char*pTData = (unsigned char*)m_pData+sizeof(FrameImageHeader);
        pTData[0] = 0;
        pTData[sizeof(char)] = 0;
        pTData[sizeof(char)*2] = 0;
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureHeader->width, textureHeader->height, 0, GL_RGB, GL_UNSIGNED_BYTE, pTData);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        _isDamaged = false;
        countUsed = 1;
        
        ret = 1;
    }else if (std::strcmp(  ext.c_str(),"pvr")==0) {
        PVRTextureHeaderV3* psTextureHeader = (PVRTextureHeaderV3*)malloc(sizeof(PVRTextureHeaderV3));
        
        uint m_uiTexture = 0;
        if (PVRTTextureLoadFromPVR(m_pData, &m_uiTexture,psTextureHeader,true) != PVR_SUCCESS) {
            printf("load PVR error\n");
            return 0;
        }
        
        if (psTextureHeader->u32MIPMapCount > 0) {
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        }else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        
        this->width  = psTextureHeader->u32Width;
        this->height = psTextureHeader->u32Height;
        this->formatTexture = GL_TEXTURE_2D;
        this->texID = m_uiTexture;
        
        
        free(psTextureHeader);
        
        _isDamaged = false;
        countUsed = 1;
        ret = 1;
    }
    
    XLOG("texture alloc %d\n",texID);
    
    delete [] m_pData; // delete texture data;
    
    return ret;
}


void Texture::releaseTexture() {
    if (countUsed > 0) {
        // used !
        countUsed--;
        if (countUsed <= 0 && !_isDamaged) deleteTexture();
    }else if (!_isDamaged){
        // not used and not deleted !
        deleteTexture();
    }
};

Texture::~Texture(){
    deleteTexture();
}

void Texture::deleteTexture() {
    
    glDeleteTextures(1, &texID);
    
    GLenum errorCode = glGetError();
    XLOG("texture dealoc %d errorID:%d\n",texID,errorCode);
 
    texID = 0;
    _isDamaged = true;
    countUsed = 0;
}


void Texture::setTextureID(uint formatTexture,uint texID){
    this->formatTexture = formatTexture;
    this->texID = texID;
}
uint Texture::getTextureID(){
    return texID;
}
uint Texture::getTextureFormat(){
    return formatTexture;
}
uint Texture::createClearTexture(int width,int height,uint formatTexture,uint formatBitsAndChanel,uint formatChanel,uint formatBits) {
    this->formatTexture = formatTexture;
    this->width  = width;
    this->height = height;

    glGenTextures(1, &texID);
    glBindTexture(formatTexture, texID);



    glTexParameteri(formatTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(formatTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(formatTexture, 0, formatBitsAndChanel, width, height, 0, formatChanel, formatBits, 0);

    glBindTexture(formatTexture, 0);
    return texID;
}
uint Texture::createFrombufferAndUploadTexture(uint formatTexture){
    this->formatTexture = formatTexture;
    glGenTextures(1, &texID);
    glBindTexture(formatTexture, texID);
    glTexParameteri(formatTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(formatTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D( formatTexture, 0, GL_RGB, width, width, 0, GL_RGB, GL_UNSIGNED_BYTE,imageData );
    glBindTexture(formatTexture, 0);
    
    return texID;
}
