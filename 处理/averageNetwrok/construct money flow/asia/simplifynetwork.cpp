#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
using namespace std;

#define Rep(i,l,r) for( i = (l); i <= (r); i++ )
#define RepD(i,l,r) for( i = (l); i >= (r); i-- )
#define SIZE  55								

long long relation[ SIZE+5 ][ SIZE+5 ];											//根据进口数据建立的国与国之间的边
int path[ SIZE+5 ];																//保存去环的遍历路径 记录树的遍历路径
int rudu[ SIZE+5 ];																//统计去环后每个点的入度 用于判断是否为根节点
int code[ SIZE+5 ];
int out[ SIZE+5 ];																//统计去环后每个点的出度 用于判断是否为叶子节点
int rootlist[ SIZE+5 ];															//存储入度为0的节点
int root_number, year;												//number表示国家数 rootnumber表示入度为0的节点的个数
long long profit[ SIZE+5 ];														//最终收益
long long MAXX=0;
bool removed;																	//判断是否删边 

long long cmp_max( long long x, long long y )	{ return x > y ? x : y; }

long long cmp_min( long long x, long long y )	{ return x < y ? x : y; }

void pre(){
	
	int i;
	
	freopen("Asia.txt","r",stdin);
	Rep( i, 1, SIZE )
		scanf( "%d", &code[ i ] );
	fclose(stdin);
}

void scan(){

	char str[ 100 ];
	int i,j;
	long long value;
	
	freopen("asia_ave_network.txt","r",stdin);
    Rep(i,1,SIZE)
        Rep(j,1,SIZE){
            scanf("%lld",&relation[i][j]);
			MAXX = cmp_max(MAXX,relation[i][j]);
        }
	
}

void remove_symmetric_flow(){

	int i, j;

	Rep( i, 1, SIZE )
		Rep( j, i+1, SIZE )
			if( relation[ i ][ j ] > relation[ j ][ i ] ){
				relation[ i ][ j ] -= relation[ j ][ i ];
				relation[ j ][ i ] = 0;
			}
			else{
				relation[ j ][ i ] -= relation[ i ][ j ];
				relation[ i ][ j ] = 0;
			}
}

void remove_loop( int loopsize, long long edgevalue ){

	int i;

	Rep( i, 1, loopsize-1 )
		relation[ path[ i ] ][ path[ i+1 ] ] -= edgevalue;
	relation[ path[ loopsize ] ][ path[ 1 ] ] -= edgevalue;

}

void remove_cyclic_flow_with( int nownode, int loopsize, int sequence, long long edgevalue ){
	int i;																		//依次表示 当前节点 环的规模 第几个节点 环上的最小边权

	path[ sequence ] = nownode;

	if( sequence == loopsize ){													//当次找环到达最后一个节点
			int first = path[ 1 ];												//这个环的起始点
			if( relation[ nownode ][ first ] ){									//找到一组环
				removed = true; 
				remove_loop( loopsize, cmp_min( edgevalue, relation[ nownode ][ first ] ) );
			}
	}
	else{
		Rep( i, 1, SIZE )
			if( relation[ nownode ][ i ] )
			{
				remove_cyclic_flow_with( i, loopsize, sequence + 1, cmp_min( edgevalue, relation[nownode][ i ] ) );				
				if( removed ) return;
			}
	}

}

void remove_cyclic_flow(){

	int i, j, count;

	Rep( i, 3, SIZE ){														//枚举环的规模
		Rep( j, 1, SIZE )														//枚举环的开始节点
		{
			while(1){
				removed = false;
				remove_cyclic_flow_with( j, i, 1, MAXX );
				if( !removed ) break;
			}
			printf("%d\n",j); 													//debug
		}			
		printf("loop %d is over\n", i);											//debug
	}													
}

void cal_rudu_out(){

	int i, j;
	memset( rudu, 0, sizeof( rudu ) );
	memset( out, 0, sizeof( out ) );

	Rep( i, 1, SIZE )
		Rep( j, 1, SIZE )
			if( relation[ i ][ j ] ){
				rudu[ j ]++;
				out[ i ]++;
			}

	root_number = 0;
	Rep( i, 1, SIZE )
		if( !rudu[ i ] )
			rootlist[ ++root_number ] = i;
			
}

void simplify_treepath( int depth, long long edgevalue ){
	
	int i,pre,next,root;

	if( depth == 1 || depth == 2 ) return; 
	removed = true;
	
	root = 1;
	pre = path[ root ];
	next = path[ depth ];
	if( !relation[ pre ][ next ] ) {
		rudu[ next ]++;
		out[ pre ]++;	
	} 
	relation[ pre ][ next ] += edgevalue;
	Rep( i, root, depth-1 ){
		pre = path[ i ];														//表示每条边上起始点
		next = path[ i+1 ];														//表示每条边上结束点
		relation[ pre ][ next ] -= edgevalue;
		if( !relation[ pre ][ next ] ){											//有节点升级为根节点
			out[ pre ]--;
			rudu[ next ]--;
			if( !rudu[ next ] )
				rootlist[ ++root_number ] = next;
		}
	}
		
}

void simplify_tree( int node, int depth, long long edgevalue ){	
	int i;																		//依次表示当前节点 树上某条路径上的最小边权	

	path[ depth ] = node;
	if( !out[ node ] )															//如果当前节点为叶子节点 
		simplify_treepath( depth, edgevalue );									//简化当前子树根节点到当前叶子节点的路径
	else{	
		Rep( i, 1, SIZE )
			if( relation[ node ][ i ] ){
				simplify_tree( i, depth+1, cmp_min( edgevalue, relation[ node ][ i ] ) );
				if( removed )	return;															
			}
	}
	path[ depth ] = 0;
}

bool check( int rootnode ){
	
	int i;
	
	Rep( i, 1, SIZE )
		if( relation[ rootnode ][ i ] )
			if( out[ i ] )
				return false;
	return true;
}

void remove_acyclic_flow(){							
	
	int i;

	cal_rudu_out();																//统计每个点的入度和出度
	Rep( i, 1, root_number ){
		int cnt =0; 
		while( !check( rootlist[ i ] ) ){										//检查是否完成路径简化 
			removed = false;
			memset( path, 0, sizeof( path ) );
			simplify_tree( rootlist[ i ], 1, MAXX );
			printf("%d ",++cnt);	
		}	
		printf("\n");
		printf("node %d is finished\n",i);
	}
										
}

void cal_profit(){

	int i, j;

	Rep( i, 1, SIZE )
		Rep( j, 1, SIZE )
			if( relation[ i ][ j ] ){
				profit[ i ] -= relation[ i ][ j ];
				profit[ j ] += relation[ i ][ j ];
			}
}

void print(){

	int i,j;

	freopen("simplify_network_asia.txt","w",stdout);

	Rep(i, 1, SIZE) {
		printf("country %d with others\n", code[i]);
		Rep(j, 1, SIZE)
			printf("contry %d %lld\n", code[i], relation[i][j]);						 //i国流向j国的资金
		printf("\n\n");
	}
}

void solve(){
	remove_symmetric_flow();
	remove_cyclic_flow();
	remove_acyclic_flow();

}

int main(){
	pre();
	scan();							
 	solve();
	print();

	return 0;
}
