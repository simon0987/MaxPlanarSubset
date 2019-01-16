//
//  main.cpp
//  Alg_hw2
//
//  Created by 柯哲邦 on 2018/11/14.
//  Copyright © 2018年 柯哲邦. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
using namespace std;
int MIS_length(int *,int);
void trace_k(vector<int>&,int*,int,int);
//global variable, to record k and the answer
int **record_k = NULL;


int main(int argc, const char * argv[]) {
    ifstream fin(argv[1]);
    int index,value,count = 0;
    int total_vertices = 0;
    bool flag = true;
    int *record_chord = NULL;
    vector<int> first_element;
    /* read the input file, and record the chords to a array
     ex: (0,4) a[0] = 4,a[4] = 0
     */
    while (1){
        if(flag){
            fin >> total_vertices;
            flag = false;
            record_chord = new int[total_vertices](); // initialize to 0
        }
        fin >> index;
        fin >> value;
        record_chord[index] = value;
        first_element.push_back(index);
        record_chord[value] = index;
        count++;
        if(count>=total_vertices/2)
            break;
    }
    ofstream outfile(argv[2]);
    outfile << MIS_length(record_chord,total_vertices) << endl;
    //answer of the vertices
    vector<int> ans;
    trace_k(ans,record_chord,0,total_vertices-1);
    for(vector<int>::iterator it = first_element.begin();it!=first_element.end();it++){
        for(vector<int>::iterator it2 = ans.begin();it2!=ans.end();it2++){
            vector<int>::iterator it3 = find(first_element.begin(),first_element.end(),*it2);
            if(it3==first_element.end()){
                *it2 = record_chord[*it2];
            }
        }
    }
    sort(ans.begin(),ans.end());
    for(vector<int>::iterator it = ans.begin();it!=ans.end();it++){
        outfile << *it << " " << record_chord[*it] << endl;
    }
    delete [] record_chord;
    
    for(int i = 0;i < total_vertices;i++){
        delete [] record_k[i];
    }
    delete [] record_k;
    fin.close();
    
    return 0;
}

int MIS_length(int *record,int size){
    //m is the 2 dim array to store
    //new a 2 dim array
    int **m = new int*[size];
    //k is to trace the max_vertices
    record_k = new int*[size];
    for(int i = 0 ; i < size;i++){
        m[i] = new int[size];
        record_k[i] = new int[size];
    }
    //inititalize the 2 dim array
    for(int j = 0; j < size;j++){
        for (int i = 0 ; i < size; i++){
            m[i][j]=0;
            record_k[i][j] = -1;
        }
    }
    //start dynamic programming
    for(int j = 1; j < size;j++){
        for(int i = 0 ; i < j;i++){
            m[i][j] = m[i][j-1];
            record_k[i][j] = record_k[i][j-1];
            
            // i < k < j
            if(record[j]<j && record[j]>i){
                
                // k = j-1
                if(record[j]+1 > j-1){
                    //make sure the total is more than the table before.
                    if(m[i][j] < 1 + m[i][record[j]-1]){
                        m[i][j] = 1 + m[i][record[j]-1];
                        //record k
                        record_k[i][j] = record[j];
                    }
                }
                // i < k < j-1
                else if(m[i][j] < m[i][record[j]-1] + 1 + m[record[j]+1][j-1]){
                    m[i][j] = m[i][record[j]-1] + 1 + m[record[j]+1][j-1];
                    record_k[i][j] = record[j];
                }
            }
            // k = i
            else if(record[j] == i){
                //i = j-1
                if(i+1 > j-1){
                    m[i][j]++;
                    record_k[i][j] = i;
                }
                
                //i < j-1
                else if(m[i][j] < m[i+1][j-1] + 1){
                    m[i][j] = m[i+1][j-1] + 1;
                    record_k[i][j] = i;
                }
            }
        }
    }
    int result = m[0][size-1];
    for(int i = 0;i < size;i++){
        delete [] m[i];
    }
    delete [] m;
    return result;
}

void trace_k(vector<int> &a,int *record,int ii,int jj){
    if(jj>ii){
        int tmp = record_k[ii][jj];
        //    cout << "first: " <<tmp << endl;
        if (tmp>=0){
            a.push_back(tmp);
            if(tmp==ii&&tmp>0){
                trace_k(a,record, ii+1, record[tmp]-1);
            }
            else if(tmp>0){
                trace_k(a,record,tmp+1,record[tmp]-1);
                trace_k(a,record,ii,tmp-1);
            }
        }
    }
}


