#include <stdio.h>
#include <stdlib.h>
#include <libfdr/graph.h>
#include <string.h>

#define MAX 100

int nsanpham;
int nkhohang;

Graph g;

typedef struct {
    char name[30];
    int idsp;
    int soluongsp;
}sanpham;

typedef struct 
{
    char name[30];
    int id;
    sanpham sp[30];
}khohang;


void readFileSP(char* filename, sanpham* sp) {
    FILE *p = fopen(filename,"r");
    char namesp[30];
    int idsp;
    fscanf(p,"%d",&nsanpham);
    for(int i=0;i<nsanpham;i++) {
        fscanf(p,"%s %d",namesp,&idsp);
        sp[i].idsp = idsp;
        strcpy(sp[i].name,namesp);
    }
    fclose(p);
}

void printSP(sanpham* sp) {
    for(int i=0;i<nsanpham;i++) {
        printf("%s %d\n",sp[i].name,sp[i].idsp);
    }
}

void readFileKho(char* filename, khohang* list, sanpham* sp) {
    FILE *fkho = fopen(filename,"r");
    char nameKho[30];
    int idKho, idSp, nSP, nlienket;
    int v1, v2;
    double w;
    fscanf(fkho,"%d",&nkhohang);
    for(int i=0;i<nkhohang;i++) {
        fscanf(fkho,"%s %d",nameKho,&idKho);
        addVertex(g, idKho, strdup(nameKho));
        list[i].id = idKho;
        strcpy(list[i].name, nameKho);
        for(int j=0;j<nsanpham;j++) {
            fscanf(fkho,"%d %d",&idSp, &nSP);
            list[i].sp[j].soluongsp = nSP;
            list[i].sp[j].idsp = idSp;
            int k;
            for(k=0;k<nsanpham;k++) {
                if(sp[k].idsp == idSp) break;
            }
            strcpy(list[i].sp[j].name, sp[k].name);
        }

    }
    fscanf(fkho,"%d",&nlienket);
    for(int i=0;i<nlienket;i++) {
        fscanf(fkho, "%d %d %lf", &v1, &v2, &w);
        addEdge_notD(g ,v1, v2, w);
    }
    fclose(fkho);
}

void printKho(khohang* list) {
    for(int i=0;i < nkhohang;i++) {
        printf("%s\n", list[i].name);
        for(int j=0;j<nsanpham;j++) {
            printf("%s %d\n",list[i].sp[j].name, list[i].sp[j].soluongsp);
        }
        printf("-----\n");
    }

}

int findSP(khohang list, int idsp){
    for(int i = 0; i < nsanpham; i++){
        if(list.sp[i].idsp == idsp) return i;
    }
}
 
int findKho(khohang* list, int idfind) {
     for(int i=0; i<nkhohang; i++) {
         if(list[i].id == idfind) return i;
     }
     
}

void kiemkeKho(khohang* list, sanpham* sp, int idsp, int idKho) {
    int i=0;
    int fkho = findKho(list, idKho);
    int fsp = findSP(list[fkho], idsp);
    printf("%s\n", list[fkho].name);
    printf("%s %d\n", list[fkho].sp[fsp].name, list[fkho].sp[fsp].soluongsp);
    printf("---Cac kho ke la: \n");
    while(i<nkhohang) {
        if(hasEdge(g, idKho, list[i].id)) {
            fkho = findKho(list, list[i].id);
            fsp = findSP(list[fkho], idsp);
            printf("%s\n", list[fkho].name);
            printf("%s %d\n", list[fkho].sp[fsp].name, list[fkho].sp[fsp].soluongsp);
        }
        i++;
    }
}

void ship(Graph g, khohang* list, int idsp, int soluongsp, int idKho1, int idKho2) {
    int fkho1 = findKho(list, idKho1);
    int fkho2 = findKho(list, idKho2);
    int fsp1 = findSP(list[fkho1], idsp);
    int fsp2 = findSP(list[fkho2], idsp);
    if(soluongsp<=list[fkho1].sp[fsp1].soluongsp) 
        printf("Dat hang thanh cong. Thoi gian giao hang la 30 phut\n");
    else if(soluongsp>list[fkho1].sp[fsp1].soluongsp)
    {
        if(soluongsp>list[fkho2].sp[fsp2].soluongsp) 
            printf("Khong dat hang thanh cong\n");
        else if(soluongsp<=list[fkho2].sp[fsp2].soluongsp)
        {
            int path[MAX];
            int length;
            double w = shortestPath(g, idKho1, idKho2, path, &length);
            double time = (w/30)*60 + 30;
            int hour = (int)time;
            int min = hour % 60;
            hour = hour / 60;
            printf("Dat hang thanh cong , thoi gian giao hang la %d gio %d phut\n", hour, min);
        }

    }

}


int main(int argc, char const *argv[]) {
    g = createGraph();
    khohang *list = (khohang*)malloc(MAX * sizeof(khohang));
    sanpham *sp = (sanpham*)malloc(MAX*sizeof(sanpham));
    if(strcmp(argv[1],"-t")==0) printf("C-Advanced, HK20182\n");
    else if (strcmp(argv[1], "-s") == 0){
        readFileSP(argv[2], sp);
        printSP(sp);
    }
    else if (strcmp(argv[1], "-w") == 0){
        readFileSP(argv[3], sp);
        readFileKho(argv[2], list, sp);
        printf("%.0lf\n", getEdgeValue(g, atoi(argv[4]), atoi(argv[5])));
    }
    else if (strcmp(argv[1], "-a") == 0){
        readFileSP(argv[3], sp);
        readFileKho(argv[2], list, sp);
        printKho(list);
    }
    else if (strcmp(argv[1], "-h") == 0) {
        readFileSP(argv[3], sp);
        readFileKho(argv[2], list, sp);
        kiemkeKho(list, sp, atoi(argv[4]), atoi(argv[5]));
    }

    else if (strcmp(argv[1], "-g") == 0) {
        readFileSP(argv[3], sp);
        readFileKho(argv[2], list, sp);
        ship(g, list, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
    }
    
}


