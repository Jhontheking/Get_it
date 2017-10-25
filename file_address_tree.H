#ifndef file_address_tree_H
#define file_address_tree_H
#include <iostream>
//#include <sys/types.h> //manejo de archivos y carpetasmmm
//#include "/usr/include/i386-linux-gnu/sys/stat.h" ////manejo de archivos y carpetas
#include <assert.h>//manejo de errores
#include <string>
#include <cstring>
#include <tuple>
#include <tpl_tree_node.H>
#include <htlist.H>
#include <tpl_dynSetTree.H>
#include <tpl_binTree.H>
using namespace std;
/*el tipo del dato del nodo lo defini como tupla debido a que la segunda posicion de esta que es de 
tipo booleano me dice si el nodo en el que estoy es etiqueta (1) o archivo (0); hay algunos mensajes de error que estoy enviando 
en algunos metodo los cuales hay que discutir despues por si de casualidad mandan a buscar algo que no existe y el programa se pare
cualquier duda, o sugerencia de mejora o cualquier comentario de cualquier indole enviar por el grupo */
typedef Tree_Node< tuple<string, bool> > Node;
typedef tuple<string, bool> Par;
struct CompararNode
{
  bool operator () (Par p1, Par p2)
  {
    if (get<0>(p1).compare(get<0>(p2)) < 0 )
			return true;

		if (get<0>(p1).compare(get<0>(p2)) > 0)
			return false;

		return  (get<1>(p1), get<1>(p2));
  }
 };
class file_address_tree
{
private:
	Node * root;
public:
file_address_tree(Node *& root){this->Get_root()=root;}
~file_address_tree();
Node *& Get_root(){return this->root;}
void SetLabel(string aux,DynList<string> list)
{
 	auto found = this->Searh_Label(this->Get_root(), aux);
  if(found == nullptr && !list.is_empty())
  {
    Tree_Node < tuple<string, bool> > * root_child = new Node(make_tuple(aux, true));
    this->Get_root()->insert_leftmost_child(root_child);
    Tree_Node < tuple<string, bool> > * child = new Node(make_tuple(list.remove_first(), false));
    root_child->insert_leftmost_child(child);
    auto insert_child = [&root_child] (string key)
    {
    	Tree_Node < tuple<string, bool> > * sibling = new Node(make_tuple(key, false));
    	root_child->get_left_child ()->insert_right_sibling(sibling);
    };
     list.for_each(insert_child);
     list.empty();
     return;
  }else if (!list.is_empty())
  {
		auto insert_child = [&found] (string key)
    {
    	Tree_Node < tuple<string, bool> > * sibling = new Node(make_tuple(key, false));
    	found->get_left_child ()->insert_right_sibling(sibling);
    };
     list.for_each(insert_child);
     list.empty();
     return;   
  }
  return;
}
Node * Searh_Label(Node * &root, string key)
{
	auto tree_key = root -> get_data();

	if(get<0>(tree_key) == key && get<1>(tree_key) == true)
   return root;

  auto children = root -> children_nodes();

  for(auto c : children)
    if(Searh_Label(c, key) != nullptr)
     return Searh_Label(c, key);

  	return nullptr;
}
//interseccion entre nodos del arbol
DynList<Node *> intersection_tree(Node *& l,Node *& r)
{
	 DynSetTree<Par, BinTree, CompararNode> tree;
	 DynList<Node *> list;
	auto children = l -> children_nodes();
  if(children.size() == 0)
    throw underflow_error("empty children nodes");
  for(auto c: children)
  {
  	auto p_key = c -> get_data();
   tree.insert(p_key);
  }
  auto childrenr = r -> children_nodes();
  if(childrenr.size() == 0)
    throw underflow_error("empty children nodes");
  for(auto c: childrenr)
  {
  	auto p_keyy = c -> get_data();
    Par * aux1 = tree.insert(p_keyy);
      if (aux1 == nullptr)
      {
      	Tree_Node < tuple<string, bool> > * sibling = new Node(make_tuple(get<0>(p_keyy), get<1>(p_keyy)));
      	list.append(sibling);
      }
  }
/* auto print_node = [] (Par key)
      {
        cout << " (" << get<0>(key) << ", " << get<1>(key) << ")";
      };
      cout << "Inorden: \n\t";
  tree.for_each_inorder(print_node);
  cout << endl; paraver que estaba dando el arbol*/
return list;
}
DynList<Node *> search_tree_all(Node *& root ,string search)
{
	DynSetTree<Par, BinTree, CompararNode> tree;
	  DynList<Node *> list;
		auto children = root -> children_nodes();
		for(auto p : children)
    {
      auto p_key = p -> get_data();
      size_t found = get<0>(p_key).find(search);
      if(found!=string::npos)
      {
	       tree.insert(p_key);
      }
    }
    auto insert_list = [&list] (Par key)
      {
      	Tree_Node < tuple<string, bool> > * sibling = new Node(key);
        	list.append(sibling);
   			};

    tree.for_each_inorder(insert_list);
    return list;
}
/*
aqui tubimos unos problemas y por eso esta definida una busqueda de nivel uno y otra de nivel dos el padre seria nivel 0 y como el arbol solo crese a lo 
ancho y no en profundida su profundidad esta acotada en padre, etiquetas y archivos
*/
DynList<Node *> search_tree_all_lv_2(Node *& root ,string search)
{
	DynSetTree<Par, BinTree, CompararNode> tree;
	  DynList<Node *> list;
		auto children = root -> children_nodes();
		for(auto p : children)
    {
    	auto p_key = p -> children_nodes();
    	for(auto w : p_key)
    {
      auto p_keyw = w -> get_data();
      size_t found = get<0>(p_keyw).find(search);
      if(found!=string::npos)
      {
	       tree.insert(p_keyw);
      }
     }
    }
    auto insert_list = [&list] (Par key)
      {
      	Tree_Node < tuple<string, bool> > * sibling = new Node(key);
        	list.append(sibling);
   			};

    tree.for_each_inorder(insert_list);
    return list;
}
/*
aqui se definio por ahora los tipos de busqueda que creo tendra el arbol aunque mas adelante se discutira sobre ello
esta busqueda se define en 3 tipos la normal que buscara en todo el arbol un archivo, una que sera entre etiquetas la cual estara separada por coma y lo que hace sera 
buscar ambas etiquetas escritas y realizara una interseccion entre ellas retornandome solo las claves repetidas
y la que sera buscar un archivo en una etiqueta especifica cuyo separador sera el punto ejemplo etiqueta.archivo a buscar en la etiqueta.
*/
DynList<Node *> search_tree(string search)
{
	size_t found0 = search.rfind(",");
	size_t found1 = search.rfind(".");
	if (found0==string::npos && found1==string::npos)
	{
		return this->search_tree_all_lv_2(this->Get_root(),search);
	}else if (found0!=string::npos)
	{
		 string aux;
	 aux=search.substr(search.rfind(",")+1,search.length());
	 search.erase(search.rfind(","), search.length());
	 auto found2 = this->Searh_Label(this->Get_root(), aux);
	 auto found3 = this->Searh_Label(this->Get_root(), search);
	 if (found2!=nullptr && found3!=nullptr)
	 {
	 	return this->intersection_tree(found2,found3);
	 }
	 	throw underflow_error("node 1 or node 2 don't exist");
	}else if (found1!=string::npos)
	{
		string aux;
	 aux=search.substr(search.rfind(".")+1,search.length());
	 search.erase(search.rfind("."), search.length());
	 auto found2 = this->Searh_Label(this->Get_root(), search);
	 if (found2!=nullptr)
	 {
	 	return this->search_tree_all(found2,aux);
	 }
	}
}
/*
no se porque tengo problemas para la eliminacion de un hijo del arbol mas sin embargo pueden ir tratando de solucionarla yo mañana hablare con un prepa a ver
si el arbol lo estoy construyendo bien y para ver en donde libero toda la memoria que estoy reservando claramente sera en el destructor de la clase perotengo algunas 
dudas al respecto
void delete_node(string _delete)
{
	string aux;
	DynList<Node *> list;
	 aux=_delete.substr(_delete.rfind(".")+1,_delete.length());
	 _delete.erase(_delete.rfind("."), _delete.length());
	 auto found = this->Searh_Label(this->Get_root(), _delete);
	auto children = found -> children_nodes();
		for(auto p : children)
    {
      auto p_key = p -> get_data();
      size_t found = get<0>(p_key).find(aux);
      if(found!=string::npos)
      {
	       delete p;
	       p=nullptr;
      }
    }
	 return;
}*/
void printf_tree(Node * root)
{
	cout << "(" << get<0>(root -> get_data()) << ", " << get<1>(root -> get_data()) << ")";  
  auto children = root -> children_nodes();
  if(children.size() == 0)
    return;
  cout << "(";
  for(auto c: children)
  {
    printf_tree(c);
    cout << "-";
  }
  cout << ")";
}
/*
este es un metodo que detecta si una direccion que se le pase es un archivo o una carpeta en estos momentos no se esta utilizando ya que se implemento en una vercion 
anterior que se realizo y pues decidi dejarla por si acaso se va autilizar mas adelante ella necesita de estas cabeceras para funconar
//#include <sys/types.h> //manejo de archivos y carpetas
//#include "/usr/include/i386-linux-gnu/sys/stat.h" ////manejo de archivos y carpetas
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
}*/
};
#endif
