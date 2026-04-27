# SAT Coloring Solver

Dieses Projekt verwendet den CaDiCaL-Solver als Submodule und baut darauf einen
kleinen eigenen Solver.

## Vorbereitung

Zuerst das Submodule initialisieren:

```bash
git submodule update --init --recursive
```

## CaDiCaL bauen

Im Verzeichnis `cadical/` den Solver und die Bibliothek bauen:

```bash
cd cadical
./configure && make
```

## Eigenes Projekt bauen

Danach das Projekt im Wurzelverzeichnis mit CMake konfigurieren:

```bash
cd ..
cmake
```

## Programm starten

Das Programm erwartet genau **eine** Eingabedatei als Argument. Die Datei kann
z. B. eine `.col`-Datei oder auch eine `.txt`-Datei sein, solange sie das
erwartete Format enthält.

```bash
./blatt_1_1 ./src/examples/small_test.col
```

Falls die Eingabedatei an einem anderen Ort liegt, einfach den Pfad übergeben:

```bash
./blatt_1_1 /pfad/zu/deiner_datei.txt
```
