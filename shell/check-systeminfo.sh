#!/bin/bash

# some useful commands to collect system infomation.
# Reference:
#    http://www.tecmint.com/commands-to-collect-system-and-hardware-information-in-linux/
#    http://www.binarytides.com/linux-commands-hardware-info/

# Check kernel,OS,CPU
uname -a

# Check CPU info
cat /proc/cpuinfo

# number of cores
nproc

# Check PCI devices
lspci -tv

# Check Disk
cat /proc/loadavg

# Check network address
ifconfig

# Memory Hardware info
dmidecode -t memory

# Disk
lsblk

# Disk partition
fdisk -l

# Network interface hardware info
lspci |grep -i 'eth'

# All hardware info
dmidecode -q
