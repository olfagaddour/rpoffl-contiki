/**
 * \addtogroup uip6
 * @{
 */
/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/**
 * \file
 *         The minrank-hysteresis objective function (OCP 1).
 *
 *         This implementation uses the estimated number of 
 *         transmissions (ETX) as the additive routing metric.
 *
 * \author Joakim Eriksson <joakime@sics.se>, Nicolas Tsiftes <nvt@sics.se>
 * \this is an implementation of the objective function based on fuzzy logic implemented by Olfa Gaddour <olfa.gaddour@ceslab.org>
 */

#include "net/rplfuzzy/rpl-private.h"
#include "net/neighbor-info.h"
#include "dev/battery-sensor.h"

#include "dev/cooja-radio.h"

//#include "net/rime/rimeaddr.h"

#define DEBUG DEBUG_ANNOTATE
#include "net/uip-debug.h"

#include "net/uip.h";

#include "lib/memb.h";

#include "fuzzify.h"

MEMB(mc_memb, rpl_metric_container_t,6);


static void reset(rpl_dag_t *);
static void parent_state_callback(rpl_parent_t *, int, int);
static rpl_parent_t *best_parent(rpl_parent_t *, rpl_parent_t *);
static rpl_rank_t calculate_rank(rpl_parent_t *, rpl_rank_t);
static void update_metric_container(rpl_dag_t *);


static rpl_dag_t *mydag;
static void action(int);

rpl_of_t rpl_of_fuzzy = {
  reset,
  parent_state_callback,
  best_parent,
  calculate_rank,
  update_metric_container,
  action,
  99
};

#define NI_ETX_TO_RPL_ETX(etx)						\
	((etx) * (RPL_DAG_MC_ETX_DIVISOR / NEIGHBOR_INFO_ETX_DIVISOR))
#define RPL_ETX_TO_NI_ETX(etx)					\
	((etx) / (RPL_DAG_MC_ETX_DIVISOR / NEIGHBOR_INFO_ETX_DIVISOR))

/* Reject parents that have a higher link metric than the following. */
#define MAX_LINK_METRIC			10

/* Reject parents that have a higher path cost than the following. */
#define MAX_PATH_COST			100

/*
 * The rank must differ more than 1/PARENT_SWITCH_THRESHOLD_DIV in order
 * to switch preferred parent.
 */
#define PARENT_SWITCH_THRESHOLD_DIV	2

typedef uint16_t rpl_path_metric_t;

struct list_t *list_metric;


static rpl_metric_container_t *energy, 
  *etx, 
  *hopcount, 
  *latency;


static uint32_t
calculate_path_metric(rpl_parent_t *p)
{

  if(p == NULL) {
    return MAX_PATH_COST * RPL_DAG_MC_ETX_DIVISOR;
  }

  rpl_metric_container_t *m,*etx_mc = NULL;
  for (m = list_head(p->mcs);m != NULL && etx_mc == NULL; m = m->next){
    switch(m->type){
    case RPL_DAG_MC_ETX:
      etx_mc = m;
      break;
    }
  }
  if (etx_mc == NULL){
    PRINTF("No ETX metric container for parent\n");
    return 0;
  }

  if(etx_mc->obj.etx == 0 && p->rank > ROOT_RANK(p->dag)) {
    return MAX_PATH_COST * RPL_DAG_MC_ETX_DIVISOR;
  }
  return etx_mc->obj.etx + NI_ETX_TO_RPL_ETX(p->link_metric);
}

static rpl_path_metric_t
calculate_fuzzy_metric(rpl_parent_t *p)
{

  rpl_metric_container_t *m;
  uint16_t energy, 
    hopcount, 
    etx,
  uint32_t latency;
  
  for (m = list_head(p->mcs);m != NULL; m = m->next){
    switch(m->type){
    case RPL_DAG_MC_ENERGY:
      energy = m->obj.energy.energy_est;
      break;
    /*case RPL_DAG_MC_THROUGHPUT:
      throughput = m->obj.throughput;
      break;*/
    case RPL_DAG_MC_HOPCOUNT:
      hopcount = m->obj.hopcount;
      break;
    case RPL_DAG_MC_LATENCY:
      latency = m->obj.latency;
      break;
    case RPL_DAG_MC_ETX:
      etx = m->obj.etx;
      break;
    /*case RPL_DAG_MC_LQL:*/
    /*  lql = m->obj.lql;*/
    /*  break;*/
    }
  }
  
  return quality(consumption(energy),
   		 qos(reliability(etx),
   		     duration(hopcount,latency)));
  /*return consumption(energy, throughput);*/
  
  
}

static rpl_path_metric_t
calculate_etx_path_metric(rpl_parent_t *p)
{
  if (mydag->rank == ROOT_RANK(mydag))
    return 0;
  return calculate_path_metric(p);
}



/*static rpl_path_metric_t
calculate_lql_path_metric(int nid)
{
  return 4;
}*/

static rpl_metric_container_t * calculate_incremental_metric(rpl_parent_t *p, int type){
  rpl_metric_container_t *m;
  if (p != NULL)
    for (m = list_head(p->mcs);m != NULL; m = m->next){
      if (m->type == type)
	return m;
    }
  return NULL;
}


static rpl_path_metric_t
calculate_hopcount_path_metric(rpl_parent_t *p){
  rpl_metric_container_t *m;
  if (p == NULL)
    return 0;
  m = calculate_incremental_metric(p, RPL_DAG_MC_HOPCOUNT);
  return m->obj.hopcount + 1;
}
  
  

static rpl_path_metric_t
calculate_latency_path_metric(rpl_parent_t *p){  
  rpl_metric_container_t *m;
  if (p == NULL)
    return 0;
  m = calculate_incremental_metric(p, RPL_DAG_MC_LATENCY);
  if (p->latency_metric ){
    return 200;/*m->obj.latency + *//*p->latency_metric;*/
  }
  return 0;
}



static struct rpl_metric_object_energy
calculate_energy_path_metric(int nid)
{
	if(nid != 1){
  /*
  SENSORS_ACTIVATE(battery_sensor);
  energy->obj.energy.energy_est = (uint16_t) battery_sensor.value(0);
  SENSORS_DEACTIVATE(battery_sensor);
  */

  /*
    ANNOTATE("TX : %lu en %lu ms, RX %lu en %lu ms throuhput %lu Kbit/s\n",cooja_radio_driver.radio_byte_transmit(),
	   cooja_radio_driver.radio_time_transmit(),
	   cooja_radio_driver.radio_byte_receive(),
	   cooja_radio_driver.radio_time_receive(),      
	   (cooja_radio_driver.radio_throughput())*8);

  */
  /* ANNOTATE("Times on %lu, Sended %lu Received %lu\n",contikimac_driver.time_ons(), */
  /* 	   contikimac_driver.sendeds(), */
  /* 	   contikimac_driver.receiveds()); */

  uint32_t energest;
  energest = (1000000-(cooja_radio_driver.radio_byte_transmit() + (cooja_radio_driver.radio_byte_receive())/4))*255/1000000;
  energy->obj.energy.energy_est = energest;
	}
	else energy->obj.energy.energy_est = 255;
  return energy->obj.energy;
}

/*static rpl_path_metric_t
calculate_throughput_path_metric(int nid){

	return cooja_radio_driver.radio_throughput();

}*/

static void action(int id){

  energy->obj.energy.energy_est = 5;
  rpl_reset_dio_timer(mydag,1);

}


static void
reset(rpl_dag_t *dag)
{
}

static void
parent_state_callback(rpl_parent_t *parent, int known, int netx)
{
  ANNOTATE("ETX %d\n",netx);
  update_metric_container(mydag);
}

static rpl_rank_t
calculate_rank(rpl_parent_t *p, rpl_rank_t base_rank)
{
  rpl_rank_t new_rank;
  rpl_rank_t rank_increase;

  if(p == NULL) {
    if(base_rank == 0) {
      return INFINITE_RANK;
    }
    rank_increase = NEIGHBOR_INFO_FIX2ETX(INITIAL_LINK_METRIC) * DEFAULT_MIN_HOPRANKINC;
  } else {
    rank_increase = NEIGHBOR_INFO_FIX2ETX(p->link_metric) * p->dag->min_hoprankinc;
    if(base_rank == 0) {
      base_rank = p->rank;
    }
  }

  
  if(INFINITE_RANK - base_rank < rank_increase) {
    /* Reached the maximum rank. */
    new_rank = INFINITE_RANK;
  } else {

   /* Calculate the rank based on the new rank information from DIO or
      stored otherwise. */
    new_rank = base_rank + rank_increase;
  }

  return new_rank;
}

static rpl_parent_t *
best_parent(rpl_parent_t *p1, rpl_parent_t *p2)
{
  rpl_dag_t *dag;
  rpl_path_metric_t min_diff;
  uint32_t p1_metric;
  uint32_t  p2_metric;

  dag = p1->dag; /* Both parents must be in the same DAG. */
  
  min_diff = RPL_DAG_MC_ETX_DIVISOR / 
             PARENT_SWITCH_THRESHOLD_DIV;
  
  min_diff = 5;

  p1_metric = calculate_fuzzy_metric(p1);
  p2_metric = calculate_fuzzy_metric(p2);

  /* Maintain stability of the preferred parent in case of similar ranks. */
  
  /*
  if(p1 == dag->preferred_parent || p2 == dag->preferred_parent) {
    if(p1_metric < p2_metric + min_diff &&
       p1_metric > p2_metric - min_diff) {
      PRINTF("RPL: MRHOF hysteresis: %u <= %u <= %u\n",
             p2_metric - min_diff,
             p1_metric,
             p2_metric + min_diff);
      return dag->preferred_parent;
    }
  }
  */
 

  /*
   * First compare ranks : any less rank
   * is the best
   */
    
  if (DAG_RANK(p1->rank,dag) + 1 < DAG_RANK(p2->rank,dag))
    return p1;
  else  if (DAG_RANK(p1->rank,dag) > DAG_RANK(p2->rank,dag) + 1)
    return p2;
  /*
   * if ranks and metrics equalities and if one is the preferred
   * parent then return the preferred parent to improve stability
   */
  if (p1 == dag->preferred_parent || p2 == dag->preferred_parent)
    if (p1_metric == p2_metric)
      return dag->preferred_parent;
  /*
   * Only ranks are equals so the best is the
   * higher metric
   */
  return  p1_metric >= p2_metric ? p1 : p2;


  

}


static initialized = 0;
static int nodeid = -1;

static void init(rpl_dag_t *dag){
  int i;
  uip_ipaddr_t ip;

  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    ip = uip_ds6_if.addr_list[i].ipaddr;
    nodeid = ip.u8[15];
  }
  if (nodeid == -1)
    return;

  mydag = dag;

  list_init(dag->mcs);

  //*****************************
  //  ENERGY
  //*****************************
  
  memb_free(&mc_memb, energy);
  energy = memb_alloc(&mc_memb);
  energy->type = RPL_DAG_MC_ENERGY;
  energy->flags = RPL_DAG_MC_FLAG_P;
  energy->obj.energy.energy_est = 255;
  /*
  energy->aggr =  RPL_DAG_MC_AGGR_ADDITIVE;
  energy->prec = 0;
  energy->length = sizeof(energy->obj.energy);
  */
  list_add(dag->mcs,energy);
  //ANNOTATE("#A E=%u\n",energy->obj.energy.energy_est);

 /* //*****************************
  //  THROUGHPUT
  //*****************************
  
  memb_free(&mc_memb, throughput);
  throughput = memb_alloc(&mc_memb);
  throughput->type = RPL_DAG_MC_THROUGHPUT;
  /*
  throughput->flags = RPL_DAG_MC_FLAG_P;
  throughput->aggr =  RPL_DAG_MC_AGGR_ADDITIVE;
  throughput->prec = 0;
  throughput->length = sizeof(throughput->obj.throughput);
  */
  list_add(dag->mcs,throughput);
  //ANNOTATE("#A T=%u\n",throughput->obj.throughput);*/

 /* //*****************************
  // LQL
  //*****************************
  
  memb_free(&mc_memb,lql);
  lql = memb_alloc(&mc_memb);
  lql->type = RPL_DAG_MC_LQL;
  /*
  lql->flags = RPL_DAG_MC_FLAG_P;
  lql->aggr =  RPL_DAG_MC_AGGR_ADDITIVE;
  lql->prec = 0;
  */
  lql->length = sizeof(lql->obj.lql);
  
  list_add(dag->mcs,lql);
  //ANNOTATE("#A LQL=%u\n",lql->obj.lql);*/

  //*****************************
  //  ETX
  //*****************************

  memb_free(&mc_memb, etx);
  etx = memb_alloc(&mc_memb);
  etx->type = RPL_DAG_MC_ETX;
  /*
  etx->flags = RPL_DAG_MC_FLAG_P;
  etx->aggr =  RPL_DAG_MC_AGGR_ADDITIVE;
  etx->prec = 0;
  etx->length = sizeof(etx->obj.etx);
  */
  list_add(dag->mcs,etx);
  //ANNOTATE("#A ETX=%u\n",etx->obj.etx);

  //*****************************
  // HopCount
  //*****************************
      
  memb_free(&mc_memb,hopcount);
  hopcount = memb_alloc(&mc_memb);
  hopcount->type = RPL_DAG_MC_HOPCOUNT;
  /*
  hopcount->flags = RPL_DAG_MC_FLAG_P;
  hopcount->aggr =  RPL_DAG_MC_AGGR_ADDITIVE;
  hopcount->prec = 0;
  hopcount->length = sizeof(hopcount->obj.hopcount);
  */
  list_add(dag->mcs,hopcount);
  //ANNOTATE("#A HC=%u\n",hopcount->obj.hopcount);

  //*****************************
  // Latency
  //*****************************
  
  memb_free(&mc_memb,latency);
  latency = memb_alloc(&mc_memb);
  latency->type = RPL_DAG_MC_LATENCY;
  /*
  latency->flags = RPL_DAG_MC_FLAG_P;
  latency->aggr =  RPL_DAG_MC_AGGR_ADDITIVE;
  latency->prec = 0;
  latency->length = sizeof(latency->obj.latency);
  */
  list_add(dag->mcs,latency);
  //ANNOTATE("#A Lat=%u\n",latency->obj.latency);

  initialized = 1;

}

static void
update_metric_container(rpl_dag_t *dag)
{
  if(!initialized)
    init(dag);
  
  energy->obj.energy = calculate_energy_path_metric(nodeid);
  hopcount->obj.hopcount = calculate_hopcount_path_metric(dag->preferred_parent);

 /* throughput->obj.throughput = calculate_throughput_path_metric(nodeid)*8;*/
  latency->obj.latency = calculate_latency_path_metric(dag->preferred_parent);
/*  lql->obj.lql = calculate_lql_path_metric(nodeid);*/
  etx->obj.etx = calculate_etx_path_metric(dag->preferred_parent);


  /* ANNOTATE("#A Q=%u\n",energy->obj.energy.energy_est); */
  /* ANNOTATE("#A H=%u\n",hopcount->obj.hopcount); */
  /* ANNOTATE("#A E=%u \n",etx->obj.etx); */
  /* ANNOTATE("#A T=%u \n",throughput->obj.throughput); */


  
}
