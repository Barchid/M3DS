Barchid Sami

Doit contenir :
- ce que vous n'avez pas fait (et pourquoi). Précisez explicitement "tout a été fait et fonctionne parfaitement" si c'est le cas.
- difficultés rencontrées.
- commentaires éventuels sur le TP (points à éclaircir, longueur du sujet, etc). 


Question 7 : expliquer le résultat obtenu
-----------------------------------------
	Comme il n'y a pas de DEPTH TEST ici, les triangles sont tracés dans l'ordre d'apparition.
	De ce fait, le triangle rouge est tracé en premier. Mais les autres triangles, tracés après, vont "écraser" le triangle rouge donc il n'apparaîtra plus.

Question 9 : expliquer l'écran blanc quand glDepthFunc(GL_GREATER)
------------------------------------------------------------------
	La profondeur (axe Z) est dorénavant prise en compte. Cependant, le test utilisé est GL_GREATER, ce qui veut dire que la couleur du pixel tracé remplace la couleur précédente UNIQUEMENT s'il a	une profondeur supérieure.

	De plus, nous savons que la valeur d'initialisation du depth destination de tous les pixels lors d'un glClear est de 1. On ne voit donc rien affiché car notre clear est déjà à la profondeur 1. On ne peut pas avoir de point avec une profondeur > 1. Notre écran prend donc la couleur du clear (ici, écran blanc).
