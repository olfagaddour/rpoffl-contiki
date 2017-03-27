#include "fuzzify.h"
#include "stdio.h"

int main(int argc, char *args[]){

  int i = 0;

  int e,t,h,l,etx,lql;

  if (argc > 2){
  e = atoi(args[1]);
  t = atoi(args[2]);
  
  lql = atoi(args[3]);
  etx = atoi(args[4]);
  
  h = atoi(args[5]);
  l = atoi(args[6]);
  
  unsigned int c, r, d, q;
  printf("cons : %u\n",consumption(e, t));
  printf("rel : %u\n",reliability(lql,etx));
  printf("dur : %u\n",duration(h,l));
  printf("%u\n",quality(consumption(e,t),qos(reliability(lql,etx), duration(h,l))));
  }
  else {
    /*
  unsigned int c, r, d, q;
  printf("cons : %u\n",consumption(e, t));
  printf("rel : %u\n",reliability(lql,etx));
  printf("dur : %u\n",duration(h,l));
  */
    /*
    for (e = 0 ; e < 255; e++)
      printf("%u,",e);
  printf("255\n");

  for (t = 0; t <= 1500; t+=10){
      printf("%u,",t);
    for (e = 0 ; e < 256; e++){
      printf("%u,",consumption(e, t));
    }
    printf("\n");
  }
    */
    /*
    for (lql = 0; lql < 7; lql++)
      printf("%u,",lql);
    printf("7\n");
    
    for (etx = 13184; etx >= 12800 ; etx-=1){
      printf("%u,",etx);
      for (lql = 1; lql <= 7; lql++){
	printf("%u,",reliability(lql,etx));
      }
      printf("\n");
    }
    */
    for (h = 21; h > 0; h--)
      printf("%u,",h);
    printf("0\n");

    for (l = 42600; l >= 0; l-=100){
      printf("%u,",l);
      for (h = 20; h > 0; h--){
	printf("%u,",duration(h,l));
    }
    printf("\n");
  }
  


    /*
  for (e = 0; e < 256; e+=51)
    for (t = 0; t < 1600; t+=300)
      for (lql = 1; lql < 8; lql++)
	for (etx = 13184; etx >= 12800 ; etx-=128)
	  for (h = 20; h >= 0; h-=5)
	    for (l = 42600; l >= 0; l -= 8520) 
	      printf("%u,%u,%u,%u,%u,%u,%u\n",e,t,lql,etx,h,l,quality(consumption(e,t),qos(reliability(lql,etx), duration(h,l))));
    */
 
}
}

 /*
  r =  reliability(103,56968);
  d = duration(60 ,20000);
  q = qos(r, d);
  printf("%u %u %u %u : %u\n",c,r,d,q, quality(c, q));
  */

 /*
  for (i = 0; i < 42600000; i+=1000)
    printf("%u  : %u %u %u\n",i,  latency_short(i), latency_avg(i),latency_long(i));
  */
  /*
  for (i = 0; i < 255; i+=1)
    printf("%u  : %u %u %u\n",i,  energy_less(i), energy_avg(i),energy_full(i));
  */
  /*
  for (i = 0; i < 10; i+=1)
    printf("%u  : %u %u %u\n",i,  lql_low(i), lql_avg(i),lql_hight(i));
  */
