#ifndef _encoder_
#define _encoder_

// ============================================================
// 360度旋转编码器 (CLK=PA6, DT=PA7)
// 顺时针旋转 → count++, 逆时针 → count--
// ============================================================

void Encoder_Init(void);
int  Encoder_GetCount(void);       // 获取累计计数值
void Encoder_ResetCount(void);     // 重置计数
int  Encoder_GetSpeed(void);       // 转换为速度值(0~100)

#endif
