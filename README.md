# Vigenere Cipher

Task 1
=============
CreateTree:
----------

I create a new tree and allocate memory to it, I put the function or value given to each element in the structure as a parameter, I initialize the size with 0 and root with null.

IsEmpty:
-----------

Check if my tree doesn't have root (this is the first one)

Search:
--------

I construct the recursive function and traverse the tree according to the comparison between the element and the node with which I traverse (left or right), until I find the node with the searched element.

Minimum:
----------
This is the leftmost element of the left subtree (bottom)

Maximum:
----------
This is the largest element in the right subtree (scroll to NULL on the right)

Successor & Predecessor:
------------------------

These functions are very similar. There are 2 situations in each of these:

For the predecessor: if there is a subtree right from the node, then it is the minimum of that subtree, otherwise I go with the parent until I find the value lower than the node

Successor : if there is a left subtree of the node, I return the maximum value, otherwise I go with the parent until I find a value higher than the node

updateHeight:
------------

I calculate the height for the left and right tree and then I update the height according to which is the largest plus the root node

AvlRotateLeft:
--------------

I change the links between the nodes x and y and the child to the left of y.
I also link the connection between their parents. Then I take the case that there are no children, or x has no parent. 
I wonder if x is a left child or a right child, to link his parent to y.
At the end I update the height for x and y.

AvlRotateRight:
----------------
It's exactly the same algorithm but here x is the child on the left for y and we change the connection with the child on the right of x.

AvlGetBalance:
---------------
I check if that node exists and if it has a right or left child then I return the formula:

(height (left) - height (right))

AvlFixup:
----------
I check the balance starting from the parent of the node
Check if the parent is unbalanced on the right or left side
or if it is in a zig-zag shape and makes the necessary rotations

AvlDeleteFixup:
----------------
A function very similar to the one above but here I check the balance right from that node and I use the balance of the node (unbalanced subtree, right or left child)

CreateTreeNode:
----------------
functie deja creata

Insert:
--------------
Aici am inceput prin a crea nodul cu elementul si informatia specifica
Parcurg arborele pana gasesc pozitia unde ar trebui sa il inserez si totodata verific daca acesta nu e duplicat
In cazul in care lista e goala, doar inserez root si ii pun legatura de end tot lui
Altfel, ma uit daca inserez in partea dreapta sau partea stanga a frunzei sau daca deja exista un nod cu acelasi element
Daca nu exista, ii fac legatura cu parintele(frunza) si cu end.
Daca e duplicat, ii fac legaturile duble intre nodul precedent si nodul urmator(daca exista acestea) si mut legatura end la nodul inserat. 
Apoi pentru cazurile in care nu e duplicat fac legatura intre next si prev-ul fiecarui nod inserat(daca nu cumva e minim sau maxim, in cazul asta nu as putea fac o legatura din acestea).
Avand in vedere ca inserez si intr-o lista dublu inlantuita in ordine crescatoare,ma folosesc de succesor si predecesor.
Maresc size-ul arborelui si verific daca e acesta e balansat corect.

Delete:
----------
Aici folosesc si o functie ajutatoare "Link_node_parent"
Aceasta face legaturile intre parintele primului nod dat ca parametru si cel de-al doilea nod.
Verific, desigur, daca exista aceste noduri si daca nu e root primul din acestea.

Si incepem delete ...
Verific prima data daca nu cumva am ramas doar cu un nod fara copii si pur si simplu ii dau delete si ii scad sizeul

Altfel, cu ajutorul lui search, salvez nodul cautat si parintele acestuia(ma va ajuta pe viitor la balansare).
Iau cazul in care nodul specificat este dublicat si il sterg de la final, fac legatura noua cu end si intre nodurile din fata acestuia si urmatorul(next si prev, daca exista).
Fac mai jos legaturile intre prev si next pentru orice nod dat ca parametru cu ajutorul succesorul si predecesorului(daca exista).
Altfel, daca nodul cautat are sau nu copii, avem 3 cazuri(un copil(de dreapta sau stanga), 2 copii si niciun copil(nod frunza)).
Ma folosesc de functia ajutatoare pentru a inlocui astfel legaturile intre parintele nodului pe care trebuie sa il sterg cu acel copil gasit(daca exista).
Daca exista ambii, il inlocuiesc cu succesorul sau si aici trebuie sa schimb legaturile si intre copiii succesorului.
Daca nu are copii, doar ii pun Null legaturii parintelui acestui nod cautat
La final sterg elementul cu functia destroy, creata deja care elibereaza si memoria totodata, miscorez size-ul si balansez arborele, daca acesta are nevoie de asta

DestroyTree:
-------------
Incep de la finalul listei si sterg fiecare element pana ajung la inceput, ma folosesc de un pointer auxiliar ca sa dau free nodului curent.
Apoi eliberez memoria arborelui.

CERINTA 2: CIPHER
-----------------------------------------------------------------------------------
Aduc in acest fisier antetul unor functii din Tema2.c

BuildTreeFromFile:
------------------
Deschid fisierul cu numele dat ca parametru, aloc memorie pentru un char "linie" pe care il citesc din fisier. Ma folosesc de while(fgets) ca sa citesc pana la finalul fisierului, apoi cu strtok iau fiecare cuvant din linia citita. Imi creez un element cu functia createStrElement(aceasta trunhiaza automat cuvintele la primele 5 litere). De asemenea, imi declar o variabila in care salvez lungimea fiecarui cuvintelor citite pana la final, acesta e transmis ca informatie in insert
Inserez nodul cu elementul si lungimea specifica, actualizez lungimea, distrug elementul si fac iarasi strtok ca sa iau urmatorul nod
Inchid fisierul si eliberez memorie pentru "linia citita"(pointerul la char alocat).

CERINTA 3:
-----------------------------------------------------------------------------------
inorderKeyQuery:
------------------
aloc memorie pentru un nou range
Ii pun sizeul initial 0 si capacitatea size-ul arborelui(puteam pune si alte valori insa aceasta cred ca este cea mai eficienta, pentru a nu realoca in alte cazuri).
Parcurg lista de la nodul minim si pun in index informatia din fiecare nod,cresc sizeul
La final returnez rangeul creat.

RangeKeyQuery:
-----------------
Aloc memorie pentru o noua cheie(range), parcurg iarasi de la nodul minim(inceputul listei),
Caut nodurile doar din intervalul specificat(elementul nodului trebuie sa fie intre charurile p si q)
In cazul in care capacitatea e prea mica si am nevoie sa o maresc, ii realoc memorie indexului si maresc capacitatea de 2 ori.
Pun in index informatia din acest nod si parcurg lista pana la final.
Returnez cheia.
