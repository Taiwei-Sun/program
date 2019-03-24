#!/bin/bash
PATH=/sbin
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 61.218.0.0/15 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 220.128.64.0/16 #Not entirely correct 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 61.220.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.23.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 59.112.0.0/13 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.75.128.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.241.224.0/19 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.59.128.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.65.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 203.69.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 36.224.0.0/12 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 111.240.0.0/12 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 118.168.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 122.118.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 125.232.0.0/15 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 168.95.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 59.120.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 218.168.0.0/13 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 61.224.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 61.216.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 218.160.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.62.248.0/21 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.20.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.72.128.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.72.0.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.242.0.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.71.128.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 203.75.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 202.39.128.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 103.25.236.0/22 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 1.160.0.0/12 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 114.32.0.0/12 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 122.120.0.0/13 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 60.249.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 60.250.0.0/15 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 59.124.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 220.128.0.0/18 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 218.164.0.0/15 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 61.217.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 61.228.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 202.39.0.0/17 #Not entirely correct 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.22.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.21.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 211.75.0.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.242.128.0/17 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 210.61.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 203.74.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 203.66.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 43.255.92.0/22 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 1.34.0.0/15 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 114.24.0.0/14 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 118.160.0.0/13 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 122.116.0.0/15 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 125.224.0.0/13 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 60.248.0.0/16 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 220.129.0.0/12 #Not entirely correct 
iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s 218.166.0.0/15 
exit 0
