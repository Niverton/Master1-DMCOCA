#include "all.h"
#include "options.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

/*  variables:
 *  numéro de variable unique
 *  numéro de sommet de k en k (s(0) = 0, s(n) = s(n-1)+k = n*k)
 *  générer les clauses à partir du graphe
 *  le solveur testeras toutes les possibilité pour dire si satisfaisable ou non (y'a-t-il un arbre?)
 */

struct commentaire {
  commentaire(bool enable, std::ostream& out)
    : enable_{enable},
      out_{out} {
  }

  commentaire& operator<<(const char* c) {
    if (enable_) {
      out_ << "c " << c;
    }
    return *this;
  }

  bool enable_;
  std::ostream& out_;
};

std::ostream& NON(std::ostream& o) {
  o << '-';
  return o;
}

std::ostream& OU(std::ostream& o) {
  o << ' ';
  return o;
}

std::ostream& ET(std::ostream& o) {
  o << "0\n";
  return o;
}

void adjacency();

/*  Fork et lance glucose
 *  fdin: fd sur lequel attacher l'entrée standard
 */
void glucose();
void buildCNF(int k, bool e_com, std::ostream& out);

int main(int argc, char **argv) {
  options opts{argc, argv};

  //Commentaires
  if (opts.display_adj) {
    adjacency();
    return 0;
  }

  std::fstream out{"cnf", out.out | out.trunc};

  //Créer le fichier
  buildCNF(opts.k, opts.coms, out);
  out.close();
  //Créer le thread
  std::cout << "Running glucose\n";
  glucose();

  std::fstream fres{"out", fres.in};
  std::string word;
  std::getline(fres, word, ' '); //Lire premier mot
  if (word == "UNSAT\n") {
    std::cout << "\nIl n'y a pas d'arbre couvrant de hauteur <= k\n";
    return 0;
  }

  std::vector<int> vh;
  int i = 1; //numéro sommet
  while(word != "0\n") {
    if (fres.fail() or fres.eof()) {
      std::cerr << "Erreur de lecture du fichier out!\n";
      return 1;
    }

    int x = std::stoi(word);
    if (x > 0) {
      //Récupèrer h depuis x
      int h = x - i - (opts.k * (i-1));
      vh.push_back(h);
      i++;
    }
    std::getline(fres, word, ' ');
  }

  std::cout << "Liste d'adjacence de l'arbre couvrant trouvé:\n";
  for (int v = 0; v < orderG(); v++) {
    std::cout << v << "(" << vh[v] << ") -> ";
    for (int w = 0; w < orderG(); w++) {
      if (vh[v] == vh[w]-1 and
          are_adjacent(v, w)) {
        std::cout << w << ", ";
      }
    }
    std::cout << "\n";
  }

  return 0;
}

void glucose() {
  int child = fork();
  switch (child) {
    case -1:
      perror("Fork error");
      exit(1);
    case 0:
      //child
      execl("./glucose", "glucose", "-verb=0", "cnf", "out", 0);
      //retourne uniquement si erreur
      perror("Glucose error");
      exit(errno);
  }
  if (wait(nullptr) == -1) {
    perror("Wait error");
    exit(errno);
  }
}

void buildCNF(int k, bool e_com, std::ostream& out) {
  commentaire com{e_com, out};

  int sommets = orderG();
  //int arretes = sizeG();

  int nb_clause = sommets * (1 + (k*(k+1))); // 1)
  nb_clause += 1 + (sommets*(sommets-1)); //2)
  nb_clause += 1; //3)
  nb_clause += sommets * k; //4)

  int nb_var = sommets * (k+1);

  //Traduire sommet + hauteur en variable
  auto VAR = [=](int s, int h) { return s + (k*(s-1)) + h; }; //+1 pour que (0, k) != (1, 0)


  out << "p cnf " << nb_var << ' ' << nb_clause << '\n';
  /*  Clauses:
   */
    //1)
  com << "Chaque sommet a une seule hauteur attribuée\n";
  for (int v = 1; v <= sommets; v++) {
    com << "Ce sommet a au moins un h\n";
    for (int h = 0; h <= k; h++) {
      out << VAR(v, h) << OU;
    }
    out << ET;
    com << "Ce sommet a au plus un h\n";
    for (int i = 0; i <= k; i++) {
      for (int j = 0; j <= k; j++) {
        if (i==j) continue;
        out << NON << VAR(v, i) << OU << NON << VAR(v, j) << OU; //Le dernier OU est necessaire
        out << ET;
      }
    }
  }
    //2)
  com << "Il y a au moins une racine\n";
  for (int v = 1; v <= sommets; v++) {
    out << VAR(v, 0) << OU;
  }
  out << ET;
  com << "Il y a au plus une racine\n";
  for (int v = 1; v <= sommets; v++) {
    for (int w = 1; w <= sommets; w++) {
      if (v == w) continue;
      out << NON << VAR(v, 0) << OU << NON << VAR(w, 0) << OU; //Pareil
      out << ET;
    }
  }

    //3)
  com << "Il y a au moins un sommet de hauteur k\n";
  for (int v = 1; v <= sommets; v++) {
    out << VAR(v, k) << OU;
  }
  out << ET;

    //4)
  com << "Si v n'est pas la racine, il a au moins un parent potentiel u\n";
  for (int v = 1; v <= sommets; v++) {
    com << "v n'est pas la racine implique qu'il existe un voisin u de hauteur h-1\n";
    for (int h = 1; h <= k; h++) {
      out << NON << VAR(v, h) << OU;
      for (int w = 1; w <= sommets; w++) {
        if (v == w or (not are_adjacent(w-1, v-1))) continue;
        out << VAR(w, h-1) << OU;
      }
      out << ET;
    }
  }
}

void adjacency() {
  for (int u = 0; u < orderG(); u++) {
    for (int v = 0; v < orderG(); v++) {
      std::cout << are_adjacent(u, v) << ' ';
    }
    std::cout << '\n';
  }
}

