# Sistem-de-conducere-pe-baza-de-microcontroller
Sistem de conducere pe baza de microcontroller ce efectueaza diferite functii: 

1. Aprindere LED la o secundă(intervalul de timp de 1 secundă se va realiza prin utilizarea unui timer al uC).

2. Aprindere/stingere LED prin intermediul comunicatiei seriale. Litera “A” trimisă de pe PC va aprinde LED-ul iar litera “S” va stinge LED-ul.

3. Va aprinde un LED atunci cand temperatura de la nivelul senzorului de temperatura va depăși pragul de 35 grade. Led-ul se va stinge atunci cand temperatura va scădea sub 35 grade.  Se va implementa cu histerezis de 0.5 grade.

4. Va aprinde un LED cu efect de dimming. Pentru aceasta se va utiliza semnal PWM cu următorul scenariu de durata 4 secunde rulat in bucla repetitiva. Scenariul împărțit pe cele 4 secunde este după cum urmează:

LED-ul va efectua un efect de FADE IN liniar pe durata unei secunde.
 - Va sta aprins pe durata unei secunde.
 - Va efectua un efect de FADE OUT liniar pe durata unei secunde.
 - Va sta stins o secunda.
 - Va masura temperatura de la un senzor de temperatura LM35, sau asemenător, pe care o va trimite pe PC prin intermediul interfeței seriale.

5. Va controla un afișaj cu 7 segmente si va afisa initialele numelui.
