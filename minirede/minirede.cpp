#include "minirede.h"
#include <cstring>
#include <string>

using namespace std;

//passar arvore para lista e depois fazer mergesort para ordenar por numero de likes
void arvParaLista(NoArvorePosts* raiz, NoRanking *&inicio)
{
    if(raiz==nullptr)return;
    
    arvParaLista(raiz->esq, inicio);
    
    NoRanking* novo = new NoRanking;
    novo->post = raiz->publicacao;
    novo->prox = inicio;
    inicio = novo;
    
    arvParaLista(raiz->dir, inicio);
}

NoRanking *merge(NoRanking *esq, NoRanking *dir)
{
    if(esq == nullptr)return dir;
    if(dir== nullptr)return esq;

    NoRanking *res = nullptr;

    if(esq->post->curtidas > dir->post->curtidas || esq->post->curtidas == dir->post->curtidas && dir->post->id
    >esq->post->id){
        res = esq;
        res->prox = merge(esq->prox, dir);
    }
    
    else
    {
        res = dir;
        res->prox=merge(esq, dir->prox);
    }
    return res;
}

void Particao(NoRanking *fonte, NoRanking *&inicio, NoRanking *&fim)
{
    NoRanking *rapido;
    NoRanking *lento;

    lento = fonte;
    rapido = fonte->prox;

    while(rapido!= nullptr){
        rapido = rapido->prox;
        if(rapido != nullptr){
            lento = lento->prox;
            rapido= rapido->prox;
        }
    }
    inicio = fonte;
    fim = lento->prox;
    lento->prox = nullptr;
}

void mergeSort(NoRanking *&inicio)
{
    if(inicio == nullptr || inicio->prox == nullptr)return;

    NoRanking *a= nullptr;
    NoRanking *b= nullptr;

    Particao(inicio, a, b);

    mergeSort(a);
    mergeSort(b);
    inicio = merge(a, b);
}
// funcoes de fila de notificacoes
void addNotificacoes(Fila &notificacoes, int tipo, int idUser, int idPost)
{
    NoNotificacao *novo = new NoNotificacao{idPost, idUser, nullptr, tipo};

    if (notificacoes.fim != nullptr)
    {
        notificacoes.fim->prox = novo;
    }
    else
        notificacoes.inicio = novo;

    notificacoes.fim = novo;
}
// chave
int funcaoHash(const char username[], int tamanhoTabela)
{
    unsigned int hash = 0;
    for (int i = 0; username[i] != '\0'; i++)
    {
        hash = (hash * 31 + username[i]) % tamanhoTabela;
    }
    return hash;
}

// inserir na tabela(q negocio complicado q é hash em)
void inserirHash(NoLista *tabela[], int tamanhoTabela, usuario *novoUsuario)
{
    if (novoUsuario == nullptr)
        return;
    int indice = funcaoHash(novoUsuario->username, tamanhoTabela);

    NoLista *novoNo = new NoLista;
    novoNo->user = novoUsuario;
    novoNo->prox = tabela[indice];
    tabela[indice] = novoNo;
}

// busca do usuario
usuario *buscarHash(NoLista *tabela[], int tamanhoTabela, const char usernameBuscado[])
{
    int indice = funcaoHash(usernameBuscado, tamanhoTabela);
    NoLista *atual = tabela[indice];

    while (atual != nullptr)
    {
        if (strcmp(atual->user->username, usernameBuscado) == 0)
        {
            return atual->user;
        }

        atual = atual->prox;
    }
    return nullptr;
}

// esvazia os ponteiros
void inicializarMiniRede(MiniRede &rede)
{
    for (int i = 0; i < TAM_HASH; i++)
    {
        rede.tabelaHash[i] = nullptr;
    }

    rede.raizArvore = nullptr;
    rede.raizArvorePosts = nullptr;
}

// funcoes de arvore de usuários aqui:

NoArvore *rotDir(NoArvore *y)
{
    if (y == nullptr || y->esq == nullptr)
        return y;

    NoArvore *u = y->esq;
    y->esq = u->dir;
    u->dir = y;
    y->FB = 0;
    u->FB = 0;
    return u;
}

NoArvore *rotEsq(NoArvore *y)
{
    if (y == nullptr || y->dir == nullptr)
        return y;

    NoArvore *u = y->dir;
    y->dir = u->esq;
    u->esq = y;
    y->FB = 0;
    u->FB = 0;
    return u;
}

NoArvore *rotEsqDir(NoArvore *a)
{
    if (a == nullptr || a->esq == nullptr || a->esq->dir == nullptr)
        return a;

    NoArvore *filho = a->esq;
    NoArvore *neto = filho->dir;
    int FBNeto = neto->FB;

    a->esq = rotEsq(a->esq);
    NoArvore *novaRaiz = rotDir(a);

    if (FBNeto == -1)
    {
        a->FB = 1;
        filho->FB = 0;
    }
    else if (FBNeto == 1)
    {
        a->FB = 0;
        filho->FB = -1;
    }
    else
    {
        a->FB = 0;
        filho->FB = 0;
    }
    novaRaiz->FB = 0;

    return novaRaiz;
}

NoArvore *rotDirEsq(NoArvore *a)
{
    if (a == nullptr || a->dir == nullptr || a->dir->esq == nullptr)
        return a;
    NoArvore *filho = a->dir;
    NoArvore *neto = filho->esq;
    int FBNeto = neto->FB;

    a->dir = rotDir(a->dir);
    NoArvore *novaRaiz = rotEsq(a);

    if (FBNeto == -1)
    {
        a->FB = 1;
        filho->FB = 0;
    }
    else if (FBNeto == 1)
    {
        a->FB = 0;
        filho->FB = -1;
    }
    else
    {
        a->FB = 0;
        filho->FB = 0;
    }
    novaRaiz->FB = 0;

    return novaRaiz;
}
// verifica rotacao
NoArvore *casoEsquerda(NoArvore *a, bool &aumentouAltura)
{
    if (a == nullptr || a->esq == nullptr)
    {
        aumentouAltura = false;
        return a;
    }
    NoArvore *z = a->esq;

    if (z->FB == 1)
    {
        a = rotDir(a);
    }
    else
    {
        a = rotEsqDir(a);
    }
    a->FB = 0;
    aumentouAltura = false;
    return a;
}

NoArvore *casoDireita(NoArvore *a, bool &aumentouAltura)
{
    if (a == nullptr || a->dir == nullptr)
    {
        aumentouAltura = false;
        return a;
    }
    NoArvore *z = a->dir;

    if (z->FB == -1)
    {
        a = rotEsq(a);
    }
    else
    {
        a = rotDirEsq(a);
    }
    a->FB = 0;
    aumentouAltura = false;
    return a;
}

NoArvore *insereAVL(NoArvore *a, usuario *novoUsuario, bool &aumentouAltura)
{
    if (a == nullptr)
    {
        NoArvore *novo = new NoArvore;
        novo->user = novoUsuario;
        novo->FB = 0;
        novo->esq = nullptr;
        novo->dir = nullptr;
        aumentouAltura = true;
        return novo;
    }
    if (novoUsuario->id < a->user->id)
    {
        a->esq = insereAVL(a->esq, novoUsuario, aumentouAltura);

        if (aumentouAltura)
        {
            switch (a->FB)
            {
            case -1:
                a->FB = 0;
                aumentouAltura = false;
                break;
            case 0:
                a->FB = 1;
                break;
            case 1:
                a = casoEsquerda(a, aumentouAltura);
                break;
            }
        }
    }

    else if (novoUsuario->id > a->user->id)
    {
        a->dir = insereAVL(a->dir, novoUsuario, aumentouAltura);

        if (aumentouAltura)
        {
            switch (a->FB)
            {
            case 1:
                a->FB = 0;
                aumentouAltura = false;
                break;
            case 0:
                a->FB = -1;
                break;
            case -1:
                a = casoDireita(a, aumentouAltura);
                break;
            }
        }
    }
    else
    {
        aumentouAltura = false;
    }
    return a;
}

usuario *buscarArvore(NoArvore *no, int idBuscado)
{
    if (no == nullptr)
        return nullptr;
    if (no->user->id == idBuscado)
        return no->user;
    if (idBuscado < no->user->id)
        return buscarArvore(no->esq, idBuscado);
    return buscarArvore(no->dir, idBuscado);
}

// Funções de publicação a partir daqui: -------------------------------------------------------------------------

NoArvorePosts *rotDir(NoArvorePosts *y)
{
    if (y == nullptr || y->esq == nullptr)
        return y;

    NoArvorePosts *u = y->esq;
    y->esq = u->dir;
    u->dir = y;
    y->FB = 0;
    u->FB = 0;
    return u;
}

NoArvorePosts *rotEsq(NoArvorePosts *y)
{
    if (y == nullptr || y->dir == nullptr)
        return y;

    NoArvorePosts *u = y->dir;
    y->dir = u->esq;
    u->esq = y;
    y->FB = 0;
    u->FB = 0;
    return u;
}

NoArvorePosts *rotEsqDir(NoArvorePosts *a)
{
    if (a == nullptr || a->esq == nullptr || a->esq->dir == nullptr)
        return a;

    NoArvorePosts *filho = a->esq;
    NoArvorePosts *neto = filho->dir;
    int FBNeto = neto->FB;

    a->esq = rotEsq(a->esq);
    NoArvorePosts *novaRaiz = rotDir(a);

    if (FBNeto == -1)
    {
        a->FB = 1;
        filho->FB = 0;
    }
    else if (FBNeto == 1)
    {
        a->FB = 0;
        filho->FB = -1;
    }
    else
    {
        a->FB = 0;
        filho->FB = 0;
    }
    novaRaiz->FB = 0;

    return novaRaiz;
}

NoArvorePosts *rotDirEsq(NoArvorePosts *a)
{
    if (a == nullptr || a->dir == nullptr || a->dir->esq == nullptr)
        return a;
    NoArvorePosts *filho = a->dir;
    NoArvorePosts *neto = filho->esq;
    int FBNeto = neto->FB;

    a->dir = rotDir(a->dir);
    NoArvorePosts *novaRaiz = rotEsq(a);

    if (FBNeto == -1)
    {
        a->FB = 1;
        filho->FB = 0;
    }
    else if (FBNeto == 1)
    {
        a->FB = 0;
        filho->FB = -1;
    }
    else
    {
        a->FB = 0;
        filho->FB = 0;
    }
    novaRaiz->FB = 0;

    return novaRaiz;
}
// verifica rotacao
NoArvorePosts *casoEsquerda(NoArvorePosts *a, bool &aumentouAltura)
{
    if (a == nullptr || a->esq == nullptr)
    {
        aumentouAltura = false;
        return a;
    }
    NoArvorePosts *z = a->esq;

    if (z->FB == 1)
    {
        a = rotDir(a);
    }
    else
    {
        a = rotEsqDir(a);
    }
    a->FB = 0;
    aumentouAltura = false;
    return a;
}

NoArvorePosts *casoDireita(NoArvorePosts *a, bool &aumentouAltura)
{
    if (a == nullptr || a->dir == nullptr)
    {
        aumentouAltura = false;
        return a;
    }
    NoArvorePosts *z = a->dir;

    if (z->FB == -1)
    {
        a = rotEsq(a);
    }
    else
    {
        a = rotDirEsq(a);
    }
    a->FB = 0;
    aumentouAltura = false;
    return a;
}

NoArvorePosts *insereAVL(NoArvorePosts *a, Publicacao *novaPublicacao, bool &aumentouAltura)
{
    if (a == nullptr)
    {
        NoArvorePosts *novo = new NoArvorePosts;
        novo->publicacao = novaPublicacao;
        novo->FB = 0;
        novo->esq = nullptr;
        novo->dir = nullptr;
        aumentouAltura = true;
        return novo;
    }
    if (novaPublicacao->id < a->publicacao->id)
    {
        a->esq = insereAVL(a->esq, novaPublicacao, aumentouAltura);

        if (aumentouAltura)
        {
            switch (a->FB)
            {
            case -1:
                a->FB = 0;
                aumentouAltura = false;
                break;
            case 0:
                a->FB = 1;
                break;
            case 1:
                a = casoEsquerda(a, aumentouAltura);
                break;
            }
        }
    }

    else if (novaPublicacao->id > a->publicacao->id)
    {
        a->dir = insereAVL(a->dir, novaPublicacao, aumentouAltura);

        if (aumentouAltura)
        {
            switch (a->FB)
            {
            case 1:
                a->FB = 0;
                aumentouAltura = false;
                break;
            case 0:
                a->FB = -1;
                break;
            case -1:
                a = casoDireita(a, aumentouAltura);
                break;
            }
        }
    }
    else
    {
        aumentouAltura = false;
    }
    return a;
}

Publicacao *buscarArvorePost(NoArvorePosts *no, int idPost)
{
    if (no == nullptr)
        return nullptr;
    if (no->publicacao->id == idPost)
        return no->publicacao;
    if (idPost < no->publicacao->id)
        return buscarArvorePost(no->esq, idPost);
    return buscarArvorePost(no->dir, idPost);
}

// Funções de arvore de publicação encerram aqui ---------------------------------------------------------------------

// em ordem
void imprimeEmOrdem(NoArvore *No, ostream &saida)
{
    if (No == nullptr)
        return;

    imprimeEmOrdem(No->esq, saida);
    saida << "USER " << No->user->id << " " << No->user->username << " " << No->user->nome << "\n";
    imprimeEmOrdem(No->dir, saida);
}

// Funcoes para listas, insere ordenado por id
bool insereLista(NoLista *&inicio, usuario *n)
{
    if (inicio == nullptr || n->id < inicio->user->id)
    {
        NoLista *novo = new NoLista;
        novo->user = n;
        novo->prox = inicio;
        inicio = novo;
        return true;
    }

    if (inicio->user->id == n->id)
        return false;

    NoLista *atual = inicio;
    while (atual->prox != nullptr && atual->prox->user->id < n->id)
    {
        atual = atual->prox;
    }
    if (atual->prox->user->id == n->id && atual->prox != nullptr)
        return false;

    NoLista *novo = new NoLista;
    novo->user == n;
    novo->prox = atual->prox;
    atual->prox = novo;
    return true;
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
    string comando;
    while (entrada >> comando)
    {
        if (comando == "END")
        {
            break;
        }
        else if (comando == "ADD_USER")
        {
            int id;
            string username;
            string nome;
            entrada >> id >> username >> nome;
            cadastrarUsuario(rede, id, username.c_str(), nome.c_str(), saida);
        }
        else if (comando == "FIND_USER")
        {
            int id;
            entrada >> id;
            buscarUsuarioPorId(rede, id, saida);
        }
        else if (comando == "FIND_USERNAME")
        {
            string username;
            entrada >> username;
            buscarUsuarioPorUsername(rede, username.c_str(), saida);
        }
        else if (comando == "LIST_USERS")
        {
            listarUsuarios(rede, saida);
        }
        else if (comando == "FOLLOW")
        {
            int id1;
            int id2;
            entrada >> id1 >> id2;
            seguirUsuario(rede, id1, id2, saida);
        }
        else if (comando == "LIST_FOLLOWING")
        {
            int id;
            entrada >> id;
            listarSeguindo(rede, id, saida);
        }
        else if (comando == "GET_NOTIFICATIONS")
        {
            int id;
            int k;
            entrada >> id >> k;
            consultarNotificacoes(rede, id, k, saida);
        }

        else if (comando == "ADD_POST")
        {

            int idPost;
            int idAutor;
            int timestamp;
            string texto;
            entrada >> idPost >> idAutor >> timestamp >> texto;
            cadastrarPublicacao(rede, idPost, idAutor, timestamp, texto.c_str(), saida);
        }
        
        else if(comando == "LIKE")
        {
            int userId;
            int postId;
            entrada >> userId >> postId;
            curtirPublicacao(rede, userId, postId, saida); 
        }
        
        else if(comando == "TOP_POSTS")
        {
            int k;
            entrada >> k;
            listarTopPosts(rede, k, saida);
        }
    }
}

void cadastrarUsuario(MiniRede &rede, int id, const char username[], const char nomeCompleto[], std::ostream &saida)
{
    // verifica o username
    if (buscarHash(rede.tabelaHash, TAM_HASH, username) != nullptr)
    {
        saida << "ERROR USER_EXISTS\n";
        return;
    }
    // verifica id
    if (buscarArvore(rede.raizArvore, id) != nullptr)
    {
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

    // lista de seguindo
    novoUsuario->seguindo = nullptr;
    novoUsuario->notificacoes = new Fila;
    novoUsuario->notificacoes->inicio = nullptr;
    novoUsuario->notificacoes->fim = nullptr;
    novoUsuario->publicacoes = nullptr;

    // hash para username
    inserirHash(rede.tabelaHash, TAM_HASH, novoUsuario);

    // insere arvore de ids
    bool aumentouAltura = false;
    rede.raizArvore = insereAVL(rede.raizArvore, novoUsuario, aumentouAltura);

    saida << "USER_ADDED\n";
}

void buscarUsuarioPorId(MiniRede &rede, int id, std::ostream &saida)
{
    usuario *userEncontrado = buscarArvore(rede.raizArvore, id);
    if (userEncontrado != nullptr)
    {
        saida << "USER " << userEncontrado->id << " " << userEncontrado->username << " " << userEncontrado->nome << "\n";
    }
    else
    {
        saida << "USER_NOT_FOUND\n";
    }
}

void buscarUsuarioPorUsername(MiniRede &rede, const char username[], std::ostream &saida)
{
    // busca na tabela hash
    usuario *userEncontrado = buscarHash(rede.tabelaHash, TAM_HASH, username);

    if (userEncontrado != nullptr)
    {
        saida << "USER " << userEncontrado->id << " " << userEncontrado->username << " " << userEncontrado->nome << "\n";
    }
    else
    {
        saida << "USER_NOT_FOUND\n";
    }
}

void listarUsuarios(MiniRede &rede, std::ostream &saida)
{
    saida << "USERS_BEGIN\n";
    imprimeEmOrdem(rede.raizArvore, saida);
    saida << "USERS_END\n";
}

void seguirUsuario(MiniRede &rede, int idSeguidor, int idSeguido, std::ostream &saida)
{
    if (idSeguido == idSeguidor)
    {
        saida << "ERROR CANNOT_FOLLOW_SELF\n";
    }
    usuario *a = buscarArvore(rede.raizArvore, idSeguidor);
    usuario *b = buscarArvore(rede.raizArvore, idSeguido);

    if (a == nullptr || b == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }

    bool existem = insereLista(a->seguindo, b);
    if (existem)
    {
        saida << "FOLLOWED\n";
        addNotificacoes(*(b->notificacoes), 1, a->id, -1);
    }
    else
    {
        saida << "ERROR ALREADY_FOLLOWING\n";
    }
}

void listarSeguindo(MiniRede &rede, int idUsuario, std::ostream &saida)
{
    usuario *a = buscarArvore(rede.raizArvore, idUsuario);
    if (a == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }

    saida << "FOLLOWING_BEGIN\n";
    NoLista *atual = a->seguindo;
    while (atual != nullptr)
    {
        saida << "USER " << atual->user->id << " " << atual->user->username << " " << atual->user->nome << "\n";
        atual = atual->prox;
    }
    saida << "FOLLOWING_END\n";
}

void cadastrarPublicacao(MiniRede &rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream &saida)
{
    usuario *auxUser = buscarArvore(rede.raizArvore, idAutor);

    if (auxUser)
    {

        if (buscarArvorePost(rede.raizArvorePosts, idPost))
        {
            saida << "ERROR POST_EXISTS\n";
            return;
        }
        else
        {
            // Instanciação e preenchimento da publicação
            int curtidas = 0;
            Publicacao *publicacao = new Publicacao;

            publicacao->id = idPost;
            publicacao->idAutor = idAutor;
            publicacao->timestamp = timestamp;
            strncpy(publicacao->texto, texto, TAM_TEXTO - 1);
            publicacao->texto[TAM_TEXTO - 1] = '\0';
            publicacao->curtidas = curtidas;
            publicacao->usersLike = nullptr;

            // add na arvore de publicaçao
            bool aumentouAltura = false;
            rede.raizArvorePosts = insereAVL(rede.raizArvorePosts, publicacao, aumentouAltura);

            // add na lista do usuário
            noListPosts *no = new noListPosts;
            no->publicacao = publicacao;
            no->prox = nullptr;

            if (auxUser->publicacoes == nullptr || publicacao->timestamp > auxUser->publicacoes->publicacao->timestamp)
            {
                no->prox = auxUser->publicacoes;
                auxUser->publicacoes = no;
            }
            else
            {
                noListPosts *aux = auxUser->publicacoes;

                while (aux->prox != nullptr && aux->prox->publicacao->timestamp > publicacao->timestamp)
                {
                    aux = aux->prox;
                }
                no->prox=aux->prox;
                aux->prox = no;
            }

            saida << "POST_ADDED\n";
        }
    }
    else
    {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
}

void curtirPublicacao(MiniRede &rede, int idUsuario, int idPost, std::ostream &saida)
{
    usuario *user = buscarArvore(rede.raizArvore, idUsuario);
    if(user == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    Publicacao *post = buscarArvorePost(rede.raizArvorePosts, idPost);
    if(post == nullptr)
    {
        saida << "ERROR POST_NOT_FOUND\n";
        return;
    }
    
    NoLista *aux= post->usersLike;
    while(aux != nullptr)
    {
        if(aux->user == user){
            saida << "ERROR ALREADY_LIKED\n";
            return;
        }
        aux = aux->prox;
    }

    post->curtidas++;
    insereLista(post->usersLike, user);
    saida << "LIKED\n";
    usuario *autor = buscarArvore(rede.raizArvore, post->idAutor);
    addNotificacoes(*(autor->notificacoes), 2, user->id, post->id);
}

void consultarNotificacoes(MiniRede &rede, int idUsuario, int k, std::ostream &saida)
{
    usuario *user = buscarArvore(rede.raizArvore, idUsuario);
    if (user == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    saida << "NOTIFICATIONS_BEGIN\n";

    while (k > 0 && user->notificacoes->inicio != nullptr)
    {
        // dequeue
        NoNotificacao *temp = user->notificacoes->inicio;

        if (temp->tipo == 1)
        {
            saida << "NOTIFICATION FOLLOW " << temp->idUser << "\n";
        }
        else if (temp->tipo == 2)
        {
            saida << "NOTIFICATION LIKE " << temp->idUser << " " << temp->idPost << "\n";
        }

        user->notificacoes->inicio = user->notificacoes->inicio->prox;
        delete temp;
        k--;
    }
    if (user->notificacoes->inicio == nullptr)
        user->notificacoes->fim = nullptr;

    saida << "NOTIFICATIONS_END\n";
}

void gerarFeed(MiniRede &rede, int idUsuario, int k, std::ostream &saida)
{
    
}

void listarTopPosts(MiniRede &rede, int k, std::ostream &saida)
{
    NoRanking *Ranking = nullptr;

    arvParaLista(rede.raizArvorePosts, Ranking);

    mergeSort(Ranking);

    saida << "TOP_POSTS_BEGIN\n";
    NoRanking *atual = Ranking;
    while(atual != nullptr && k>0){
        saida<< "POST " << atual->post->id << " " << atual->post->idAutor << " " << atual->post->timestamp << " "
        << atual->post->curtidas << " " << atual->post->texto << "\n";

        k--;
        atual = atual->prox;
    }
    saida << "TOP_POSTS_END\n";
    atual = Ranking;
    while(atual != nullptr){
        NoRanking *aux = atual;
        atual = atual->prox;
        delete aux;
    }
}

int main()
{
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);

    return 0;
}