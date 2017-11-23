# Betriebssysteme

## Organisatorisches

Jana Becker | Hendrik Diemer | Alice Rühle
------------ | ------------- | -------------
beja1025 | dihe1011 | rual1017
57452 | 56412 | 56623

**Termine**<br>
:calendar:  Teil 1: 08.11.2017 - 06.12.2017<br>
:calendar:  Teil 2: 13.12.2017 - 24.01.2018<br>
:calendar:  Letzter Abgabetermin Mittwoch 24.01.2018<br>

**Aktuelle TO DO's**
- [ ] Aufgabe 1
- [ ] Aufgabe 2


## Aufgabe 1 - Read-Only File System

* Eine Containerdatei wird mittels eines Kommandos mkfs.myfs erstellt
* Beim Erstellen werden ausgewählte Dateien in die Containerdatei kopiert
(einmalig)
* Wenn eine Containerdatei mittels FUSE ins Dateisystem eingebunden wird,
können die enthaltenen Dateien gelesen, aber (noch) nicht verändert oder
gelöscht werden

#### Aufgabe 1a) - Definition des Aufbaus von MyFS-Containerdateien

In MyFS-Containerdatei sollte (zunächst) folgendes abgelegt werden:
* Superblock: Informationen zum File-System
* File Allocation Table (FAT)
* Blöcke mit Metadaten (Inodes) für Dateien im Dateisystem mit folgenden Einträgen:
  * Dateiname
  * Dateigröße
  * Benutzer / Gruppen-ID
  * Zugriffsberechtigungen (mode)
  * Zeitpunkt letzter Zugriff (atime) / letzte Veränderung (mtime) / letzte Statusänderung (ctime)
  * Zeiger auf ersten Datenblock
* Root-Verzeichnis mit Zeiger auf Inode für jede enthaltenden Datei
* Datenblocks der Dateien

Hinweis: Die Containerdatei soll nur ein einziges (Root-)Verzeichnis haben, d.h. es müssen
keine Verzeichnisbäume implementiert werden





#### Teilaufgabe 1b: Erstellen und Befüllen von Containerdateien mittels mkfs.myfs
#### Teilaufgabe 1c: Einbinden („mounten“) von Containerdateien mittels mount.myfs


## Aufgabe 2 - Read-Write File System

## Aufgabe 3 - Dokumentation
* etwa 10 Seiten für Aufgabe 1 und 2 zusammen
* Inhalt: 
  * Wie wurde die Aufgabenstellung gelöst (Lösungsweg)?
  * Wie wurde getestet?
  * Besonderheiten und Probleme ...
