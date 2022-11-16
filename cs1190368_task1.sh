if [[ "$1" == "train" ]]; then
    rm -f cs1190368_task1_last_adj.csv
    ln -s $3 cs1190368_task1_last_adj.csv
    python Q1/main.py $2 $3 $4 d1 10 gcn 10
else
    python Q1/test.py $2 cs1190368_task1_last_adj.csv $3 $4 gcn
fi
