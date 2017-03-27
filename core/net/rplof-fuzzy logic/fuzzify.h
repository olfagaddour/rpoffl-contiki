/*
 *  fuzzify.h
 *  
 *
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */


unsigned short consumption(unsigned short e, unsigned short t);
unsigned short reliability(unsigned short l, unsigned short e);
unsigned short duration(unsigned short h, unsigned short l);
unsigned short qos(unsigned short rel,unsigned short  dur);
unsigned short quality(unsigned short c, unsigned short q);

#define TRUE 100
#define FALSE 0


#define min(x,y) ((x)>(y))?(y):(x)

#define max(x,y) ((x)<(y))?(y):(x)


/*******************************
 *           Energy            *
 ******************************/

#define ENERGY_MAX 255

#define E_B_1 ENERGY_MAX / 5
#define E_B_2 E_B_1 * 2
#define E_B_3 E_B_1 * 3
#define E_B_4 E_B_1 * 4

#define energy_less(e) FIRST_T_NORM(E_B_1, E_B_2, (e))
#define energy_avg(e) T_NORM(E_B_1, E_B_2, E_B_3, E_B_4, (e))
#define energy_full(e) LAST_T_NORM(E_B_3, E_B_4, (e))

/*******************************
 *        Throughput           *
 ******************************/

#define THROUGHPUT_MAX 1500

#define T_B_1 THROUGHPUT_MAX / 5
#define T_B_2 T_B_1 * 2
#define T_B_3 T_B_1 * 3
#define T_B_4 T_B_1 * 4

#define throughput_low(e) FIRST_T_NORM(T_B_1, T_B_2, (e))
#define throughput_avg(e) T_NORM(T_B_1, T_B_2, T_B_3, T_B_4, (e))
#define throughput_hight(e) LAST_T_NORM(T_B_3, T_B_4, (e))

/*******************************
 *            ETX              *
 ******************************/

#define ETX_MAX 65535
/*
#define ETX_B_1 ETX_MAX / 5
#define ETX_B_2 ETX_B_1 * 2
#define ETX_B_3 ETX_B_1 * 3
#define ETX_B_4 ETX_B_1 * 4
*/

#define ETX_B_1 12800
#define ETX_B_2 ETX_B_1 + 128
#define ETX_B_3 ETX_B_2 + 128
#define ETX_B_4 ETX_B_3 + 128


#define etx_short(e) FIRST_T_NORM(ETX_B_1, ETX_B_2, (e))
#define etx_avg(e) T_NORM(ETX_B_1, ETX_B_2, ETX_B_3, ETX_B_4, (e))
#define etx_long(e) LAST_T_NORM(ETX_B_3, ETX_B_4, (e))

/*******************************
 *            LQL              *
 ******************************/

#define LQL_MAX  7

#define LQ_B_1 2
#define LQ_B_2 3
#define LQ_B_3 5
#define LQ_B_4 6

#define lql_low(x) FIRST_T_NORM(LQ_B_1, LQ_B_2, (x))
#define lql_avg(x) T_NORM(LQ_B_1, LQ_B_2, LQ_B_3, LQ_B_4, (x))
#define lql_hight(x) LAST_T_NORM(LQ_B_3, LQ_B_4, (x))


/*******************************
 *        Hop Count            *
 ******************************/

#define HOP_COUNT_MAX 20

#define H_B_1 HOP_COUNT_MAX / 5
#define H_B_2 H_B_1 * 2
#define H_B_3 H_B_1 * 3
#define H_B_4 H_B_1 * 4

#define hop_near(x) FIRST_T_NORM(H_B_1, H_B_2, (x))
#define hop_vicinity(x) T_NORM(H_B_1, H_B_2, H_B_3, H_B_4, (x))
#define hop_far(x) LAST_T_NORM(H_B_3, H_B_4, (x))

/*******************************
 *          Latency            *
 ******************************/

#define LATENCY_MAX  500

#define L_B_1 LATENCY_MAX / 5
#define L_B_2 L_B_1 * 2
#define L_B_3 L_B_1 * 3
#define L_B_4 L_B_1 * 4

#define latency_short(x) FIRST_T_NORM(L_B_1, L_B_2, (x))
#define latency_avg(x) T_NORM(L_B_1, L_B_2, L_B_3, L_B_4, (x))
#define latency_long(x) LAST_T_NORM(L_B_3, L_B_4, (x))


/*******************************
 *       Output metrics 1      *
 ******************************/

#define O1_B1 15
#define O1_B2 25
#define O1_B3 35
#define O1_B4 45
#define O1_B5 55
#define O1_B6 65
#define O1_B7 75
#define O1_B8 85

#define duration_very_slow(x) FIRST_T_NORM(O1_B1, O1_B2,(x))
#define duration_slow(x) T_NORM(O1_B1, O1_B2, O1_B3, O1_B4, (x))
#define duration_avg(x) T_NORM(O1_B3, O1_B4, O1_B5, O1_B6, (x))
#define duration_fast(x) T_NORM(O1_B5, O1_B6, O1_B7, O1_B8, (x))
#define duration_very_fast(x) LAST_T_NORM(O1_B7, O1_B8, (x))

#define reliability_very_weak(x) FIRST_T_NORM(O1_B1, O1_B2,(x))
#define reliability_weak(x) T_NORM(O1_B1, O1_B2, O1_B3, O1_B4, (x))
#define reliability_avg(x) T_NORM(O1_B3, O1_B4, O1_B5, O1_B6, (x))
#define reliability_strong(x) T_NORM(O1_B5, O1_B6, O1_B7, O1_B8, (x))
#define reliability_very_strong(x) LAST_T_NORM(O1_B7, O1_B8, (x))

#define consumption_very_costly(x) FIRST_T_NORM(O1_B1, O1_B2,(x))
#define consumption_costly(x) T_NORM(O1_B1, O1_B2, O1_B3, O1_B4, (x))
#define consumption_avg(x) T_NORM(O1_B3, O1_B4, O1_B5, O1_B6, (x))
#define consumption_cheap(x) T_NORM(O1_B5, O1_B6, O1_B7, O1_B8, (x))
#define consumption_very_cheap(x) LAST_T_NORM(O1_B7, O1_B8, (x))

/*******************************
 *       Output metrics 2      *
 ******************************/

#define O2_B1 8
#define O2_B2 14
#define O2_B3 19
#define O2_B4 25
#define O2_B5 30
#define O2_B6 36
#define O2_B7 41
#define O2_B8 47
#define O2_B9 52
#define O2_B10 58
#define O2_B11 63
#define O2_B12 69
#define O2_B13 74
#define O2_B14 80
#define O2_B15 85
#define O2_B16 90

#define qos_awful(x) FIRST_T_NORM(O2_B1, O2_B2, (x))
#define qos_very_bad(x) T_NORM(O2_B1, O2_B2, O2_B3, O2_B4, (x))
#define qos_bad(x) T_NORM(O2_B3, O2_B4, O2_B5, O2_B6, (x))
#define qos_degraded(x) T_NORM(O2_B5, O2_B6, O2_B7, O2_B8, (x))
#define qos_avg(x) T_NORM(O2_B7, O2_B8, O2_B9, O2_B10, (x))
#define qos_acceptable(x) T_NORM(O2_B9, O2_B10, O2_B11, O2_B12, (x))
#define qos_good(x) T_NORM(O2_B11, O2_B12, O2_B13, O2_B14, (x))
#define qos_very_good(x) T_NORM(O2_B13, O2_B14, O2_B15, O2_B16, (x))
#define qos_excellent(x) LAST_T_NORM(O2_B15, O2_B16, (x))
