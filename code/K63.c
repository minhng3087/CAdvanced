#include <stdio.h>
#include "graph.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

Graph g;

int ndinh;
int ncanh;

void docfile() {
    FILE* p = fopen("data.txt", "r");
    int id;
    char name[30];
    fscanf(p,"%d %d",&ndinh,&ncanh);
    for(int i=0;i<ndinh;i++) {
        fscanf(p,"%d %s",&id,name);
        addVertex(g, id, strdup(name));
    }
    int id1, id2;
    double w;
    for(int i=0;i<ncanh;i++) {
        fscanf(p,"%d %d %lf",&id1,&id2,&w);
        addEdge_notD(g, id1, id2, w);
    }
    fclose(p);
}

void printDinh() {
    JRB iter;
    jrb_traverse(iter, g.vertices)
    printf("%d %s\n",jval_i(iter->key),jval_s(iter->val));
    
}

void chucnang2() {
    int id;
    int output[100];
    printf("Nhap id :");scanf("%d",&id);
    double max = -1;
    int check;
    check = outdegree(g, id, output);
    if(check==0) {
        printf("-1\n");
    }
    else {
    for(int i = 1; i < ndinh; i++){
        if(getEdgeValue(g, id, output[i-1])> max) max = getEdgeValue(g, id, output[i-1]);
    }
    printf("%0.1lf\n",max);
    for(int i = 1; i < ndinh; i++){
        if(getEdgeValue(g, id, output[i-1]) == max) {
            printf("%d %s\n",output[i-1],getVertex(g, output[i-1]));
        }
    }
    }

}

void chucnang3() {
    int id1, id2;
    int length, path[100];
    double dis;

    printf("Nhap id1 va id2: ");scanf("%d %d",&id1, &id2);
    dis = shortestPath(g, id1, id2, path, &length);
    if(length==0||length==1||length==2||dis==INFINITY) {
        printf("-1\n");
    }
    else {
        printf("1\n");

    }
}

void chucnang4() {
    int id1, id2;
    printf("Nhap id1 va id2: ");scanf("%d %d",&id1,&id2);
    int outputid1[100];
    int outputid2[100];
    int check1 = outdegree(g, id1, outputid1);
    int check2 = outdegree(g, id2, outputid2);
    int k=0;
    int a[100];
    int m=0;
    for(int i=0;i<check1;i++) {
        for(int j=0;j<check2;j++) {
            if(outputid1[i]==outputid2[j]) {
                k++;
                a[m] = outputid1[i];
                m++;
            }
        }
    }
    if(k==0) printf("-1\n");
    else {
        printf("%d\n", k);
        for(int i=0;i<m;i++) {
            printf("%d %s\n",a[i],getVertex(g, a[i]));

        }
    }
}

void Menu() {
    printf("1.In danh sach dinh\n");
    printf("2.Tim ban than thiet nhat\n");
    printf("3.Kiem tra quan he bac cau\n");
    printf("4.Tim ban chung\n");
    printf("5.Thoat\n");
}

int main () {
    g = createGraph();
    int chucnang;
    do{
	    Menu();
        do{
            printf("Nhap chuc nang ban muon: ");
	        scanf("%d", &chucnang);
        }while(chucnang < 1 || chucnang > 5);
        if(chucnang==1) {
            docfile();
            printDinh();
        }
        if(chucnang==2) {
            chucnang2();
        }
        if(chucnang==3) {
            chucnang3();
        }

        if(chucnang==4) {
            chucnang4();
        }
        if(chucnang==5) break;
        
    }while(1);
    dropGraph(g);
}
