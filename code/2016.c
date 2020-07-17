#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libfdr/graph.h>
#include <ctype.h>

Graph g;

typedef struct {
    int id;
    char name[100];
}sanpham;

int nsanpham;

void docfile1(sanpham* sp) {
    FILE* p = fopen("products.txt","r");
    int id;
    char name[100];
    int i=1;
    while (!feof(p))
    {
        fscanf(p,"%d %s\n",&id,name);
        sp[i].id = id;
        strcpy(sp[i].name, name);
        addVertex(g, id, strdup(name)); 
        i++;
    }
    nsanpham = i;

    fclose(p);
}
int checkId(sanpham* sp, int id) {
    for(int i=1;i< nsanpham;i++) {
        if(sp[i].id == id) return i;
    }
}

void docfile2() {
    char str[1000], *tmp;
    int n;
    int arr_id[1000];
    FILE* f = fopen("orderhistory.txt", "r");
    while (fgets(str, 1000, f))
    {
        n = 0;
        arr_id[n++] = atoi(strtok(str, " "));
        while (1)
        {
            tmp = strtok(NULL, " ");
            if (tmp == NULL)
                break;
            arr_id[n++] = atoi(tmp);
        }
        for (int i = 0; i < n-1; ++i)
            for (int j = i+1; j < n; ++j)
            {
                addEdge(g, arr_id[i], arr_id[j], 1); // Da sua lai ham nay (graph.c)
                addEdge(g, arr_id[j], arr_id[i], 1); // Da sua lai ham nay (graph.c)
            }
    }
    fclose(f);
}

void printFile2(sanpham* sp) {
    FILE* p = fopen("orderhistory.txt","r");
    char str[100], *tmp;
    int id1, id2;
    double w = 1;
    while (!feof(p))
    {
        fgets(str, 100, p);
        id1 = atoi(strtok(str, " "));
        int check1 = checkId(sp, id1);
        printf("%s ",sp[check1].name);
        while (1)
        {
            tmp = strtok(NULL, " ");
            if (tmp == NULL)
                break;
            id2 = atoi(tmp);
            int check2 = checkId(sp, id2);
            printf("%s ",sp[check2].name);
        }
        printf("\n");
    }
    fclose(p);
}

void chucnang3() {
    int id1, id2;
    printf("Nhap id cua 2 sp: ");scanf("%d %d",&id1, &id2);
    printf("Do lien quan giua 2 san pham: ");
    if(!hasEdge(g, id1, id2)) {
        printf("-1\n");
    } 
    else if(hasEdge(g, id1, id2)) {
        printf("%.0lf\n",getEdgeValue(g, id1, id2));
    }
}

void chucnang4() {
    int id;
    int tmp;
    printf("Nhap id: ");
    scanf("%d",&id);
    int output[100];
    int check = outdegree(g, id, output);

    if(check==0) {
        printf("Cac san pham lien quan: \n");
    }
    else {
        for (int i = 0; i < check-1; ++i)
        for (int j = i+1; j < check; ++j)
            if (getEdgeValue(g, id, output[i]) < getEdgeValue(g, id, output[j]))
            {
                tmp = output[i];
                output[i] = output[j];
                output[j] = tmp;
            }
        printf("Cac san pham lien quan: ");
        for(int i=1;i<=check;i++) {
            printf("%s ",getVertex(g, output[i-1]));
        }
        printf("\n");
    }
}

void chucnang5() {
    int path[100];
    int length;
    int id1, id2;
    printf("Nhap 2 id: ");scanf("%d %d",&id1,&id2);
    double dis = shortestPath(g, id1, id2, path, &length);
    if(dis == INFINITY) {
        printf("Hai san pham nay khong co lien he voi nhau\n");        
    }
    else {
        for(int i=0;i<length-1;i++) 
            printf("%s-",getVertex(g, path[i]));
            printf("%s\n", getVertex(g, path[length-1]));
    }
}
 
void printSP(sanpham* sp) {
    for(int i=1;i<=nsanpham-1;i++) {
        printf("Ma san pham %d: \n", sp[i].id);
        printf("Ten san pham: %s \n", sp[i].name);
    } 
}

void Menu() {
    printf("1. In danh sach san pham\n");
    printf("2. In danh sach giao dich\n");
    printf("3. Hien thi muc do lien quan giua 2 san pham\n");
    printf("4. In danh sach cac san pham lien quan\n");
    printf("5. Hien thi moi lien he giua hai san pham\n");
    printf("6. Thoat \n");
}

int main () {
    g = createGraph();
    int chucnang;
    sanpham* list = (sanpham*)malloc(100*sizeof(sanpham));
    do{
	    Menu();
        do{
            printf("Nhap chuc nang ban muon: ");
	        scanf("%d", &chucnang);
        }while(chucnang < 1 || chucnang > 6);
	    if(chucnang==1) {
            docfile1(list);
            printSP(list);
        }
        if(chucnang==2) {
            docfile2();
            printFile2(list);
        }

        if(chucnang==3) {
            chucnang3();
        }

        if(chucnang==4) {
            chucnang4();
        }

        if(chucnang==5) {
            chucnang5();
        }
        if(chucnang==6) break;
        
    }while(1);
}