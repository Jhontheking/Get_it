#ifndef file_address_tree_H
#define file_address_tree_H
#include <iostream>
#include <sys/types.h> //manejo de archivos y carpetas
#include "/usr/include/i386-linux-gnu/sys/stat.h" ////manejo de archivos y carpetas
#include <assert.h>//manejo de errores
#include <string>
#include <cstring>
#include <tuple>
#include <tpl_tree_node.H>
#include <htlist.H>
#include <tpl_dynSetTree.H>
#include <tpl_binTree.H>
using namespace std;
typedef Tree_Node< tuple<string, bool> > Node;
typedef tuple<string, bool> Par;
struct CompararNode
{
	//template <class T>
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
DynList<Node *> intersection_tree(Node *& l,Node *& r)
{
	 DynSetTree<Par, BinTree, CompararNode> tree;
	 DynList<Node *> list;
	auto children = l -> children_nodes();
  //if(children.size() == 0)
    //return;
  for(auto c: children)
  {
  	auto p_key = c -> get_data();
   tree.insert(p_key);
  }
  auto childrenr = r -> children_nodes();
  //if(childrenr.size() == 0)
    //return;
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
};
#endif