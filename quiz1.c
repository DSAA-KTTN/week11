#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jval.h"
#include"dllist.h"
#include"jrb.h"
#define INFINITY_VALUE 1000000
typedef struct{
    JRB edges;
    JRB vertices;
} Graph;
Graph createGraph ();
void dropGraph (Graph graph);
void addVertex (Graph graph, int id, char name);
char getVertex(Graph graph, int id);
int getVertexID(Graph graph, char c);
void addEdge (Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
typedef struct {
    Graph graph;
    JRB root;
} HuffmanTree;
typedef struct{
    int size;
    char bits[10];
 } Coding;
void insertQueue ( JRB q, int id, int freq);
int findMinInQueue (JRB queue, int *freq);
void delInQueue(JRB queue, int id);
HuffmanTree makeHuffman (char *buffer, int size);
void find_code (HuffmanTree H, int ID, Coding htable[],char code[]);
void createHuffmanTable (HuffmanTree H,Coding htable[]);
void compress(char *input, char *output, Coding huffmanTable[]);
Graph createGraph(){
    Graph new;
    new.edges=make_jrb();
    new.vertices=make_jrb();
    return new;
}
JRB jrb_find_id(JRB tree, Jval value){
  JRB tmp;
  jrb_traverse(tmp,tree){
    if((tmp->key).i==(value).i){
      return tmp;
    }
  }
  return NULL;
}
void dropGraph(Graph graph){
    JRB node;
    jrb_traverse(node, graph.edges){
    jrb_free_tree( jval_v(node->val) );
    }
    jrb_traverse(node, graph.vertices){
    jrb_free_tree( jval_v(node->val) );
    }
}
void addVertex(Graph graph, int id, char name){
    JRB node= jrb_find_id(graph.vertices,new_jval_i(id));
    if(node!=NULL){
    }else{
    jrb_insert_int(graph.vertices,id,new_jval_c(name));
    }
}
char getVertex(Graph graph, int id){
    JRB node=jrb_find_id(graph.vertices,new_jval_i(id));
    if(node==NULL) return '\0';
    return jval_c(node->val);
}
int getVertexID(Graph graph, char c){
    JRB node;
    jrb_traverse(node,graph.vertices){
        if(jval_c(node->val)==c)
        return jval_i(node->key);
    }
    return INFINITY_VALUE;
}
void addEdge(Graph graph, int v1, int v2,double weight){
    JRB node=jrb_find_int(graph.edges,v1);
    JRB tree;
    if(node==NULL){
        tree = make_jrb();
        jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }else{
        tree = (JRB) jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
}
double getEdgeValue(Graph graph, int v1, int v2){
    JRB node=jrb_find_int(graph.edges,v1);
    JRB node2=jrb_find_int(graph.edges,v2);
    if(node==NULL || node2==NULL){
        return 0;
    }
    JRB tree;
    tree = (JRB) jval_v(node->val);
    double total = INFINITY_VALUE;
    jrb_traverse(node, tree){
        if(jval_i(node->key)==v2) total=jval_d(node->val);
        break;
    }
    if(total==INFINITY_VALUE){
        tree = (JRB) jval_v(node2->val);
        jrb_traverse(node, tree){
        if(jval_i(node2->key)==v1) total=jval_d(node2->val);
        break;
    }
    }
    return total;
}
//
void insertQueue ( JRB q, int id, int freq){
    JRB node;
    node=jrb_find_int(q,id);
    if(node==NULL)
    jrb_insert_int(q,id,new_jval_i(freq));
    else{
        int add=jval_i(node->val);
        add=add+freq;
        node->val=new_jval_i(add);
    }
}
int findMinInQueue (JRB queue, int *freq){
    JRB node;
    int min=INFINITY_VALUE;
    int id=0;
    jrb_traverse(node,queue){
        if(jval_i(node->val)<min){
            min=jval_i(node->val);
            id=jval_i(node->key);
        }
    }
    *freq=min;
    return id;

}
void delInQueue(JRB queue, int id){
    JRB node;
    node=jrb_find_int(queue,id);
    jrb_delete_node(node);
}   
HuffmanTree makeHuffman(char *buffer, int size){
    HuffmanTree tree;
    tree.graph.edges=make_jrb();
    tree.graph.vertices=make_jrb();
    JRB queue=make_jrb();
    int i,count=0;
    int id;
    JRB node;
    for(i=0;i<size;i++){
         id=getVertexID(tree.graph,buffer[i]);
        if(id==INFINITY_VALUE){
            count++;
            id=count;
            addVertex(tree.graph,id,buffer[i]);
            insertQueue(queue,id,1);
        }else{
        insertQueue(queue,id,1);
        }
        jrb_traverse(node,queue);
    }
while(1){
  int min1, min2;
  int freq1, freq2;
  min1 = findMinInQueue (queue, &freq1);
  delInQueue (queue, min1);
  min2 = findMinInQueue (queue, &freq2);
  if (min2 == 0)
    break;
  delInQueue (queue, min2);
  count++;
  int id = count; 
  addEdge (tree.graph, id, min1, 0);
  addEdge (tree.graph, id, min2, 1);
  insertQueue (queue, id, freq1 + freq2);
}
int idRoot = count;
tree.root = jrb_find_int(tree.graph.edges, idRoot);
return tree;
}

void find_code (HuffmanTree H, int ID, Coding htable[],char code[]){
    JRB node,tree,buf;
    char c;
    node=jrb_find_int(H.graph.vertices,ID);
    if(node!=NULL){
        c=jval_c(node->val);
        strcpy(htable[(int)c].bits,code);
        htable->size=strlen(code);
        return;
    }else{
        node=jrb_find_int(H.graph.edges,ID);
        tree=(JRB)jval_v(node->val);
        jrb_traverse(buf,tree){
            if(jval_d(buf->val)==1.0) strcat(code,"1");
            else strcat(code,"0");
        find_code(H,jval_i(buf->key),htable,code);
        code[strlen(code)-1]='\0';
        }
    }
}
void createHuffmanTable (HuffmanTree H, Coding htable[]){
    int i;
    char code[256]="";
    for (i=0; i<256; i++)
        htable[i].size = 0;
   find_code(H, jval_i(H.root->key), htable, code);
}
void compress (char *input, char *output, Coding huffmanTable[]){
    for (int i=0; i < strlen(input); i++){
        printf("%-3c: %10s\n",input[i],huffmanTable[input[i]].bits);
        strcat(output, huffmanTable[input[i]].bits);
    }
}
int main(){
    char s[30]="kttn";
    char output[512];
    Coding HuffmanTable[256];
    HuffmanTree tree=makeHuffman(s,strlen(s));
    createHuffmanTable(tree,HuffmanTable);
    compress(s,output,HuffmanTable);
    printf("%s\n",output);
    printf("\n");

}

