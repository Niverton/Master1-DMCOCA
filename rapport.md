% Devoir Maison
% MAUGEY Rémy
% Novembre 2017

# II- HAC est NP-difficile
1. Un chemin passe par tout les sommets du graphe, c'est donc un arbre couvrant de hauteur n-1 (n le nombre de sommets du graphe).  
On peut donc réduire CH à HAC en temps constant, donc polynomial.

2. Comme CH est NP (vu en TD), et que CH se réduit à HAC alors HAC est NP-difficile.

# III- Réduction de HAC vers SAT

1. $\bigwedge_{v \in V} \left( \bigvee_{h \le k} x_{v,h} \wedge \bigwedge_{i,j \le k, i \ne j} \left( \neg x_{v,i} \vee \neg x_{v,j}\right)\right)$  
Chaque sommet n'a qu'une seule position dans l'arbre.
2. $\bigvee_{v \in V} \left( x_{v,0} \wedge \bigwedge_{v, w \in V, v \ne w} \left( \neg x_{v,0} \vee \neg x_{v,0}\right)\right)$  
L'arbre a une seule racine.
3. $\bigvee_{v\in V} x_{v,h}$  
L'arbre est bien de hauteur k
4. $\bigvee_{v \in V} \left[ \bigwedge_{h \ge 1, h \le k} \left( \neg x_{v,h} \vee \bigvee_{u \in V\setminus v, (u,v) \in E} x_{u,h-1} \right) \right]$  
Les sommets sont reliés entre eux pour former l'arbre couvrant.


