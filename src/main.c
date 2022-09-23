#include <stdio.h>
#include <capi.h>
#include <allheaders.h>

void ThrowTesseractException(const char *errstr);

int main(int argc, char **argv) 
{
    if (argc != 4)
    {
        printf("Usage: %s image-file text-file lang\n",argv[0]);
        exit(0);
    }

    TessBaseAPI *handle;
	PIX *img;
	char *text;

    printf("Processing image %s",argv[1]);
    if((img = pixRead(argv[1])) == NULL)
		ThrowTesseractException("Error reading image file\n");

    handle = TessBaseAPICreate();
	if(TessBaseAPIInit3(handle, NULL, argv[3]) != 0)
		ThrowTesseractException("Error initializing tesseract\n");

    TessBaseAPISetImage2(handle, img);
	if(TessBaseAPIRecognize(handle, NULL) != 0)
		ThrowTesseractException("Error in Tesseract recognition\n");

    if((text = TessBaseAPIGetUTF8Text(handle)) == NULL)
		ThrowTesseractException("Error getting text\n");

    printf("Text identified: %s",text);

    FILE* ptr = fopen(argv[2],"w");
    if (ptr == NULL) 
       ThrowTesseractException("Error creating output file\n");
    fputs(text,ptr);
    fclose(ptr);

    TessDeleteText(text);
	TessBaseAPIEnd(handle);
	TessBaseAPIDelete(handle);
	pixDestroy(&img);

    return 0;
}

void ThrowTesseractException(const char *errstr) {
	fputs(errstr, stderr);
	exit(-1);
}

