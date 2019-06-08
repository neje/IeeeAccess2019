#!/bin/bash

START="1"
STOP="100"

CURRENT_RUN_START="1" 
CURRENT_RUN_STOP="100" 

SIM_TIME="600"

# 1=OLSR; 2=AODV; 3=DSDV; 4=DSR
ROUTING="2"

PROTOCOL_NAME="APLRE"
ALPHA="0.005"
BETA="60"

VEHICLES="50 100 200 300 400 500 600"

PROGRAM_NAME="multi-run-power"

echo Experiment starts...
echo scenario 1

for b in $BETA
do
  for a in $ALPHA
  do
    for v in $VEHICLES
    do
      for r in $ROUTING
      do
        for (( run=$CURRENT_RUN_START; run<=$CURRENT_RUN_STOP; run++ ))
        do
          echo 
          echo xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
          echo x   Run = $run
          echo x   "./waf --run \"$PROGRAM_NAME --simTime=$SIM_TIME --scenario=1 --nNodes=$v"
          echo x   "      --routingProtocol=$r --routingProtocolName=$PROTOCOL_NAME"
          echo x   "      --startRngRun=$START --currentRngRun=$run --stopRngRun=$STOP --externalRngRunControl=1"
          echo x   "      --a=$a --b=$b"
          echo xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
          echo 
          echo "Start time:"
          date
          ./waf --run "$PROGRAM_NAME --scenario=1 --nNodes=$v --simTime=$SIM_TIME --routingProtocol=$r --routingProtocolName=$PROTOCOL_NAME --startRngRun=$START --currentRngRun=$run --stopRngRun=$STOP --externalRngRunControl=1 --a=$a --b=$b"
          echo "StopTime:"
          date
          echo --------------------------------------------------------------------------
        done
      done
    done
  done
done

