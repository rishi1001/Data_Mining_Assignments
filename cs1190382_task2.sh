if [[ "$1" == "train" ]]; then
    python adj_mx_conv.py $5 cs1190382_task1_last_adj.csv
    python Q2/main.py $2 $3 $4 $5 $6 d2 10 64
else
    python Q2/test.py $2 $3 $4 cs1190382_task1_last_adj.csv $5 $6
fi 
