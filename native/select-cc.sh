#!/bin/sh
# Explicit CC in the environment or make command line always wins.
knl=$(awk -F: '/vendor_id/{v=$2} /cpu family/{f=$2} /^model[[:space:]]*:/{m=$2} /^$/{exit} END{gsub(/ /,"",v); print (v=="GenuineIntel" && f==6 && m==87)}' /proc/cpuinfo)
if [ "$knl" = 1 ] && command -v gcc-14 >/dev/null; then
  echo gcc-14
  exit
fi
for version in $(seq 30 -1 4); do
  if command -v "gcc-$version" >/dev/null 2>&1; then
    if "gcc-$version" --version | head -1 | grep -Eiq 'experimental|snapshot|prerelease'; then continue; fi
    echo "gcc-$version"
    exit
  fi
done
command -v gcc
