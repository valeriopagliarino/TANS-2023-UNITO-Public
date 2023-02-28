# Simulazione Monte Carlo e ricostruzione di vertici primari in un tracciatore
### Tecniche di analisi numerica e simulazione - AA 2022-2023

Authors: Enrica Bergalla, Valerio Pagliarino

[RELAZIONE TECNICA E DOCUMENTAZIONE](./TANS_Report_Bergalla_Pagliarino.pdf)

<b> Istruzioni rapide per testare il programma, per informazioni più dettagliate fare riferimento alla relazione tecnica </b>

### Requisiti Minimi

Sistema operativo Linux o MacOS con installazione CERN ROOT 6.24/04 (rilasciata il 26/08/2021) o superiore. Versioni meno recenti richiedono una leggera modifica al codice sorgente, poiché alcuni metodi che coinvolgono TRandom3 non sono supportati. Per ulteriori informazioni consultare il paragrafo X della relazione tecnica.

## Simulazione di 100 mila eventi

1. Entrare nella cartella radice del progetto e avviare un terminale in tale posizione, assicurarsi che tutti gli alias e le variabili d'ambiente di ROOT siano correttamente impostati nella sessione di terminale corrente.
2. Avviare la compilazione del codice sorgente tramite ACLiC digitando `root start.cxx`, eventualmente pulire l'output della compilazione digitando `clear`.
3. ***[FACOLTATIVO]*** - Avviare la GUI per la generazione del file di configurazione digitando `root simulationConfig.cxx`
4. ***[FACOLTATIVO]*** - Compilare i campi con le impostazioni generali opportunamente, in particolare:
    - In **input simulation file path** indicare il file ROOT in cui inserire le distribuzioni di partenza. Il percorso indicato di default si riferisce ad un file ROOT in cui sono state inserite le distribuzioni fornite dal docente e in cui verranno salvate ulteriori distribuzioni per variabili di input specificate dall'utente. **Inizialmente si può lasciare il percorso di default**
    - In **output simulation file path** indicare il file ROOT in cui si vuole che venga salvato l'output della simulazione. **Inizialmente si può lasciare il percorso di default**
    - In **configuration file path** indicare il file ROOT in cui si vuole che venga scritto il file txt con le impostazioni, che dovrà essere passato al programma di simulazione. **Inizialmente si può lasciare il percorso di default**
    - Inserire il numero di eventi da simulare, ad esempio 100 mila
    - Compilare gli altri parametri della colonna di sinistra, ad esempio: 
    
    | Parametro                  | Valore |  Descrizione |
    |--------------------------- | ------ | ------------ | 
    | beam pipe tickness         | 800 um |              |
    | inner silicon radius       | 40  mm |              |
    | inner silicon length       | 270 mm |              |
    | outer silicon radius       | 70  mm |              | 
    | outer silicon length       | 270 mm |              | 
    | silicon tickness           | 200 um |              | 
    | beam pipe radius           | 30  mm |              | 
    | beam pipe length           | 5   m  |              | 
    | single collision in event  | true   | Una sola collision per evento (no pileup) (ALPHA VERSION) | 
    | single collision persist.  | false  | Modalità persistente per l'utilizzo dell'event display | 
    | enable hit gaus. smearing  | true   | Smearing gaussiano dei punti di impatto| 
    | hit cluster activation     | false  | Accensione cluster di pixel (ALPHA VERSION) | 

    A questo punto, attraverso il pannello di destra è possibile assegnare le apposite distribuzioni alle variabili di input: per utilizzare le distribuzioni di pseudorapidità e di molteplicità fornite dal docente, selezionare nella prima listbox sotto a "Input distributions" l'opzione "Existing Object inside the input .root file". Digitare in "choose name" il nome esatto con cui è stato salvata la distribuzione sul file di ROOT in input (oggetto TH1D). Nella listbox sottostante selezionare la variabile a cui assegnare tale distribuzione, e poi premere su "assign". Per visualizzare distribuzioni già presenti nel file ROOT in input, utilizzare il TBrowser.
    
    Al termine delle operazioni, premere su "Write configuration and input files" in basso a sinistra, poi passare al terminale da cui è stata avviata la gui, premere `.q` e poi invio.

5. A questo punto, nel medesimo terminale, digitare nuovamente `root start.cxx` e digitare nella riga di comando `Cli::Simulation("./simulationConfig.txt")`. Dove alla funzione viene passato il percorso relativo al file txt di configurzione generato dalla precedente GUI.

6. Al termine verrà prodotto un file ROOT di output, nel percorso specificato nel file di configurazione, che potrà essere visualizzato con un TBrowser e utilizzato per la ricostruzione e per l'analisi.

## Simulazione con event display

Per eseguire una simulazione con persistenza completa di tutte le tracce e le hit generate (che sono classi custom di ROOT che supportano la persistenza su disco), seguire la procedura seguente:

1. Creare un file di configurazione come descritto in precedenza, ma con un numero di eventi nell'ordine del centinaio

2. Digitare `root start.cxx` per avviare il programma

3. Digitare a questo punto `auto * rm = Cli::Simulation("./simulationConfig.txt", true)` dove l'ultimo parametro booleano forza la persistenza degli eventi anche se questa non è stata abilitata nel file di configurazione. Il metodo Simulation restituisce sempre un puntatore all'oggetto della classe RunManager, ma questa volta vogliamo tenerne traccia per poterlo passare all'event display.

4. Al termine della simulazione avviare l'event display con `Cli::DrawEvent(rm, 36)` dove l'ultimo parametro intero è il numero identificativo dell'evento.

## Ricostruzione e analisi

1. Avviare la compilazione del codice sorgente tramite ACLiC digitando `root start.cxx`

2. ***[FACOLTATIVO]*** Avviare la GUI per la generazione del file di configurazione digitando `root reconstructionConf.cxx`

3. ***[FACOLTATIVO]*** - Compilare i campi con le impostazioni generali opportunamente, in particolare:
    - Sulla sinistra, nei vari file path, si possono scegliere, in ordine, il nome del file .txt di configurazione per la ricostruzione, il nome del file .root in output dalla simulazione, il nome del file .root in output della ricostruzione e il nome del file .root in output dell'analisi.
    - In alto a destra si possono impostare alcuni parametri per la ricostruzione. In ordine, l'ampiezza della running window, lo step con cui viene fatta scorrere la running window (espresso in percentuale rispetto all'ampiezza della running window stessa), il numero minimo di candidati vertici in una finestra affinchè questa venga presa in considerazione per la ricostruzione e il limite superiore di candidati vertici che possono contenere le altre finestre, rispetto a quella che contiene il maggior numero di candidati vertici, perchè la ricostruzione venga eseguita (espresso in percentuale rispetto al maggior numero di candidati vertici in una finestra).
    - Nella medesima sezione è anche possibile disabilitare il calcolo del deltaPhiMax svolto a partire dalla verità MC. In tal caso verrà impostato il valore di default di 20 mrad.
    - In basso a destra si possono impostare alcuni parametri per l'analisi. In ordine, in numero di deviazioni standard del vertice primario all'interno delle quali verranno presi in considerazione gli evendi durante l'analisi, la minima e massima molteplicità all'interno delle quali gli eventi verranno presi in considerazione per l'analisi.
All'apertura della GUI, tutti i campi presentano dei valori di default: se non vengono modificati, saranno questi ad essere scritti nel file di configurazione. Al termine delle operazioni, premere su "Generate Configuration File" in basso a destra, poi passare al terminale da cui è stata avviata la GUI, premere `.q` e poi invio.

4. A questo punto, nel medesimo terminale, digitare nuovamente `root start.cxx` e digitare nella riga di comando `Cli::Reconstruction("./reconstructionConfig.txt")`. Dove alla funzione viene passato il percorso relativo al file .txt di configurzione generato dalla precedente GUI.

5. Al termine verranno prodotti due file .root di output: quello contenente i risultati della ricostruzione e quello contenente i risultati dell'analisi. All'interno di quest'ultimo saranno presenti tutti i grafici con lo studio di efficienza e risoluzione.

  
