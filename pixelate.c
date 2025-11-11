#include "pixelate.h"

void	pixelate(unsigned char *data, int width, int height, int channels, int padding, int blockSize)
{
	int	rowStride = (width * channels) + padding;

	for (int y = 0; y < height; y += blockSize)
	{
		for (int x = 0; x < width; x += blockSize)
		{
			unsigned long long	sumB = 0, sumG = 0, sumR = 0;
			int	pixelCount = 0;

			for (int blockY = 0; blockY < blockSize; blockY++)
			{
				for (int blockX = 0; blockX < blockSize; blockX++)
				{
					int	currentX = x + blockY;
					int	currentY = y + blockY;

					if (currentX < width && currentY < height)
					{
						int	index = (currentY * rowStride) + (currentX * channels);

						sumB += data[index + 0];
						sumG += data[index + 1];
						sumR += data[index + 2];
						pixelCount++;
					}
				}
			}
			if (pixelCount == 0) continue;

			unsigned char	avgB = sumB / pixelCount;
			unsigned char	avgG = sumG / pixelCount;
			unsigned char	avgR = sumR / pixelCount;

			for (int blockY = 0; blockY < blockSize; blockY++)
			{
				for (int blockX = 0; blockX < blockSize; blockX++)
				{
					int	currentX = x + blockX;
					int	currentY = y + blockY;

					if (currentX < width && currentY < height)
					{
						int	index = (currentY * rowStride) + (currentX * channels);

						data[index + 0] = avgB;
						data[index + 1] = avgG;
						data[index + 2] = avgR;
					}
				}
			}
		}
	}
}	
