/**
 *
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *	@date: May 5, 2014
 *
 */

//--- C++ ---//
//-----------//
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

//--- SVT DAQ ---//
//---------------//
#include <TriggerEvent.h>
#include <TriggerSample.h>
#include <DataRead.h>
#include <Data.h>

//--- SVT QA ---//
//--------------//
#include <BaselineAnalysis.h>

using namespace std; 

void displayUsage(); 

int main(int argc, char **argv)
{
	string input_file_name;

	bool run_baseline = false;	

	// Parse all the command line arguments. If there are no valid command line
	// arguments passed, print the usage and exit.
	int option_char; 
	while((option_char = getopt(argc, argv, "i:b")) != -1){
		switch(option_char){
			case 'i': 
				input_file_name = optarg; 
				break;
			case 'b': 
				run_baseline = true; 
				break;
			default:
               displayUsage();  
				break;
		}
	}

	// If an input file (binary or EVIO) was not specified, exit the program
	if(input_file_name.length() == 0){
		cerr << "\nPlease specify a file to process.\n" << endl;
		return EXIT_FAILURE; 
	}

	// 
	TriggerEvent trigger_event;
    TriggerSample* trigger_samples = new TriggerSample(); 
	DataRead *data_reader = new DataRead(); 

	// Open the input file.  If the input file can't be opened, exit.
	if(!data_reader->open(input_file_name)){
		cerr << "\nError! File " << input_file_name << " cannot be opened." << endl;
		return EXIT_FAILURE; 
	}

	cout << "[SVT QA]: Processing file: " << input_file_name << endl;

    // TODO: All analyses should be loaded dynamically
    BaselineAnalysis* baseline_analysis = new BaselineAnalysis(); 
	
   	
    if(run_baseline){
        cout << "[SVT QA]: Running baseline analysis" << endl;
        baseline_analysis->initialize();
    }

    int event_number = 0; 
	int channel;
	while(data_reader->next(&trigger_event)){
		++event_number;
		if(event_number%500 == 0) cout << "[SVT QA]: Processing event " << event_number << endl;

		for(uint sample_set_n = 0; sample_set_n < trigger_event.count(); ++sample_set_n){
			trigger_event.sample(sample_set_n, trigger_samples);
			baseline_analysis->processEvent(trigger_samples);
		}
	}
    
	if(run_baseline) baseline_analysis->finalize(); 

	return EXIT_SUCCESS;
}

void displayUsage()
{
    cout << "\nUsage: svt_qa [OPTIONS] -i [INPUT_FILE]" << endl;
    cout << "Either a binary or EVIO INPUT_FILE must be specified.\n" << endl;
    cout << "OPTIONS:\n"
         << "\t -i Input binary or EVIO file name \n"
         << "\t -b Run the baseline analysis \n"
         << "\t --help Display this help and exit \n"
         << endl; 
}
