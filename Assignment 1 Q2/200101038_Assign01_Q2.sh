#! /bin/bash
#to generate random numbers
RANDOM=$$;

#Accepting command line arguments
count=$1;
length=$2;
alphabets="";
read alphabets;

#Initialising variables to store substrings and lengths
substr="";
length_of_string=${#alphabets};
length_of_substr=0;
prev_index=-1;
random=$RANDOM;

#Handling the error that might arise if length of string is insufficient
if [ $length_of_string -le 1 ]
then
	echo "ERROR: Please provide a string of more than one characters!!";
	exit 1;
fi


#random length of string to be generated(less than count)
# gen_length=`expr $random % $count`;
# gen_length=`expr $length - $gen_length`;

#Run the loop until the length of substring is less than the given length
while [ $length_of_substr -lt $length ]
do
	random=$RANDOM;
	#random number to access the index of the string
	index=`expr $random % $length_of_string`;

	#if the same index is generated again then skip it
	if [ $index = $prev_index ]
	then
		continue;
	fi

	#generate another random number to get the number of repetitions
	random=$RANDOM;
	prev_index=index;

	#modulus ensures that rep is always less than count
	rep_no=`expr $random % $count`;
	
	#If repetition number is zero, increase by one.
	if [ $rep_no == 0 ]
	then
		rep_no=`expr $rep_no + 1`;
	fi 


	#Expected length of the substr
	expec_len=`expr $length_of_substr + $rep_no`;
	
	#If expected length is greater than allowed length,
	#then truncate the substring to the allowed length
	if [ $expec_len -gt $length ]
	then	
		rep_no=`expr $length - $length_of_substr`;
	fi
	
	#Loop to add the character to the existing substring
	i=0;
	while [ $i -lt $rep_no ]
	do
		substr="$substr${alphabets:index:1}";
		i=`expr $i + 1`;
	done

	#Update the length of current substr
	length_of_substr=`expr $length_of_substr + $rep_no`;
	
	#Print the substring on each step
	echo $substr;
done

