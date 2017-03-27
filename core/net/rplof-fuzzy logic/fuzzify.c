/*
 *  fuzzify.c
 *  
 *
 * 
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "fuzzify.h"

unsigned short DOWN(unsigned short x1, unsigned short x2, unsigned short X){
	return (TRUE * ((long)(X) - (long)(x2)))/((long)(x1) - (long)(x2));
}

unsigned short UP(unsigned short x1, unsigned short x2, unsigned short X){
	return DOWN(x2, x1, X);
}

unsigned short FIRST_T_NORM(unsigned short b1, unsigned short b2, unsigned short px){
  if (px <= b1) return TRUE;
  if (px > b1 && px <= b2) return DOWN(b1, b2, px);
  return FALSE;
}

unsigned short LAST_T_NORM(unsigned short b1, unsigned short b2, unsigned short px){
  if (px <= b1) return FALSE;
  if (px > b1 && px <= b2) return UP(b1, b2, px);
  return TRUE;
}

unsigned short T_NORM(unsigned short b1, unsigned short b2, unsigned short b3, unsigned short b4, unsigned short px){
  if (px <= b1) return 0;
  if (px > b1 && px <= b2) return UP(b1, b2, px);
  if (px > b2 && px <= b3) return TRUE;
  if (px > b3 && px <= b4) return DOWN(b3, b4, px);
  return FALSE;
}

unsigned short COG_1(unsigned short fv1,unsigned short fv2,unsigned short fv3,unsigned short fv4, unsigned short fv5){

  return (fv1*10 + fv2 * 30 + fv3 * 50 + fv4 * 70 + fv5 * 90)/(fv1 + fv2 + fv3 + fv4 + fv5);
}

unsigned short compose(unsigned short c1, unsigned short c2, unsigned short c3, 
		     unsigned short l1, unsigned short l2, unsigned short l3){
  unsigned short b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0;

  if(c1 && l1) b1 = min(c1,l1);
  if (c1 && l2 || c2 && l1) b2 = max(min(c1,l2),min(c2,l1));
  if (c3 && l1 || c2 && l2 || c1 && l3) b3 = max(min(c3,l1),max(min(c2,l2),min(c1,l3)));
  if (c3 && l2 || c2 && l3) b4 = max(min(c3,l2),min(c2,l3));
  if (c3 && l3) b5 = min(c3,l3);
  return COG_1(b1,b2,b3,b4,b5);
}

unsigned short consumption(unsigned short e, unsigned short t){

  return compose(energy_less(e),energy_avg(e),energy_full(e),throughput_low(t),throughput_avg(t),throughput_hight(t));
}


unsigned short reliability(unsigned short lql, unsigned short etx){

  return compose(lql_low(lql),lql_avg(lql),lql_hight(lql),etx_long(etx),etx_avg(etx),etx_short(etx));

}

unsigned short duration(unsigned short hc, unsigned short lat){

  return compose(hop_far(hc),hop_vicinity(hc),hop_near(hc),latency_long(lat),latency_avg(lat),latency_short(lat));
}
