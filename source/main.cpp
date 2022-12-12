#include <bits/stdc++.h>
#include <iostream>
using namespace std;

class Node
{
public:
    int data;
    Node **next;

    Node(int _data, int _lvl){
        data = _data;
        next = new Node*[_lvl+1]; //asignamos memoria
        memset(next, 0, sizeof(Node*)*(_lvl+1)); // Llenamos con ceros
    }
};

class PriorityQueue
{
private:
    int MxLVL; // Maximo nivel dado por el usuario
    int lvl; // Maximo nivel que se obtiene aleatoriamente por nodo
    float P;

    Node *header;
    Node *tail;

public:
    PriorityQueue(int _maxLevel, float _semilla)
    {
        MxLVL = _maxLevel;
        P = _semilla;
        lvl = 0;
        // make header node and initialize key with -1
        header = new Node(-1, MxLVL);
        tail = new Node(-2, MxLVL);

        for(int i = 0; i < MxLVL; i++){
            header->next[i] = tail;
        }
    }

    int randomLvl()
    {
        float r = (float)rand()/RAND_MAX;
        int aux_lvl = 0;
        //cout << "Aleatorio: " << r << endl;
        while (r < P && aux_lvl < MxLVL) // crea aleatoriamente el nivel de cada nodo
        {
            aux_lvl++;
            r = (float)rand()/RAND_MAX;
            //cout << "Aleatorio2: " << r << endl;
        }

        cout << "EL NIVEL SERA: " << aux_lvl << endl;
        return aux_lvl;
    }

    Node* createNode(int key, int level)
    {
        Node *n = new Node(key, level);
        return n;
    }

    bool onlyFind(int numero){
        cout << "############################ Buscando " << numero << " ##############################" << endl;
        Node *current = header;

        // ------------- BUSQUEDA -------------------------------
        for (int i = lvl; i >= 0; i--)  // Busqueda en maximo nivel
        {
            if(current->next[i]->data == numero){
                cout << "El Elemento ya esta insertado. Se encuentra en el nivel " << i << endl;
                cout  << endl;
                return true;
            }

            while (current->next[i]->data != -2 && current->next[i]->data > numero){
                current = current->next[i];

                if(current->next[i]->data == numero){
                    cout << "El Elemento ya esta insertado. Se encuentra en el nivel " << i << endl;
                    return true;
                }
            }
        }

        cout << "El elemento no esta en la cola, puede ser insertado" << endl;
        return false;
    }

    bool findInsert(int numero, Node *current, Node *beforeList[], Node *afterList[]){
        cout << "######################## Buscando " << numero << " para insertar ##############################" << endl;
        current = header;

        // ------------- BUSQUEDA -------------------------------
        for (int i = lvl; i >= 0; i--)  // Busqueda en maximo nivel
        {
            if(current->next[i]->data == numero){
                cout << "El Elemento ya esta insertado. Se encuentra en el nivel " << i << endl;
                return true;
            }

            while (current->next[i]->data != -2 && current->next[i]->data > numero){
                current = current->next[i];
                //cout << "actual: " << current->data << endl;

                if(current->next[i]->data == numero){
                    cout << "El Elemento ya esta insertado. Se encuentra en el nivel " << i << endl;
                    return true;
                }
            }

            beforeList[i] = current;
            afterList[i] = current->next[i];
        }

        cout << "El elemento no esta en la cola" << endl;

        current = current->next[0];
        return false;
    }

    bool findDelete(int numero, Node *current, Node *beforeList[], Node *afterList[]){
        cout << "############################ Buscando " << numero << " para eliminar ##############################" << endl;
        current = header;

        // ------------- BUSQUEDA -------------------------------
        for (int i = lvl; i >= 0; i--)  // Busqueda en maximo nivel
        {
            while (current->next[i]->data != -2 && current->next[i]->data > numero){
                current = current->next[i];
            }
            //cout << "-----------------------" << endl;
            beforeList[i] = current;

            if(current->next[i]->data == -2){
                afterList[i] = current->next[i];
            } else{
            afterList[i] = current->next[i]->next[i];
            }
        }

        cout << "El elemento si esta en la cola" << endl;

        current = current->next[0];

        if(current->data == numero){
            return true;
        } else{
            return false;
        }

    }

    void insert(int k)
    {
        Node *current = header;

        Node *beforeList[MxLVL+1];
        memset(beforeList, 0, sizeof(Node*)*(MxLVL+1));

        Node *afterList[MxLVL+1];
        memset(afterList, 0, sizeof(Node*)*(MxLVL+1));

        bool isInQueue;
        isInQueue = findInsert(k, current, beforeList, afterList);

        if(current->data == -2 || !isInQueue)
        {
            int rlevel = randomLvl();
            Node* n = createNode(k, rlevel);

            if (rlevel > lvl) // El nivel del nuevo nodo es mayor al maximo actual en el queue?
            {
                //Actualizamos before y after pues los niveles adicionales no estan llenos
                for (int j = lvl+1 ; j < rlevel+1 ; j++){
                    //Las redireccionamos directamente con header y tail pues sabemos con certeza que es un nuevo enlace. y no hay nadie a sus costados
                    beforeList[j] = header;
                    afterList[j] = tail;
                }

                //actualizamos el mayor nivel del random
                lvl = rlevel;
            }

            for (int z = 0; z <= rlevel ; z++)
            {
                n->next[z] = afterList[z];
                beforeList[z]->next[z] = n;
            }
            cout << "Se inserto el elemento : " << k << "\n";
        }
        else{
            //cout << "El elemento ya se encuentra en la cola" << endl;
        }
    }

    void deleteData(int k)
    {
        Node *current = header;

        Node *beforeList[MxLVL+1];
        memset(beforeList, 0, sizeof(Node*)*(MxLVL+1));

        Node *afterList[MxLVL+1];
        memset(afterList, 0, sizeof(Node*)*(MxLVL+1));

        bool isInQueue;
        isInQueue = findDelete(k, current, beforeList, afterList);

        if(current->data == -2 || isInQueue)
        {
            //cout << "Verificando lvl" << endl;
            for(int i = lvl; i >= 0; i--){
                //cout << "lvl es: " << lvl << endl;
                Node *aux1 = beforeList[i];
                Node *aux2 = afterList[i];
                //cout << aux1->data << " | " << aux2->data << endl;
                if( aux1->data != -1 || aux2->data != -2 ){
                    break;
                }
                //cout << "Bajamos nivel" << endl;
                lvl--;
            }

            for (int z = 0; z <= lvl ; z++)
            {
                beforeList[z]->next[z] = afterList[z];
            }

            cout << "Se elimino el elemento: " << k << "\n";
        }
        else{
            //cout << "El elemento ya se encuentra en la cola" << endl;
        }
    }

    void displaySkipList()
    {
        cout << endl;
        cout<<" -------- Skip  List View -----------  " << endl;
        for (int i = 0; i <= lvl; i++)
        {
            Node *node = header->next[i];
            cout << "Level " << i << ": ";
            while (node->data != -2)
            {
                cout << node->data<<" ";
                node = node->next[i];
            }
            cout << endl;
        }
    }

    void displayPriorityQueue()
    {
        cout << endl;
        cout<<" -------- Priority Queue View -----------  " << endl;
        Node *aux = header->next[0];
        while (aux->data != -2)
        {
            cout << aux->data<<" ";
            aux = aux->next[0];
        }
        cout << endl;
    }

    void getPQ(){
        cout << "-----------------------------------------" << endl;
        cout << "Dato con mas prioridad: " << header->next[0]->data << endl;
        cout << "-----------------------------------------" << endl;
    }
};

int main()
{
    // Seed random number generator
    srand((unsigned)time(0));

    PriorityQueue l(3, 0.5);
    l.insert(26);
    l.insert(11);
    l.insert(30);
    l.insert(20);
    l.insert(13);
    l.insert(10);
    l.insert(2);
    l.insert(8);
    l.insert(22);
    l.insert(19);

    cout << endl;
    cout << endl;

    l.displaySkipList();
    l.displayPriorityQueue();
    l.getPQ();

    cout << endl;
    cout << endl;

    l.onlyFind(26);
    l.onlyFind(11);
    l.onlyFind(30);
    l.onlyFind(20);
    l.onlyFind(13);
    l.onlyFind(14);

    cout << endl;
    cout << endl;

    l.insert(2);
    l.insert(19);
    l.insert(30);

    cout << endl;
    cout << endl;

    l.deleteData(13);
    l.deleteData(30);

    cout << endl;
    cout << endl;

    l.displaySkipList();
    l.displayPriorityQueue();
    l.getPQ();
}