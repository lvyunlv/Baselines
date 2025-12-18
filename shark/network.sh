#!/bin/bash
######
# Taken from https://github.com/emp-toolkit/emp-readme/blob/master/scripts/throttle.sh
######

## replace DEV=lo with your card (e.g., eth0)
DEV=lo
if [ "$1" == "del" ]
then
        sudo tc qdisc del dev $DEV root
fi

if [ "$1" == "loc" ]
then
tc qdisc del dev $DEV root
## about 1Gbps
tc qdisc add dev $DEV root handle 1: tbf rate 10000mbit burst 100000 limit 10000
## about 0.3ms ping latency
tc qdisc add dev $DEV parent 1:1 handle 10: netem delay 0.03msec
fi
if [ "$1" == "lan" ]
then
tc qdisc del dev $DEV root
## about 1Gbps
tc qdisc add dev $DEV root handle 1: tbf rate 1000mbit burst 100000 limit 10000
## about 0.3ms ping latency
tc qdisc add dev $DEV parent 1:1 handle 10: netem delay 0.03msec
fi
if [ "$1" == "wan" ]
then
tc qdisc del dev $DEV root
## about 400Mbps
tc qdisc add dev $DEV root handle 1: tbf rate 200mbit burst 100000 limit 10000
## about 40ms ping latency
tc qdisc add dev $DEV parent 1:1 handle 10: netem delay 50msec
fi

