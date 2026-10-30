/* produit.c - Implémentation du module Catalogue Produits */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produit.h"
#include "types.h"

// 1. Ajouter un produit au catalogue avec allocation dynamique et realloc si besoin
int ajouterProduit(Boutique *b, char *reference, char *designation, char *categorie, float prixUnitaireHT, int quantiteStock, int seuilAlerte) {
    // Contrôle d'unicité de la référence (règle n°8 du sujet : vérifie partout, même si actif == 0)
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0) {
            printf("Erreur : La reference '%s' existe deja dans le catalogue.\n", reference);
            return 0; // Échec
        }
    }

    // Vérification de la capacité et redimensionnement dynamique (realloc)
    if (b->nbProduits >= b->capaciteProduits) {
        int nouvelleCapacite = b->capaciteProduits * 2;
        Produit *temp = (Produit *)realloc(b->produits, nouvelleCapacite * sizeof(Produit));
        if (temp == NULL) {
            printf("Erreur : Allocation memoire echouee pour le catalogue.\n");
            return 0;
        }
        b->produits = temp;
        b->capaciteProduits = nouvelleCapacite;
    }

    // Remplissage des champs de la structure Produit
    strcpy(b->produits[b->nbProduits].reference, reference);
    strcpy(b->produits[b->nbProduits].designation, designation);
    strcpy(b->produits[b->nbProduits].categorie, categorie);
    b->produits[b->nbProduits].prixUnitaireHT = prixUnitaireHT;
    b->produits[b->nbProduits].quantiteStock = quantiteStock;
    b->produits[b->nbProduits].seuilAlerte = seuilAlerte;
    b->produits[b->nbProduits].actif = 1; // Produit actif par défaut

    b->nbProduits++;
    return 1; // Succès
}

// 2. Mettre à jour un produit (prix, stock, seuil)
int mettreAJourProduit(Boutique *b, char *reference, float nouveauPrix, int nouveauStock, int nouveauSeuil) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0) {
            if (nouveauPrix > 0) b->produits[i].prixUnitaireHT = nouveauPrix;
            if (nouveauStock >= 0) b->produits[i].quantiteStock = nouveauStock;
            if (nouveauSeuil >= 0) b->produits[i].seuilAlerte = nouveauSeuil;
            printf("Produit %s mis a jour avec succes.\n", reference);
            return 1;
        }
    }
    printf("Erreur : Produit %s introuvable.\n", reference);
    return 0;
}

// 3. Suppression logique d'un produit (actif = 0) avec vérification dans les commandes
int supprimerProduit(Boutique *b, char *reference) {
    int index = -1;
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Erreur : Produit introuvable.\n");
        return 0;
    }

    // Contrôle métier : Refuser si le produit est utilisé dans une commande non annulée
    for (int c = 0; c < b->nbClients; c++) {
        for (int cmd = 0; cmd < b->clients[c].nbCommandes; cmd++) {
            Commande *commande = &b->clients[c].commandes[cmd];
            if (strcmp(commande->statut, "ANNULEE") != 0) {
                for (int l = 0; l < commande->nbLignes; l++) {
                    if (strcmp(commande->lignes[l].refProduit, reference) == 0) {
                        printf("Erreur : Impossible de supprimer ce produit, il est rattache a une commande active (N°%d).\n", commande->numero);
                        return 0;
                    }
                }
            }
        }
    }

    // Suppression logique
    b->produits[index].actif = 0;
    printf("Produit %s supprime logiquement avec succes.\n", reference);
    return 1;
}

// 4. Afficher le catalogue et la valorisation du stock
void afficherCatalogue(Boutique *b) {
    printf("\n--- CATALOGUE DES PRODUITS ---\n");
    printf("%-12s | %-25s | %-12s | %-10s | %-6s | %-6s | %-6s\n", "Reference", "Designation", "Categorie", "Prix HT", "Stock", "Seuil", "Actif");
    printf("----------------------------------------------------------------------------------------\n");

    float valeurStockTotal = 0;
    for (int i = 0; i < b->nbProduits; i++) {
        Produit p = b->produits[i];
        printf("%-12s | %-25s | %-12s | %10.2f | %6d | %6d | %6d\n",
               p.reference, p.designation, p.categorie, p.prixUnitaireHT, p.quantiteStock, p.seuilAlerte, p.actif);
        if (p.actif == 1) {
            valeurStockTotal += p.prixUnitaireHT * p.quantiteStock;
        }
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Valeur totale du stock actif : %.2f EUR\n\n", valeurStockTotal);
}

// 5. Rechercher un produit par référence exacte ou par mot-clé dans la désignation
void rechercherProduit(Boutique *b, char *recherche) {
    printf("\n--- RESULTATS DE RECHERCHE POUR : \"%s\" ---\n", recherche);
    int trouve = 0;
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, recherche) == 0 || strstr(b->produits[i].designation, recherche) != NULL) {
            Produit p = b->produits[i];
            printf("Ref : %s | %s | Cat : %s | Prix : %.2f EUR | Stock : %d | Actif : %d\n",
                   p.reference, p.designation, p.categorie, p.prixUnitaireHT, p.quantiteStock, p.actif);
            trouve = 1;
        }
    }
    if (!trouve) {
        printf("Aucun produit ne correspond a votre recherche.\n");
    }
    printf("---------------------------------------------------\n\n");
}

// 6. Afficher les alertes de stock (produits dont le stock <= seuil d'alerte)
void afficherAlertesStock(Boutique *b) {
    printf("\n--- ALERTES DE REAPPROVISIONNEMENT DU STOCK ---\n");
    int alerteTrouvee = 0;
    for (int i = 0; i < b->nbProduits; i++) {
        Produit p = b->produits[i];
        if (p.actif == 1 && p.quantiteStock <= p.seuilAlerte) {
            printf("ALERTE : Produit %s (%s) - Stock actuel : %d (Seuil d'alerte : %d)\n",
                   p.reference, p.designation, p.quantiteStock, p.seuilAlerte);
            alerteTrouvee = 1;
        }
    }
    if (!alerteTrouvee) {
        printf("Aucun produit en stock critique.\n");
    }
    printf("---------------------------------------------------\n\n");
}