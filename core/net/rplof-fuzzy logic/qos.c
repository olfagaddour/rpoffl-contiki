#include "fuzzify.h"


unsigned short qos(unsigned short rel, unsigned short dur){
  unsigned short vwrel = reliability_very_weak(rel);
  unsigned short wrel = reliability_weak(rel);
  unsigned short arel = reliability_avg(rel);
  unsigned short srel = reliability_strong(rel);
  unsigned short vsrel = reliability_very_strong(rel);

  unsigned short vsdur = duration_very_slow(dur);
  unsigned short sdur = duration_slow(dur);
  unsigned short adur = duration_avg(dur);
  unsigned short fdur = duration_fast(dur);
  unsigned short vfdur = duration_very_fast(dur);


  unsigned short awf = 0, vb = 0, b = 0, deg = 0, avg = 0, acc = 0, g = 0, vg = 0, exc = 0;
  
  if (vwrel && vsdur) awf += min(vwrel, vsdur);
  if (vwrel && sdur)  vb  =  min(vwrel, sdur)  <  vb  ? vb  : min(vwrel, sdur);
  if (vwrel && adur)  b   =  min(vwrel, adur)  <  b   ? b   : min(vwrel, adur);
  if (vwrel && fdur)  deg =  min(vwrel, fdur)  <  deg ? deg : min(vwrel, fdur);
  if (vwrel && vfdur) avg =  min(vwrel, vfdur) <  avg ? avg : min(vwrel, vfdur);
  if (wrel && vsdur)  vb  =  min(wrel , vsdur) <  vb  ?  vb : min(wrel , vsdur);
  if (wrel && sdur)   b   =  min(wrel , sdur)  <  b   ? b   : min(wrel , sdur);
  if (wrel && adur)   deg =  min(wrel , adur)  <  deg ? deg : min(wrel , adur);
  if (wrel && fdur)   avg =  min(wrel ,fdur)   <  avg ? avg : min(wrel ,fdur);
  if (wrel && vfdur)  acc =  min(wrel , vfdur) <  acc ? acc : min(wrel , vfdur);
  if (arel && vsdur)  b   =  min(arel , vsdur) <  b   ? b   : min(arel , vsdur);
  if (arel && sdur)   deg =  min(arel , sdur)  <  deg ? deg : min(arel , sdur);
  if (arel && adur)   avg =  min(arel , adur)  <  avg ? avg : min(arel , adur);
  if (arel && fdur)   acc =  min(arel ,  fdur) <  acc ? acc : min(arel , fdur);
  if (arel && vfdur)  g   =  min(arel , vfdur) <  g   ? g   : min(arel , vfdur);
  if (srel && vsdur)  deg =  min(srel , vsdur) <  deg ? deg : min(srel , vsdur);
  if (srel && sdur)   avg =  min(srel , sdur)  <  avg ? avg : min(srel , sdur);
  if (srel && adur)   acc =  min(srel , adur)  <  acc ? acc : min(srel , adur);
  if (srel && fdur)   g   =  min(srel , fdur)  <  g   ? g   : min(srel , fdur);
  if (srel && vfdur)  vg  =  min(srel , vfdur) <  vg  ? vg  : min(srel , vfdur);
  if (vsrel && vsdur) avg =  min(vsrel, vsdur) <  avg ? avg : min(vsrel, vsdur);
  if (vsrel && sdur)  acc =  min(vsrel, sdur)  <  acc ? acc : min(vsrel, sdur);
  if (vsrel && adur)  g   =  min(vsrel, adur)  <  g   ? g   : min(vsrel, adur);
  if (vsrel && fdur)  vg  =  min(vsrel, fdur)  <  vg  ? vg  : min(vsrel, fdur);
  if (vsrel && vfdur) exc += min(vsrel, vfdur);


  return (awf*6 + vb *16 + b * 28 + deg * 38 + avg * 50 + acc * 60 + g * 72 + vg * 82 + exc * 93)/(exc + vg + g + acc + avg + deg + b + vb + awf);
}
