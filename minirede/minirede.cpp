#include "minirede.h"
#include <cstring>

using namespace std;

//chave
int funcaoHash(const char username[], int tamanhoTabela) {
    unsigned int hash = 0;
    for(int i = 0; username[i]!= '\0'; i++){
        hash= (hash * 31 + username[i]) % tamanhoTabela;
    }
    return hash; 
} 

// inserir na tabela(q negocio complicado q é hash em)
void inserirHash(NoLista* tabela[], int tamanhoTabela, usuario* novoUsuario){
    if(novoUsuario == nullptr)return;
    int indice = funcaoHash(novoUsuario->username, tamanhoTabela);

    NoLista* novoNo = new NoLista;
    novoNo->user = novoUsuario;
    novoNo->prox = tabela[indice];
    tabela[indice]= novoNo;
}

// busca do usuario
usuario* buscarHash(NoLista* tabela[], int tamanhoTabela, const char usernameBuscado[]){
    int indice = funcaoHash(usernameBuscado, tamanhoTabela);
    NoLista* atual = tabela[indice];

    while(atual != nullptr){
        if(strcmp(atual->user->username, usernameBuscado)==0){
            return atual->user;
        }

        atual = atual->prox;
    }
    return nullptr;
}

//esvazia os ponteiros
void inicializarMiniRede(MiniRede &rede)
{
    for(int i = 0; i< TAM_HASH; i++){
        rede.tabelaHash[i]= nullptr;
    }

    rede.raizArvore = nullptr;

}


//funcoes de arvore aqui:

 NoArvore* rotDir(NoArvore* y){
    if(y==nullptr || y->esq == nullptr)return y;

    NoArvore* u= y->esq;
    y->esq= u->dir;
    u->dir=y;
    y->FB= 0;
    u->FB= 0;
    return u;
 }

 NoArvore* rotEsq(NoArvore* y){
    if(y==nullptr || y->dir== nullptr)return y;

    NoArvore* u= y->dir;
    y->dir= u->esq;
    u->esq=y;
    y->FB= 0;
    u->FB= 0;
    return u;
 }

 NoArvore* rotEsqDir(NoArvore* a) {
    if(a==nullptr || a->esq==nullptr || a->esq->dir==nullptr)return a;

    NoArvore* filho = a->esq;
    NoArvore* neto = filho->dir;
    int FBNeto = neto->FB;

    a->esq = rotEsq(a->esq);
    NoArvore* novaRaiz = rotDir(a);

    if(FBNeto == -1){a->FB=1; filho->FB=0;}
    else if(FBNeto == 1){a->FB=0; filho->FB=-1;}
    else{a->FB=0; filho->FB=0;}
    novaRaiz->FB=0;

    return novaRaiz;
}

NoArvore* rotDirEsq(NoArvore* a) {
    if(a==nullptr || a->dir==nullptr || a->dir->esq== nullptr)return a;
   NoArvore* filho = a->dir;
    NoArvore* neto = filho->esq;
    int FBNeto = neto->FB;

    a->dir = rotDir(a->dir);
    NoArvore* novaRaiz = rotEsq(a);

    if(FBNeto == -1){a->FB=1; filho->FB=0;}
    else if(FBNeto == 1){a->FB=0; filho->FB=-1;}
    else{a->FB=0; filho->FB=0;}
    novaRaiz->FB=0;

    return novaRaiz;
}
// verifica rotacao 
NoArvore* casoEsquerda(NoArvore* a, bool& aumentouAltura) {
    if (a == nullptr || a->esq == nullptr) {
        aumentouAltura = false;
        return a;
    }
    NoArvore* z = a->esq;

    if (z->FB == 1) {
        a = rotDir(a);              
    } else {
        a = rotEsqDir(a);     
    }
    a->FB = 0;
    aumentouAltura = false;
    return a;
}

NoArvore* casoDireita(NoArvore* a, bool& aumentouAltura) {
    if (a == nullptr || a->dir == nullptr) {
        aumentouAltura = false;
        return a;
    }
    NoArvore* z = a->dir;

    if (z->FB == -1) {
        a = rotEsq(a);             
    } else {
        a = rotDirEsq(a);      
    }
    a->FB = 0;
    aumentouAltura = false;
    return a;
}

NoArvore* insereAVL(NoArvore* a, usuario* novoUsuario, bool& aumentouAltura){
    if(a==nullptr){
        NoArvore* novo= new NoArvore;
        novo->user = novoUsuario;
        novo->FB=0;
        novo->esq= nullptr;
        novo->dir= nullptr;
        aumentouAltura = true;
        return novo;
    }
    if(novoUsuario->id < a->user->id){
        a->esq = insereAVL(a->esq, novoUsuario, aumentouAltura);

        if(aumentouAltura){
            switch(a->FB){
                case -1:
                    a->FB=0;
                    aumentouAltura= false;
                    break;
                case 0:
                    a->FB= 1;
                    break;
                case 1:
                    a= casoEsquerda(a, aumentouAltura);
                    break;
            }
        }
    }

    else if(novoUsuario->id > a->user->id){
        a->dir = insereAVL(a->dir, novoUsuario, aumentouAltura);

        if(aumentouAltura){
            switch(a->FB){
                case 1:
                    a->FB=0;
                    aumentouAltura= false;
                    break;
                case 0:
                    a->FB= -1;
                    break;
                case -1:
                    a= casoDireita(a, aumentouAltura);
                    break;
            }
        }
    }
    else{ 
        aumentouAltura= false;
    }
    return a;
}

usuario* buscarArvore(NoArvore* no, int idBuscado){
    if(no==nullptr)return nullptr;
    if(no->user->id==idBuscado)return no->user;
    if(idBuscado < no->user->id)return buscarArvore(no->esq, idBuscado);
    return buscarArvore(no->dir, idBuscado);
}

void liberarMiniRede(MiniRede &rede)
{
    // TODO
}

void processarComandos(MiniRede &rede, std::istream &entrada, std::ostream &saida)
{
    // TODO: ler comandos da entrada padrao ate END.
    // Para cada comando, chamar a funcao correspondente.
    // Nao imprimir menu, prompt ou texto extra.
}

void cadastrarUsuario(MiniRede &rede, int id, const char username[], const char nomeCompleto[], std::ostream &saida)
{
    //verifica o username
    if(buscarHash(rede.tabelaHash, TAM_HASH, username) != nullptr){
        saida << "ERROR USER_EXISTS\n";
        return;
    }
    //verifica id
    if(buscarArvore(rede.raizArvore, id)!=nullptr){
        saida << "ERROR USER_EXISTS\n";
        return;
    }
    
    
    // inicialização do novo usuário
    usuario *novoUsuario = new usuario;

    // preenchimento da struct usuário
    novoUsuario->id = id;
    strncpy(novoUsuario->username, username, TAM_USERNAME - 1);
    novoUsuario->username[TAM_USERNAME - 1] = '\0';
    strncpy(novoUsuario->nome, nomeCompleto, TAM_NOME - 1);
    novoUsuario->nome[TAM_NOME - 1] = '\0';
   
    //lista de seguindo e seguidores
    novoUsuario->seguidores = nullptr;
    novoUsuario->seguindo = nullptr;
    
    //hash para username
    inserirHash(rede.tabelaHash, TAM_HASH, novoUsuario);

    //insere arvore de ids
    bool aumentouAltura=false;
    rede.raizArvore = insereAVL(rede.raizArvore, novoUsuario, aumentouAltura);

    saida << "USER_ADDED\n";
}

void buscarUsuarioPorId(MiniRede &rede, int id, std::ostream &saida)
{
    usuario* userEncontrado = buscarArvore(rede.raizArvore, id);
    if(userEncontrado != nullptr){
        saida << "USER " << userEncontrado->id << " " << userEncontrado->username << " " << userEncontrado->nome << "\n";
    }
    else{
        saida << "USER_NOT_FOUND\n";
    }
}

void buscarUsuarioPorUsername(MiniRede &rede, const char username[], std::ostream &saida)
{
    // busca na tabela hash
    usuario* userEncontrado = buscarHash(rede.tabelaHash, TAM_HASH, username);

    if(userEncontrado != nullptr){
        saida << "USER " << userEncontrado-> id << " " << userEncontrado->username << " " << userEncontrado->nome << "\n";
    }
    else{
        saida << "USER_NOT_FOUND\n";
    }
}

void listarUsuarios(MiniRede &rede, std::ostream &saida)
{
    // TODO
}

void seguirUsuario(MiniRede &rede, int idSeguidor, int idSeguido, std::ostream &saida)
{
    // TODO
}

void listarSeguindo(MiniRede &rede, int idUsuario, std::ostream &saida)
{
    // TODO
}

void cadastrarPublicacao(MiniRede &rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream &saida)
{
    // TODO
}

void curtirPublicacao(MiniRede &rede, int idUsuario, int idPost, std::ostream &saida)
{
    // TODO
}

void consultarNotificacoes(MiniRede &rede, int idUsuario, int k, std::ostream &saida)
{
    // TODO
}

void gerarFeed(MiniRede &rede, int idUsuario, int k, std::ostream &saida)
{
    // TODO
}

void listarTopPosts(MiniRede &rede, int k, std::ostream &saida)
{
    // TODO
}

int main()
{
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);

    return 0;
}