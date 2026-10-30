/* produit.h - Interface du module Catalogue Produits */
#ifndef PRODUIT_H
#define PRODUIT_H

#include "types.h"

// Prototypes des fonctions du module produit
int ajouterProduit(Boutique *b, char *reference, char *designation, char *categorie, float prixUnitaireHT, int quantiteStock, int seuilAlerte);
int mettreAJourProduit(Boutique *b, char *reference, float nouveauPrix, int nouveauStock, int nouveauSeuil);
int supprimerProduit(Boutique *b, char *reference);
void afficherCatalogue(Boutique *b);
void rechercherProduit(Boutique *b, char *recherche);
void afficherAlertesStock(Boutique *b);

#endif // PRODUIT_H