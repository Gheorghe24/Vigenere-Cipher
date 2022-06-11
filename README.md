TEMA 2
Grosu Gheorghe 314CD

CERINTA 1
---------------------------------------------------------------------------------
CreateTree:
creez un nou tree si ii aloc memorie,ii pun fiecarui element din structura functia sau valoarea data ca parametru, initializez sizeul cu 0 si root cu null.

isEmpty:
verific daca nu are elementul root(acesta e primul)

search:
construiesc functia recursiv si parcurg arborele in functie de comparatia intre element si nodul cu care parcurg(la stanga sau la dreapta), pana gasesc nodul cu elementul cautat.

minimum:
Acesta este elementul cel mai din stanga al subarborelui stang(cel mai jos)

maximum:
Acesta este elementul cel mai mare din subarborele drept(parcurg pana la NULL in dreapta)

succesor si predecesor:
Aceste functii sunt foarte asemanatoare. Exista 2 situatii in fiecare din acestea:
Ptu predecesor: daca exista subarborele drept de la nod, atunci e minimul din acel subarbore, altfel parcurg cu parintele pana gasesc valoarea mai mica ca nodul
Ptu succesor: daca exista subarborele stang al nodului, returnez valoarea maxima, altfel la fel parcurg cu parintele pana gasesc o valoare mai mare decat nodul

updateHeight:
calculez inaltimea pentru arborele stang si cel drept si apoi actualizez inaltimea in functie de care e cea mai mare plus nodul radacina

avlrotateleft:
Schimb linkurile dintre nodurile x si y si copilul din stanga al lui y.
Linkuesc si legaturile dintre parintii acestora. Iau si cazul in care nu exista copii, sau nu x nu are parinte.Ma intreb daca x e copil de stanga sau copil de dreapta, ca sa ii leg parintele de y.
La final actualizez inaltimea pentru x si y.

avlrotateRight:
Este exact acelasi algoritm insa aici x e copil de stanga pentru y si schimbam legatura cu copilul din dreapta a lui x.

avlGetBalance:
Verific daca exista acel nod si daca are copil de dreapta sau de stanga apoi returnez formula:
(height(left) - height(right))

avlFixup:
verific balansarea incepand de la parintele nodului
verific daca parintele este debalansat in partea dreapta sau in partea stanga
sau daca e in forma zig-zag si fac rotatiile necesare 

avlDeleteFixup:
o functie foarte asemanatoare cu cea de sus insa aici verific balansarea chiar de la acel nod
si ma folosesc de balansarea nodului(subarborelui debalansat, copilul de dreapta sau de stanga)

createTreeNode:
functie deja creata

insert:
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
Incep de la finalul listei si sterg fiecare element pana ajung la inceput, ma folosesc de un pointer auxiliar ca sa dau free nodului curent.
Apoi eliberez memoria arborelui.

CERINTA 2: CIPHER
-----------------------------------------------------------------------------------
Aduc in acest fisier antetul unor functii din Tema2.c

BuildTreeFromFile:
Deschid fisierul cu numele dat ca parametru, aloc memorie pentru un char "linie" pe care il citesc din fisier. Ma folosesc de while(fgets) ca sa citesc pana la finalul fisierului, apoi cu strtok iau fiecare cuvant din linia citita. Imi creez un element cu functia createStrElement(aceasta trunhiaza automat cuvintele la primele 5 litere). De asemenea, imi declar o variabila in care salvez lungimea fiecarui cuvintelor citite pana la final, acesta e transmis ca informatie in insert
Inserez nodul cu elementul si lungimea specifica, actualizez lungimea, distrug elementul si fac iarasi strtok ca sa iau urmatorul nod
Inchid fisierul si eliberez memorie pentru "linia citita"(pointerul la char alocat).

CERINTA 3:
-----------------------------------------------------------------------------------
inorderKeyQuery:
aloc memorie pentru un nou range
Ii pun sizeul initial 0 si capacitatea size-ul arborelui(puteam pune si alte valori insa aceasta cred ca este cea mai eficienta, pentru a nu realoca in alte cazuri).
Parcurg lista de la nodul minim si pun in index informatia din fiecare nod,cresc sizeul
La final returnez rangeul creat.

RangeKeyQuery:
Aloc memorie pentru o noua cheie(range), parcurg iarasi de la nodul minim(inceputul listei),
Caut nodurile doar din intervalul specificat(elementul nodului trebuie sa fie intre charurile p si q)
In cazul in care capacitatea e prea mica si am nevoie sa o maresc, ii realoc memorie indexului si maresc capacitatea de 2 ori.
Pun in index informatia din acest nod si parcurg lista pana la final.
Returnez cheia.
