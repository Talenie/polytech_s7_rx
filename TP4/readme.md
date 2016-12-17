# Readme Serveur / Client TCP

Travail réalisé par Abonnenc Alicia, Bonhoure Gilles et Chambonnet Simon.

##Compilation et exécution
Pour compiler simplement le programme et le rendre utilisable directement, lancer le script autocompile.sh

`bash autocompile.sh`

Cela aura pour effet de compiler les deux programmes `transfertcl` et `transfertse` ainsi que de les déplacer dans les dossiers serveur et client. Pour tester le programme, exécuter la commande `./transfertse` dans le dossier serveur, puis `./transfertcl <localhost> <25564>` dans le dossier client. Les valeurs entre `<>` peuvent changer en fonction de la configuration du serveur. Si cela ne fonctionne pas, vérifiez les variables annoncées par le serveur lors de son lancement.

##Utilisation

Les commandes disponibles sur le serveur sont les suivantes (sur le client uniquement) :

* ls : afficher le contenu du dossier courant du serveur. Aucune option supplémentaire possible (cf. commentaires du fichier transfertse.c).
* get [filename] : permet de récupérer un ou plusieurs fichiers depuis le serveur distant (théoriquement, plusieurs fichiers peuvent être téléchargés, mais seule la fonctionnalité avec un unique fichier est pleinement fonctionnelle; télécharger plusieurs fichiers en même temps ne provoque pas d'erreur à l'exécution, mais leur contenu est erroné.)
* put [filename] : permet d'envoyer au serveur des fichiers. Même problème qu'avec get.
