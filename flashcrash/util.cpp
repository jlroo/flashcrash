
/**
  util.cpp
  flash

  Created by Jose Luis Rodriguez on 11/15/17.
  Copyright © 2017 Jose Luis Rodriguez. All rights reserved.
  
  icpc -qopenmp -qopt-report util.cpp
  install_name_tool -change @rpath/libiomp5.dylib /opt/intel/compilers_and_libraries_2018.1.126/mac/compiler/lib/libiomp5.dylib a.out

**/

#include "util.hpp"
#include <iostream> 
#include <fstream>
#include <map>
#include <list>
//#include <cstring>
#include <string>
#include <my_timer.h>

using namespace std;

void help()
{
    fprintf(stderr,"\t--help        | -h    : Print help message.\n");
    fprintf(stderr,"\t--path        | -p    : Complete path to the fixdata.\n");
    fprintf(stderr,"\t--tag         | -t    : FixTag to search in the data e.i 52.\n");
    fprintf(stderr,"\t--num_start   | -n    : Index # for the start of the string.\n");
    fprintf(stderr,"\t--num_end     | -m    : Index # for the end of the string.\n");
    fprintf(stderr,"\t--tag_start   | -s    : FixTag start, default (\x01).\n");
    fprintf(stderr,"\t--tag_end     | -e    : FixTag end, default ('=').\n");
    fprintf(stderr,"\t--search      | -r    : Search for an string in the fixdata.\n");
    fprintf(stderr,"\t--buffer      | -b    : Read the data to the Buffer.\n");
}


void tagSearch(const vector<string> &data,
               vector<string> &search,
               string fixtag,
               int num_start,
               int num_end,
               size_t nsize) {
#pragma omp parallel
    {
        vector<string> vec_private;
#pragma omp for schedule(static) nowait
        for(int i=0; i<nsize; i++) {
            string smatch = data[i].substr(data[i].find(fixtag.c_str())+num_start,num_end);
            vec_private.push_back(smatch);
        }
#pragma omp critical
        search.insert(search.end(), vec_private.begin(), vec_private.end());
    }
}


void searchBuffer(char * buffer,
                  vector<string> &search,
                  vector<int64_t> &ixdrange,
                  string &fixtag,
                  int num_start,
                  int num_end) {
    
#pragma omp parallel
    {
        vector<string> vec_private;
        const size_t nsize = ixdrange.size();
#pragma omp for schedule(static) nowait
        for (int i=0; i<nsize; i++) {
            if (i+1 != nsize) {
                int numchars = ixdrange[i+1] - ixdrange[i] ;
                string line = std::string(&buffer[ixdrange[i]], &buffer[ixdrange[i]] + numchars);
                string smatch = line.substr(line.find(fixtag.c_str()) + num_start,num_end);
                vec_private.push_back(smatch);
            }
        }
#pragma omp critical
        search.insert(search.end(), vec_private.begin(), vec_private.end());
    }
}



void dateVolume(vector<string> &search, map<string, int> &count_dates, size_t nsize){
#pragma omp parallel
    {
        map<string, int> count_private;
        #pragma omp for schedule(static) nowait
        for (int i = 0; i < nsize; ++i)
        {
            count_private[search[i]]+=1;
        }
        
        #pragma omp critical
        for (auto& iter : count_private)  {
            count_dates[iter.first] +=iter.second;
        }
    }
}


int main (int argc, char* argv[])
{
    
    
    string path;
    string fixtag;
    int num_start = 0;
    int num_end = 0;
    int use_buffer = 0;
    const char * tag_start = NULL;
    const char * tag_end = NULL;
    const char * line_end = NULL;
    const char * tag_search = NULL;

    /**
    const char * line_end = NULL;
    const char * tag_search= NULL;
    string fixtag = "52";
    string path = "/Users/jlroo/cme/data/2010/XCME";
    //string path = "/Users/jlroo/cme/data/2010/XCME_MD_ES_20100104_20100108";
    //string  path = "/work/05191/jlroo/stampede2/data/01/XCME_MD_ES_20100104_20100108";
    const char * tag_start = "\x01";
    const char * tag_end = "=";
    int num_start = 4;
    int num_end = 8;
    **/
    
    vector<string> data;
    vector<string> search;
    vector<int64_t> ixdrange;
    map<string, int> volume;
    int week_volume = 0;
    
    for (int i = 1; i < argc; ++i)
    {
#define check_index(i,str) \
if ((i) >= argc) \
{ fprintf(stderr,"Missing 2nd argument for %s\n", str); return 1; }
        if ( strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0)
        {
            help();
            return 1;
        }
        else if (strcmp(argv[i],"--path") == 0 || strcmp(argv[i],"-p") == 0)
        {
            check_index(i+1,"--path|-p");
            i++;
            path = argv[i];
        }
        else if (strcmp(argv[i],"--tag") == 0 || strcmp(argv[i],"-t") == 0)
        {
            check_index(i+1,"--tag|-t");
            i++;
            fixtag = argv[i];
        }
        else if (strcmp(argv[i],"--num_start") == 0 || strcmp(argv[i],"-n") == 0)
        {
            check_index(i+1,"--num_start|-n");
            i++;
            num_start = atoi( argv[i] );
        }
        else if (strcmp(argv[i],"--num_end") == 0 || strcmp(argv[i],"-m") == 0)
        {
            check_index(i+1,"--num_end|-m");
            i++;
            num_end = atoi( argv[i] );
        }
        else if (strcmp(argv[i],"--tag_start") == 0 || strcmp(argv[i],"-s") == 0)
        {
            check_index(i+1,"--tag_start|-s");
            i++;
            tag_start = argv[i];
        }
        else if (strcmp(argv[i],"--tag_end") == 0 || strcmp(argv[i],"-d") == 0)
        {
            check_index(i+1,"--tag_end|-d");
            i++;
            tag_end = argv[i];
        }
        else if (strcmp(argv[i],"--line_end") == 0 || strcmp(argv[i],"-l") == 0)
        {
            check_index(i+1,"--line_end|-l");
            i++;
            line_end = argv[i];
        }
        else if (strcmp(argv[i],"--search") == 0 || strcmp(argv[i],"-r") == 0)
        {
            check_index(i+1,"--search|-r");
            i++;
            tag_search = argv[i];
        }
        else if (strcmp(argv[i],"--buffer") == 0 || strcmp(argv[i],"-b") == 0)
        {
            i++;
            use_buffer = 1;
        }
        else
        {
            fprintf(stderr,"Unknown option %s\n", argv[i]);
            help();
            return 1;
        }
    }
    
    if (tag_start == NULL || tag_end == NULL )  {
        const char * tag_start = "\x01";
        const char * tag_end = "=";
        fixtag = tag_start + fixtag + tag_end;
    }else if (tag_search != NULL){
        fixtag = tag_search;
    }
    else{
        fixtag = tag_start + fixtag + tag_end;
    }
    
    if (line_end == NULL)  {
        const char endline = '\n';
        line_end = &endline;
    }
    
    // Record time spent in each function.
    
    if (use_buffer==0) {
        
        double t_read = 0, t_search = 0, t_volume = 0;

        myTimer_t t0 = getTimeStamp();
        read_fix(path.c_str(), data);
        
        myTimer_t t1 = getTimeStamp();
        const size_t N = data.size();
        tagSearch(data, search, fixtag,num_start,num_end, N);
        
        myTimer_t t2 = getTimeStamp();
        size_t n = search.size();
        dateVolume(search, volume, n);
        
        myTimer_t t3 = getTimeStamp();
        t_read = getElapsedTime(t0,t1);
        t_search = getElapsedTime(t1,t2);
        t_volume = getElapsedTime(t2,t3);
        
        std::cout << "date,volume"<<std::endl;
        for (auto& iter : volume)  {
            std::cout << iter.first << "," << iter.second <<std::endl;
            //std::cout << "volume[" << iter.first << "] = " << iter.second <<std::endl;
            week_volume +=iter.second;
        }
        std::cout << "total_msgs,read_time,search_time,volume_time" <<std::endl;
        std::cout << week_volume << "," << t_read  << "," <<  t_search << "," << t_volume <<std::endl;
        
    }else{
        
        double t_read = 0, t_endline = 0 , t_search = 0, t_volume = 0;
        
        size_t buff_size;
        myTimer_t t0 = getTimeStamp();
        char * buffer = read_buffer(path.c_str(), buff_size);
        
        myTimer_t t1 = getTimeStamp();
        KMPSearch(line_end, buffer, ixdrange, buff_size);
        
        myTimer_t t2 = getTimeStamp();
        searchBuffer(buffer, search, ixdrange, fixtag, num_start, num_end);
        
        myTimer_t t3 = getTimeStamp();
        size_t n = search.size();
        dateVolume(search, volume, n);
        
        myTimer_t t4 = getTimeStamp();
        t_read = getElapsedTime(t0,t1);
        t_endline = getElapsedTime(t1,t2);
        t_search = getElapsedTime(t2,t3);
        t_volume = getElapsedTime(t3,t4);
        
        std::cout << "date,volume"<<std::endl;
        for (auto& iter : volume)  {
            std::cout << iter.first << "," << iter.second <<std::endl;
            //std::cout << "volume[" << iter.first << "] = " << iter.second <<std::endl;
            week_volume +=iter.second;
        }
        std::cout << "total_msgs,read_time,search_kmp,search_time,volume_time" <<std::endl;
        std::cout   << week_volume << ","
                    << t_read << ","
                    << t_endline << ","
                    << t_search << ","
                    << t_volume <<std::endl;
    }
    
    return 0;
}


