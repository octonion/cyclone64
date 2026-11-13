#!/bin/sh
knl=$(awk -F: '/vendor_id/{v=$2} /cpu family/{f=$2} /^model[[:space:]]*:/{m=$2} /^$/{exit} END{gsub(/ /,"",v); print (v=="GenuineIntel" && f==6 && m==87)}' /proc/cpuinfo)
if [ "$knl" = 1 ]; then
  if [ "$("$1" -dumpversion)" = 14 ]; then echo -march=knl; else echo -march=x86-64; fi
else
  echo -march=native
fi
