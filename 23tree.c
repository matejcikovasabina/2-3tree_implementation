#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node {
    int value1, value2, middlevalue;
    struct node *leftchild, *leftmiddle, *rightmiddle, *rightchild, *parent;
    int hadsecond;
} NODE;

void siblingborrow2node(NODE *node, NODE **root);
NODE * parentborrow2node(NODE *node, NODE **root);
void parentborrow3node(NODE *node, NODE **root);
void siblingborrow3node(NODE *node, NODE **root);
void basicdelete(NODE *found, int value, NODE **root);
NODE *createNode(int value);
NODE* twowayssplit (NODE *current);
void values(NODE *current, int value);
void split(NODE *current, NODE **root);
void findposition(NODE **root, NODE *current, int value, int *first);
void insertion(NODE **root, int value);
NODE * search(int num, NODE *root);
void replace(NODE *found, int replacement, int value);
NODE * nearest(NODE *current);
void show(NODE *current, int depth);

NODE *createNode(int value) {
    NODE *new = (NODE*)malloc(sizeof(NODE));
    new->hadsecond = 0;
    new->leftchild = NULL;
    new->leftmiddle = NULL;
    new->parent = NULL;
    new->rightchild = NULL;
    new->rightmiddle = NULL;
    new->value1 = value;
    new->value2 = 0;
    new->middlevalue = 0;
    return new;
}

NODE* twowayssplit (NODE *current){
    NODE *left = createNode(current->value1);
    NODE *right = createNode(current->value2); 
    NODE *parent =createNode(current->middlevalue);
    parent->leftchild = left;
    parent->rightchild = right;
    left->parent = parent;
    right->parent = parent;
    left->leftchild = current->leftchild;
    left->rightchild = current->leftmiddle;
    right->leftchild = current->rightmiddle;
    right->rightchild = current->rightchild;
    if(left->leftchild) left->leftchild->parent = left;
    if(left->rightchild)left->rightchild->parent =left;
    if(right->rightchild)right->leftchild->parent = right;
    if(right->rightchild)right->rightchild->parent = right;
    
    return parent;
}

void values(NODE *current, int value){
     if(current->value2 && current->value1){
        if(value > current->value1 && value < current->value2){
            current->middlevalue = value;
        }else if(value < current->value1){
            current->middlevalue = current->value1;
            current->value1 = value;
        }else{
            current->middlevalue = current->value2;
            current->value2 = value;
        }
    }else{
        if(value > current->value1){
            current->value2 = value;
        }else{
            current->value2 = current->value1;
            current->value1 = value;
        }
    }
}

void split(NODE *current, NODE **root) {
    
    if (current->parent == NULL) {
        NODE *new = twowayssplit(current);
        *root = new;  
    } else {
        if (current->parent->hadsecond == 0) {
            NODE *new = twowayssplit(current);
            if(current->parent->leftchild == current){
                values(current->parent, new->value1);
                current->parent->leftmiddle = new->rightchild;
                current->parent->leftchild = new->leftchild;
                new->leftchild->parent = current->parent;
                new->rightchild->parent = current->parent;
                current->parent->hadsecond = 1;
                free(new);
            }
            else if(current->parent->rightchild == current){
                values(current->parent, new->value1);
                current->parent->leftmiddle = new->leftchild;
                current->parent->rightchild = new->rightchild;
                new->leftchild->parent = current->parent;
                new->rightchild->parent = current->parent;
                current->parent->hadsecond = 1;
                //printf("%d %d %d", current->value1, current->middlevalue, current->value2);
                free(new);
            }
        } else {
            NODE *new = twowayssplit(current);
            if(current->parent->leftchild == current){
                    values(current->parent, new->value1);
                    current->parent->leftchild = new->leftchild;
                    current->parent->rightmiddle = current->parent->leftmiddle;
                    current->parent->leftmiddle = new->rightchild;
                    new->leftchild->parent = current->parent;
                    new->rightchild->parent = current->parent;
                    
                    
            }
            else if(current->parent->rightchild == current){
                    values(current->parent, new->value1);
                    current->parent->rightchild = new->rightchild;
                    current->parent->rightmiddle = new->leftchild;
                    new->leftchild->parent = current->parent;
                    new->rightchild->parent = current->parent;
                    
            }
            else if(current->parent->leftmiddle == current){
                    values(current->parent, new->value1);
                    current->parent->rightmiddle = new->rightchild;
                    current->parent->leftmiddle = new->leftchild;   
                    new->rightchild->parent = current->parent;
                    new->leftchild->parent = current->parent;
            }
            split(current->parent, root);
        }
    }
}


void findposition(NODE **root, NODE *current, int value, int *first) {
    if (*first == 1) {
        current = *root;
        *first = 0; 
    }
    
    if (value < current->value1) {
        if (current->leftchild) findposition(root, current->leftchild, value, first);
        else {
           
            if (!current->hadsecond) {
                
                current->value2 = current->value1;
                current->value1 = value;
                current->hadsecond = 1;
            } else {
                current->middlevalue = current->value1;
                current->value1 = value;
                split(current, root);
            }
        }
    } else if (current->hadsecond && value > current->value1 && value < current->value2) {
        if (current->leftmiddle) findposition(root, current->leftmiddle, value, first);
        else {
            current->middlevalue = value;
            split(current, root);
        }
    } else {
        if (current->rightchild) findposition(root, current->rightchild, value, first);
        else {
            if (!current->hadsecond) {
                current->value2 = value;
                current->hadsecond = 1;
            } else {
                if(current->value2 > value){
                    current->middlevalue = value;
                }
                else{
                    current->middlevalue = current->value2;
                    current->value2 = value;
                }
                split(current, root);
            }
        }
    }
}

void insertion(NODE **root, int value) {
    if (!*root) {
        *root = createNode(value);
        return;
    }
    int first = 1;
    findposition(root, *root, value, &first);
}

NODE * search(int num, NODE *root) {
    if (root == NULL) {
        return NULL;
    }

    if (root->value1 == num || (root->hadsecond && root->value2 == num)) {
        return root;
    }

    if (num < root->value1) {
        return search(num, root->leftchild);
    } 
    else if (root->hadsecond && num > root->value2) {
        return search(num, root->rightchild);
    } 
    else if (root->hadsecond && num > root->value1 && num < root->value2) {
        return search(num, root->leftmiddle);
    }
    else {
        return search(num, root->rightchild);  
    }
}

NODE * nearest(NODE *current) {
    while (current->rightchild != NULL) {
        current = current->rightchild;
    }
    return current;
}

void replace(NODE *found, int replacement, int value){
    if(found->value1 == value){
        found->value1 = replacement;   
    }
    else if(found->value2 == value){
        found->value2 = replacement;
    }
}

void siblingborrow2node(NODE *node, NODE **root) {
    if (node == NULL) return; 

    if (node->leftchild != NULL && node->leftchild->value1 == -1) {
        if (node->rightchild != NULL && node->rightchild->hadsecond) {
            node->leftchild->value1 = node->value1;
            node->value1 = node->rightchild->value1;
            node->rightchild->value1 = node->rightchild->value2;
            node->rightchild->value2 = 0;
            node->rightchild->hadsecond = 0;

            if (node->leftchild->leftmiddle){
                node->leftchild->leftchild = node->leftchild->leftmiddle;
                node->leftchild->leftmiddle = NULL;
            } 
            if (node->rightchild->leftchild) node->leftchild->rightchild = node->rightchild->leftchild;
            if (node->leftchild->rightchild) node->leftchild->rightchild->parent = node->leftchild;
            if (node->rightchild->leftmiddle) {
                node->rightchild->leftchild = node->rightchild->leftmiddle;
                node->rightchild->leftmiddle = NULL;
            }
        }
    } else {
        if (node->rightchild != NULL && node->rightchild->value1 == -1) {
            if (node->leftmiddle != NULL) {
                node->rightchild->value1 = node->value1;
                node->value1 = node->leftchild->value2;
                node->leftchild->value2 = 0;
                node->leftchild->hadsecond = 0;

                if (node->rightchild->leftmiddle) node->rightchild->rightchild = node->rightchild->leftmiddle;
                node->rightchild->leftmiddle = NULL;
                if (node->rightchild->rightchild) node->rightchild->leftchild = node->leftchild->rightchild;
                node->rightchild->leftchild->parent = node->rightchild;
                if (node->rightchild->leftmiddle) {
                    node->leftchild->rightchild = node->leftchild->leftmiddle;
                    node->leftchild->leftmiddle = NULL;
                }
            }
        }
    }
}


NODE *parentborrow2node(NODE *node, NODE **root) {
    if (node == NULL) {
        printf("Error: Parent node is NULL.\n");
        return NULL;  
    }

    

    
    if (node->leftchild != NULL && node->leftchild->value1 == -1) {
        if (node->rightchild != NULL && node->rightchild->hadsecond) {
            siblingborrow2node(node->leftchild, root);
        } else {
            if (node->rightchild != NULL) {
                
                if (node->rightchild->value1 != -1) {
                  
                    node->rightchild->value2 = node->rightchild->value1;
                    node->rightchild->value1 = node->value1;
                    node->rightchild->hadsecond = 1;

                    if (node->rightchild->leftchild) node->rightchild->leftmiddle = node->rightchild->leftchild;
                    if (node->leftchild->leftmiddle) node->rightchild->leftchild = node->leftchild->leftmiddle;
                    if (node->rightchild->leftchild) node->rightchild->leftchild->parent = node->rightchild;

                    node->leftmiddle = node->rightchild;
                   
                    node->leftchild = NULL;
                    node->rightchild = NULL;
                    node->value1 = -1;
                    
                } else {
               
                    return NULL; 
                }
            }
        }
    }

    else if (node->rightchild != NULL && node->rightchild->value1 == -1) {
        if (node->leftchild != NULL && node->leftchild->hadsecond) {
            siblingborrow2node(node->rightchild, root);
        } else {
            if (node->leftchild != NULL) {
                node->leftchild->value2 = node->value1;
                if(node->leftchild->rightchild)node->leftchild->leftmiddle = node->leftchild->rightchild;
                if(node->rightchild->leftmiddle)node->leftchild->rightchild =  node->rightchild->leftmiddle;
                if(node->leftchild->rightchild)node->leftchild->rightchild->parent = node->leftchild;
                node->leftchild->hadsecond = 1;
                node->leftmiddle = node->leftchild;
                node->leftchild = NULL;
                node->rightchild = NULL;
                node->value1 = -1;
                
            }
            
        }
    }


    if (node->parent == NULL) {
        if (node->leftmiddle != NULL) {
            *root = node->leftmiddle;
            node->leftmiddle->parent = NULL;
           
            return node->leftmiddle;
        } else {
            return NULL;
        }
    } 
    if(node->value1 == -1) {
        if(node->parent->hadsecond){
            if(node->parent->leftchild == node){
                if(node->parent->leftmiddle->hadsecond){
                    siblingborrow3node(node->parent, root);
                }
                else{
                    parentborrow3node(node->parent, root);
                }
            }
            else if(node->parent->leftmiddle == node){
                if(node->parent->leftchild->hadsecond || node->parent->rightchild->hadsecond){
                    siblingborrow3node(node->parent,root);
                }
                else{
                    parentborrow3node(node->parent,root);
                }
            }
            else if(node->parent->rightchild == node){
                if(node->parent->leftmiddle->hadsecond){
                    siblingborrow3node(node->parent,root);
                }
                else{
                    parentborrow3node(node->parent, root);
                }
            }

        }
        else{
            if(node->parent->leftchild == node){
                if(node->parent->rightchild->hadsecond){
                    siblingborrow2node(node->parent,root);
                }
                else{
                    return parentborrow2node(node->parent, root);
                }
            }
            if(node->parent->rightchild == node){
                if(node->parent->leftchild->hadsecond){
                    siblingborrow2node(node->parent, root);
                }
                else{
                    return parentborrow2node(node->parent, root);
                }
            }
        }
        
    }
    return NULL;
}

void parentborrow3node(NODE *node, NODE **root) {
    if (node == NULL) return; 

  
    if (node->leftchild != NULL && node->leftchild->value1 == -1) {
        if (node->leftmiddle != NULL) {
            node->leftmiddle->value2 = node->leftmiddle->value1;
            node->leftmiddle->value1 = node->value1;
            node->leftmiddle->hadsecond = 1;
            node->value1 = node->value2;
            
            node->value2 = 0;
            node->hadsecond = 0;

            if (node->leftmiddle->leftchild) node->leftmiddle->leftmiddle = node->leftmiddle->leftchild;
            if (node->leftchild->leftmiddle) node->leftmiddle->leftchild = node->leftchild->leftmiddle;
            node->leftchild = node->leftmiddle;
            node->leftmiddle = NULL;
            if(node->leftchild->leftchild)node->leftchild->leftchild->parent = node->leftchild;
        }
    }

    else if (node->leftmiddle != NULL && node->leftmiddle->value1 == -1) {
        if (node->leftchild != NULL) {
            node->leftchild->value2 = node->value1;
            node->leftchild->hadsecond = 1;
            node->value1 = node->value2;
            node->value2 = 0;
            node->hadsecond = 0;
        

            if (node->leftchild->rightchild) node->leftchild->leftmiddle = node->leftchild->rightchild;
            if (node->leftmiddle != NULL) node->leftchild->rightchild = node->leftmiddle->leftmiddle;
            if(node->leftchild->rightchild)node->leftchild->rightchild->parent = node->leftchild;
            
            node->leftmiddle = NULL;
            if(node->parent == NULL){
                *root = node;
            }
        }
    }

    else {
        if (node->rightchild != NULL && node->rightchild->value1 == -1) {
            if (node->leftmiddle != NULL) {
                node->leftmiddle->value2 = node->value2;
                node->leftmiddle->hadsecond = 1;
                node->value2 = 0;
                node->hadsecond = 0;

                if (node->leftmiddle->rightchild) node->leftmiddle->leftmiddle = node->leftmiddle->rightchild;
                if (node->rightchild->leftmiddle) node->leftmiddle->rightchild = node->rightchild->leftmiddle;
                node->rightchild = node->leftmiddle;
                node->leftmiddle = NULL;
                if(node->rightchild->rightchild)node->rightchild->rightchild->parent = node->rightchild;
            }
        }
    }
}


void siblingborrow3node(NODE *node, NODE **root) {
    if (node == NULL) return; 

    if (node->leftchild != NULL && node->leftchild->value1 == -1) {
        if (node->leftmiddle != NULL && node->leftmiddle->hadsecond) {
            node->leftchild->value1 = node->value1;
            node->value1 = node->leftmiddle->value1;
            node->leftmiddle->value1 = node->leftmiddle->value2;
            node->leftmiddle->hadsecond = 0;

            if (node->leftchild->leftmiddle) node->leftchild->leftchild = node->leftchild->leftmiddle;
            if (node->leftmiddle->leftchild) node->leftchild->rightchild = node->leftmiddle->leftchild;
            if (node->leftmiddle->leftmiddle) {
                node->leftmiddle->leftchild = node->leftmiddle->leftmiddle;
                node->leftmiddle->leftmiddle = NULL;
            }
            if(node->leftchild->rightchild)node->leftchild->rightchild->parent = node->leftchild;
        } else {
            parentborrow3node(node, root);
        }
    } else if (node->rightchild != NULL && node->rightchild->value1 == -1) {
        if (node->leftmiddle != NULL && node->leftmiddle->hadsecond) {
            node->rightchild->value1 = node->value2;
            node->value2 = node->leftmiddle->value2;
            node->leftmiddle->hadsecond = 0;
            node->leftmiddle->value2 = 0;

            if (node->rightchild->leftmiddle) node->rightchild->rightchild = node->rightchild->leftmiddle;
            node->rightchild->leftmiddle = NULL;
            if (node->leftmiddle->rightchild) node->rightchild->leftchild = node->leftmiddle->rightchild;
            if(node->rightchild->leftchild)node->rightchild->leftchild->parent = node->rightchild;
            if (node->leftmiddle->leftmiddle) {
                node->leftmiddle->rightchild = node->leftmiddle->leftmiddle;
                node->leftmiddle->leftmiddle = NULL;
            }
        } else {
            parentborrow3node(node, root);
        }
    } else {
        if (node->leftchild != NULL && node->leftchild->hadsecond) {
            node->leftmiddle->value1 = node->value1;
            node->value1 = node->leftchild->value2;
            node->leftchild->value2 = 0;
            node->leftchild->hadsecond = 0;

            if (node->leftmiddle->leftmiddle) node->leftmiddle->rightchild = node->leftmiddle->leftmiddle;
            if (node->leftchild->rightchild) node->leftmiddle->rightchild = node->leftchild->rightchild;
            if (node->leftchild->leftmiddle) {
                node->leftchild->rightchild = node->leftchild->leftmiddle;
                node->leftchild->leftmiddle = NULL;
            }
            if(node->leftmiddle->leftchild)node->leftmiddle->leftchild->parent = node->leftmiddle;
        } else if (node->rightchild != NULL && node->rightchild->hadsecond) {
            node->leftmiddle->value1 = node->value2;
            node->value2 = node->rightchild->value1;
            node->rightchild->value1 = node->rightchild->value2;
            node->rightchild->value2 = 0;
            node->rightchild->hadsecond = 0;

            if (node->leftmiddle->leftmiddle) node->leftmiddle->leftchild = node->leftmiddle->leftmiddle;
            if (node->rightchild->leftchild) node->leftmiddle->rightchild = node->rightchild->leftchild;
            node->rightchild->leftchild = node->rightchild->leftmiddle;
            node->rightchild->leftmiddle = NULL;
            if(node->leftchild->rightchild)node->leftchild->rightchild->parent = node->leftchild;
        } else {
            parentborrow3node(node, root);
        }
    }
}


void basicdelete(NODE *found, int value, NODE **root) {
    if (found == NULL) {

        return; 
    }

   

   
    if (found->hadsecond) {
        if (found->value2 == -1) {
            found->value2 = 0;
            found->hadsecond = 0;
            
        } else {
            found->value1 = found->value2;
            found->value2 = 0;
            found->hadsecond = 0;
           
        }
    } else {
        found->value1 = -1;


       
        if (found->parent != NULL) {
            if(found->parent->hadsecond == 0){
                if (found->parent->leftchild == found) {
                    if (found->parent->rightchild->hadsecond) {
                        siblingborrow2node(found->parent, root);
                    } else {
                        parentborrow2node(found->parent, root);
                    }
                } else {
                    if (found->parent->leftchild->hadsecond) {
                        siblingborrow2node(found->parent, root);
                    } else {
                        parentborrow2node(found->parent, root);
                    }
                }
            }
        
            else{
                siblingborrow3node(found->parent, root);
            }
        }
    }
}


void deletion(NODE **root, int value) {
    if (root == NULL || *root == NULL) {
 
        return;  
    }

    NODE *found = search(value, *root);
    if (found == NULL) {
      
        return; 
    }



    NODE *near = NULL;
    int replacement;

    if (found->value1 == value) {
        if (found->leftchild != NULL) {
            near = nearest(found->leftchild);
            
            if (near->hadsecond == 0) {
                replacement = near->value1;
                near->value1 = -1;
            } else {
                replacement = near->value2;
                near->value2 = -1;
            }
            replace(found, replacement, value);
        } else {
            near = found;
        }
        basicdelete(near, value, root);
    } else if (found->value2 == value) {
        if (found->rightchild != NULL) {
            near = nearest(found->rightchild);
            replacement = near->value1;
            if (near->hadsecond == 0) {
                near->value1 = -1;
            } else {
                near->value1 = near->value2;
                near->value2 = 0;
                near->hadsecond = 0;
            }
            replace(found, replacement, value);
        } else {
            found->value2 = -1;
            near = found;
        }
        basicdelete(near, value, root);
    }
}

int main() {
    int command, value;
    NODE *root = NULL;
    int hodnota =1;
    int *first = &hodnota;

    FILE *file = fopen("generator.txt", "r"); 
    if (file == NULL) {
        perror("Chyba pri otváraní súboru");
        return 1;
    }

    FILE *output = fopen("graph.txt", "a");
    if (output == NULL) {
        perror("Chyba pri otváraní súboru");
        return 1;
    }

    char funkcia[10];  
    int cislo;

    clock_t start_time = clock();

    while (fscanf(file, "%s %d", funkcia, &cislo) != EOF) {
     
        if (strcmp(funkcia, "insert") == 0) {
            insertion(&root, cislo);
    
        } else if (strcmp(funkcia, "delete") == 0) {
            deletion(&root, cislo);
   
        } else if (strcmp(funkcia, "search") == 0) {
            NODE *found =search(cislo, root);
            if(found!=NULL){
                if(found->value1 == cislo){
                    //printf("Found %d \n", cislo);
                }
                else if(found->value2 == cislo){
                    //printf("Found %d \n", cislo);
                }
            }
            else{
                //printf("%d not found \n", cislo);
            }
            
        } else {
            return 0;
        }
    }
    //show(root,0);

    fclose(file);
    free(root);
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(output, "23 -  %.6f\n", time_spent);
    fclose(output);
    //printf("Program bežal %.6f sekúnd.\n", time_spent);
    return 0;
}
