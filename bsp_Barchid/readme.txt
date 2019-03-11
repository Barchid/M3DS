BARCHID Sami

Tout a été fait et fonctionne parfaitement.

Difficultés rencontrées : 
----------------------------
Principalement le manque de temps (à cause du rythme de travail général + alternance) qui rend compliqué le processus d'apprentissage.


Question 5 :

L'élimination de partie cachée par Depth Buffer ne trace que ce qui est visible (donc on ne trace plus les faces situées derrière d'autres par rapport à l'observateur).
De ce fait, la transparence se calcule mal : le fragment destination n'est plus le fragment correspondant à la face arrière (non-visible) du loup. On mélange donc le fragment 
source avec rien de correcte, ce qui donne ce rendu pas très jolie.