# README

codice per una installazione che usa sensori capacitivi e striscia LED indirizzabili.
WIP.

## logica di funzionamento

La console prevede che ogni blocco sia sempre in uno stato di pulsazione luminosa (stato _teaser_).
Quando un blocco subisce un tocco, la luminosità relativa sale al 100% e rimane nello stato _sustain_ per un tempo limite prima di avviare la fase di _release_.
Opzionalmente i blocchi che si trovano ai lati di quello selezionato, si spengono per accentuare l'attenzione sul blocco appena toccato.

E' multiutente? Sì. Tutti i blocchi possono essere toccati simultaneamente (21).
Il tocco non viene disabilitato dai pulsanti che hanno luminosità pari a zero.
in altri termini, non disabilitiamo il tocco di quelli che si spengono, restano attivi. 


