#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"
#include "pixelate.h"

int	main(void)
{
	FILE	*inFile = fopen("input.bmp", "rb");
	if (!inFile)
	{
		printf("ERROR; could not open input file, make sure 'input.bmp' is in the folder.\n");
		return (1);
	}

	FILE	*outFile = fopen("output.bmp", "wb");
	if (!outFile)
	{
		printf("ERROR; could not create output file.\n");
		fclose(inFile);
		return (1);
	}

	BITMAPFILEHEADER	fileHeader;
	BITMAPINFOHEADER	infoHeader;

	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, inFile);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, inFile);

	if (fileHeader.bfType != 0x4D42)
	{
		printf("ERROR: only 24bit uncompressed BMP's are supported.\n");
		fclose(inFile);
		fclose(outFile);
		return (1);
	}

	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, outFile);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, outFile);

	int	width = infoHeader.biWidth;
	int	height = infoHeader.biHeight;
	int	channels = 3;
	int	padding = (4 - (width * channels) % 4) % 4;
	int	imageSize = (width * channels + padding) * height;

	unsigned char	*pixelData = (unsigned char *)malloc(imageSize);
	if (!pixelData)
	{
		printf("ERROR: could not allocate memory for pixel data.\n");
		fclose(inFile);
		fclose(outFile);
		return (1);
	}

	fseek(inFile, fileHeader.bf0ffBits, SEEK_SET);
	fread(pixelData, 1, imageSize, inFile);

	int	blockSize = 20;

	pixelate(pixelData, width, height, channels, padding, blockSize);
	fwrite(pixelData, 1, imageSize, outFile);

	printf("Pixelation complete and Saved to 'output.bmp' (Block size: %d)\n", blockSize);
	free(pixelData);
	fclose(inFile);
	fclose(outFile);

	return (0);

}
