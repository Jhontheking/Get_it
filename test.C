#include <iostream>
#include <assert.h>//manejo de errores
#include <string>
#include <cstring>
#include <tuple>
#include <tpl_tree_node.H>
#include <htlist.H>
using namespace std;
typedef Tree_Node< tuple<string, bool> > Node;
#include "file_address_tree.H"

int main()
{
  file_address_tree * w=new file_address_tree(); //creacion del arbol directamente de home
  size_t n = 0;
  int i = 0,j=0;
  DynList<string> aux;

  ///busqueda
  int x=1;
while(x!=0){
string label;
  cout << "\nIntroduzca lo que desea buscar:  ";
  getline(cin,label);

 DynList<Node *> list= w->search_tree(label);
  cout<< "resultado de la busqueda"<<endl; 
    auto print_key = [] (Node * clave)
      {
        cout << " (" << get<0>(clave->get_data())<<" "<< get<1>(clave->get_data())<<")"<<endl;
      };
list.for_each(print_key);
cout << "\ndesea salir Introduzca 0 para continuar buscando ingresar cualquier otro numero:  ";
  cin>>x;
  cin.ignore();
}

cout << "Introduzca el número de etiquetas a almacenar en el árbol: ";
  cin >> n;
  cout << endl;
cin.ignore();
while(i < n)
  {
    
    i++;
    cout<<"insertar nombre de la etiqueta #"<< i <<": ";
    string name;
    getline(std::cin,name);
   // cin.getline();

    cout << "\n\tIntroduzca numero de archivos a etiquetar: ";
    size_t ne;
    cin >> ne;
    cin.ignore();
    while(j<ne)
    {
      j++;
      cout<<"insertar direccion #"<< j <<": ";
      string dir;
      getline(std::cin,dir);
     // cin.getline();
      aux.append(dir);
    }

    w->SetLabel(name,aux);
    aux.empty();
    j=0;
  }

  x=1;
while(x!=0){
string label;
  cout << "\nIntroduzca lo que desea buscar:  ";
  getline(cin,label);

 DynList<Node *> list= w->search_tree(label);
  cout<< "resultado de la busqueda"<<endl; 
    auto print_key = [] (Node * clave)
      {
        cout << " (" << get<0>(clave->get_data())<<" "<< get<1>(clave->get_data())<<")"<<endl;
      };
list.for_each(print_key);
cout << "\ndesea salir Introduzca 0 para continuar buscando ingresar cualquier otro numero:  ";
  cin>>x;
  cin.ignore();
}

  //cout << "\nárbol final: " << endl;

  //w->printf_tree(w->Get_root());
  //cout << endl;
  return 0;
}