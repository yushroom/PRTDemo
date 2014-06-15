#ifndef _SHEVAL_H_
#define _SHEVAL_H_

typedef void (*SHEvalFunc)(const float, const float, const float, float*);
void SHEval3(const float fX, const float fY, const float fZ, float *pSH);
void SHEval4(const float fX, const float fY, const float fZ, float *pSH);
void SHEval5(const float fX, const float fY, const float fZ, float *pSH);
void SHEval6(const float fX, const float fY, const float fZ, float *pSH);
void SHEval7(const float fX, const float fY, const float fZ, float *pSH);
void SHEval8(const float fX, const float fY, const float fZ, float *pSH);
void SHEval9(const float fX, const float fY, const float fZ, float *pSH);
void SHEval10(const float fX, const float fY, const float fZ, float *pSH);

#endif