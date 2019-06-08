# IeeeAccess2019
NS-3.29 source code for the paper entitled 
"Novel ETX-based metrics for overhead reduction in dynamic ad hoc networks"
submitted for publication in IEEE Access.

NPAF - Network Performance Analysis Framework
----------------------------------------------
NPAF provides support for network analysis based on Key Performance Indicators (KPIs) such as throughput, Packet Loss Ratio, overhead, End-to-End delay, jitter, etc.
Installation:
Step 1: Copy the contents of the folder ns-3.29/src/applications to the corresponding folder in your ns-3.29 installation. Overwrite wscript file. 
Step 2: Examples are in the folder ns-3.29/src/scratch: multi-run.cc and multi-run-power.cc

ETX-based metrics implementations in AODV protocol
--------------------------------------------------
1. ETX
Installation:
Step 1: Delete the contents of the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 2: Copy the contents of the folder ns-3.29/src/aodv-etx to the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 3: Example: ns-3.29/scratch/multi-run.cc
Step 4: Bach script: ns-3.29/multi-run.sh
2. L-ETX (Light ETX)
Installation:
Step 1: Delete the contents of the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 2: Copy the contents of the folder ns-3.29/src/aodv-light-etx to the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 3: Example: ns-3.29/scratch/multi-run.cc
Step 4: Bach script: ns-3.29/multi-run.sh
3. LR-ETX (Light Reverse ETX)
Installation:
Step 1: Delete the contents of the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 2: Copy the contents of the folder ns-3.29/src/aodv-light-reverse-etx to the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 3: Example: ns-3.29/scratch/multi-run.cc
Step 4: Bach script: ns-3.29/multi-run.sh
4. PLR-ETX (Power Light Reverse ETX)
Installation:
Step 1: Delete the contents of the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 2: Copy the contents of the folder ns-3.29/src/aodv-power-light-etx to the folder ns-3.29/src/aodv in your ns-3.29 installation.
Step 3: Copy the contents of the folder ns-3.29/src/wifi-power-tag to the folder ns-3.29/src/wifi in your ns-3.29 installation.  Overwrite wscript and yans-wifi-channel.cc files.
Step 4: Example: ns-3.29/scratch/multi-run-power.cc
Step 5: Bach script: ns-3.29/multi-run-power.sh

