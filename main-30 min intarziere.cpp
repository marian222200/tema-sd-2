#include<fstream>
using namespace std;
ifstream fin("fibo_heap.in");
ofstream fout("fibo_heap.out");
struct nod{
    nod *father=NULL,*son=NULL,*left=NULL,*right=NULL;///links father son-one of the sons(the smallest one) left right(kids with the same parent)
    int key=-1;/// the value
    int kids=0;///number of children
}*h,*stiva[1001];
int nst;
void push(nod *p){
    stiva[++nst]=p;
}
nod* top(){
    return stiva[nst];
}
void pop(){
    nst--;
}
bool empty(){
    return nst==0;
}
void ins(nod *p){
    if(h==NULL){
        p->right=p;
        p->left=p;
        h=p;
    }
    else{
        p->right=h;
        p->left=h->left;
        h->left->right=p;
        h->left=p;
        if(p->key<h->key)h=p;
    }
}
nod* cauta(int x){
    if(x<h->key)return NULL;
    nod *p,*poz;
    push(h);
    while(!empty()){
        poz=top();
        pop();

        if(x==poz->key){
            while(!empty())pop();
            return poz;
        }
        p=poz->right;
        while(p!=poz){
            if(p->key==x){
                while(!empty())pop();
                return p;
            }
            p=p->right;
        }

        if(poz->kids>0&&poz->son->key>=x)push(poz->son);
        p=poz->right;
        while(p!=poz){
            if(p->kids>0&&p->son->key>=x)push(p->son);
            p=p->right;
        }
    }
    return NULL;
}
void del(int x){
    nod *p,*poz=cauta(x);
    if(poz==NULL){fout<<x<<" nu exista in arbore!\n";}
    else{
        if(poz->father!=NULL){
            poz->father->kids--;
            if(poz->father->son==poz)
                if(poz->father->kids>0)poz->father->son=poz->right;
                else poz->father->son=NULL;
        }
        if(poz->left!=NULL){
            poz->left->right=poz->right;
            poz->right->left=poz->left;
        }
        if(poz->son!=NULL){
            ins(poz->son);
            p=poz->son->right;
            while(p!=poz->son){
                ins(p);
                p=p->right;
            }
        }
        if(h==poz)h=h->left;
        delete poz;
    }
}
void build(){
    nod *v[1001]={0};
    nod *p=h->right,*cp;
    v[h->kids]=h;
    while(p!=h){
        if(v[p->kids]==NULL){
            v[p->kids]=p;
            p=p->left;
        }
        else{
            if(v[p->kids]->key<p->key){
                cp=p->right;
                ///h!=p din while

                p->left->right=p->right;
                p->right->left=p->left;

                if(v[p->kids]->son==NULL){
                    p->right=p;
                    p->left=p;
                }
                else{
                    p->right=v[p->kids]->son;
                    p->left=v[p->kids]->son->left;
                    v[p->kids]->son->left->right=p;
                    v[p->kids]->son->left=p;
                    if(p->key<v[p->kids]->son->key)v[p->kids]->son=p;
                }

                if(h==v[p->kids])h=h->left;

                v[p->kids]->left->right=v[p->kids]->right;
                v[p->kids]->right->left=v[p->kids]->left;

                v[p->kids]->right=cp;
                v[p->kids]->left=cp->left;
                cp->left->right=v[p->kids];
                cp->left=v[p->kids];

                p=v[p->kids];
                v[p->kids]->kids++;
                v[p->kids]=NULL;
            }
            else{
                if(v[p->kids]==h)h=h->left;

                v[p->kids]->left->right=v[p->kids]->right;
                v[p->kids]->right->left=v[p->kids]->left;

                if(p->son==NULL){
                    v[p->kids]->right=v[p->kids];
                    v[p->kids]->left=v[p->kids];
                }
                else{
                    v[p->kids]->right=p->son;
                    v[p->kids]->left=p->son->left;
                    p->son->left->right=v[p->kids];
                    p->son->left=v[p->kids];
                    if(v[p->kids]->key<p->son->key)p->son=v[p->kids];
                }
                v[p->kids]=NULL;
                p->kids++;
            }
        }
    }
}
void delmin(){
    nod *ch,*p;
    del(h->key);
    build();
    ch=h;
    p=h->left;
    while(p!=h){
        if(p->key<ch->key)ch=p;
        p=p->left;
    }
    h=ch;
}
nod* merge_heap(nod* h1,nod* h2){
    h2->left->right=h1->left;
    h1->left->right=h2->left;
    h2->left=h1;
    h1->left=h2;
    if(h1->key>h2->key)return h2;
    else return h1;

}
int main()
{
    h=NULL;
    int nr,i,x,op;
    nod *p;
    fin>>nr;
    for(i=1;i<=nr;i++){
        fin>>op;
        if(op==1){
            fin>>x;
            p=new nod;
            p->key=x;
            ins(p);
        }
        else if(op==2){
            fin>>x;
            if(x==h->key)delmin();
            else del(x);
        }
        else if(op==3)fout<<h->key<<"\n";
        else delmin();
    }
    ///merge_heap este implementat dar nefolosit
    return 0;
}
