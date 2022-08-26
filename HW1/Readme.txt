GITHUB REPO: https://github.com/rishi1001/Data_Mining_Assignments.git  

Team Members
1. Krishnanshu Jain   2019CS10368
2. Prakhar Jagwani    2019CS10382
3. Rishi Shah         2019CS10394

Contributions
1. Rishi Shah         34%
Implemented aprori and optimized fptree
2. Krishnanshu Jain   33%
Implemented fptree and optimized aprioi
3. Prakhar Jagwani    33%
Implemented fptree and optimized aprioi

Explanation for Q3:
1. The time for both apriori and fptree algorithm is equal for 5% support, this
is because of the timeout of 1hr for both of them, which is less than the actual time 
required to complete.

2. The runtime for fptree is significantly smaller as compared to the apriori in case 
of low support, this is because of the number of file reads in case of fptree(2 for all supports)
is significantly smaller as compared to that in apriori(same as the maximum length 
of the frequent itemset).

3. The runtime for both the algorithms is almost similar for higher thresholds, this 
is becuase the maximum length of the frequent itemsets is small, so the number of file 
reads in apriori is low.



