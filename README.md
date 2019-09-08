# X-CubeSat


## Concernant les parties hard et soft

Un conseil (le premier...): soyez archi-pragmatiques.

Normalement vous devriez savoir (ou décider très rapidement) quelle plateforme vous allez utiliser (quelle carte ODB, avec quel CPU, etc...).  A moins d'être vraiment à l'aise en conception de circuits électroniques, choisissez une carte "sur étagère" (Gomspace, Clyde Space, etc...), de préférence une des plus simples: évitez les monstres hybrides CPU/FGPA type Xilinx Zynq (à moins d'avoir une bonne raison de le faire), et regardez plutôt du côté des processeurs à base de Cortex M3/M4, ils sont très bien: FPU pour l'arithétique à virgule flottante en hard, beaucoup de flash, beaucoup de RAM, et plein de périphériques à disposition (contrôleurs SPI, I2C, CAN, USB, etc...).

Par ailleurs, assurez-vous d'avoir un minimum de soft livré avec pour démarrer sans douleur.  L'idéal est d'avoir un exemple de code qui boot, qui fait clignotter une led, ou qui envoie un message sur un port série: quelque-chose avec lequel vous pouvez interagir et expérimenter.  Les histoires de bootloader peuvent vite devenir assez velues, il se passe un tas de choses avant d'arriver dans main()...

Vous pouvez reprendre la carte ODB et adapter le soft de X-CubeSat si vous le souhaitez, mais attendez-vous à devoir faire quelques retouches sur le hardware: rien ne vous garantit que votre carte bande S sera compatible, par exemple, au niveau de l'affectation des signaux électriques sur le bus PC-104.  Rien de catastrophique, mais ça prend du temps (1 ou 2 semaines pour faire fabriquer un PCB + la livraison) et ça peut poser des problèmes administratifs (est-ce que le prestataire est référencé auprès de l'Ecole) ou pratiques (on n'a pas toujours forcément sous la main quelqu'un qui sait souder correctement du CMS).

Loin de moi l'envie de vous décourager !  Mais c'est sur ce genre de "détails" que vous risquez de galérer.  En résumé, dans le doute, choisissez le chemin de moindre résistance.

## Le soft de X-CubeSat

Je reste sur des généralités, et vous propose de voir les points techniques au cas par cas si vous le souhaitez.

Globalement le système se comporte comme une simple machine à états:

  . a tout instant, le satellite est dans un mode (= état) donné;  on a donc une liste de modes: démarrage (INIT), économie d'énergie (POWER), nominal (WODEX), etc... (les modes sont déclarés dans sat/ODB/Config.h),
  . pour chaque mode, on a un ou plusieurs thread(s) actif(s);  un changement de mode (= transition) peut donc entraîner la suspension de certains threads, et la reprise de certains autres.

ControlThread est un cas particulier: il est toujours actif. Son rôle est de recevoir tous les évènements asynchrones du système (commandes reçues du sol, alerte sur les niveau de charge des batteries, etc...), et de décider de déclancher ou non un changement de mode en fonction de chaque évènement reçu.

-> Si vous cherchez un "point d'entrée" pour comprendre le code, c'est la méthode ControlThread::run(), dans sat/ODB/ControlThread.cpp.

## FreeRTOS

FreeRTOS apporte trois choses importantes:

  . un scheduler (ordonnanceur) préemptif avec une gestion des priorités (nos "threads" sont simplement des tâches FreeRTOS avec un peu d'objet autour)
  . un mécanisme de synchronisation et de communication entre les tâches (les sémaphores sont une interface idéale entre le soft et les interruptions hardware)
  . plusieurs stratégies d'allocation de mémoire (allocation dynamique ou non, implémentations rudimentaires de malloc(), sbrk(), etc...)

Il est très bien documenté, et simple à prendre en main.

## Les drivers

C'est la plus grosse partie du boulot, et c'est ici qu'on trouve toutes les choses vraiment techniques et intéressantes: I/O asynchrones, gestion des canaux DMA, etc... tout ce qui peut faire que le processeur n'est jamais "bloqué" en attendant quelque-chose, et qu'il peut communiquer avec le sol tout en transférant de données sur son bus SPI.

Le principe est simple: on a une collection de classes génériques dans sat/sys/device/*.h (la plupart sont des classes purement virtuelles, c'est à dire des interfaces), et les implémentations correspondantes, spécifiques à la plateforme dans sat/sys/device/STM32/.  C'est venu d'une (courte) période d'incertitude initiale quand au choix du CPU de X-CubeSat (STM32 ou autre chose);  il s'agissait alors de se laisser la possibilité de changer de plateforme en cours de route, et de n'avoir "que" les drivers à réécrire (= sans toucher à l'application).

Il y a également quelques "pseudo-drivers" (les FlashArray, FlashCache, etc...), qui permettent d'abstraire le type et le nombre de chips de mémoire flash présents sur le PCB, et de maintenir un cache de quelques pages de flash en RAM, afin de ne pas bourriner en permanence des composants relativement limités en nombre de cycles d'écriture.
