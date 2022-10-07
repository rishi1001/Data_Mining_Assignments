To run Q1 - make Q1 {support} {infile} {outfile}
In our case - infile = 'formatted.txt' and outfile = 'performance_plot.png'

Explaination of Files - 
Q1) 
1. convert.py -> To convert the given formatted files(Yeast format) into format required by gSpan,FSG,gaston.
2. plot.py -> To plot the graph by running gSpan, FSG, gaston.
3. output_plot.png -> The final plot which has the output.

Q2)
1. convert.cpp -> To convert the given formatted files into format required by gSpan.(In cpp to save index construction time)
2. index.cpp -> To make the indexes from the output file of gSpan.
3. query.cpp -> To run the queries and generate output. 

Q3) elbow_plot.py -> To generate elbow plots using the given dataset.


Instructions to Run - 
Q1) Make sure Yeast dataset is in the same folder.
python3 convert.py
python3 plot.py inputFile outputFile totGraphFile

Q2) 
sh index.sh <graph_dataset>
sh query.sh
Enter the query file location

Q3) 
sh elbow_plot.sh <dataset> <dimensions> <output>

Team Members
1. Rishi Shah         2019CS10394
2. Prakhar Jagwani    2019CS10382
3. Krishnanshu Jain   2019CS10368

Contributions
1. Prakhar Jagwani   33%
2. Rishi Shah   34%
3. Krishnanshu Jain    33%
