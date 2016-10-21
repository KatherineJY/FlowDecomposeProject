#ifndef getfilename
#define getfilename value

	char year_string[5][5]={"","2011","2012","2013","2014"};
	const char input_string[30]="comtrade_trade_data_Import_";

	char* get_inputname(int year){
		char inputstring[40]="";
		strcpy(inputstring,input_string);
		strcat(inputstring, year_string[year]);
		strcat(inputstring,".txt"); 
		return inputstring;
	}	


#endif