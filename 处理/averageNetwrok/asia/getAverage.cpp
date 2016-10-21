#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "getfilename.h"
using namespace std;

#define Rep(i,l,r) for(i=(l);i<=(r);i++)
#define Size 55

long long relation[Size+5][Size+5];
int code[Size+5];

void pre(){
	int i;
	freopen("Asia.txt","r",stdin);
	Rep( i, 1, Size )
		scanf( "%d", &code[ i ] );
	fclose(stdin);
}

int search_code( int codenum ){
	int i;
	Rep( i, 1, Size )
		if( code[ i ] == codenum )
			return i;
	return -1;
}

void scan(int year){
	char str[ 100 ];
	int report_code, trade_flow_code, partner_code;
	long long value;
    char* p = get_inputname( year );
	char input_name[40]="";
    strcpy(input_name,p);
	freopen(input_name,"r",stdin);
	gets( str );
	int count = 0;																//debug
	while( scanf( "%d%d%d%d%lld", &year, &report_code, &trade_flow_code, &partner_code, &value) != EOF ){
        if( report_code == partner_code )	continue;							//自己对自己进出口 舍去 
		report_code = search_code( report_code );
		partner_code = search_code( partner_code );
		if( report_code == -1 || partner_code == -1 )	continue;						
		if( trade_flow_code == 1 )	
			relation[ report_code ][ partner_code ] += value;					//report从partner进口 
		else relation[ partner_code ][ report_code ] += value;
	}
	fclose(stdin);
}

void ave(){
    int i,j;
    Rep(i,1,Size)
        Rep(j,1,Size)
            relation[i][j] /= 10;
}

void print(){
    freopen("asia_ave_network.txt","w",stdout);
    int i,j;
    Rep(i,1,Size){
        Rep(j,1,Size)
            printf("%lld\t",relation[i][j]);
        printf("\n");
    }
}

int main(){
    pre();
    int i;
    Rep(i,1,4)
        scan(i);
    ave();
    print();

    return 0;
}
