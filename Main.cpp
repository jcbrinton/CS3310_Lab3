/*
Joseph Brinton
Group 9 Lab #3
CS 3310 Section 001
*/

#define _SCL_SECURE_NO_WARNINGS 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <dos.h>
#include <iomanip>
#include <vector>
#include <algorithm>      
#include <random>
#include <unordered_map>

using namespace std;
using std::setw;

int basic_operations = 0;
int cache_hit = 0;

std::vector<int> w, p, k;

std::unordered_map<std::string, int> hash1;
std::vector<double>  r;
int W=0, knapsack=0, n;

//randomly generates weights and values for each item
void lootGen() {
	p.resize(n);
	w.resize(n);
	r.resize(n);
	
	for (int i = 0; i < n; i++) {
	
		p[i] = (rand() % 20) *10;
		w[i] = rand() % 20 +1;
		r[i] = p[i] / w[i];
	}

}
// greedy alorithm
int greedy() {
	int price = 0;
	for(int j = 0; j < n; j++){

		double max = 0;
		int index = -1;

		for (int i = 0; i < n; i++) {
			basic_operations++; 
			if (r[i] >= max) { //basic opertion
				max = r[i];
				index = i;
			}
		}
		knapsack += w[index];
		if (W < knapsack) {
			knapsack -= w[index];
		}
		else {
			k.push_back(index);
			price += p[index];
		}
		r[index] = -1;
	}
	return price;

}
// dynamic recursive algorithm
int recursive(int i, int V ) {
	
	std::string key = "(" + std::to_string(i) +" , " + std::to_string(V) + ")";
	basic_operations++;
	if (hash1.count(key) == 0) {//basic operation
		if (i == 0 || V == 0)
			hash1[key] = 0;
		else if (V >= w[i - 1]) {
			hash1[key] = max(recursive(i - 1, V), p[i - 1] + recursive(i - 1, V - w[i - 1]));
		}
		else
			hash1[key] = recursive(i - 1, V);
	}
	else
		cache_hit++;

	return hash1[key];
}


//main function
int main()
{
	while (true) {

		int count = 0;
		cout << "Enter the number of items n: ";
		cin >> n;
		while (!cin || n<0) {
			cin.clear();
			cin.ignore();
			cout << "Input invalid. Enter the number of items n: ";
			cin >> n;
		}
		cout << "Enter the size of the knapsack W: ";
		cin >> W;
		while (!cin || W<0) {
			cin.clear();
			cin.ignore();
			cout << "Input invalid. Enter the size of the knapsack W: ";
			cin >> W;
		}

		count = n;
		
		lootGen();
		/*cout << "price:  " << " ";
		for (int k = 0; k < n; k++)
			cout << setw(4) << std::left << p[k] << " " ;
		cout << endl;
		cout << "weight: " << " ";
		for (int k = 0; k < n; k++)
			cout << setw(4) << std::left << w[k] << " ";
		cout << endl;
		cout << "ratio:  " << " ";
		for (int k = 0; k < n; k++)
			cout << setw(4) << std::left << r[k] << " ";
		cout << endl;*/
		cout << setw(11) << std::left << "" << setw(11) << std::left << "Number" << setw(11) << std::left  <<"Max" << setw(11) << std::left << "Time" << setw(11) << std::left << "Basic" << endl;
		cout << setw(11) << std::left << "Algorithm" << setw(11) << std::left << "Items (n)" << setw(11) << std::left <<"Weight (W) " << setw(11) << std::left << "Elapsed" << setw(11) << std::left << "Operations" << endl;

		clock_t start;
		clock_t end;
		clock_t elapsed_clock;
		clock_t elapsed_time;

		start = clock();

		int pgreedy = greedy();

		end = clock();
		elapsed_clock = end - start;
		elapsed_time = ((end - start) / (CLK_TCK));
		
		cout << setw(11) << "Greedy" << setw(11) << count << setw(11) << W << setw(11) << elapsed_clock << setw(11) << basic_operations << endl;

		//reset global variables
		
		basic_operations = 0;
		start = 0;
		end = 0;
		elapsed_clock = 0;
		elapsed_time = 0;

		start = clock();

		int ks = recursive(n, W);

		end = clock();
		elapsed_clock = end - start;
		elapsed_time = ((end - start) / (CLK_TCK));
		
		cout << setw(11) << "Dynamic" << setw(11) << count << setw(11) << W << setw(11) << elapsed_clock << setw(11) << basic_operations << endl;
		cout << "Greedy knapsack:  " << " ";
		for (size_t j = 0; j < k.size(); j++)
			cout << setw(4) << std::left << k[j] << " ";
		cout << endl;
		k.clear();
		
		for (int i = n; i >= 0; i--) {
			std::string key = "(" + std::to_string(i) + " , " + std::to_string(W) + ")";
			std::string nextKey = "(" + std::to_string(i - 1) + " , " + std::to_string(W) + ")";
			if (hash1[key] != hash1[nextKey]) {
				k.push_back(i - 1);
				W = W - w[i - 1];
			}

		}
		std::reverse(k.begin(), k.end());
		cout << "Dynamic knapsack: " << " ";
		for (size_t j = 0; j < k.size(); j++)
			cout << setw(4) << std::left << k[j] << " ";
		cout << endl;
		cout << "Greedy total value:  " << pgreedy<<endl;
		cout << "Dynamic total value: " << ks << endl;
		
		cout <<"cache_hits: "<< cache_hit << " percentage: " <<(double)cache_hit/ basic_operations << endl;
		//reset stats
		basic_operations = 0;
		knapsack = 0;
		k.clear();
		r.clear();
		w.clear();
		p.clear();
		
		cache_hit = 0;
		hash1.clear();
		n = 0;

	}

}

