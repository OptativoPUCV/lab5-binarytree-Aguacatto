#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap *map = (TreeMap *)malloc(sizeof(TreeMap));
    if(map == NULL) return NULL;

    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if(tree == NULL) return;
    if(searchTreeMap(tree,key) != NULL) return;

    TreeNode *nuevoNodo = (TreeNode *)malloc(sizeof(TreeNode));
    if(nuevoNodo == NULL) return;
    Pair *nuevoPair = (Pair *)malloc(sizeof(Pair));
    if(nuevoPair == NULL) 
    {
        free(nuevoNodo);
        return;
    }
    nuevoNodo = createTreeNode(key, value);

    if(tree->root == NULL)
    {
        tree->root = nuevoNodo;
        tree->current = nuevoNodo;
        return;
    }

    TreeNode *aux = tree->root;
    TreeNode* parent = NULL;
    while(aux != NULL)
    {
        parent = aux;
        if(tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left;
        }
        else
        {
            aux = aux->right;
        }
    }

    nuevoNodo->parent = parent;
    if(tree->lower_than(key, parent->pair->key))
    {
        parent->left = nuevoNodo;
    }
    else
    {
        parent->right = nuevoNodo;
    }

    tree->current = nuevoNodo;
}

TreeNode * minimum(TreeNode * x)
{
    while(x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node)
{
    if(node == NULL) return;

    if(node->left == NULL && node->right == NULL)
    {
        if(node->parent != NULL)
        {
            if(node->parent->left == node)
            {
                node->parent->left = NULL;
            }
            else
            {
                node->parent->right = NULL;
            }
        }
        else
        {
            tree->root = NULL;
        }
        free(node->pair);
        free(node);
    }

    else if(node->left == NULL || node->right == NULL)
    {
        TreeNode *hijo = (node->left != NULL) ? node->left : node->right;
        if(node == tree->root)
        {
            tree->root = hijo;
            hijo->parent = NULL;
        }
        else
        {
            TreeNode *parent = node->parent;
            if(parent->left == node)
            {
                parent->left = hijo;
            }
            else
            {
                parent->right = hijo;
            }
            hijo->parent = parent;
        }
        free(node->pair);
        free(node);
    }
    else
    {
        TreeNode *sucesor = minimum(node->right);
        node->pair->key = sucesor->pair->key;
        node->pair->value = sucesor->pair->value;
        removeNode(tree, sucesor);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    TreeNode *aux = tree->root;

    while(aux != NULL)
    {
        if(is_equal(tree,key,aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }
        else
        {
            if(tree->lower_than(key,aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
    if(tree->root == NULL) return NULL;

    TreeNode *min = minimum(tree->root);

    tree->current = min;

    return min->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
    if(tree->current == NULL) return NULL;
    if(tree->current->right != NULL)
    {
        TreeNode *min = minimum(tree->current->right);
        tree->current = min;
        return min->pair;
    }
    else
    {
        TreeNode *aux = tree->current;
        while(aux->parent != NULL && aux->parent->right == aux)
        {
            aux = aux->parent;
        }
        tree->current = aux->parent;
        if(aux->parent != NULL)
        {
            return aux->parent->pair;
        }
        else
        {
            return NULL;
        }
    }
}
