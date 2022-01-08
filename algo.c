#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------Structers--------------------------

typedef struct node_ {
    char node_num;
    double *weight;
    struct node_ **close_nodes; 
    int size_close_nodes;
} Node;

typedef struct graph_ {
    int size_all_nodes;
    struct node_ **node_ID; 
} Graph;



//------------------------------------Graph-------------------------------

Graph *build_graph() {
    Graph *new_graph = (Graph *)malloc(sizeof(Graph));

    new_graph->size_all_nodes = 0;
    new_graph->node_ID = (Node **)malloc(new_graph->size_all_nodes * sizeof(Node *));
    return new_graph;
}

void release_graph(struct graph_ *G) {
    int i;
    for(i = 0 ; i < G->size_all_nodes; i++) {
        free(G->node_ID[i]->close_nodes);
        free(G->node_ID[i]);
        free(G->node_ID[i]->weight);
    }
    free(G->node_ID);
    free(G);
}


//---------------------------------Edge----------------------------------------------

/* 
This function will add an edge from the source node to the destination node, and determine the weight of this edge.
*/
void insert_edge(Node *src, Node *dest, double weight) {
    
    src->weight = realloc(src->weight, (src->size_close_nodes + 1) * sizeof(double));
    if(src->weight == NULL){
        exit(1);
    }
   
    src->close_nodes = realloc(src->close_nodes, (src->size_close_nodes + 1) * sizeof(Node *));
    if(src->close_nodes == NULL){
        exit(1);
    }

    src->close_nodes[src->size_close_nodes] = dest;
    src->weight[src->size_close_nodes] = weight;
    src->size_close_nodes++;
}



/* 
This function will delet the edge you asked for.
Parameters: G - the spesific graph we want to remove the edge.
            src - will be the source node_ID of the edge
            dest - will be the destination node_ID of the edge
*/
void remove_edge(Graph *G, Node *src, Node *dest) {
    int temp = 0;
    int s_all = src->size_close_nodes;
    for (int i = 0; i < s_all; i++) {
        for(int j = 0; j < s_all; j++){
            if(src->close_nodes[i]->node_num != dest->node_num){
                src->close_nodes[j] = src->close_nodes[i];
            }
            else
                temp=i;
        }
    }
    int curr = temp + 1;
    for (int i = curr; i < s_all; i++) {
        src->weight[temp] = src->weight[i];
        temp++;
    }
    
    src->close_nodes = (Node **)realloc(src->close_nodes, (s_all - 1) * sizeof(Node *));
    if(src->close_nodes == NULL){
        exit(1);
    }
    s_all--;

    src->weight = (double *)realloc(src->weight, (s_all) * sizeof(double));
    if(src->weight == NULL){
        exit(1);
    }
}

//--------------------------------Node-----------------------------------------

/*
This function will creat a new node by the data recive into the graph.
*/
Node *init_node(Graph *G, char data) {

    for(int i = 0; i < G->size_all_nodes; i++){
        if(data == G->node_ID[i]->node_num){
            return G->node_ID[i];
        }
    }
    Node *new_node;
    new_node = (Node *) malloc(sizeof(Node));
    if(new_node != NULL){
        new_node->node_num = data;
        new_node->size_close_nodes = 0;
        new_node->close_nodes = (Node **) malloc(new_node->size_close_nodes * sizeof(Node *));
    }
    exit(1);
    
    if(new_node->close_nodes != NULL){
        new_node->weight = (double *) malloc(new_node->size_close_nodes * sizeof(double));
    }
    exit(1);

    if(new_node->weight==NULL){
        exit(1);
    }
    return new_node;
}

/*
This function return the index number of the node in the list.
*/
int getNodeID(Graph *G, Node *node) {
    for(int i = 0; i < G->size_all_nodes; i++) {
        if(G->node_ID[i]->node_num == node->node_num) {
            return i;
        }
    }
    printf("In index: %c\n", node->node_num);
    return -1;
}

/*
This function will add a new node to the graph.
*/
void insert_node(Graph *G, Node *new_node) {
    for(int i = 0; i < G->size_all_nodes; i++){
        if(G->node_ID[i]->node_num == new_node->node_num){
            return; 
        }
    }
    if((Node **)realloc(G->node_ID, (G->size_all_nodes + 1) * sizeof(Node *)) == NULL){
        exit(1);
    }
    G->node_ID[G->size_all_nodes] = new_node;
    G->size_all_nodes++;
}

/*
This function will delet the node from the grapg, and will remove the all our edges of this spisific node.
*/
void remove_node(Graph *G, Node *node) {
    Node *curr;
    for (int i = 0; i < G->size_all_nodes; i++) {
        for(int j = 0; j < G->size_all_nodes; j++){
            curr = G->node_ID[i];
            if (curr->node_num != node->node_num) {
                G->node_ID[j] = curr;
            }
            else{
                free(curr->weight);    
                free(curr->close_nodes);
                
            }
        }
    }

    if((Node **) realloc(G->node_ID, (G->size_all_nodes - 1) * sizeof(Node *)) == NULL){
        exit(1);
    }
    G->size_all_nodes--;
    for (int i = 0; i < G->size_all_nodes; i++){
        Node *curr =G->node_ID[i];
        for (int j = 0; j < G->node_ID[i]->size_close_nodes; j++){
            if(curr->close_nodes[j]->node_num == node->node_num){
                remove_edge(G, curr, node);
            }
        }
    }
}

//---------------------------------Helper Functions-------------------------------------------

int sameNodes(Node *id1, Node *id2) {
    return id1 == id2 && id1->node_num == id2->node_num;
}

int Dijsktra(Graph *graph, Node *start, Node *end) {
    Node *neighbor,*node,*cur;
    int dist[graph->size_all_nodes];
    int previous[graph->size_all_nodes];
    int visit[graph->size_all_nodes];
    double alternative,shortest ;
    int n_visit, indx, shortest_indx,i;

    for(i = 0; i < graph->size_all_nodes; i++) {
        node = graph->node_ID[i];
        if(node->node_num == start->node_num) {
            dist[i] = 0;}
        else {
            dist[i] = 1000;}
        previous[i] = -1;
        visit[i] = 0;
    }

    n_visit = 0;
    while(n_visit < graph->size_all_nodes) {
        shortest = 1000;
        shortest_indx = 0;
        for(i = 0; i < graph->size_all_nodes; i++) {
            if(dist[i] < shortest && !visit[i]) {
                shortest_indx = i;
                shortest = dist[i];
            }
        }

        cur = graph->node_ID[shortest_indx];
        visit[shortest_indx] = 1;
        n_visit += 1;

        if(sameNodes(cur, end)) {
            break;
        }

        for(i = 0; i < cur->size_close_nodes; i++) {
            neighbor = cur->close_nodes[i];
            indx = getNodeID(graph, neighbor);
            alternative = dist[shortest_indx] + cur->weight[i];

            if(dist[indx] > alternative) {
                dist[indx] = alternative;
                previous[indx] = shortest_indx;
            }
        }
    }

    i = getNodeID(graph, end);
    int w = dist[i];
    return w;
}





//-------------------------------Functions-----------------------------------

Graph * build_graph_cmd(char *ans, int len ){
    Graph *g = build_graph();
    int i=1;
    while (i<len-1){
        if (ans[i]=='n'){
            i++;
            
            Node *src = init_node(g,ans[i]);
            insert_node(g,src);
            if (i>=len-2){
                break;
            }
            i++;
            while(ans[i]!='n'){
                Node *dest = init_node(g,ans[i]);
                insert_node(g,dest);
                i++;
                insert_edge(src,dest, ((ans[i])-'0'));
                i++;
            }

        }
    }

    return g;
}
///////////////////////////////////////////////////////////
void B(char ans [],Graph *graph){
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    if(node==NULL){
        exit(1);
    }
    node->node_num = ans[0];
     for(int i=0; i<graph->size_all_nodes; i++){
        if(node->node_num == graph->node_ID[i]->node_num){
          free(graph->node_ID[i]->close_nodes);
          free(graph->node_ID[i]->weight);
          graph->node_ID[i]->size_close_nodes=0;
          graph->node_ID[i]->close_nodes = (Node **) malloc(graph->node_ID[i]->size_close_nodes * sizeof(Node *));
            if(graph->node_ID[i]->close_nodes==NULL){
                exit(1);
            }
            graph->node_ID[i]->weight = (double *) malloc(graph->node_ID[i]->size_close_nodes * sizeof(double));
            if(graph->node_ID[i]->weight==NULL){
                exit(1);
            }
            Node *s;
            for(int i=0; i<graph->size_all_nodes; i++){
            if(node->node_num == graph->node_ID[i]->node_num){
                s= graph->node_ID[i];}}
                free(node);
                while(ans[i]!='\0'){
                Node *dest = init_node(graph,ans[i]);
                insert_node(graph,dest);
                i++;
                insert_edge(s,dest, ((ans[i])-'0'));
                i++;
            }
            return;
            
        }
    }
    node->size_close_nodes = 0;
    node->close_nodes = (Node **) malloc(node->size_close_nodes * sizeof(Node *));
    if(node->close_nodes==NULL){
        exit(1);
    }
    node->weight = (double *) malloc(node->size_close_nodes * sizeof(double));
    if(node->weight==NULL){
        exit(1);
    }

    insert_node(graph,node);
    int i=1;
    while(ans[i]!='\0'){
        Node *dest = init_node(graph,ans[i]);
        insert_node(graph,dest);
        i++;
        insert_edge(node,dest, ((ans[i]) - '0'));
        i++;
    }
}

/////////////////////////////////////////////////////////
void D(char ans [],Graph *graph){
    Node *save;
    for (int i = 0; i <graph->size_all_nodes ; ++i) {
        if(graph->node_ID[i]->node_num == ans[0]){
            save=graph->node_ID[i];
        }
    }
    remove_node(graph,save);
    free(save);
}


//////////////////////////////////////////////////////////
void S(char ans [],Graph *graph){
    Node *src;
    for (int i = 0; i <graph->size_all_nodes ; ++i) {
        if(graph->node_ID[i]->node_num == ans[0]){
            src=graph->node_ID[i];
        }
    }
    Node *dest;
    for (int i = 0; i <graph->size_all_nodes ; ++i) {
        if(graph->node_ID[i]->node_num == ans[1]){
            dest=graph->node_ID[i];
        }
    }
    printf("Dijsktra shortest path: %d \n", Dijsktra(graph, src, dest));
    
}

////////////////////////////////////////////////////
void to_start(char *list, int len, Node *s) {
    char cnt[len];
    strcpy(cnt, list);
    int j=2;
    for (int i = 1; i <len; i++) {
        list[j] = cnt[i];
        j++;
    }
    list[1]=s->node_num;

}


//////////////////////////////////////////////

int tsp(Graph *graph, char list [], int len){
    if (len==1||len==0){
        return -1;
    }
    if (len==2){
        return -1;
    }
    int w=0;
    for(int d=1; d<len-1; d++) {
        Node *save_id1;
        for (int i = 0; i < graph->size_all_nodes; ++i) {
            if (graph->node_ID[i]->node_num == list[d]) {
                save_id1 = graph->node_ID[i];
            }
        }
        Node *save_id2;
        for (int i = 0; i < graph->size_all_nodes; ++i) {
            if (graph->node_ID[i]->node_num == list[d + 1]) {
                save_id2 = graph->node_ID[i];
            }
        }
        if(save_id1->size_close_nodes>0) {
            w += Dijsktra(graph, save_id1, save_id2);
        }
        else{
            w += Dijsktra(graph, save_id2, save_id1);
        }
    }
    if (w<10){
        return w;
    }else {

        for (int i = 0; i < len; ++i) {
            Node *s;
            for (int i = 0; i < graph->size_all_nodes; ++i) {
                if (graph->node_ID[i]->node_num == list[len-1]) {
                    s = graph->node_ID[i];
                    to_start(list,len,s);
                }

            }
        }

        int z=0;
        for(int d=1; d<len-1; d++) {
            Node *save_id1;
            for (int i = 0; i < graph->size_all_nodes; ++i) {
                if (graph->node_ID[i]->node_num == list[d]) {
                    save_id1 = graph->node_ID[i];
                }
            }
            Node *save_id2;
            for (int i = 0; i < graph->size_all_nodes; ++i) {
                if (graph->node_ID[i]->node_num == list[d + 1]) {
                    save_id2 = graph->node_ID[i];
                }
            }
                z += Dijsktra(graph, save_id1, save_id2);

        }

        if(z>=1000){
            return -1;
        }

        return z;
    }


}

void T(char ans [],Graph *graph){
    printf( "TSP shortest path: %d \n", tsp(graph,ans, strlen(ans)));
}

//////////////////////////////////////////////////////
char* replace(char *arr,char* str , int len, int index){
    int i = index;
    int j = 0;
    for (int n= 0; n < len; n++) {
        str[j++]=arr[i++];
    }
    str[len]='\0';
    return str;
}

int main(){

    char sen[1024]; //max size
    gets(sen); 

    char sen_co[strlen(sen)+1];

    char* str=(char*)malloc((strlen(sen)+1) + 1); 

     if(str == NULL){ //check if the pointer is null
        exit(1);
    }
    
    strcpy(sen_co,replace(sen, str , strlen(sen)+1, 0 ));
    free(str);

    int r=0;
    for(int i = 0; i < strlen(sen)+1; i++){
        if(sen_co[i]==' '){
            r++;
        }
        i++;
    }
    
    char new[strlen(sen)+1];
    int j=0;
    for(int i=0; i < strlen(sen)+1; i++){

        if(sen_co[i]!=' '){
            new[j]=sen_co[i];
            j++;
        }
        i++;
    }

    char str_n[strlen(new)-1];
    char* p_strn=(char*)malloc(strlen(new)-1 + 1);

     if(p_strn == NULL){
        exit(1);
    }

    strcpy(str_n,replace(new ,p_strn , strlen(new)-1, 1));
    free(p_strn);

    int i=0;
    int k =0;
    while (k < strlen(str_n)) {
        if ( str_n[k] =='B'|| str_n[k] =='D'|| str_n[k] =='S'|| str_n[k] =='T'|| str_n[k] =='\0'||  str_n[k]=='A'){
            break;
        }else{
            i++;
        }
        k++;
    }

    int c=0;
    char curr[i+1];
    for (int j = 0; j <= i-1 ; j++) {
        curr[c]= str_n[j];
        c++;
    }

    curr[i]='\0';
    Graph *g= build_graph_cmd(curr, i); //create 
    int len = strlen(str_n)-i;
    char arr[len];

    char* p = (char*)malloc(len + 1);
     if(p==NULL){
        exit(1);
    }

    strcpy(arr,replace(str_n,p , len,i)) ;
    free(p);

    arr[len]='\0';
    while (strlen(arr)!=0) {

        if (arr[0] == 'B') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                } else {
                    l++;
                }
            }
           
            char* p=(char*)malloc(l + 1);
            if(p == NULL){
                exit(1);
            }

            B(replace(arr,p , l, 1), g);
            free(p);
            
            int l_n =strlen(arr) - l - 1; 
            char* n_p=(char*)malloc(l_n + 1);
            if(n_p == NULL){
            exit(1);
            }
            strcpy(arr, replace(arr,n_p,l_n, l + 1));
            free(n_p);
          
        }


        if (arr[0] == 'D') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                    
                } else {
                    l++;
                }
            }

            
            char* po=(char*)malloc(l + 1);
            if(po == NULL){
                exit(1);
            }

            D(replace(arr,po , l, 1), g);
            free(po);
            int len = strlen(arr) - l - 1;
            char* p=(char*)malloc(len + 1);

             if(p == NULL){
                exit(1);
            }

            strcpy(arr, replace(arr,p , len, l + 1));
            free(p);
        }

        if (arr[0] == 'A') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0'|| arr[j] =='A') {
                    break;
                } else {
                    l++;
                }
            }

            char* p=(char*)malloc(l + 1);
             if(p==NULL){
                exit(1);
            }
            release_graph(g);
            g= build_graph_cmd(replace(arr,p , l, 1), l);
            free(p);
            
            
            char* po=(char*)malloc(len + 1);
             if(po==NULL){
                exit(1);
            }
            strcpy(arr, replace(arr,po , len, l + 1));
            free(po);
        }


        if (arr[0] == 'S') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                } else {
                    l++;
                }
            }

            char* p=(char*)malloc(l + 1);
            if(p == NULL){
                exit(1);
            }

            S(replace(arr,p , l, 1), g);
            free(p);

            char* po=(char*)malloc(len + 1);

             if(po == NULL){
                exit(1);
            }
            strcpy(arr, replace(arr,po , len, l + 1));
            free(po);
        }


        if (arr[0] == 'T') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                } else {
                    l++;
                }
            }
            char* p=(char*)malloc(l + 1);
             if(p==NULL){
                exit(1);
            }

            T(replace(arr,p, l, 1), g);
            free(p);

            char* po=(char*)malloc(len + 1);
            if(po == NULL){
                exit(1);
            }
            strcpy(arr, replace(arr,po ,len, l + 1));
            free(po);
        }
    }


    release_graph(g);
    
return 0;
}