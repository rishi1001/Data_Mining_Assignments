if [[ "$1" == "train" ]]; then
    rm -f cs1190368_task1_last_adj.csv
    ln -s $5 cs1190368_task1_last_adj.csv
    python Q2/main.py $2 $3 $4 $5 $6 d2 20 64
else
    python Q2/test.py $2 $3 $4 cs1190368_task1_last_adj.csv $5 $6
fi 
