import sys
import pandas as pd
import math

def convert_adj_matrix(df):
	sigma = 100
	zeros = (df == 0)
	df = math.e ** (-(df/sigma)**2)
	df[zeros] = 0
	return df

if __name__ == "__main__":
	if len(sys.argv) < 3:
		print("Arguments needed: <input_csv> <output_csv>")
	df = pd.read_csv(sys.argv[1],index_col=0)
	df = convert_adj_matrix(df)
	df.to_csv(sys.argv[2])
