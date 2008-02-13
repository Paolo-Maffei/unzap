    /***************************************************
     * code db/tvbgone-100.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 34760:    pwm8:  28 ( 35714), pwm1: 230 ( 34782)
     * error:         954,                22
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (230 << 8) | 1, 21535,
    /* pattern (on, length): */
    682, 912,
    115, 345,
    115, 230,
    115, 230,
    345, 682,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    115, 230,
    230, 345,
    115, 345,
    115, 230,
    115, 0,
    /***************************************************
     * code db/tvbgone-101.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 29510:    pwm8:  33 ( 30303), pwm1: 271 ( 29520)
     * error:         793,                10
     * selected freq8 */
    /* pwm, repeat, repeat_delay */
    (33 << 8) | 0, 0,
    /* pattern (on, length): */
    135, 810,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 5402,
    135, 805,
    135, 407,
    262, 535,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 30057,
    135, 805,
    135, 407,
    262, 535,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 267,
    135, 0,
    /***************************************************
     * code db/tvbgone-102.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 32450:    pwm8:  30 ( 33333), pwm1: 246 ( 32520)
     * error:         883,                70
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (246 << 8) | 1, 20750,
    /* parameters */
    /* no header */
     0, 0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x7b, 0x508, 0x283,
    /* flags, bitcount */ 
     0xc,
    /* data: */
     0xb8c,
    /***************************************************
     * code db/tvbgone-103.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 7138,
    /* pattern (on, length): */
    612, 762,
    307, 457,
    152, 302,
    307, 457,
    152, 302,
    307, 457,
    152, 302,
    152, 302,
    307, 457,
    152, 302,
    152, 302,
    152, 302,
    152, 0,
    /***************************************************
     * code db/tvbgone-104.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 36090:    pwm8:  27 ( 37037), pwm1: 221 ( 36199)
     * error:         947,               109
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (221 << 8) | 1, 35000,
    /* parameters */
    /* header on timing, header length */
     0x37f, 0x53f,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x76, 0x1c2, 0xe4,
    /* flags, bitcount */ 
     0x330,
    /* data: */
     0x2002, 0x80, 0xbd3d, 
    /***************************************************
     * code db/tvbgone-105.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 11055,
    /* pattern (on, length): */
    67, 552,
    67, 270,
    67, 270,
    67, 270,
    67, 270,
    67, 270,
    67, 552,
    67, 552,
    67, 270,
    67, 552,
    67, 270,
    67, 270,
    67, 270,
    67, 552,
    67, 270,
    67, 12387,
    67, 552,
    67, 270,
    67, 270,
    67, 270,
    67, 270,
    67, 552,
    67, 270,
    67, 270,
    67, 552,
    67, 270,
    67, 552,
    67, 552,
    67, 552,
    67, 270,
    67, 552,
    67, 0,
    /***************************************************
     * code db/tvbgone-106.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37430:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:        1031,               128
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 27500,
    /* parameters */
    /* header on timing, header length */
     0x492, 0x942,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x8a, 0x23f, 0x12a,
    /* flags, bitcount */ 
     0x320,
    /* data: */
     0x707, 0xfd02, 
    /***************************************************
     * code db/tvbgone-107.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 2, 15000,
    /* parameters */
    /* header on timing, header length */
     0x86b, 0xca1,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x85, 0x217, 0x109,
    /* flags, bitcount */ 
     0x210,
    /* data: */
     0x1703,
    /***************************************************
     * code db/tvbgone-108.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xaf50, 0xe817,
    /***************************************************
     * code db/tvbgone-109.yaml
     ***************************************************/

    (uint16_t)send_rc5,
    /* freq 36000:    pwm8:  27 ( 37037), pwm1: 222 ( 36036)
     * error:        1037,                36
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (222 << 8) | 1, 28500,
    /* data: */
    0x180,
    /***************************************************
     * code db/tvbgone-110.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xbf40, 0xed12,
    /***************************************************
     * code db/tvbgone-111.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0x4931, 0x9c63,
    /***************************************************
     * code db/tvbgone-112.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37430:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:        1031,               128
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 27500,
    /* parameters */
    /* header on timing, header length */
     0x8fc, 0xd2f,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x8c, 0x242, 0x120,
    /* flags, bitcount */ 
     0x32a,
    /* data: */
     0x807b, 0x3f0, 0x3fc, 
    /***************************************************
     * code db/tvbgone-113.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 6138,
    /* pattern (on, length): */
    2055, 3077,
    127, 257,
    127, 257,
    127, 535,
    127, 257,
    127, 535,
    127, 257,
    127, 257,
    127, 257,
    127, 1150,
    127, 535,
    127, 257,
    127, 535,
    127, 257,
    127, 535,
    127, 257,
    127, 257,
    127, 257,
    127, 0,
    /***************************************************
     * code db/tvbgone-114.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 2, 23000,
    /* parameters */
    /* header on timing, header length */
     0x88e, 0xcce,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x8c, 0x21a, 0x113,
    /* flags, bitcount */ 
     0x320,
    /* data: */
     0x55aa, 0xe31c, 
    /***************************************************
     * code db/tvbgone-115.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 32450:    pwm8:  30 ( 33333), pwm1: 246 ( 32520)
     * error:         883,                70
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (246 << 8) | 1, 13750,
    /* parameters */
    /* header on timing, header length */
     0x0, 0x0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x4e, 0x26f, 0x134,
    /* flags, bitcount */ 
     0x10,
    /* data: */
     0x247,
    /***************************************************
     * code db/tvbgone-116.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xc738, 0xe31c,
    /***************************************************
     * code db/tvbgone-117.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 6358,
    /* pattern (on, length): */
    2170, 3237,
    132, 265,
    132, 535,
    132, 265,
    132, 265,
    132, 265,
    132, 265,
    132, 265,
    132, 265,
    132, 1202,
    132, 265,
    132, 535,
    132, 535,
    132, 535,
    132, 265,
    132, 265,
    132, 265,
    132, 265,
    132, 0,
    /***************************************************
     * code db/tvbgone-118.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 34800:    pwm8:  28 ( 35714), pwm1: 229 ( 34934)
     * error:         914,               134
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (229 << 8) | 1, 7705,
    /* pattern (on, length): */
    127, 895,
    127, 2692,
    127, 900,
    127, 2692,
    127, 900,
    127, 2692,
    127, 900,
    127, 900,
    127, 900,
    127, 900,
    127, 900,
    127, 900,
    127, 900,
    127, 900,
    127, 900,
    127, 1795,
    127, 0,
    /***************************************************
     * code db/tvbgone-119.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 2, 27500,
    /* data: */
    0x7a83, 0xf708,
    /***************************************************
     * code db/tvbgone-120.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xac53, 0xe817,
    /***************************************************
     * code db/tvbgone-121.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0x1818, 0x3fc0,
    /***************************************************
     * code db/tvbgone-122.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xc738, 0xef10,
    /***************************************************
     * code db/tvbgone-123.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 30460:    pwm8:  32 ( 31250), pwm1: 262 ( 30534)
     * error:         790,                74
     * selected freq8 */
    /* pwm, repeat, repeat_delay */
    (32 << 8) | 0, 0,
    /* pattern (on, length): */
    132, 770,
    132, 387,
    262, 390,
    132, 260,
    132, 260,
    132, 260,
    132, 6230,
    132, 772,
    132, 387,
    262, 517,
    132, 260,
    132, 260,
    132, 260,
    132, 30975,
    132, 772,
    132, 387,
    262, 517,
    132, 260,
    132, 260,
    132, 260,
    132, 0,
    /***************************************************
     * code db/tvbgone-124.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 2, 27500,
    /* data: */
    0x55aa, 0x3ac5,
    /***************************************************
     * code db/tvbgone-125.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 29510:    pwm8:  33 ( 30303), pwm1: 271 ( 29520)
     * error:         793,                10
     * selected freq8 */
    /* pwm, repeat, repeat_delay */
    (33 << 8) | 0, 0,
    /* pattern (on, length): */
    135, 815,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 5405,
    135, 810,
    135, 412,
    135, 272,
    262, 402,
    135, 412,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 31245,
    135, 810,
    135, 412,
    135, 272,
    262, 402,
    135, 412,
    135, 272,
    135, 272,
    135, 272,
    135, 272,
    135, 0,
    /***************************************************
     * code db/tvbgone-126.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 33600:    pwm8:  29 ( 34482), pwm1: 238 ( 33613)
     * error:         882,                13
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (238 << 8) | 0, 0,
    /* pattern (on, length): */
    135, 772,
    135, 382,
    267, 387,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 3840,
    135, 775,
    135, 382,
    135, 257,
    267, 387,
    135, 382,
    135, 257,
    135, 257,
    135, 257,
    267, 515,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 28410,
    135, 775,
    135, 382,
    135, 257,
    267, 387,
    135, 382,
    135, 257,
    135, 257,
    135, 257,
    267, 515,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 257,
    135, 0,
    /***************************************************
     * code db/tvbgone-127.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xfb04, 0xf708,
    /***************************************************
     * code db/tvbgone-128.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37430:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:        1031,               128
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 2, 15000,
    /* parameters */
    /* header on timing, header length */
     0x86b, 0xca1,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x8c, 0x224, 0x111,
    /* flags, bitcount */ 
     0x10,
    /* data: */
     0xd71f,
    /***************************************************
     * code db/tvbgone-129.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 32520:    pwm8:  30 ( 33333), pwm1: 246 ( 32520)
     * error:         813,                 0
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (246 << 8) | 1, 20750,
    /* parameters */
    /* header on timing, header length */
     0x0, 0x0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x82, 0x50f, 0x28d,
    /* flags, bitcount */ 
     0xc,
    /* data: */
     0x38c,
    /***************************************************
     * code db/tvbgone-130.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37430:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:        1031,               128
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 7783,
    /* pattern (on, length): */
    132, 512,
    132, 1017,
    132, 1017,
    132, 1017,
    132, 510,
    132, 510,
    132, 0,
    /***************************************************
     * code db/tvbgone-131.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xe718, 0xf708,
    /***************************************************
     * code db/tvbgone-133.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0x8e71, 0xf708,
    /***************************************************
     * code db/tvbgone-134.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (205 << 8) | 1, 29000,
    /* parameters */
    /* header on timing, header length */
     0x0, 0x0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x26, 0x6e5, 0x49a,
    /* flags, bitcount */ 
     0xb,
    /* data: */
     0x19f,
    /***************************************************
     * code db/tvbgone-135.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 34800:    pwm8:  28 ( 35714), pwm1: 229 ( 34934)
     * error:         914,               134
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (229 << 8) | 1, 27000,
    /* parameters */
    /* header on timing, header length */
     0x37f, 0x6fe,
    /* on length, length one (on+pause), length zero (on+pause) */
     0xdf, 0x37f, 0x1c0,
    /* flags, bitcount */ 
     0x318,
    /* data: */
     0x9296, 0xd6,
    /***************************************************
     * code db/tvbgone-136.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 32479:    pwm8:  30 ( 33333), pwm1: 246 ( 32520)
     * error:         854,                41
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (246 << 8) | 1, 7828,
    /* pattern (on, length): */
    122, 887,
    130, 2685,
    130, 890,
    130, 2685,
    130, 890,
    130, 2685,
    130, 890,
    130, 1787,
    130, 1787,
    130, 1787,
    130, 1787,
    130, 890,
    130, 890,
    130, 0,
    /***************************************************
     * code db/tvbgone-137.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0x6f80, 0xf50a,
    /***************************************************
     * code db/tvbgone-138.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 40580:    pwm8:  24 ( 41666), pwm1: 197 ( 40609)
     * error:        1086,                29
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (197 << 8) | 0, 0,
    /* pattern (on, length): */
    2667, 13490,
    2672, 0,
    /***************************************************
     * code db/tvbgone-139.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xb748, 0xff00,
    /***************************************************
     * code db/tvbgone-000.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37520:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         941,                38
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 6888,
    /* pattern (on, length): */
    612, 762,
    307, 457,
    152, 302,
    307, 457,
    152, 302,
    307, 457,
    152, 302,
    152, 302,
    307, 457,
    152, 302,
    152, 302,
    152, 302,
    152, 0,
    /***************************************************
     * code db/tvbgone-001.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 55600:    pwm8:  17 ( 58823), pwm1: 143 ( 55944)
     * error:        3223,               344
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (143 << 8) | 1, 1988,
    /* pattern (on, length): */
    1027, 2052,
    130, 642,
    130, 642,
    130, 642,
    130, 642,
    130, 385,
    130, 385,
    130, 642,
    130, 385,
    130, 642,
    130, 385,
    130, 642,
    130, 385,
    130, 385,
    130, 385,
    130, 385,
    130, 385,
    130, 642,
    130, 642,
    130, 385,
    130, 642,
    130, 385,
    130, 642,
    130, 385,
    130, 642,
    130, 0,
    /***************************************************
     * code db/tvbgone-002.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 36130:    pwm8:  27 ( 37037), pwm1: 221 ( 36199)
     * error:         907,                69
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (221 << 8) | 1, 34500,
    /* parameters */
    /* header on timing, header length */
     0x37f, 0x53f,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x6e, 0x1c0, 0xdf,
    /* flags, bitcount */ 
     0x330,
    /* data: */
     0x2002, 0x80, 0xbd3d, 
    /***************************************************
     * code db/tvbgone-003.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 11693,
    /* pattern (on, length): */
    70, 542,
    72, 275,
    72, 275,
    72, 275,
    72, 275,
    72, 275,
    72, 545,
    72, 545,
    72, 275,
    72, 545,
    72, 275,
    72, 275,
    72, 275,
    72, 545,
    72, 275,
    72, 11747,
    72, 545,
    72, 275,
    72, 275,
    72, 275,
    72, 275,
    72, 545,
    72, 275,
    72, 275,
    72, 545,
    72, 275,
    72, 545,
    72, 545,
    72, 545,
    72, 275,
    72, 545,
    72, 0,
    /***************************************************
     * code db/tvbgone-004.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xbf40, 0xed12,
    /***************************************************
     * code db/tvbgone-005.yaml
     ***************************************************/

    (uint16_t)send_rc5,
    /* freq 36000:    pwm8:  27 ( 37037), pwm1: 222 ( 36036)
     * error:        1037,                36
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (222 << 8) | 1, 30000,
    /* data: */
    0x180,
    /***************************************************
     * code db/tvbgone-006.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 27250,
    /* parameters */
    /* header on timing, header length */
     0x483, 0x929,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x85, 0x23a, 0x120,
    /* flags, bitcount */ 
     0x320,
    /* data: */
     0x707, 0xfd02, 
    /***************************************************
     * code db/tvbgone-007.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 38200:    pwm8:  26 ( 38461), pwm1: 209 ( 38277)
     * error:         261,                77
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (209 << 8) | 1, 31043,
    /* pattern (on, length): */
    130, 252,
    130, 1177,
    130, 1435,
    130, 252,
    130, 1177,
    130, 252,
    130, 1177,
    130, 1435,
    130, 252,
    130, 1177,
    130, 1435,
    130, 252,
    130, 1177,
    130, 1435,
    130, 1435,
    130, 252,
    130, 0,
    /***************************************************
     * code db/tvbgone-008.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 28000,
    /* parameters */
    /* header on timing, header length */
     0x906, 0xd89,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x94, 0x244, 0x127,
    /* flags, bitcount */ 
     0x320,
    /* data: */
     0x8e71, 0xb54a, 
    /***************************************************
     * code db/tvbgone-009.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 37430:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:        1031,               128
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 2, 27500,
    /* data: */
    0x2287, 0x1fe0,
    /***************************************************
     * code db/tvbgone-010.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 2, 15000,
    /* parameters */
    /* header on timing, header length */
     0x86b, 0xca1,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x85, 0x217, 0x109,
    /* flags, bitcount */ 
     0x210,
    /* data: */
     0x1703,
    /***************************************************
     * code db/tvbgone-011.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xaf50, 0xe817,
    /***************************************************
     * code db/tvbgone-012.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 26000,
    /* parameters */
    /* header on timing, header length */
     0x357, 0x6b1,
    /* on length, length one (on+pause), length zero (on+pause) */
     0xd5, 0x35f, 0x1ae,
    /* flags, bitcount */ 
     0x318,
    /* data: */
     0xa1ad, 0xe4,
    /***************************************************
     * code db/tvbgone-013.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37520:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         941,                38
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 0, 0,
    /* pattern (on, length): */
    2295, 3442,
    140, 277,
    140, 277,
    140, 565,
    140, 565,
    140, 565,
    140, 277,
    140, 277,
    140, 277,
    140, 565,
    140, 277,
    140, 277,
    140, 277,
    140, 277,
    140, 565,
    140, 565,
    140, 277,
    140, 277,
    140, 277,
    140, 565,
    140, 565,
    140, 565,
    140, 277,
    140, 565,
    140, 565,
    140, 565,
    140, 565,
    140, 277,
    140, 565,
    140, 277,
    140, 277,
    140, 565,
    140, 277,
    140, 277,
    140, 277,
    140, 565,
    140, 277,
    140, 565,
    140, 565,
    140, 277,
    140, 565,
    140, 565,
    140, 565,
    140, 6050,
    2300, 3445,
    140, 24190,
    2300, 3445,
    140, 0,
    /***************************************************
     * code db/tvbgone-014.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0x9f60, 0xfc03,
    /***************************************************
     * code db/tvbgone-015.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xbd42, 0xfe01,
    /***************************************************
     * code db/tvbgone-016.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 33600:    pwm8:  29 ( 34482), pwm1: 238 ( 33613)
     * error:         882,                13
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (238 << 8) | 1, 13000,
    /* parameters */
    /* header on timing, header length */
     0x0, 0x0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x4b, 0x267, 0x12f,
    /* flags, bitcount */ 
     0x10,
    /* data: */
     0x247,
    /***************************************************
     * code db/tvbgone-017.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xc738, 0xed12,
    /***************************************************
     * code db/tvbgone-018.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 2, 23000,
    /* parameters */
    /* header on timing, header length */
     0x887, 0xcd0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x85, 0x21f, 0x10e,
    /* flags, bitcount */ 
     0x320,
    /* data: */
     0x55aa, 0xe31c, 
    /***************************************************
     * code db/tvbgone-019.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 25750,
    /* parameters */
    /* header on timing, header length */
     0x346, 0x4e5,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x6c, 0x1a7, 0xd5,
    /* flags, bitcount */ 
     0x330,
    /* data: */
     0x6314, 0x8400, 0x284, 
    /***************************************************
     * code db/tvbgone-020.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xfb04, 0xf708,
    /***************************************************
     * code db/tvbgone-021.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 6140,
    /* pattern (on, length): */
    2052, 3075,
    127, 257,
    127, 257,
    127, 535,
    127, 257,
    127, 535,
    127, 257,
    127, 257,
    127, 257,
    127, 1150,
    127, 535,
    127, 257,
    127, 535,
    127, 257,
    127, 535,
    127, 257,
    127, 257,
    127, 257,
    127, 0,
    /***************************************************
     * code db/tvbgone-022.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0x1818, 0x3fc0,
    /***************************************************
     * code db/tvbgone-023.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 39010:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:         990,                14
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 1, 26940,
    /* pattern (on, length): */
    127, 1417,
    127, 1417,
    127, 257,
    127, 1172,
    127, 257,
    127, 1172,
    127, 1417,
    127, 257,
    127, 1172,
    127, 1417,
    127, 257,
    127, 1172,
    127, 1417,
    127, 1417,
    127, 257,
    127, 1172,
    127, 257,
    127, 1172,
    127, 1417,
    127, 257,
    127, 1172,
    127, 0,
    /***************************************************
     * code db/tvbgone-024.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 1, 6578,
    /* pattern (on, length): */
    612, 762,
    307, 457,
    307, 457,
    307, 457,
    307, 457,
    152, 302,
    307, 457,
    152, 302,
    307, 457,
    152, 302,
    152, 302,
    152, 302,
    152, 0,
    /***************************************************
     * code db/tvbgone-025.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 27000,
    /* parameters */
    /* header on timing, header length */
     0x37d, 0x6fc,
    /* on length, length one (on+pause), length zero (on+pause) */
     0xdc, 0x37f, 0x1c0,
    /* flags, bitcount */ 
     0x318,
    /* data: */
     0x7f, 0xf8,
    /***************************************************
     * code db/tvbgone-026.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 38200:    pwm8:  26 ( 38461), pwm1: 209 ( 38277)
     * error:         261,                77
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (209 << 8) | 1, 32293,
    /* pattern (on, length): */
    130, 252,
    130, 1177,
    130, 1435,
    130, 252,
    130, 1177,
    130, 252,
    130, 1177,
    130, 1435,
    130, 1435,
    130, 252,
    130, 1177,
    130, 1435,
    130, 252,
    130, 1177,
    130, 1435,
    130, 252,
    130, 0,
    /***************************************************
     * code db/tvbgone-027.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 55600:    pwm8:  17 ( 58823), pwm1: 143 ( 55944)
     * error:        3223,               344
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (143 << 8) | 1, 16500,
    /* parameters */
    /* header on timing, header length */
     0x404, 0x805,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x82, 0x283, 0x181,
    /* flags, bitcount */ 
     0x318,
    /* data: */
     0x154e, 0xab,
    /***************************************************
     * code db/tvbgone-028.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37430:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:        1031,               128
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 26750,
    /* parameters */
    /* header on timing, header length */
     0x29c, 0x54e,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x134, 0x3e8, 0x267,
    /* flags, bitcount */ 
     0x310,
    /* data: */
     0xa36a,
    /***************************************************
     * code db/tvbgone-029.yaml
     ***************************************************/

    (uint16_t)send_rc5,
    /* freq 36000:    pwm8:  27 ( 37037), pwm1: 222 ( 36036)
     * error:        1037,                36
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (222 << 8) | 1, 28500,
    /* data: */
    0x320,
    /***************************************************
     * code db/tvbgone-030.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xad50, 0xff00,
    /***************************************************
     * code db/tvbgone-031.yaml
     ***************************************************/

    (uint16_t)send_rc5,
    /* freq 36000:    pwm8:  27 ( 37037), pwm1: 222 ( 36036)
     * error:        1037,                36
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (222 << 8) | 1, 28500,
    /* data: */
    0x180,
    /***************************************************
     * code db/tvbgone-032.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xad50, 0xfd02,
    /***************************************************
     * code db/tvbgone-033.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (213 << 8) | 1, 27500,
    /* parameters */
    /* header on timing, header length */
     0x35c, 0x4ea,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x6c, 0x1a2, 0xd7,
    /* flags, bitcount */ 
     0x330,
    /* data: */
     0x6314, 0x8400, 0x8a, 
    /***************************************************
     * code db/tvbgone-034.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0xaf50, 0xc03f,
    /***************************************************
     * code db/tvbgone-035.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 2, 27500,
    /* data: */
    0xe718, 0xf708,
    /***************************************************
     * code db/tvbgone-036.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 40580:    pwm8:  24 ( 41666), pwm1: 197 ( 40609)
     * error:        1086,                29
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (197 << 8) | 1, 15425,
    /* parameters */
    /* header on timing, header length */
     0x0, 0x0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0xfd, 0x3d9, 0x1e8,
    /* flags, bitcount */ 
     0xa,
    /* data: */
     0xa8,
    /***************************************************
     * code db/tvbgone-037.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 36090:    pwm8:  27 ( 37037), pwm1: 221 ( 36199)
     * error:         947,               109
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (221 << 8) | 0, 0,
    /* parameters */
    /* header on timing, header length */
     0x0, 0x0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0xdc, 0x6ac, 0x35a,
    /* flags, bitcount */ 
     0xa,
    /* data: */
     0x1d1,
    /***************************************************
     * code db/tvbgone-038.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 40580:    pwm8:  24 ( 41666), pwm1: 197 ( 40609)
     * error:        1086,                29
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (197 << 8) | 0, 0,
    /* pattern (on, length): */
    1325, 1742,
    105, 777,
    105, 777,
    105, 777,
    425, 840,
    105, 777,
    425, 840,
    425, 840,
    105, 777,
    105, 777,
    105, 0,
    /***************************************************
     * code db/tvbgone-039.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 2, 27500,
    /* data: */
    0xea15, 0xed12,
    /***************************************************
     * code db/tvbgone-041.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 0, 0,
    /* pattern (on, length): */
    295, 550,
    295, 552,
    295, 807,
    295, 7240,
    295, 552,
    295, 552,
    295, 807,
    292, 0,
    /***************************************************
     * code db/tvbgone-042.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 74940:    pwm8:  13 ( 76923), pwm1: 106 ( 75471)
     * error:        1983,               531
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (106 << 8) | 1, 7075,
    /* pattern (on, length): */
    625, 782,
    302, 460,
    150, 307,
    302, 460,
    150, 307,
    302, 460,
    150, 307,
    150, 307,
    302, 460,
    150, 307,
    150, 307,
    150, 307,
    150, 0,
    /***************************************************
     * code db/tvbgone-043.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 2, -28888,
    /* pattern (on, length): */
    2310, 3387,
    140, 572,
    140, 572,
    140, 305,
    140, 305,
    140, 305,
    140, 305,
    140, 305,
    140, 572,
    140, 305,
    140, 572,
    140, 305,
    140, 572,
    140, 572,
    140, 572,
    140, 572,
    140, 305,
    140, 305,
    140, 305,
    140, 305,
    140, 572,
    140, 305,
    140, 305,
    140, 305,
    140, 305,
    140, 572,
    140, 572,
    140, 572,
    140, 305,
    140, 572,
    140, 572,
    140, 572,
    140, 572,
    140, 10660,
    2310, 2887,
    140, 22387,
    2310, 2887,
    137, 0,
    /***************************************************
     * code db/tvbgone-044.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 39010:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:         990,                14
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 1, 8958,
    /* pattern (on, length): */
    115, 422,
    115, 422,
    115, 422,
    115, 422,
    115, 422,
    340, 452,
    115, 422,
    115, 422,
    115, 422,
    115, 422,
    115, 422,
    115, 0,
    /***************************************************
     * code db/tvbgone-045.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 2, 27500,
    /* data: */
    0xf807, 0xfd02,
    /***************************************************
     * code db/tvbgone-046.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 37470:    pwm8:  26 ( 38461), pwm1: 213 ( 37558)
     * error:         991,                88
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (213 << 8) | 0, 0,
    /* pattern (on, length): */
    1172, 2320,
    152, 285,
    152, 577,
    152, 577,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 577,
    152, 577,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 577,
    152, 577,
    152, 577,
    152, 577,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 285,
    152, 577,
    152, 577,
    152, 577,
    152, 577,
    152, 11682,
    1175, 2322,
    152, 577,
    152, 24995,
    1175, 2322,
    152, 577,
    152, 0,
    /***************************************************
     * code db/tvbgone-047.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 28670:    pwm8:  34 ( 29411), pwm1: 279 ( 28673)
     * error:         741,                 3
     * selected freq8 */
    /* pwm, repeat, repeat_delay */
    (34 << 8) | 0, 0,
    /* pattern (on, length): */
    140, 845,
    140, 405,
    270, 400,
    140, 270,
    140, 270,
    140, 270,
    140, 6620,
    140, 845,
    140, 405,
    140, 270,
    140, 270,
    140, 270,
    140, 270,
    140, 270,
    140, 33015,
    140, 845,
    140, 405,
    140, 270,
    140, 270,
    140, 270,
    140, 270,
    140, 270,
    140, 0,
    /***************************************************
     * code db/tvbgone-048.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 2, 27500,
    /* data: */
    0xef10, 0x10ef,
    /***************************************************
     * code db/tvbgone-049.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 0, 0,
    /* pattern (on, length): */
    1110, 2225,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 262,
    135, 542,
    135, 542,
    135, 542,
    135, 542,
    135, 10645,
    2270, 2830,
    135, 24552,
    2270, 2830,
    135, 0,
    /***************************************************
     * code db/tvbgone-050.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 44330:    pwm8:  22 ( 45454), pwm1: 180 ( 44444)
     * error:        1124,               114
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (180 << 8) | 0, 0,
    /* pattern (on, length): */
    705, 2895,
    705, 5800,
    705, 5800,
    705, 2895,
    705, 2895,
    705, 5800,
    705, 2895,
    705, 5800,
    705, 5800,
    705, 2895,
    702, 0,
    /***************************************************
     * code db/tvbgone-052.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 54160:    pwm8:  18 ( 55555), pwm1: 147 ( 54421)
     * error:        1395,               261
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (147 << 8) | 1, 9610,
    /* pattern (on, length): */
    922, 1770,
    207, 430,
    207, 430,
    207, 430,
    207, 430,
    207, 430,
    207, 430,
    207, 430,
    207, 430,
    207, 430,
    207, 430,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 857,
    207, 430,
    207, 0,
    /***************************************************
     * code db/tvbgone-053.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38000:    pwm8:  26 ( 38461), pwm1: 210 ( 38095)
     * error:         461,                95
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (210 << 8) | 2, 27500,
    /* data: */
    0x3bc4, 0xe718,
    /***************************************************
     * code db/tvbgone-054.yaml
     ***************************************************/

    (uint16_t)send_nec,
    /* freq 38970:    pwm8:  25 ( 40000), pwm1: 205 ( 39024)
     * error:        1030,                54
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (205 << 8) | 2, 27500,
    /* data: */
    0xc837, 0xed12,
    /***************************************************
     * code db/tvbgone-055.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 54070:    pwm8:  18 ( 55555), pwm1: 147 ( 54421)
     * error:        1485,               351
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (147 << 8) | 0, 0,
    /* pattern (on, length): */
    132, 725,
    132, 2162,
    132, 725,
    132, 2162,
    132, 725,
    132, 2162,
    132, 725,
    132, 1442,
    132, 1442,
    132, 725,
    132, 725,
    132, 725,
    132, 725,
    132, 1442,
    132, 7530,
    132, 725,
    132, 2162,
    132, 725,
    132, 2162,
    132, 725,
    132, 2162,
    132, 725,
    132, 1442,
    132, 1442,
    132, 725,
    132, 725,
    132, 725,
    132, 725,
    132, 1442,
    132, 0,
    /***************************************************
     * code db/unzap-200.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 40000:    pwm8:  25 ( 40000), pwm1: 200 ( 40000)
     * error:           0,                 0
     * selected freq8 */
    /* pwm, repeat, repeat_delay */
    (25 << 8) | 2, 5000,
    /* pattern (on, length): */
    2000, 3000,
    125, 250,
    125, 250,
    125, 500,
    125, 250,
    125, 500,
    125, 500,
    125, 500,
    125, 500,
    125, 1125,
    125, 500,
    125, 500,
    125, 250,
    125, 250,
    125, 250,
    125, 250,
    125, 250,
    125, 250,
    125, 0,
    /***************************************************
     * code db/unzap-201.yaml
     ***************************************************/

    (uint16_t)send_pause,
    /* freq 36000:    pwm8:  27 ( 37037), pwm1: 222 ( 36036)
     * error:        1037,                36
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
     (222 << 8) | 3, 30500,
    /* parameters */
    /* header on timing, header length */
     0x0, 0x0,
    /* on length, length one (on+pause), length zero (on+pause) */
     0x26, 0x792, 0x508,
    /* flags, bitcount */ 
     0xb,
    /* data: */
     0x5de,
