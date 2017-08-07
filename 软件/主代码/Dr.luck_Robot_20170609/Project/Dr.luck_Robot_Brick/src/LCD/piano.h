/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/piano.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef __PIANO_H__
#define __PIANO_H__	

#include "sys.h"

#define PIANO_adr(z)  1024*(1024*28+z)        //3328+7*i

extern u8 piano_Buff_flag;
extern u32 piano_index;
extern u8 piano_num;

extern u8 piano_Buff[];	

extern const float PIANO_fre[];
extern const unsigned int PIANO_LEN[];

//#define PIANO_40  1024*(1024*28+3328)   //C  	261.63
//#define PIANO_41  1024*(1024*28+3335)   //C#  277.18
//#define PIANO_42  1024*(1024*28+3342)   //D  	293.67
//#define PIANO_43  1024*(1024*28+3349)   //D#  311.13
//#define PIANO_44  1024*(1024*28+3356)   //E  	329.63
//#define PIANO_45  1024*(1024*28+3363)   //F  	349.23
//#define PIANO_46  1024*(1024*28+3370)   //F#  369.99
//#define PIANO_47  1024*(1024*28+3377)   //G  	392
//#define PIANO_48  1024*(1024*28+3384)   //G#  415.31
//#define PIANO_49  1024*(1024*28+3391)   //A  	440
//#define PIANO_50  1024*(1024*28+3398)   //A#  466.16
//#define PIANO_51  1024*(1024*28+3405)   //B  	493.88
//#define PIANO_52  1024*(1024*28+3412)   //C  	523.25
//#define PIANO_53  1024*(1024*28+3419)   //C#  554.37
//#define PIANO_54  1024*(1024*28+3426)   //D  	587.33
//#define PIANO_55  1024*(1024*28+3433)   //D#  622.25
//#define PIANO_56  1024*(1024*28+3440)   //E  	659.26
//#define PIANO_57  1024*(1024*28+3447)   //F  	698.46
//#define PIANO_58  1024*(1024*28+3454)   //F#  739.99
//#define PIANO_59  1024*(1024*28+3461)   //G  	783.99
//#define PIANO_60  1024*(1024*28+3468)   //G#  830.61
//#define PIANO_61  1024*(1024*28+3475)   //A  	880
//#define PIANO_62  1024*(1024*28+3482)   //A#  932.33
//#define PIANO_63  1024*(1024*28+3489)   //B  	987.77
//#define PIANO_64  1024*(1024*28+3496)   //C  	1046.5
//#define PIANO_65  1024*(1024*28+3503)   //C#  1108.73
//#define PIANO_66  1024*(1024*28+3510)   //D  	1174.66
//#define PIANO_67  1024*(1024*28+3517)   //D  	1244.51
//#define PIANO_68  1024*(1024*28+3524)   //E  	1318.51
//#define PIANO_69  1024*(1024*28+3531)   //F   1396.91
//#define PIANO_70  1024*(1024*28+3538)   //F#  1479.98
//#define PIANO_71  1024*(1024*28+3545)   //G  	1567.98
//#define PIANO_72  1024*(1024*28+3552)   //G#  1661.22
//#define PIANO_73  1024*(1024*28+3559)   //A  	1760
//#define PIANO_74  1024*(1024*28+3566)   //A#  1864.66
//#define PIANO_75  1024*(1024*28+3573)   //B  	1975.53
//#define PIANO_76  1024*(1024*28+3580)   //C  	2093

//#define PIANO_LEN_40  6366
//#define PIANO_LEN_41  6679
//#define PIANO_LEN_42  7097
//#define PIANO_LEN_43  6679
//#define PIANO_LEN_44  6366
//#define PIANO_LEN_45  6470
//#define PIANO_LEN_46  6470
//#define PIANO_LEN_47  6157
//#define PIANO_LEN_48  6470
//#define PIANO_LEN_49  6261
//#define PIANO_LEN_50  6575
//#define PIANO_LEN_51  6157
//#define PIANO_LEN_52  6157
//#define PIANO_LEN_53  5948
//#define PIANO_LEN_54  6052
//#define PIANO_LEN_55  5843
//#define PIANO_LEN_56  6261
//#define PIANO_LEN_57  6575
//#define PIANO_LEN_58  6261
//#define PIANO_LEN_59  6993
//#define PIANO_LEN_60  6784
//#define PIANO_LEN_61  4381
//#define PIANO_LEN_62  4590
//#define PIANO_LEN_63  5112
//#define PIANO_LEN_64  5217
//#define PIANO_LEN_65  4903
//#define PIANO_LEN_66  4172
//#define PIANO_LEN_67  3649
//#define PIANO_LEN_68  6157
//#define PIANO_LEN_69  6366
//#define PIANO_LEN_70  3127
//#define PIANO_LEN_71  2918
//#define PIANO_LEN_72  3858
//#define PIANO_LEN_73  3858
//#define PIANO_LEN_74  5634
//#define PIANO_LEN_75  5425
//#define PIANO_LEN_76  2918

extern const unsigned char PIANO_DATA_40[];
extern const unsigned char PIANO_DATA_41[];
extern const unsigned char PIANO_DATA_42[];
extern const unsigned char PIANO_DATA_43[];
extern const unsigned char PIANO_DATA_44[];
extern const unsigned char PIANO_DATA_45[];
extern const unsigned char PIANO_DATA_46[];
extern const unsigned char PIANO_DATA_47[];
extern const unsigned char PIANO_DATA_48[];
extern const unsigned char PIANO_DATA_49[];
extern const unsigned char PIANO_DATA_50[];
extern const unsigned char PIANO_DATA_51[];
extern const unsigned char PIANO_DATA_52[];
extern const unsigned char PIANO_DATA_53[];
extern const unsigned char PIANO_DATA_54[];
extern const unsigned char PIANO_DATA_55[];
extern const unsigned char PIANO_DATA_56[];
extern const unsigned char PIANO_DATA_57[];
extern const unsigned char PIANO_DATA_58[];
extern const unsigned char PIANO_DATA_59[];
extern const unsigned char PIANO_DATA_60[];
extern const unsigned char PIANO_DATA_61[];
extern const unsigned char PIANO_DATA_62[];
extern const unsigned char PIANO_DATA_63[];
extern const unsigned char PIANO_DATA_64[];
extern const unsigned char PIANO_DATA_65[];
extern const unsigned char PIANO_DATA_66[];
extern const unsigned char PIANO_DATA_67[];
extern const unsigned char PIANO_DATA_68[];
extern const unsigned char PIANO_DATA_69[];
extern const unsigned char PIANO_DATA_70[];
extern const unsigned char PIANO_DATA_71[];
extern const unsigned char PIANO_DATA_72[];
extern const unsigned char PIANO_DATA_73[];
extern const unsigned char PIANO_DATA_74[];
extern const unsigned char PIANO_DATA_75[];
extern const unsigned char PIANO_DATA_76[];

#endif
