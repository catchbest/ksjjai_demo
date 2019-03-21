#ifndef _KSJ_BAYER_FILTER_H_
#define _KSJ_BAYER_FILTER_H_

#ifdef KSJBAYERFILTER_EXPORTS
#define KSJBAYERFILTER_API __declspec(dllexport) int WINAPI
#elif ( defined(KSJBAYERFILTER_STATIC))
#define KSJBAYERFILTER_API int WINAPI 
#else
#define KSJBAYERFILTER_API __declspec(dllimport) int WINAPI
#endif

#ifdef __cplusplus
extern "C"{
#endif

#ifndef _WIN64

    // 使用32位汇编
	int BayerFilterBGGR24(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterRGGB24(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGRBG24(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGBRG24(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);

	int BayerFilterRGGB24Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterBGGR24Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGRBG24Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGBRG24Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);

	int BayerFilterRGGB32(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterBGGR32(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGRBG32(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGBRG32(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);

	int BayerFilterRGGB32Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterBGGR32Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGRBG32Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	int BayerFilterGBRG32Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);

#endif

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_BGGR(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GRBG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GBRG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_RGGB_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_BGGR_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GRBG_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GBRG_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

    // 8位原始数据直接转为8位灰度图象
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB_Gray(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR_Gray(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG_Gray(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG_Gray(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);

    // 8位原始数据直接转为8位灰度图象
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB_Gray_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR_Gray_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG_Gray_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG_Gray_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData);

    // 16位原始数据直接转为16位每像素的灰度图象
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB_Gray_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR_Gray_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG_Gray_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG_Gray_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);

    // 16位原始数据直接转为16位每像素的灰度图象
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB_Gray_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR_Gray_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG_Gray_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG_Gray_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData);


    // 16位原始数据转换为24位或32位彩色图像
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_BGGR_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GRBG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GBRG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_RGGB_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_BGGR_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GRBG_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge1_GBRG_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_RGGB_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_BGGR_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GRBG_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_GBRG_Flip_16Bits(unsigned short *pRawData, int nWidth, int nHeight, unsigned short *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_Bilinear_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Bilinear_BGGR(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Bilinear_GRBG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Bilinear_GBRG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_Bilinear_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Bilinear_BGGR_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Bilinear_GRBG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Bilinear_GBRG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_BGGR(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_GRBG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_GBRG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_EdgeSensing2_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_EdgeSensing2_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_BGGR_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_GRBG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_EdgeSensing1_GBRG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_SmoothHue_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_SmoothHue_BGGR(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_SmoothHue_GRBG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_SmoothHue_GBRG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_SmoothHue_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_SmoothHue_BGGR_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_SmoothHue_GRBG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_SmoothHue_GBRG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_BGGR(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_GRBG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_GBRG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_BGGR_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_GRBG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_NearestNeighbor_Edge2_GBRG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_Laplacian_RGGB(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Laplacian_BGGR(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Laplacian_GRBG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Laplacian_GBRG(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);

	KSJBAYERFILTER_API KSJCFA_Laplacian_RGGB_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Laplacian_BGGR_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Laplacian_GRBG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);
	KSJBAYERFILTER_API KSJCFA_Laplacian_GBRG_Flip(unsigned char *pRawData, int nWidth, int nHeight, unsigned char *pRgbData, unsigned short wBitCount);


#ifdef __cplusplus
}
#endif

#endif