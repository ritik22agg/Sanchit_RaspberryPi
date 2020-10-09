#!/bin/bash
cd readings
read n
i=0 
  
# Read upto the size of  
# given array starting from 
# index, i=0 
while [ $i -lt $n ] 
do
    # To input from user 
    a[$i]=1
  
    # Increment the i = i + 1 
    i=`expr $i + 1` 
done

cpabe-setup 

enc="cpabe-enc pub_key security.txt"
for i in "${a[@]}"
do
    # access each element as $i 
    enc="$enc $i"  
done
$enc
key="cpabe-keygen -o sanchit pub_key master_key"
for i in "${a[@]}"
do
    # access each element as $i 
    key="$key $i"  
done
$key
cpabe-dec pub_key sanchit security.txt.cpabe sanchit
rm pub_key master_key sanchit revo.txt

