#include <iostream>
#include <sys/types.h> //manejo de archivos y carpetas
#include "/usr/include/i386-linux-gnu/sys/stat.h" ////manejo de archivos y carpetas
#include <assert.h>//manejo de errores
#include <string>
#include <cstring>
#include <tuple>
#include <tpl_tree_node.H>
#include <htlist.H>
using namespace std;
typedef Tree_Node< tuple<string, bool> > Node;
#include "file_address_tree.H"

bool path_is_file(char *);
void to_charp(char *&, const string &);
int main()
{
  size_t n = 0;
  int i = 0,j=0;
  DynList<string> aux;


  cout << "Introduzca el número de etiquetas a almacenar en el árbol: ";
  cin >> n;

  cout << endl;


  Node *root = new Node(make_tuple("/home/jesus/", true));
  file_address_tree * z=new file_address_tree(root);
  ///Inserción

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

    z->SetLabel(name,aux);
    aux.empty();
    j=0;
  }



  ///Búsqueda

  string label;
  cout << "\nIntroduzca etiqueta a buscar 1:  ";
  cin >> label;

  auto found = z->Searh_Label(z->Get_root(), label);
 cout << "\nIntroduzca etiqueta a buscar 1:  ";
  cin >> label;

  auto foundd = z->Searh_Label(z->Get_root(), label);


  if(found != nullptr && foundd != nullptr)
  {
   DynList<Node *> list= z->intersection_tree(found,foundd);
   cout<< "resultado de la interseccion"<<endl; 
    auto print_key = [] (Node * clave)
      {
        cout << " (" << get<0>(clave->get_data())<<" "<< get<1>(clave->get_data())<<")"<<endl;
      };
list.for_each(print_key);
    //cout << "Clave encontrada: " << get<0>(found -> get_key()) << endl;
    cout<<"el arbol"<<endl; 
    z->printf_tree(found);
    cout<<endl;
    z->printf_tree(foundd);
  }
  else
  {
    cout <<"Clave no encontrada" << endl;
  }







  cout << "\nárbol final: " << endl;

  z->printf_tree(z->Get_root());
  cout << endl;
  return 0;
}
bool path_is_file(char * path) 
{
 struct stat st_info; 
 assert(path != nullptr);
 if (stat(path, &st_info) < 0) 
 {
  return false;
 }
 return S_ISREG(st_info.st_mode);
}
void to_charp(char *& cstr, const string & linea)
{
 cstr = new char [linea.length()+1];
 strcpy (cstr, linea.c_str());
}