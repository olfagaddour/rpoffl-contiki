
#include "fuzzify.h"


unsigned long quality(unsigned long cons, unsigned long qos){

  unsigned long vccons = consumption_very_costly(cons);
  unsigned long ccons = consumption_costly(cons);
  unsigned long acons = consumption_avg(cons);
  unsigned long chcons = consumption_cheap(cons);
  unsigned long vchcons = consumption_very_cheap(cons);

  unsigned long awqos = qos_awful(qos);
  unsigned long vbqos = qos_very_bad(qos);
  unsigned long bqos = qos_bad(qos);
  unsigned long dqos = qos_degraded(qos);
  unsigned long aqos = qos_avg(qos);
  unsigned long acqos = qos_acceptable(qos);
  unsigned long gqos = qos_good(qos);
  unsigned long vgqos = qos_very_good(qos);
  unsigned long exqos = qos_excellent(qos);
  
  unsigned long vb = 0, b = 0, bb = 0, avg = 0, lg = 0, g = 0, vg = 0;
  
  if (vccons && (awqos || vbqos)) vb += min(vccons, max (awqos, vbqos));
  if (vccons && (bqos || dqos)) 
    b = min(vccons, max(bqos, dqos)) < b ? b : min(vccons, max(bqos, dqos));
  if (vccons && aqos) 
    bb = min(vccons, aqos) < bb ? bb : min(vccons, aqos);
  if (vccons && (acqos || gqos || vgqos))
    avg = min(vccons, max(acqos, max(gqos, vgqos))) < avg ? avg : min(vccons, max(acqos, max(gqos, vgqos)));
  if (vccons && exqos) 
    lg = min(vccons, exqos) < lg ? lg : min(vccons, exqos);
  if (ccons && awqos)
    vb = min(ccons, awqos) < vb ? vb : min(ccons, awqos);
  if (ccons && (vbqos || bqos)) 
    b = min(ccons, max(vbqos, bqos)) < b ? b : min(ccons, max(vbqos, bqos));
  if (ccons && dqos) 
    bb = min(ccons, dqos) < bb ? bb : min(ccons, dqos);
  if (ccons   && (aqos  || acqos || gqos)) 
    avg = min(ccons, max(aqos, max(acqos,gqos))) < avg ? avg : min(ccons, max(aqos, max(acqos,gqos)));
  if (ccons && vgqos ) 
    lg = min(ccons, vgqos) < lg ? lg : min(ccons, vgqos);
  if (ccons && exqos ) 
    g = min(ccons, exqos) < g ? g : min(ccons, exqos);
  if (acons && (awqos || vbqos)) 
    b = min(acons, max(awqos, vbqos)) < b ? b : min(acons, max(awqos, vbqos));
  if (acons && bqos) 
    bb = min(acons, bqos) < bb ? bb : min(acons, bqos);
  if (acons && (dqos  || aqos || acqos)) 
    avg = min(acons, max(dqos, max(aqos,acqos))) < avg ? avg : min(acons, max(dqos, max(aqos,acqos)));
  if (acons && gqos) 
    lg = min(acons, gqos) < lg ? lg : min(acons, gqos);
  if (acons && (vgqos || exqos)) 
    g = min(acons, max(vgqos, exqos)) < g ? g : min(acons, max(vgqos, exqos));
  if (chcons  && awqos ) 
    b = min(chcons, awqos) < b ? b : min(chcons, awqos);
  if (chcons  && vbqos ) 
    bb = min(chcons,vbqos) < bb ? bb : min(chcons,vbqos);
  if (chcons  && (bqos  || dqos || aqos)) 
  avg = min(chcons, max(bqos, max(dqos, aqos))) < avg ? avg : min(chcons, max(bqos, max(dqos, aqos)));
  if (chcons  && acqos) 
    lg = min(chcons, acqos) < lg ? lg : min(chcons, acqos);
  if (chcons  && (gqos  || vgqos)) 
  g = min(chcons, max(gqos, vgqos)) < g ? g : min(chcons, max(gqos, vgqos));
  if (chcons  && exqos ) 
    vg = min(chcons, exqos) < vg ? vg : min(chcons, exqos);
  if (vchcons && awqos ) 
    bb = min(vchcons, awqos) < bb ? bb : min(vchcons, awqos);
  if (vchcons && (vbqos || bqos || dqos)) 
    avg = min(vchcons, max(vbqos, max(bqos, dqos))) < avg ? avg : min(vchcons, max(vbqos, max(bqos, dqos)));
  if (vchcons && aqos) 
    lg = min(vchcons, aqos) < lg ? lg : min(vchcons, aqos);
  if (vchcons && (acqos || gqos)) 
    g = min(vchcons, max(acqos, gqos)) < g ? g : min(vchcons, max(acqos, gqos));
  if (vchcons && (vgqos || exqos)) vg += min(vchcons, max(vgqos, exqos));
  
  //printf("quality : %u %u %u %u %u %u %u -> %u\n", vb, b, bb, avg, lg, g, vg, (vb*12 + b*24 + bb*36 + avg*48 + lg*60 + g*72 + vg*84)/(vb + b + bb + avg + lg + g + vg));
  return (vb*12 + b*24 + bb*36 + avg*48 + lg*60 + g*72 + vg*84)/(vb + b + bb + avg + lg + g + vg);
}
