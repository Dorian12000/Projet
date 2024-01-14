<img src="img/github-header-image.png" alt="robot chat">

# Robot Chat et Souris

Ce projet consiste à créer un robot capable de jouer au jeu du chat et de la souris. Le robot est équipé de plusieurs fonctionnalités clés, notamment le contrôle de mouvement à l'aide de moteurs à courant continu (MCC), la localisation avec un Lidar X4, et la détection du vide à l'aide de capteurs de bordure.

## Schéma Fonctionnel : 
https://miro.com/app/dashboard/
<img src="img/Schema_fonctionnel.png" alt="schéma fonctionnel" width="1375">

## Fonctionnalités principales

1. **Contrôle de Mouvement (MCC) :**
   - Le robot est capable de se déplacer dans son environnement grâce à des moteurs à courant continu. 
2. **Localisation avec Lidar X4 :**
   - Utilisation du Lidar X4 pour permettre au robot de s'orienter et de se repérer dans son environnement.

3. **Détection du Vide avec Capteurs de Bordure :**
   - Les capteurs de bordure sont utilisés pour détecter les zones sans sol, afin d'empêcher le robot de tomber.

## Utilisation

1. **Exécution du Programme :**
   - Pour démarrer le robot, exécutez le programme principal. Assurez-vous que toutes les dépendances sont installées et que le robot est correctement alimenté. Le robot démarre en mode Souris.

2. **Interface Utilisateur :**
   - Pour connaître les événements enregistrés par le robot, veuillez connecter une ST-link entre le robot et votre ordinateur, puis ouvrir une console (TeraTerm, Putty, ...) configurée en 115200 bauds.
   - Trois LEDs sont présentes sur le robot pour indiquer son état en temps réel :
        - Led verte : allumée pour l'état chat, éteinte pour l'état souris.
        - Led orange : changement d'état pour chaque détection de bordure.
        - Led rouge : le robot a rencontré un problème et doit être redémarré.



