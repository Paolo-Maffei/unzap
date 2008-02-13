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

    (uint16_t)send_raw,
    /* freq 34800:    pwm8:  28 ( 35714), pwm1: 229 ( 34934)
     * error:         914,               134
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (229 << 8) | 1, 24020,
    /* pattern (on, length): */
    230, 460,
    460, 690,
    230, 460,
    230, 460,
    230, 460,
    230, 460,
    230, 460,
    230, 460,
    230, 690,
    230, 460,
    460, 690,
    230, 0,
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
     * code db/tvbgone-132.yaml
     ***************************************************/

    (uint16_t)send_raw,
    /* freq 34800:    pwm8:  28 ( 35714), pwm1: 229 ( 34934)
     * error:         914,               134
     * selected freq1 */
    /* pwm, repeat, repeat_delay */
    (229 << 8) | 1, 23270,
    /* pattern (on, length): */
    230, 460,
    460, 690,
    230, 460,
    230, 460,
    230, 460,
    230, 460,
    230, 460,
    230, 460,
    230, 690,
    230, 460,
    460, 690,
    230, 0,
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
     0x26, 0x76c, 0x4e2,
    /* flags, bitcount */ 
     0xb,
    /* data: */
     0x5de,
