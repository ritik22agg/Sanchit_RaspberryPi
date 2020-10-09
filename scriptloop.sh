echo -n "Enter the Total numbers :"
read n 
echo "Enter numbers :"
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

# Print the array starting 
# from index, i=0 
str="Output :"
  
for i in "${a[@]}"
do
    # access each element as $i 
    str="$str $i"  
done
echo $str
str1="We welcome you"  
  
#Declaring the Second String  
str2=" on Javatpoint."  
  
#Combining first and second string  
str3="$str1$str2"  
  
#Printing a new string by combining both   
echo $str3  