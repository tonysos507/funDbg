#include "..\dbgext.h"
#include <strsafe.h>
#include <string>
#include "..\out.h"
#include <regex>
#include "..\lgparser.h"
#include "..\adapters\adapterParser.h"

#include "..\..\lib\png\png.h"
#include "..\..\lib\png\pngconf.h"

#include "writeImg2Png.h"

extern std::string executecommandout2buf(const char* cmd);

#define BITDEPTH_16 16

VOID user_flush_data(
    png_structp png_ptr)
{
    return;
}

VOID user_write_data(
    png_structp png_ptr,
    png_bytep data,
    png_size_t length)
{
    FILE *fp = (FILE *)png_get_io_ptr(png_ptr);
    fwrite(data, length, 1, fp);
    return;
}

HRESULT CALLBACK
writeImg2Png(PDEBUG_CLIENT4 Client, PCSTR args)
{
    std::vector<std::string> cmdLineArgs;
    std::istringstream iss(args);
    std::string token;

    while(iss >> token)
    {
        cmdLineArgs.push_back(token);
    }

    if(cmdLineArgs.empty())
    {
        std::string printMsg = "[Usage]: writeImg2Png path\\to\\png vmid address imageHeight imageWidth bitsperpixel\r\n";
        printMsg += "expamle: !funDbg.writeImg2Png D:\\test.png 0.10 0x00`00c25000 512 512 32\r\n";
        dprintf("%s\n", printMsg.c_str());
    }
    else
    {
        FILE* fp = NULL;

        // demo hard code part begin
        fopen_s(&fp, cmdLineArgs[0].c_str(), "wb");
        UINT imageHeight = std::stoi(cmdLineArgs[3]);
        UINT imageWidth = std::stoi(cmdLineArgs[4]);
        UINT bitDepth = 8;
        UINT colorType = PNG_COLOR_TYPE_RGBA;
        UINT bpp = std::stoi(cmdLineArgs[5]) >> 3;  // bits per pixel divided by 8
        // demo hard code part end

        BOOL        success = TRUE;
        png_structp png_ptr = NULL;
        png_infop   info_ptr = NULL;
        PBYTE       pImage = reinterpret_cast<PBYTE>(data);

        if(bpp == 8)
        {
            bitDepth = 16;
        }
        else if(bpp == 4)
        {
            bitDepth = 8;
        }

        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        info_ptr = png_create_info_struct(png_ptr);
        setjmp(png_jmpbuf(png_ptr));

        png_bytep* ppRows = NULL;
        png_set_write_fn(png_ptr, fp, user_write_data, user_flush_data);

        png_set_IHDR(png_ptr, info_ptr, imageWidth, imageHeight, bitDepth, colorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_write_info(png_ptr, info_ptr);
        png_set_bgr(png_ptr);
        if (bitDepth = BITDEPTH_16)
        {
            png_set_swap(png_ptr);
        }

        ppRows = reinterpret_cast<png_bytep *>(new CHAR[sizeof(png_bytep) * imageHeight]);
        memset(ppRows, 0, sizeof(png_bytep) * imageHeight);

        for (UINT i = 0; i < imageHeight; i++)
        {
            ppRows[i] = reinterpret_cast<png_bytep>(pImage + (imageWidth * i * bpp));
        }

        png_write_image(png_ptr, ppRows);
        png_write_end(png_ptr, info_ptr);

        delete[] ppRows;

        png_destroy_write_struct(&png_ptr, png_infopp_NULL);

        fclose(fp);
    }

    return S_OK;
}