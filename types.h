#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================= */
/* CONSTANTES DU PROJET          */
/* ============================= */

/* Tailles des chaines (caractere '\0' inclus) */
#define TAILLE_REFERENCE   12
#define TAILLE_DESIGNATION 60
#define TAILLE_CATEGORIE   30
#define TAILLE_NOM         50
#define TAILLE_PRENOM      50
#define TAILLE_EMAIL       80
#define TAILLE_TELEPHONE   15
#define TAILLE_RUE         80
#define TAILLE_CODE_POSTAL 6
#define TAILLE_VILLE       50
#define TAILLE_PAYS        50
#define TAILLE_STATUT      20

/* Statuts possibles d'une commande */
#define STATUT_EN_COURS "EN_COURS"
#define STATUT_EXPEDIEE "EXPEDIEE"
#define STATUT_LIVREE   "LIVREE"
#define STATUT_ANNULEE  "ANNULEE"

/* Regles metier : facturation */
#define TAUX_TVA          20.0f   /* % applique sur le HT apres remise */
#define FRAIS_PORT        5.90f   /* euros, non soumis a la TVA */
#define SEUIL_PORT_OFFERT 60.0f   /* HT brut a partir duquel le port est offert */
#define EUROS_PAR_POINT   10.0f   /* 1 point de fidelite par tranche de 10 euros TTC */

/* Regles metier : paliers de remise (sur le HT brut) */
#define SEUIL_REMISE_5    100.0f
#define SEUIL_REMISE_10   300.0f
#define SEUIL_REMISE_15   600.0f
#define TAUX_REMISE_0     0.0f
#define TAUX_REMISE_5     5.0f
#define TAUX_REMISE_10    10.0f
#define TAUX_REMISE_15    15.0f

/* Contraintes de validation */
#define ANNEE_MIN         2000
#define ANNEE_MAX         2100
#define CAPACITE_INITIALE 4       /* capacite de depart des tableaux dynamiques */

/* ============================= */
/* STRUCTURE DATE                */
/* ============================= */

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

/* ============================= */
/* STRUCTURE ADRESSE             */
/* ============================= */

typedef struct {
    char rue[TAILLE_RUE];
    char ville[TAILLE_VILLE];
    char codePostal[TAILLE_CODE_POSTAL];
    char pays[TAILLE_PAYS];
} Adresse;

/* ============================= */
/* STRUCTURE PRODUIT             */
/* ============================= */

typedef struct {
    char reference[TAILLE_REFERENCE];
    char designation[TAILLE_DESIGNATION];
    char categorie[TAILLE_CATEGORIE];

    float prixUnitaireHT;

    int quantiteStock;
    int seuilAlerte;

    int actif;                      /* 1 = actif, 0 = supprime logiquement */
} Produit;

/* ============================= */
/* STRUCTURE LIGNE DE COMMANDE   */
/* ============================= */

typedef struct {
    char refProduit[TAILLE_REFERENCE];
    char designation[TAILLE_DESIGNATION];   /* fige au moment de la vente */

    int quantite;

    float prixUnitaireHT;                   /* fige au moment de la vente */
    float sousTotalHT;                      /* quantite * prixUnitaireHT */
} LigneCommande;

/* ============================= */
/* STRUCTURE COMMANDE            */
/* ============================= */

typedef struct {
    int numero;
    int idClient;

    Date dateCommande;

    char statut[TAILLE_STATUT];

    int nbLignes;
    int capaciteLignes;
    LigneCommande *lignes;                  /* tableau dynamique */

    float montantBrutHT;
    float tauxRemise;
    float montantRemise;
    float montantHT;
    float montantTVA;
    float montantTTC;
    float fraisPort;

    Adresse adresseLivraison;               /* instantane a la creation */
} Commande;

/* ============================= */
/* STRUCTURE CLIENT              */
/* ============================= */

typedef struct {
    int idClient;

    char nom[TAILLE_NOM];
    char prenom[TAILLE_PRENOM];
    char email[TAILLE_EMAIL];
    char telephone[TAILLE_TELEPHONE];

    Adresse adresse;

    int pointsFidelite;

    int nbCommandes;
    int capaciteCommandes;
    Commande *commandes;                    /* tableau dynamique */

    int actif;                              /* 1 = actif, 0 = supprime logiquement */
} Client;

/* ============================= */
/* STRUCTURE BOUTIQUE (racine)   */
/* ============================= */

typedef struct {
    char nom[TAILLE_NOM];

    Produit *produits;                      /* tableau dynamique */
    int nbProduits;
    int capaciteProduits;

    Client *clients;                        /* tableau dynamique */
    int nbClients;
    int capaciteClients;

    int numeroSuivant;                      /* compteur des numeros de commande */
    int idClientSuivant;                    /* compteur des identifiants clients */
} Boutique;

#endif /* TYPES_H */;
