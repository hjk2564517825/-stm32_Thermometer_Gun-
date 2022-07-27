#ifndef __SYS_H
#define __SYS_H

//                                       GPIOF_BASE  ODR                 n    
//#define GPIOF_ODR10 *(unsigned int*)(((0x40021400+0x14-0x40000000)*8+10)*4+0x42000000)



////GPIOx ODR位段别名访问/////
#define PAout(n)   *(unsigned int*)(((GPIOA_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PBout(n)   *(unsigned int*)(((GPIOB_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PCout(n)   *(unsigned int*)(((GPIOC_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PDout(n)   *(unsigned int*)(((GPIOD_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PEout(n)   *(unsigned int*)(((GPIOE_BASE+0x14-0x40000000)*8+n)*4+0x42000000)	
#define PFout(n)   *(unsigned int*)(((GPIOF_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PGout(n)   *(unsigned int*)(((GPIOG_BASE+0x14-0x40000000)*8+n)*4+0x42000000)	
#define PHout(n)   *(unsigned int*)(((GPIOH_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PIout(n)   *(unsigned int*)(((GPIOI_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PJout(n)   *(unsigned int*)(((GPIOJ_BASE+0x14-0x40000000)*8+n)*4+0x42000000)
#define PKout(n)   *(unsigned int*)(((GPIOK_BASE+0x14-0x40000000)*8+n)*4+0x42000000)


////GPIOx IDR位段别名访问/////
#define PAin(n)   *(unsigned int*)(((GPIOA_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PBin(n)   *(unsigned int*)(((GPIOB_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PCin(n)   *(unsigned int*)(((GPIOC_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PDin(n)   *(unsigned int*)(((GPIOD_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PEin(n)   *(unsigned int*)(((GPIOE_BASE+0x10-0x40000000)*8+n)*4+0x42000000)	
#define PFin(n)   *(unsigned int*)(((GPIOF_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PGin(n)   *(unsigned int*)(((GPIOG_BASE+0x10-0x40000000)*8+n)*4+0x42000000)	
#define PHin(n)   *(unsigned int*)(((GPIOH_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PIin(n)   *(unsigned int*)(((GPIOI_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PJin(n)   *(unsigned int*)(((GPIOJ_BASE+0x10-0x40000000)*8+n)*4+0x42000000)
#define PKin(n)   *(unsigned int*)(((GPIOK_BASE+0x10-0x40000000)*8+n)*4+0x42000000)



#define EXTI_PR(n)   *(unsigned int*)(((EXTI_BASE+0x14-0x40000000)*8+n)*4+0x42000000)


#endif
