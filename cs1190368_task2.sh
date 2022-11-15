cd Q2
if [[ "$1" == "train" ]]; then
    python main.py $2 $3 $4 $5 $6 d2 64
    ln -s $5 cs1190368_task1_last_adj.csv
else
    python test.py $2 $3 $4 cs1190368_task1_last_adj.csv $5 $6
fi 
cd ..