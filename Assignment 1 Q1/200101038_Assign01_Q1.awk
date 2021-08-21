BEGIN {
    #Set the file delimiter to be whitespace
    FS=" "
}

{
    #Read every line of the input file into the array
    lines[NR] = $0
}
{
    #Store the sums of first two columns of each row
    sums[NR] = $2 + $3
    
}
END {
    #Using bubble sort to sort the sums in ascending order
    #And also swap the lines according the swaps of sums

    #To check if sum is calculated correctly
    # for(s=1; s<=NR; s++){
    #     print sums[s]
    # }

    #Implement Bubble Sort Algorithm
    for(i=1; i < NR; i++){
        for(j=1; j <= NR-i; j++){
            if(sums[j+1]<sums[j]){
                #If sums of jth row is greater than j+1th row,
                #then swap the sums and the lines.


                #printf("%d and %d switched\n", j, j+1);
                tempsum = sums[j+1]
                sums[j+1] = sums[j]
                sums[j] = tempsum

                templine = lines[j+1]
                lines[j+1] = lines[j]
                lines[j] = templine
            }
        }
    }

    #Print the sorted lines
    for(k=1; k <=NR; k++){
        print lines[k]
    }
}