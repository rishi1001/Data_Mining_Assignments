#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#ifndef WIN32
#include <unistd.h>
#endif

#include <cinttypes>
#include <stdio.h>
#include <iostream>
#include "VFLib.h"

#define VF3PGSS	 (1)
#define VF3PWLS  (2)

struct OptionStructure
{
	char *pattern;
	char *target;
	bool undirected;
	bool storeSolutions;
	bool verbose;
	std::string format;
	float repetitionTimeLimit;

	OptionStructure():
		pattern(nullptr),
		target(nullptr),
		undirected(false),
		storeSolutions(false),
		verbose(0),
		format("vf"),
		repetitionTimeLimit(0){}
};

typedef OptionStructure Options;


void PrintUsage()
{
	std::string outstring = "vf3 [pattern] [target] ";
	outstring += "-u -s -f [graph format]";
	std::cout<<outstring<<std::endl;
}


bool GetOptions(Options &opt, int argc, char **argv)
{
	/*
	* -c Start CPU for the pool allocation
	* -t Number of threads. Default [1]
	* -a Version of the matcher to use. Default -1 is VF3
	* -h SSR limit for the global stack. Default 3
	* -l Local Stack limit. Default is pattern size
	* -u Load graphs as undirected
	* -k LockFree Version
	* -r Minimum time in second for benchmark repetitions. Default 1.
	* -s Print Solutions
	* -f Graph format [vf, edge]
	* -v Verbose: show all time
	*/
  std::string optionstring = ":r:f:suv";

	char option;
	while ((option = getopt (argc, argv, optionstring.c_str())) != -1)
	{
		switch (option)
		{
      case 's':
				opt.storeSolutions = true;
				break;
			case 'r':
				opt.repetitionTimeLimit = atof(optarg);
        break;
      		case 'u':
				opt.undirected = true;
				break;
			case 'v':
				opt.verbose = true;
				break;
			case 'f':
				opt.format = std::string(optarg);
				break;
			case '?':
				PrintUsage();
				return false;
		}

	}
	//additional parameter
	if(argc < 2)
	{
		PrintUsage();
		return false;
	}

	opt.pattern = argv[optind];
	opt.target = argv[optind+1];
	return true;
}

template<typename Node, typename Edge>
vflib::ARGLoader<Node, Edge>* CreateLoader(const Options& opt, std::istream &in)
{

	if(opt.format == "vf")
	{
		return new vflib::FastStreamARGLoader<Node, Edge>(in, opt.undirected);
	}
		else if(opt.format == "edge")
	{
		return new vflib::EdgeStreamARGLoader<Node, Edge>(in, opt.undirected);
	}
	else
	{
		return nullptr;
	}
}

vflib::MatchingEngine<state_t>* CreateMatchingEngine(const Options& opt)
{
    return new vflib::MatchingEngine<state_t >(opt.storeSolutions);
}

#endif /* OPTIONS */
